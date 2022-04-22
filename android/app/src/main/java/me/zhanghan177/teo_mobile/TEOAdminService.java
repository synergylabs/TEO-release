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

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import static me.zhanghan177.teo_mobile.GlobalConfig.ADMIN_PORT;
import static me.zhanghan177.teo_mobile.GlobalConfig.EVAL_MODE_SKIP_NOTIFICATION;
import static me.zhanghan177.teo_mobile.GlobalConfig.G_DATA_BUF_SIZE;
import static me.zhanghan177.teo_mobile.NetworkUtils.bytesToHex;
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
    String notificationContent = "A new user requests for pre auth token!";

    ServerSocket serverSocket;
    Thread serverThread = null;

    private final TEOServiceConnection TOTConnection = new TEOServiceConnection();

    byte[] pending_token = null;
    Socket pending_socket = null;

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

    public TEOAdminService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TOTConnection, Context.BIND_AUTO_CREATE);

        startAdminServer();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        stopAdminServer();

        unbindService(TOTConnection);
        TOTConnection.setmBound(false);
    }

    private void stopAdminServer() {
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    class AdminServerThread implements Runnable {
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

                    CommunicationThread commThread = new CommunicationThread(socket);
                    new Thread(commThread).start();

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    class CommunicationThread implements Runnable {

        private final Socket clientSocket;

        public CommunicationThread(Socket clientSocket) {
            this.clientSocket = clientSocket;
        }

        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    InputStream inputStream = this.clientSocket.getInputStream();

                    assert (message_type_fltbuffers_size != 0);
                    byte[] messageType = new byte[message_type_fltbuffers_size];
                    int bytesRead = inputStream.read(messageType);
                    Log.v(TAG, "Message type read: " + bytesToHex(messageType) + ", total bytes: " + bytesRead);

                    if (!checkMessageTypeAdminPreAuthJNI(messageType)) {
                        Log.v(TAG, "Wrong message type");
                        continue;
                    }

                    Log.v(TAG, "Receive new request for pre auth token!");
                    byte[] request_content = new byte[G_DATA_BUF_SIZE];
                    bytesRead = inputStream.read(request_content);
                    Log.v(TAG, "Content read: " + bytesToHex(request_content) + ", total bytes: " + bytesRead);

                    pending_token = processPreAuthTokenJNI(request_content, TOTConnection.getTOTBinder().getClientPubkey(), TOTConnection.getTOTBinder().getClientPrivkey());
                    pending_socket = this.clientSocket;

                    sendNotification();
                    return;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    private void startAdminServer() {
        this.serverThread = new Thread(new AdminServerThread());
        this.serverThread.start();
    }

    public void sendNotification() {
        // Create an explicit intent for an Activity in your app
        Intent intent = new Intent(this, TEOAdminService.class);
        intent.putExtra(EXTRA_TYPE, PRE_AUTH_APPROVE);

        if (EVAL_MODE_SKIP_NOTIFICATION) {
            startService(intent);
        } else {
            PendingIntent pendingIntent = PendingIntent.getService(this, 0, intent, 0);

            createNotificationChannel();
            NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                    .setSmallIcon(R.drawable.ic_notification_admin)
                    .setContentTitle(notificationTitle)
                    .setContentText(notificationContent)
                    .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                    .setContentIntent(pendingIntent)
                    .setAutoCancel(true);


            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);

            // notificationId is a unique int for each notification that you must define
            int notificationId = 1;
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
                        try {
                            pending_socket.getOutputStream().write(pending_token);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                } else if (type.equals(SEND_NOTIFICATION)) {
                    sendNotification();
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