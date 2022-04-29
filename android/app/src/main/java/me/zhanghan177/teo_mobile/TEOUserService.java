package me.zhanghan177.teo_mobile;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static me.zhanghan177.teo_mobile.GlobalConfig.EVAL_MODE_SKIP_NOTIFICATION;
import static me.zhanghan177.teo_mobile.GlobalConfig.G_DATA_BUF_SIZE;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_DATA_ACCESS_REQUEST;
import static me.zhanghan177.teo_mobile.GlobalConfig.USER_PORT;
import static me.zhanghan177.teo_mobile.NetworkUtils.bytesToHex;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_TYPE;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.consumeNotificationId;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.message_type_fltbuffers_size;
import static me.zhanghan177.teo_mobile.Utilities.createNotificationChannel;
import static me.zhanghan177.teo_mobile.activities.UserDashboardActivity.BROADCAST_ACTION_UPDATE_USER_OWNED_DATA;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

public class TEOUserService extends Service {

    private static final String REAL_TIME_APPROVE = "Real time approved";
    private static final String REAL_TIME_DENIED = "Real time denied";

    private final ExecutorService executor;
    public Socket pending_socket = null;
    public byte[] pending_fetch_payload;
    public byte[] pending_accessor_pubkey;

    String CHANNEL_ID = "user";
    String notificationTitle = "Real Time Access Request";
    String notificationContent = "Click to approve an app's request to your data!";

    public byte[] requester_pubkey;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TEO User Service";

    ServerSocket serverSocket;
    Thread serverThread = null;

    OutputStream pendingOutputStream = null;

    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();

    public void setPendingFetchPayload(byte[] bytes_in) {
        pending_fetch_payload = bytes_in;
    }

    public void setPendingAccessorPubkey(byte[] bytes_in) {
        pending_accessor_pubkey = bytes_in;
    }

    public byte[] getPendingFetchPayload() {
        if (pending_fetch_payload == null) {
            Log.e(TAG, "Null pending fetch payload!");
        }
        return pending_fetch_payload;
    }

    public byte[] getPendingAccessorPubkey() {
        if (pending_accessor_pubkey == null) {
            Log.e(TAG, "Null pending fetch payload!");
        }
        return pending_accessor_pubkey;
    }

    public void setSieveKey(byte[] sieveKey) {
        TEOConnection.getTEOBinder().setSieveKey(sieveKey);
    }

    public void setSieveKeyNonce(byte[] sieveKeyNonce) {
        TEOConnection.getTEOBinder().setSieveKeyNonce(sieveKeyNonce);
    }

    public byte[] getSieveKey() {
        return TEOConnection.getTEOBinder().getSieveKey();
    }

    public byte[] getSieveKeyNonce() {
        return TEOConnection.getTEOBinder().getSieveKeyNonce();
    }

    public void setMetadataUUID(byte[] uuid_in) {
        TEOConnection.getTEOBinder().setMetadataUUID(uuid_in);
    }

    public byte[] getMetadataUUID() {
        return TEOConnection.getTEOBinder().getMetadataUUID();
    }

    public void setSieveDataUUID(byte[] uuid_in) {
        TEOConnection.getTEOBinder().setSieveDataUUID(uuid_in);
    }

    public byte[] getSieveDataUUID() {
        return TEOConnection.getTEOBinder().getSieveDataUUID();
    }

    public void setRequesterPubkey(byte[] requester_in) {
        requester_pubkey = requester_in;
    }

    public byte[] getRequesterPubkey() {
        return requester_pubkey;
    }

    public TEOUserService() {
        executor = Executors.newFixedThreadPool(2);
    }


    @Override
    public void onCreate() {
        super.onCreate();

        Log.v(TAG, "Service start!");

        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TEOConnection, Context.BIND_AUTO_CREATE);

        startUserServer();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "Receive start command!");

        if (intent != null) {
            String type = intent.getStringExtra(INTENT_EXTRA_TYPE);
            if (type != null) {
                if (type.equals(INTENT_EXTRA_DATA_ACCESS_REQUEST)) {
                    Log.v(TAG, "Data access request Approved!");
                    // continue with phase 2 of data access fetch handler
                    if (getPendingFetchPayload() == null || getPendingAccessorPubkey() == null) {
                        Log.v(TAG, "Null pending accessor pubkey or fetch payload!");
                    } else if (pending_socket != null) {
                        byte[] msg = processDataAccessFetch2JNI(
                                TEOConnection.getTEOBinder().getClientPubkey(),
                                TEOConnection.getTEOBinder().getClientPrivkey(),
                                TEOConnection.getTEOBinder().getSieveKey(),
                                TEOConnection.getTEOBinder().getSieveKeyNonce(),
                                getPendingFetchPayload(),
                                getPendingAccessorPubkey()
                        );
                        setPendingFetchPayload(null);
                        setPendingAccessorPubkey(null);
                        if (msg != null) {
                            executor.execute(() -> {
                                try {
                                    pending_socket.getOutputStream().write(msg);
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            });
                        } else {
                            try {
                                pending_socket.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                }
            }
        }

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();


        stopUserServer();

        unbindService(TEOConnection);
        TEOConnection.setmBound(false);
    }

    private void stopUserServer() {
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    class UserServerThread implements Runnable {
        private final Context pkgContext;

        UserServerThread(Context pkgContext) {
            this.pkgContext = pkgContext;
        }

        @Override
        public void run() {
            try {
                serverSocket = new ServerSocket(USER_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    Socket socket = serverSocket.accept();
                    CommunicationThread commThread = new CommunicationThread(socket, pkgContext);
                    new Thread(commThread).start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }


//    public void sendNotification() {
//        // Create an explicit intent for an Activity in your app
//        Intent approveIntent = new Intent(this, TEOUserService.class);
//        approveIntent.putExtra(INTENT_EXTRA_TYPE, REAL_TIME_APPROVE);
//
//        Intent denyIntent = new Intent(this, TEOUserService.class);
//        denyIntent.putExtra(INTENT_EXTRA_TYPE, REAL_TIME_DENIED);
//
//        if (EVAL_MODE_SKIP_NOTIFICATION) {
//            startService(approveIntent);
//        } else {
//            PendingIntent pendingApprove = PendingIntent.getService(this,
//                    0,
//                    approveIntent,
//                    0);
//            PendingIntent pendingDeny = PendingIntent.getService(this,
//                    0,
//                    denyIntent,
//                    0);
//
//
//            createNotificationChannel();
//            NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
//                    .setSmallIcon(R.drawable.ic_notification_admin)
//                    .setContentTitle(notificationTitle)
//                    .setContentText(notificationContent)
//                    .setPriority(NotificationCompat.PRIORITY_DEFAULT)
//                    .addAction(R.drawable.ic_check, getString(R.string.approve), pendingApprove)
//                    .addAction(R.drawable.ic_deny, getString(R.string.deny), pendingDeny)
//                    .setAutoCancel(true);
//
//
//            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
//
//            // notificationId is a unique int for each notification that you must define
//            int notificationId = consumeNotificationId();
//            notificationManager.notify(notificationId, builder.build());
//        }
//    }

    class CommunicationThread implements Runnable {
        private final Socket clientSocket;
        private final Context pkgContext;

        public CommunicationThread(Socket clientSocket, Context pkgContext) {
            this.clientSocket = clientSocket;
            this.pkgContext = pkgContext;
        }

        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    InputStream inputStream = this.clientSocket.getInputStream();
                    OutputStream outputStream = this.clientSocket.getOutputStream();

                    assert (message_type_fltbuffers_size != 0);
                    byte[] messageType = new byte[message_type_fltbuffers_size];
                    int bytesRead = inputStream.read(messageType);
                    Log.v(TAG, "Message type read: " + bytesToHex(messageType, bytesRead) +
                            ", total bytes: " + bytesRead);

                    if (checkMessageTypeDataStoreSieveCredRequestJNI(messageType)) {
                        byte[] request_content = new byte[G_DATA_BUF_SIZE];
                        bytesRead = inputStream.read(request_content);

                        byte[] response_content = processSieveCredRequestJNI(request_content,
                                TEOConnection.getTEOBinder().getClientPubkey(),
                                TEOConnection.getTEOBinder().getClientPrivkey(),
                                TEOConnection.getTEOBinder().getClaimedDevice());

                        outputStream.write(response_content);

                        bytesRead = inputStream.read(messageType);
                        if (!checkMessageTypeUploadNotificationJNI(messageType)) {
                            Log.e(TAG, "Did not receive upload notification");
                            continue;
                        }

                        bytesRead = inputStream.read(request_content);
                        int err = processUploadNotificationJNI(request_content,
                                TEOConnection.getTEOBinder().getClientPubkey(),
                                TEOConnection.getTEOBinder().getClientPrivkey(),
                                getRequesterPubkey());

                        if (err != 0) {
                            Log.v(TAG, "Error processing upload notification!");
                        } else {
                            Intent broadcast = new Intent();
                            broadcast.setAction(BROADCAST_ACTION_UPDATE_USER_OWNED_DATA);
                            sendBroadcast(broadcast);
                        }
                    } else if (checkMessageTypeDataAccessFetchJNI(messageType)) {
                        byte[] request_content = new byte[G_DATA_BUF_SIZE];
                        bytesRead = inputStream.read(request_content);

                        int err = processDataAccessFetch1JNI(request_content,
                                TEOConnection.getTEOBinder().getClientPubkey(),
                                TEOConnection.getTEOBinder().getClientPrivkey());

                        if (err == 0) {
                            pending_socket = clientSocket;
                            sendNotification(pkgContext);
                        }

//                        byte[] response_content = processDataAccessFetchJNI(request_content,
//                                TOTConnection.getTOTBinder().getClientPubkey(),
//                                TOTConnection.getTOTBinder().getClientPrivkey());
//
//                        outputStream.write(response_content);

//                    } else if (checkMessageTypeUtilRealTimeAccessRequestJNI(messageType)) {
//                        byte[] request_content = new byte[G_DATA_BUF_SIZE];
//                        bytesRead = inputStream.read(request_content);
//
//                        pendingOutputStream = outputStream;
//                        sendNotification();
                    } else {
                        Log.v(TAG, "Wrong message type");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void sendNotification(Context pkgContext) {
        // Create an explicit intent for an Activity in your app
        Intent intent = new Intent(pkgContext, TEOUserService.class);
        intent.putExtra(INTENT_EXTRA_TYPE, INTENT_EXTRA_DATA_ACCESS_REQUEST);

        if (EVAL_MODE_SKIP_NOTIFICATION) {
            startService(intent);
        } else {
            PendingIntent pendingIntent = PendingIntent.getService(pkgContext, 0, intent, 0);

            createNotificationChannel(pkgContext, CHANNEL_ID);
            NotificationCompat.Builder builder = new NotificationCompat.Builder(pkgContext, CHANNEL_ID)
                    .setSmallIcon(R.drawable.ic_notification_admin)
                    .setContentTitle(notificationTitle)
                    .setContentText(notificationContent)
                    .setPriority(NotificationCompat.PRIORITY_HIGH)
                    .setContentIntent(pendingIntent)
                    .setAutoCancel(true);


            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(pkgContext);

            // notificationId is a unique int for each notification that you must define
            int notificationId = consumeNotificationId();
            notificationManager.notify(notificationId, builder.build());
        }
    }

    private void startUserServer() {
        this.serverThread = new Thread(new UserServerThread(this));
        this.serverThread.start();
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }


    /**
     * Java Native Interface
     */

    private native boolean checkMessageTypeDataStoreSieveCredRequestJNI(byte[] messageType);

    private native byte[] processSieveCredRequestJNI(byte[] request_content,
                                                     byte[] userPubkey, byte[] userPrivkey,
                                                     byte[] claimedDevice);

    private native boolean checkMessageTypeUploadNotificationJNI(byte[] messageType);

    private native int processUploadNotificationJNI(byte[] request_content,
                                                    byte[] userPubkey, byte[] userPrivkey,
                                                    byte[] requesterPubkey);

    private native boolean checkMessageTypeDataAccessFetchJNI(byte[] messageType);

    private native int processDataAccessFetch1JNI(byte[] request_content,
                                                  byte[] userPubkey, byte[] userPrivkey);

    private native byte[] processDataAccessFetch2JNI(byte[] userPubkey, byte[] userPrivkey,
                                                     byte[] sieveKeyContent, byte[] sieveKeyNonce,
                                                     byte[] fetchPayload, byte[] accessorPubkey);

//    private native boolean checkMessageTypeUtilRealTimeAccessRequestJNI(byte[] messageType);

//    private native byte[] processUtilRealTimeAccessJNI(boolean granted);
}