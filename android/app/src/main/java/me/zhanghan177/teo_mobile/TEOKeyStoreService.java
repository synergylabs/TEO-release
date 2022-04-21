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
import static me.zhanghan177.teo_mobile.Utilities.bytesToHex;
import static me.zhanghan177.teo_mobile.Utilities.stripLineBreak;

public class TEOKeyStoreService extends Service {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    final static String TAG = "TOT Key Store Service";
    final static String defaultNull = "";


    byte[] sieveKey = null;
    byte[] sieveKeyNonce = null;

    String dataUUID;
    String encMetaUUID;


    // Binder given to clients
    private final IBinder binder = new TOTLocalBinder();

    public class TOTLocalBinder extends Binder {
        public void flushKeyPair() {
            TEOKeyStoreService.this.flushKeyPair();
        }

        public String getDeviceSecretB64() {
            return TEOKeyStoreService.this.getDeviceSecretB64();
        }

        public int setDeviceSecret(@NonNull String secretB64) {
            // Skipping redundant assignment
            if (TEOKeyStoreService.this.getDeviceSecretB64().equals(secretB64)) {
                return -1;
            }

            try {
                TEOKeyStoreService.this.setDeviceSecret(Base64.decode(secretB64, Base64.DEFAULT));
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
                Log.e(TAG, "Incorrect padding for Base64 string");
                return -1;
            }
            return 0;
        }

        public void setDeviceInfo(String devicePubkeyB64, String deviceIp, String devicePort) {
            setDevicePubkey(Base64.decode(devicePubkeyB64, Base64.DEFAULT));
            setDeviceIp(deviceIp);
            setDevicePort(Integer.parseInt(devicePort));
        }

        public int initDevice() {
            return TEOKeyStoreService.this.initDevice();
        }

        public void setAdminInfo(String adminPubkeyB64, String adminIp, String adminPort) {
            setAdminPubkey(Base64.decode(adminPubkeyB64, Base64.DEFAULT));
            if (adminIp.equals("") || adminPort.equals("")) {
                TEOKeyStoreService.this.resolveAdminIpPort();
            } else {
                setAdminIp(adminIp);
                setAdminPort(Integer.parseInt(adminPort));
            }

            Log.v(TAG, "Admin IP: " + getAdminIp());
        }

        public String getAdminPubkeyB64() {
            return TEOKeyStoreService.this.getAdminPubkeyB64();
        }

        public int acquirePreAuthToken() {
            return TEOKeyStoreService.this.acquirePreAuthToken();
        }

        public String getPreAuthTokenB64() {
            return TEOKeyStoreService.this.getPreAuthTokenB64();
        }

        public int claimDevice() {
            return TEOKeyStoreService.this.claimDevice();
        }

        public String getClaimedDeviceB64() {
            return TEOKeyStoreService.this.getClaimedDeviceB64();
        }

        public void setStorageInfo(String issuerPubkeyB64, String issuerIP, String issuerPort) {
            setStoragePubkey(Base64.decode(issuerPubkeyB64, Base64.DEFAULT));
            setStorageIp(issuerIP);
            setStoragePort(Integer.parseInt(issuerPort));
        }

        public String getStoragePubkeyB64() {
            return TEOKeyStoreService.this.getStoragePubkeyB64();
        }

        public String getClientPubkeyB64() {
            return TEOKeyStoreService.this.getClientPubkeyB64();
        }

        public byte[] getUserPubkey() {
            return TEOKeyStoreService.this.getUserPubkey();
        }

        public byte[] getUserPrivkey() {
            return TEOKeyStoreService.this.getUserPrivkey();
        }

        public void setSieveKey(byte[] in) {
            sieveKey = in;
        }

        public void setSieveKeyNonce(byte[] in) {
            sieveKeyNonce = in;
        }

        public byte[] getSieveKey() {
            return sieveKey;
        }

        public byte[] getSieveKeyNonce() {
            return sieveKeyNonce;
        }

        public void reencrypt() {
            TEOKeyStoreService.this.reencrypt();
        }

        public void setDataUUID(String dataBlockUUID) {
            TEOKeyStoreService.this.dataUUID = dataBlockUUID;
        }

        public void setEncMetaUUID(String encMetaBlockUUID) {
            TEOKeyStoreService.this.encMetaUUID = encMetaBlockUUID;
        }

        public int releaseDevice() {
            return TEOKeyStoreService.this.releaseDevice();
        }

        public int removeRealTimeAccess() {
            return TEOKeyStoreService.this.removeRealTimeAccess();
        }

        public void sendProximityHeartbeat(byte[] nonce) {
            TEOKeyStoreService.this.sendProximityHeartbeat(nonce);
        }
    }

    private void sendProximityHeartbeat(byte[] proximityNonce) {
        if (claimedDevice == null) {
            return;
        }

        proximityHeartbeatJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, proximityNonce, userPubkey);

        Log.d(TAG, "Heartbeat beep, nonce: " + bytesToHex(proximityNonce));

    }

    private int releaseDevice() {
        if (claimedDevice != null) {
            releaseDeviceJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, userPubkey);

            claimedDevice = null;
        }
        return 0;
    }

    private int removeRealTimeAccess() {
        if (claimedDevice != null) {
            removeRealTimeAccessJNI(claimedDevice, claimedDeviceIp, claimedDevicePort, userPubkey);
        }
        return 0;
    }

    private void reencrypt() {
        reencryptJNI(userPubkey, userPrivkey, dataUUID, encMetaUUID, sieveKey, sieveKeyNonce, storageIp, storagePort);
    }

    public String getAdminIp() {
        return adminIp;
    }

    private void resolveAdminIpPort() {
        if (getStorageIp() == null || getStorageIp().equals("") || getStoragePort() == 0) {
            Toast.makeText(this, "You need to load Storage info first!!", Toast.LENGTH_SHORT).show();
            return;
        }

        setAdminIp(resolveIpJNI(getAdminPubkey(), getStorageIp(), getStoragePort()));
        setAdminPort(ADMIN_PORT);
    }

    private byte[] getAdminPubkey() {
        return adminPubkey;
    }

    private String getClientPubkeyB64() {
        if (userPrivkey == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(userPubkey, Base64.DEFAULT);
        }
    }

    private String getStoragePubkeyB64() {
        if (storagePubkey == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(storagePubkey, Base64.DEFAULT);
        }
    }

    private String getClaimedDeviceB64() {
        if (claimedDevice == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(claimedDevice, Base64.DEFAULT);
        }
    }

    private int claimDevice() {
        do {
            claimedDevice = claimDeviceJNI(userPubkey, userPrivkey, preAuthToken, deviceIp, devicePort, adminPubkey);
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

    private int acquirePreAuthToken() {
        do {
            preAuthToken = acquirePreAuthTokenJNI(userPubkey, userPrivkey, adminIp, adminPort, adminPubkey);
        } while (preAuthToken == null || byteArrayAllEmpty(preAuthToken));
        return 0;
    }

    private String getAdminPubkeyB64() {
        if (adminPubkey == null) {
            return "NULL";
        } else {
            return base64EncodeStrip(adminPubkey, Base64.DEFAULT);
        }
    }

    /**
     * Internal core functions.
     */
    byte[] userPubkey = null;
    byte[] userPrivkey = null;

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

    byte[] preAuthToken = null;

    byte[] claimedDevice = null;
    String claimedDeviceIp = null;
    int claimedDevicePort = 0;

    static int message_type_fltbuffers_size = 0;

    public byte[] getUserPubkey() {
        return userPubkey;
    }

    public byte[] getUserPrivkey() {
        return userPrivkey;
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

    public void setUserPubkey(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting user's public key to be (b64): " + base64EncodeStrip(in, Base64.DEFAULT) + ", length: " + in.length));
        userPubkey = in;
    }

    public void setUserPrivkey(byte[] in) {
        Log.d(TAG, stripLineBreak("Setting user's private key to be (b64): " + base64EncodeStrip(in, Base64.DEFAULT) + ", length: " + in.length));
        userPrivkey = in;
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

        registerIPKmsJNI(userPubkey, ip_addr, 0, storageIp, storagePort);
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

    private void flushKeyPair() {
        Log.d(TAG, "Regenerate user key pair");
        generateKeypairJNI();
        assert (userKeypairValid());
    }

    private void loadUserCredentials() {
        SharedPreferences sharedPref = getSharedPreferences();
        String userPubkeyB64 = sharedPref.getString(getString(R.string.user_pubkey_b64), defaultNull);
        String userPrivkeyB64 = sharedPref.getString(getString(R.string.user_privkey_b64), defaultNull);

        if (userPubkeyB64.equals(defaultNull) || userPrivkeyB64.equals(defaultNull)) {
            flushKeyPair();
        } else {
            setUserPubkey(Base64.decode(userPubkeyB64, Base64.DEFAULT));
            setUserPrivkey(Base64.decode(userPrivkeyB64, Base64.DEFAULT));
        }
    }

    private void storeUserCredentials() {
        if (userKeypairValid()) {
            SharedPreferences.Editor editor = getSharedPreferences().edit();
            editor.putString(getString(R.string.user_pubkey_b64), base64EncodeStrip(userPubkey, Base64.DEFAULT));
            editor.putString(getString(R.string.user_privkey_b64), base64EncodeStrip(userPrivkey, Base64.DEFAULT));
            editor.apply();
        }
    }


    private int initDevice() {
        if (!(deviceSecret != null && hasDeviceInfo())) {
            String err = "No device info! Scan QR";
            Log.e(TAG, err);
//            Toast.makeText(this, "err", Toast.LENGTH_SHORT).show();
            return -1;
        }

        int res = prepareInitializationRequestJNI(userPubkey, userPrivkey, deviceSecret, deviceIp, devicePort, devicePubkey);
        Log.d(TAG, "Return result is: " + res);

        if (res == 0) {
            // Successfully initialize devices
            // Launch admin services
            Intent intent = new Intent(this, TEOAdminService.class);
            startService(intent);
        }

        return res;
    }


    private boolean hasDeviceInfo() {
        return devicePubkey != null && deviceIp != null && !deviceIp.isEmpty();
    }


    private SharedPreferences getSharedPreferences() {
        Context context = getApplicationContext();
        return context.getSharedPreferences(
                getString(R.string.crypto_key_file_key), Context.MODE_PRIVATE);
    }

    private boolean userKeypairValid() {
        return userPubkey != null && userPrivkey != null;
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

    private native void reencryptJNI(byte[] userPubkey, byte[] userPrivkey,
                                     String uuid, String encMetaUUID,
                                     byte[] sieveKey, byte[] sieveKeyNonce,
                                     String storageIp, int storagePort);

    private native void releaseDeviceJNI(byte[] claimedDevice,
                                         String claimedDeviceIp,
                                         int claimedDevicePort,
                                         byte[] userPubkey);

    private native void removeRealTimeAccessJNI(byte[] claimedDevice,
                                                String claimedDeviceIp,
                                                int claimedDevicePort,
                                                byte[] userPubkey);


    private native void proximityHeartbeatJNI(byte[] claimedDevice,
                                              String claimedDeviceIp,
                                              int claimedDevicePort,
                                              byte[] proximityNonce,
                                              byte[] userPubkey);
}