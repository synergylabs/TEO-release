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
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import static me.zhanghan177.teo_mobile.GlobalConfig.ADMIN_PORT;
import static me.zhanghan177.teo_mobile.GlobalConfig.EVAL_MODE_SKIP_NOTIFICATION;
import static me.zhanghan177.teo_mobile.GlobalConfig.G_DATA_BUF_SIZE;
import static me.zhanghan177.teo_mobile.NetworkUtils.bytesToHex;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.consumeNotificationId;
import static me.zhanghan177.teo_mobile.TEOKeyStoreService.message_type_fltbuffers_size;

public class TEOAdminService extends Service {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TOT Admin Service";
    public static final String EXTRA_TYPE = "type";
    public static final String PRE_AUTH_APPROVE = "pre_auth_approve";
    public static final String SEND_NOTIFICATION = "send notification";

    String CHANNEL_ID = "admin";
    String notificationTitle = "Pre Auth Token Request";
    String notificationContent = "Click to approve new user's request for pre auth token!";

    ServerSocket serverSocket;
    Thread serverThread = null;

    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();

    byte[] pending_token = null;
    Socket pending_socket = null;

    private ExecutorService executor;


    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_HIGH;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

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
                            TEOConnection.getTOTBinder().getClientPubkey(), TEOConnection.getTOTBinder().getClientPrivkey());
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
        // Create an explicit intent for an Activity in your app
        Intent intent = new Intent(pkgContext, TEOAdminService.class);
        intent.putExtra(EXTRA_TYPE, PRE_AUTH_APPROVE);

        if (EVAL_MODE_SKIP_NOTIFICATION) {
            startService(intent);
        } else {
            PendingIntent pendingIntent = PendingIntent.getService(pkgContext, 0, intent, 0);

            createNotificationChannel();
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

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "Receive start command!");

        if (intent != null) {
            String type = intent.getStringExtra(EXTRA_TYPE);
            if (type != null) {
                if (type.equals(PRE_AUTH_APPROVE)) {
                    Log.v(TAG, "Pre Auth Request Approved!");
                    if (pending_socket != null && pending_token != null) {
                        executor.execute(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    pending_socket.getOutputStream().write(pending_token);
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                        });
                    }
                } else if (type.equals(SEND_NOTIFICATION)) {
                    sendNotification(this);
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