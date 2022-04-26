package me.zhanghan177.teo_mobile;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Binder;
import android.os.IBinder;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;

import static me.zhanghan177.teo_mobile.GlobalConfig.*;
import static me.zhanghan177.teo_mobile.Utilities.base64EncodeStrip;
import static me.zhanghan177.teo_mobile.Utilities.displayDialog;
import static me.zhanghan177.teo_mobile.Utilities.stripLineBreak;

import me.zhanghan177.teo_mobile.activities.TEOBinderClass;

public class TEOKeyStoreService extends Service {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TOT Key Store Service";
    final static String defaultNull = "";

    /**
     * Internal global variables
     */
    public byte[] sieveKey = null;
    public byte[] sieveKeyNonce = null;

    public String dataUUID;
    public String encMetaUUID;

    byte[] clientPubkey = null;
    byte[] clientPrivkey = null;

    byte[] deviceSecret = null;
    byte[] devicePubkey = null;
    String deviceIp = null;
    int devicePort = 0;

    byte[] adminPubkey = null;
    String adminIp = null;
    int adminPort = 0;

    byte[] storagePubkey = null;
    String storageIp = null;
    int storagePort = 0;
    byte[] adminManagedDevice = null;

    byte[] preAuthToken = null;

    byte[] claimedDevice = null;
    String claimedDeviceIp = null;
    int claimedDevicePort = 0;

    static int notificationId = 1;

    static int message_type_fltbuffers_size = 0;

    // Binder object to be given to clients/other activities
    private final IBinder binder = new TEOBinderClass(TEOKeyStoreService.this);

    public String getAdminManagedDevicePubkeyB64() {
        if (adminManagedDevice == null) {
            return "NULL";
        }
        return base64EncodeStrip(adminManagedDevice, Base64.DEFAULT);
    }

    public static int consumeNotificationId() {
        return ++notificationId;
    }

//    private void sendProximityHeartbeat(byte[] proximityNonce) {
//        if (claimedDevice == null) {
//            return;
//        }
//
//        proximityHeartbeatJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, proximityNonce, userPubkey);
//
//        Log.d(TAG, "Heartbeat beep, nonce: " + bytesToHex(proximityNonce));
//
//    }

//    private int releaseDevice() {
//        if (claimedDevice != null) {
//            releaseDeviceJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, userPubkey);
//
//            claimedDevice = null;
//        }
//        return 0;
//    }

//    private int removeRealTimeAccess() {
//        if (claimedDevice != null) {
//            removeRealTimeAccessJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, userPubkey);
//        }
//        return 0;
//    }

//    private void reencrypt() {
//        reencryptJNI(userPubkey, userPrivkey, dataUUID, encMetaUUID, sieveKey, sieveKeyNonce, storageIp, storagePort);
//    }

    public String getAdminIp() {
        return adminIp;
    }

    private int resolveAdminIpPort(Context context) {
        if (validateStorageInfo(context) != 0) return -1;

        setAdminIp(resolveIpJNI(getAdminPubkey(), getStorageIp(), getStoragePort()));
        setAdminPort(ADMIN_PORT);
        return 0;
    }

    private byte[] getAdminPubkey() {
        return adminPubkey;
    }

    private int validateStorageInfo(Context context) {
        if (!hasStorageInfo()) {
            String err = "You need to load Storage info first!! Try scan Storage's QR code.";
            Log.e(TAG, err);
            displayDialog(context, err);
            return -1;
        }
        return 0;
    }

    private int validateAdminInfo(Context context) {
        if (getAdminPubkey() == null) {
            String err = "You need to retrieve the device's current Admin " +
                    "info first!! Try scan device's QR code";
            Log.e(TAG, err);
            displayDialog(context, err);
            return -1;
        }

        return resolveAdminIpPort(context);
    }

    private int validateDeviceInfo(Context context) {
        if (!hasDeviceInfo()) {
            String err = "No device info! Scan Device's info QR code";
            Log.e(TAG, err);
            displayDialog(context, err);
            return -1;
        }
        return 0;
    }

    private int validatePreAuthToken(Context context) {
        if (!hasPreAuthToken()) {
            String err = "Empty pre-auth token for claiming this device! Try acquire one first.";
            Log.e(TAG, err);
            displayDialog(context, err);
            return -1;
        }
        return 0;
    }


    public String getClientPubkeyB64() {
        if (clientPubkey == null || clientPrivkey == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(clientPubkey, Base64.DEFAULT);
        }
    }

    public String getStoragePubkeyB64() {
        if (storagePubkey == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(storagePubkey, Base64.DEFAULT);
        }
    }

    public String getClaimedDeviceB64() {
        if (claimedDevice == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(claimedDevice, Base64.DEFAULT);
        }
    }

    public int claimDevice(Context context) {
        if (validateAdminInfo(context) != 0) {
            return -1;
        }

        if (!hasPreAuthToken()) {
            acquirePreAuthToken(context);
        }

        if (validatePreAuthToken(context) != 0
                || validateDeviceInfo(context) != 0) {
            return -1;
        }

        do {
            claimedDevice = claimDeviceJNI(clientPubkey, clientPrivkey,
                    preAuthToken, deviceIp,
                    devicePort, getAdminPubkey());
            claimedDeviceIp = deviceIp;
            claimedDevicePort = devicePort;
        } while (claimedDevice == null || byteArrayAllEmpty(claimedDevice));

        Intent intent = new Intent(this, TEOUserService.class);
        startService(intent);

        return 0;
    }

    private String getPreAuthTokenB64() {
        if (preAuthToken == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(preAuthToken, Base64.DEFAULT);
        }
    }

    private boolean byteArrayAllEmpty(byte[] in) {
        for (byte b : in) {
            if (b != 0) return false;
        }
        return true;
    }


    private boolean hasPreAuthToken() {
        return getPreAuthToken() != null;
    }

    private boolean hasStorageInfo() {
        return (getStorageIp() != null && !(getStorageIp().equals("")) && getStoragePort() != 0);
    }

    private byte[] getPreAuthToken() {
        return preAuthToken;
    }

    private int acquirePreAuthToken(Context context) {
        // validate Admin info is not empty
        if (validateAdminInfo(context) != 0) {
            return -1;
        }

        do {
            preAuthToken = acquirePreAuthTokenJNI(clientPubkey, clientPrivkey, adminIp, adminPort, adminPubkey);
        } while (preAuthToken == null || byteArrayAllEmpty(preAuthToken));
        return 0;
    }

    public String getAdminPubkeyB64() {
        if (getAdminPubkey() == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(getAdminPubkey(), Base64.DEFAULT);
        }
    }

    /**
     * Internal core functions.
     */
    public byte[] getClientPubkey() {
        return clientPubkey;
    }

    public byte[] getClientPrivkey() {
        return clientPrivkey;
    }

    public void setStoragePubkey(byte[] storagePubkey) {
        this.storagePubkey = storagePubkey;
        registerIPtoKMS();
    }

    public void setStorageIp(String storageIp) {
        this.storageIp = storageIp;
        registerIPtoKMS();
    }

    public String getStorageIp() {
        return storageIp;
    }

    public void setStoragePort(int storagePort) {
        this.storagePort = storagePort;
        registerIPtoKMS();
    }

    public int getStoragePort() {
        return storagePort;
    }

    public void setAdminPubkey(byte[] adminPubkey) {
        this.adminPubkey = adminPubkey;
    }

    public void setAdminIp(String adminIp) {
        this.adminIp = adminIp;
    }

    public void setAdminPort(int adminPort) {
        this.adminPort = adminPort;
    }

    public void setClientPubkey(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting user's public key to be (b64): " + base64EncodeStrip(in, Base64.DEFAULT) + ", length: " + in.length));
        clientPubkey = in;
    }

    public void setClientPrivkey(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting user's private key to be (b64): " + base64EncodeStrip(in, Base64.DEFAULT) + ", length: " + in.length));
        clientPrivkey = in;
    }

    public void setDeviceSecret(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting device secrets: " + base64EncodeStrip(in, Base64.DEFAULT)));
        deviceSecret = in;
    }

    public void setDevicePubkey(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting device pubkey: " + base64EncodeStrip(in, Base64.DEFAULT)));
        devicePubkey = in;
    }

    public void setDeviceIp(String in) {
        deviceIp = in;
    }

    public void setDevicePort(int devicePort) {
        this.devicePort = devicePort;
    }

    public String getDeviceSecretB64() {
        if (deviceSecret == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(deviceSecret, Base64.DEFAULT);
        }
    }

    public TEOKeyStoreService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        loadUserCredentials();

        loadLIBTOTKeySizes();

        message_type_fltbuffers_size = getMessageTypeFltbuffersSizeJNI();
    }

    private void registerIPtoKMS() {
        if (storageIp == null || storageIp.isEmpty() || storagePort == 0 || storagePubkey == null) {
            return;
        }

        String ip_addr = NetworkUtils.getIPAddress(true);

        registerIPKmsJNI(clientPubkey, ip_addr, 0, storageIp, storagePort);
    }

    private void loadLIBTOTKeySizes() {
        AsymmetricEncryptionKeySet_FULL_PK_SIZE = getAsymmetricEncryptionKeySet_FULL_PK_SIZE_JNI();
        G_CHALLENGE_SIZE = getG_CHALLENGE_SIZE_JNI();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        storeUserCredentials();
    }

    public void flushKeyPair() {
        Log.d(TAG, "Regenerate user key pair");
        generateKeypairJNI();
        assert (clientKeypairValid());
    }

    private void loadUserCredentials() {
        SharedPreferences sharedPref = getSharedPreferences();
        String userPubkeyB64 = sharedPref.getString(getString(R.string.user_pubkey_b64), defaultNull);
        String userPrivkeyB64 = sharedPref.getString(getString(R.string.user_privkey_b64), defaultNull);

        if (userPubkeyB64.equals(defaultNull) || userPrivkeyB64.equals(defaultNull)) {
            flushKeyPair();
        } else {
            setClientPubkey(Base64.decode(userPubkeyB64, Base64.DEFAULT));
            setClientPrivkey(Base64.decode(userPrivkeyB64, Base64.DEFAULT));
        }
    }

    private void storeUserCredentials() {
        if (clientKeypairValid()) {
            SharedPreferences.Editor editor = getSharedPreferences().edit();
            editor.putString(getString(R.string.user_pubkey_b64), base64EncodeStrip(clientPubkey, Base64.DEFAULT));
            editor.putString(getString(R.string.user_privkey_b64), base64EncodeStrip(clientPrivkey, Base64.DEFAULT));
            editor.apply();
        }
    }


    private int validateDeviceSecret(Context context) {
        if (!(hasDeviceSecret() && hasDeviceInfo())) {
            String err = "No device info! Scan Device's setup QR code";
            Log.e(TAG, err);
            displayDialog(context, err);
            return -1;
        }
        return 0;
    }

    public int initDevice(Context context) {
        if (validateStorageInfo(context) != 0
                || validateDeviceSecret(context) != 0) {
            return -1;
        }

        int res = prepareInitializationRequestJNI(clientPubkey, clientPrivkey,
                deviceSecret, deviceIp,
                devicePort, devicePubkey);
        Log.d(TAG, "Return result is: " + res);

        if (res == 0) {
            // Successfully initialize devices
            // Launch admin services
            adminManagedDevice = devicePubkey;
            Intent intent = new Intent(this, TEOAdminService.class);
            startService(intent);
        }

        return res;
    }


    private boolean hasDeviceInfo() {
        return devicePubkey != null && deviceIp != null && !deviceIp.isEmpty();
    }

    private boolean hasDeviceSecret() {
        return deviceSecret != null;
    }


    private SharedPreferences getSharedPreferences() {
        Context context = getApplicationContext();
        return context.getSharedPreferences(
                getString(R.string.crypto_key_file_key), Context.MODE_PRIVATE);
    }

    private boolean clientKeypairValid() {
        return clientPubkey != null && clientPrivkey != null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "Receive start command!");
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, "Receive bind command!");
        return binder;
    }


    /**
     * Java Native Interface
     */
    public native int generateKeypairJNI();

    public native int getAsymmetricEncryptionKeySet_FULL_PK_SIZE_JNI();

    public native int getG_CHALLENGE_SIZE_JNI();

    public native int getMessageTypeFltbuffersSizeJNI();

    public native int prepareInitializationRequestJNI(byte[] userPubkey, byte[] userPrivkey,
                                                      byte[] deviceSecret, String deviceIp,
                                                      int devicePort, byte[] devicePubkey);

    public native byte[] acquirePreAuthTokenJNI(byte[] userPubkey, byte[] userPrivkey,
                                                String adminIp, int adminPort, byte[] adminPubkey);

    public native byte[] claimDeviceJNI(byte[] userPubkey, byte[] userPrivkey, byte[] preAuthToken,
                                        String deviceIp, int devicePort, byte[] adminPubkey);

    public native int registerIPKmsJNI(byte[] clientPubkey, String clientIp, int clientPort,
                                       String storageIp, int storagePort);

    public native String resolveIpJNI(byte[] queryPubkey, String storageIp, int storagePort);

//    private native void reencryptJNI(byte[] userPubkey, byte[] userPrivkey,
//                                     String uuid, String encMetaUUID,
//                                     byte[] sieveKey, byte[] sieveKeyNonce,
//                                     String storageIp, int storagePort);
//
//    private native void releaseDeviceJNI(byte[] claimedDevice,
//                                         String claimedDeviceIp,
//                                         int claimedDevicePort,
//                                         byte[] userPubkey);
//
//    private native void removeRealTimeAccessJNI(byte[] claimedDevice,
//                                                String claimedDeviceIp,
//                                                int claimedDevicePort,
//                                                byte[] userPubkey);

//    private native void proximityHeartbeatJNI(byte[] claimedDevice,
//                                              String claimedDeviceIp,
//                                              int claimedDevicePort,
//                                              byte[] proximityNonce,
//                                              byte[] userPubkey);
}