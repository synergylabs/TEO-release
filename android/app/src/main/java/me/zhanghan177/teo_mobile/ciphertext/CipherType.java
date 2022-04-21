package me.zhanghan177.teo_mobile.ciphertext;

public enum CipherType {
    no_type(0),

    initialization_request(1),
    initialization_device_info(2),
    initialization_admin_reply(3),

    acquire_pre_auth_token(4),

    claim_device_request(5),
    clain_device_response(6),

    data_store_sieve_cred_request(7),
    data_store_sieve_cred_response(8),
    data_store_upload_notification(9),

    data_access_response(10),

    data_reencryption_request(11);

    private final byte value;
    public final int length = 1;

    CipherType(int val) {
        this.value = (byte) val;
    }

    public byte getValue() {
        return value;
    }
}
