package me.zhanghan177.teo_mobile;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import static me.zhanghan177.teo_mobile.GlobalConfig.ADMIN_PORT;
import static me.zhanghan177.teo_mobile.GlobalConfig.EVAL_MODE_SKIP_NOTIFICATION;
import static me.zhanghan177.teo_mobile.GlobalConfig.G_DATA_BUF_SIZE;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_DISMISS;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_NOTIFICATION_ID;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_PRE_AUTH_APPROVE;
import static me.zhanghan177.teo_mobile.GlobalConfig.INTENT_EXTRA_TYPE;
import static me.zhanghan177.teo_mobile.GlobalConfig.REQUEST_CODE_APPROVE;
import static me.zhanghan177.teo_mobile.GlobalConfig.REQUEST_CODE_DENY;
import static me.zhanghan177.teo_mobile.NetworkUtils.bytesToHex;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.consumeNotificationId;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.message_type_fltbuffers_size;
import static me.zhanghan177.teo_mobile.Utilities.createNotificationChannel;

public class TEOAdminService extends Service {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TEO Admin Service";

    String CHANNEL_ID = "admin";
    String notificationTitle = "Pre Auth Token Request";
    String notificationContent = "Click to approve new user's request for pre auth token!";

    ServerSocket serverSocket;
    Thread serverThread = null;

    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();

    byte[] pending_token = null;
    Socket pending_socket = null;

    private final ExecutorService executor;

    public TEOAdminService() {
        executor = Executors.newFixedThreadPool(2);
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TEOConnection, Context.BIND_AUTO_CREATE);

        startAdminServer();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        stopAdminServer();

        unbindService(TEOConnection);
        TEOConnection.setmBound(false);
    }

    private void stopAdminServer() {
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    class AdminServerThread implements Runnable {
        private final Context pkgContext;

        AdminServerThread(Context pkgContext) {
            this.pkgContext = pkgContext;
        }

        @Override
        public void run() {
            Socket socket = null;
            try {
                serverSocket = new ServerSocket(ADMIN_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }
            while (!Thread.currentThread().isInterrupted()) {

                try {

                    socket = serverSocket.accept();

                    CommunicationThread commThread = new CommunicationThread(socket, pkgContext);
                    new Thread(commThread).start();

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    class CommunicationThread implements Runnable {

        private final Socket clientSocket;
        private final Context pkgContext;

        public CommunicationThread(Socket clientSocket, Context pkgContext) {
            this.clientSocket = clientSocket;
            this.pkgContext = pkgContext;
        }

        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    InputStream inputStream = this.clientSocket.getInputStream();

                    assert (message_type_fltbuffers_size != 0);
                    byte[] messageType = new byte[message_type_fltbuffers_size];
                    int bytesRead = inputStream.read(messageType);
                    Log.v(TAG, "Message type read: " + bytesToHex(messageType, bytesRead) + ", total bytes: " + bytesRead);

                    if (!checkMessageTypeAdminPreAuthJNI(messageType)) {
                        Log.v(TAG, "Wrong message type");
                        continue;
                    }

                    Log.v(TAG, "Receive new request for pre auth token!");
                    byte[] request_content = new byte[G_DATA_BUF_SIZE];
                    bytesRead = inputStream.read(request_content);
                    Log.v(TAG, "Content read: " + bytesToHex(request_content, bytesRead) + ", total bytes: " + bytesRead);

                    pending_token = processPreAuthTokenJNI(request_content,
                            TEOConnection.getTEOBinder().getClientPubkey(), TEOConnection.getTEOBinder().getClientPrivkey());
                    pending_socket = this.clientSocket;

                    sendNotification(pkgContext);
                    return;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    private void startAdminServer() {
        this.serverThread = new Thread(new AdminServerThread(this));
        this.serverThread.start();
    }

    public void sendNotification(Context pkgContext) {
        int notificationId = consumeNotificationId();

        Intent intent = new Intent(pkgContext, TEOAdminService.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0 /* Request code */, intent,
                PendingIntent.FLAG_ONE_SHOT);

        // Create an explicit intent for an Activity in your app
        Intent approveInent = new Intent(pkgContext, TEOAdminService.class);
        Bundle approveExtras = new Bundle();
        approveExtras.putString(INTENT_EXTRA_TYPE, INTENT_EXTRA_PRE_AUTH_APPROVE);
        approveExtras.putInt(INTENT_EXTRA_NOTIFICATION_ID, notificationId);
        approveInent.putExtras(approveExtras);

        Intent dismissIntent = new Intent(pkgContext, TEOAdminService.class);
        Bundle dismissExtras = new Bundle();
        dismissExtras.putString(INTENT_EXTRA_TYPE, INTENT_EXTRA_DISMISS);
        dismissExtras.putInt(INTENT_EXTRA_NOTIFICATION_ID, notificationId);
        dismissIntent.putExtras(dismissExtras);

        if (EVAL_MODE_SKIP_NOTIFICATION) {
            startService(approveInent);
        } else {
            PendingIntent approvePendingIntent = PendingIntent.getService(pkgContext, REQUEST_CODE_APPROVE, approveInent, 0);
            PendingIntent dismissPendingIntent = PendingIntent.getService(pkgContext, REQUEST_CODE_DENY, dismissIntent, 0);

            createNotificationChannel(pkgContext, CHANNEL_ID);
            NotificationCompat.Builder builder = new NotificationCompat.Builder(pkgContext, CHANNEL_ID)
                    .setSmallIcon(R.drawable.ic_notification_admin)
                    .setContentTitle(notificationTitle)
                    .setContentText(notificationContent)
                    .setPriority(NotificationCompat.PRIORITY_HIGH)
                    .addAction(R.drawable.ic_deny, "Deny", dismissPendingIntent)
                    .addAction(R.drawable.ic_check, "Approve", approvePendingIntent)
                    .setContentIntent(pendingIntent)
                    .setAutoCancel(true)
                    .setOnlyAlertOnce(true);


            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(pkgContext);

            // notificationId is a unique int for each notification that you must define
            Notification notification = builder.build();
            notification.flags = Notification.FLAG_AUTO_CANCEL;
            notificationManager.notify(notificationId, notification);
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "Receive start command!");

        if (intent != null && intent.getExtras() != null) {
            Bundle extras = intent.getExtras();
            int notification_id = extras.getInt(INTENT_EXTRA_NOTIFICATION_ID, -1);
            if (notification_id != -1) {
                ((NotificationManager)getSystemService(NOTIFICATION_SERVICE)).cancelAll();
            }

            String type = extras.getString(INTENT_EXTRA_TYPE);
            if (type != null) {
                if (type.equals(INTENT_EXTRA_PRE_AUTH_APPROVE)) {
                    Log.v(TAG, "Pre Auth Request Approved!");
                    if (pending_socket != null && pending_token != null) {
                        executor.execute(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    pending_socket.getOutputStream().write(pending_token);

                                    pending_socket.close();
                                    pending_socket = null;
                                    pending_token = null;
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                        });
                    }
                } else if (type.equals(GlobalConfig.INTENT_EXTRA_SEND_NOTIFICATION)) {
                    sendNotification(getApplicationContext());
                } else if (type.equals(INTENT_EXTRA_DISMISS)) {
                    Log.v(TAG, "Pre Auth Request Denied!");
                    if (pending_socket != null && pending_token != null) {
                        try {
                            pending_socket.close();
                            pending_socket = null;
                            pending_token = null;
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * Java Native Interface
     */
    public native boolean checkMessageTypeAdminPreAuthJNI(byte[] messageType);

    private native byte[] processPreAuthTokenJNI(byte[] content, byte[] userPubkey, byte[] userPrivkey);

}