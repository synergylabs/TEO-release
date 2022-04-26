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

import static me.zhanghan177.teo_mobile.GlobalConfig.EVAL_MODE_SKIP_NOTIFICATION;
import static me.zhanghan177.teo_mobile.GlobalConfig.G_DATA_BUF_SIZE;
import static me.zhanghan177.teo_mobile.GlobalConfig.USER_PORT;
import static me.zhanghan177.teo_mobile.NetworkUtils.bytesToHex;
import static me.zhanghan177.teo_mobile.TEOAdminService.EXTRA_TYPE;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.consumeNotificationId;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.message_type_fltbuffers_size;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

public class TEOUserService extends Service {

    private static final String REAL_TIME_APPROVE = "Real time approved";
    private static final String REAL_TIME_DENIED = "Real time denied";

    String CHANNEL_ID = "user";
    String notificationTitle = "Real Time Access Request";
    String notificationContent = "An app request real time data access!";


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TOT User Service";

    ServerSocket serverSocket;
    Thread serverThread = null;

    OutputStream pendingOutputStream = null;

    byte[] sessionKey = null;

    private final TEOServiceConnection TOTConnection = new TEOServiceConnection();

    public void setSessionKey(byte[] sessionKey) {
        this.sessionKey = sessionKey;
    }

    public void setSieveKey(byte[] sieveKey) {
        TOTConnection.getTOTBinder().setSieveKey(sieveKey);
    }

    public void setSieveKeyNonce(byte[] sieveKeyNonce) {
        TOTConnection.getTOTBinder().setSieveKeyNonce(sieveKeyNonce);
    }

    public byte[] getSessionKey() {
        return sessionKey;
    }

    public byte[] getSieveKey() {
        return TOTConnection.getTOTBinder().getSieveKey();
    }

    public byte[] getSieveKeyNonce() {
        return TOTConnection.getTOTBinder().getSieveKeyNonce();
    }

    public TEOUserService() {
    }


    @Override
    public void onCreate() {
        super.onCreate();

        Log.v(TAG, "Service start!");

        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TOTConnection, Context.BIND_AUTO_CREATE);

        startUserServer();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "Receive start command!");

        if (intent != null) {
            String type = intent.getStringExtra(EXTRA_TYPE);
            if (type != null) {
                boolean granted = false;
                if (type.equals(REAL_TIME_APPROVE)) {
                    Log.v(TAG, "Real time access Request Approved!");
                    granted = true;
                } else if (type.equals(REAL_TIME_DENIED)) {
                    Log.v(TAG, "Real time access request Denied!");
                    granted = false;
                }

                NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
                int notificationId = consumeNotificationId();
                manager.cancel(notificationId);

                if (pendingOutputStream == null) {
                    Log.v(TAG, "Pending output stream is empty");
                } else {
                    byte[] response_content = processUtilRealTimeAccessJNI(granted);

                    Thread sender = new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                pendingOutputStream.write(response_content);
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    });

                    sender.start();
                }
            }
        }

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();


        stopUserServer();

        unbindService(TOTConnection);
        TOTConnection.setmBound(false);
    }

    private void stopUserServer() {
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    class UserServerThread implements Runnable {

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
                    CommunicationThread commThread = new CommunicationThread(socket);
                    new Thread(commThread).start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    public void sendNotification() {
        // Create an explicit intent for an Activity in your app
        Intent approveIntent = new Intent(this, TEOUserService.class);
        approveIntent.putExtra(EXTRA_TYPE, REAL_TIME_APPROVE);

        Intent denyIntent = new Intent(this, TEOUserService.class);
        denyIntent.putExtra(EXTRA_TYPE, REAL_TIME_DENIED);

        if (EVAL_MODE_SKIP_NOTIFICATION) {
            startService(approveIntent);
        } else {
            PendingIntent pendingApprove = PendingIntent.getService(this,
                    0,
                    approveIntent,
                    0);
            PendingIntent pendingDeny = PendingIntent.getService(this,
                    0,
                    denyIntent,
                    0);


            createNotificationChannel();
            NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                    .setSmallIcon(R.drawable.ic_notification_admin)
                    .setContentTitle(notificationTitle)
                    .setContentText(notificationContent)
                    .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                    .addAction(R.drawable.ic_check, getString(R.string.approve), pendingApprove)
                    .addAction(R.drawable.ic_deny, getString(R.string.deny), pendingDeny)
                    .setAutoCancel(true);


            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);

            // notificationId is a unique int for each notification that you must define
            int notificationId = consumeNotificationId();
            notificationManager.notify(notificationId, builder.build());
        }
    }

    class CommunicationThread implements Runnable {
        private final Socket clientSocket;

        public CommunicationThread(Socket clientSocket) {
            this.clientSocket = clientSocket;
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
                                TOTConnection.getTOTBinder().getClientPubkey(),
                                TOTConnection.getTOTBinder().getClientPrivkey());

                        outputStream.write(response_content);

                        bytesRead = inputStream.read(messageType);
                        if (!checkMessageTypeUploadNotificationJNI(messageType)) {
                            Log.e(TAG, "Did not receive upload notification");
                            continue;
                        }

                        bytesRead = inputStream.read(request_content);
                        byte[] sieveDataUUID = processUploadNotificationJNI(request_content,
                                TOTConnection.getTOTBinder().getClientPubkey(),
                                TOTConnection.getTOTBinder().getClientPrivkey(),
                                getSessionKey());



                    } else if (checkMessageTypeDataAccessFetchJNI(messageType)) {
                        byte[] request_content = new byte[G_DATA_BUF_SIZE];
                        bytesRead = inputStream.read(request_content);

                        byte[] response_content = processDataAccessFetchJNI(request_content,
                                TOTConnection.getTOTBinder().getClientPubkey(),
                                TOTConnection.getTOTBinder().getClientPrivkey());

                        outputStream.write(response_content);

                    } else if (checkMessageTypeUtilRealTimeAccessRequestJNI(messageType)) {
                        byte[] request_content = new byte[G_DATA_BUF_SIZE];
                        bytesRead = inputStream.read(request_content);

                        pendingOutputStream = outputStream;
                        sendNotification();
                    } else {
                        Log.v(TAG, "Wrong message type");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void startUserServer() {
        this.serverThread = new Thread(new UserServerThread());
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
                                                     byte[] userPubkey, byte[] userPrivkey);

    private native boolean checkMessageTypeUploadNotificationJNI(byte[] messageType);

    private native byte[] processUploadNotificationJNI(byte[] request_content,
                                                       byte[] userPubkey, byte[] userPrivkey,
                                                       byte[] sessionKey);

    private native boolean checkMessageTypeDataAccessFetchJNI(byte[] messageType);

    private native byte[] processDataAccessFetchJNI(byte[] request_content,
                                                    byte[] userPubkey, byte[] userPrivkey);

    private native boolean checkMessageTypeUtilRealTimeAccessRequestJNI(byte[] messageType);

    private native byte[] processUtilRealTimeAccessJNI(boolean granted);
}