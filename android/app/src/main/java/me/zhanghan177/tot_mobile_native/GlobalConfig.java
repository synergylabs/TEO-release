package me.zhanghan177.tot_mobile_native;

public class GlobalConfig {
    public static final int PUBKEY_SIZE = 64;
    public static final int PRIVKEY_SIZE = 64;

    public static final String JSON_MESSAGE_ISSUER_TAG = "issuer";
    public static final String JSON_MESSAGE_ISSUER_DEVICE = "device";
    public static final String JSON_MESSAGE_ISSUER_ADMIN = "admin";
    public static final String JSON_MESSAGE_ISSUER_STORAGE = "storage";
    public static final String JSON_MESSAGE_ISSUER_DATA_BLOCK = "data-store";
    public static final String JSON_MESSAGE_TYPE_TAG = "type";
    public static final String JSON_MESSAGE_TYPE_ADMIN = "admin";
    public static final String JSON_MESSAGE_TYPE_USER = "user";
    public static final String JSON_DEVICE_SECRET_TAG = "DeviceSecret";
    public static final String JSON_DEVICE_ADMIN_TAG = "admin";
    public static final String JSON_ISSUER_PUBKEY_TAG = "Pubkey";
    public static final String JSON_ISSUER_IP_TAG = "IP";
    public static final String JSON_ISSUER_PORT_TAG = "Port";
    public static final String JSON_DATA_BLOCK_UUID_TAG = "dataUUID";
    public static final String JSON_ENC_META_BLOCK_UUID_TAG = "encMetaUUID";


    public static final int G_FBS_SIZE = 1024;
    public static final int G_DATA_BUF_SIZE = 4096;

    public static int AsymmetricEncryptionKeySet_FULL_PK_SIZE = 0;
    public static int G_CHALLENGE_SIZE = 0;

    public static final int ADMIN_PORT = 9002;
    public static final int USER_PORT = 9010;

    public static boolean ENABLE_BEACON_SCAN = false;
    public static boolean EVAL_MODE_SKIP_NOTIFICATION = true;
}
