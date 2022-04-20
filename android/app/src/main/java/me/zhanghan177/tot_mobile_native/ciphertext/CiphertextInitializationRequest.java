package me.zhanghan177.tot_mobile_native.ciphertext;

import static me.zhanghan177.tot_mobile_native.GlobalConfig.*;

public class CiphertextInitializationRequest implements Ciphertext {
    CipherType type = CipherType.no_type;
    final int type_idx = 0;
    byte[] admin_pubkey = new byte[AsymmetricEncryptionKeySet_FULL_PK_SIZE];
    final int admin_pubkey_idx = type_idx + type.length;
    byte[] admin_challenge = new byte[G_CHALLENGE_SIZE];
    final int admin_challenge_idx = admin_pubkey_idx + admin_pubkey.length;

    public CiphertextInitializationRequest(byte[] admin_pubkey_in, byte[] admin_challenge_in) {
        assert admin_pubkey.length == admin_pubkey_in.length && admin_challenge.length == admin_challenge_in.length;
        type = CipherType.initialization_request;
        admin_pubkey = admin_pubkey_in;
        admin_challenge = admin_challenge_in;
    }

    @Override
    public byte[] serialize() {
        byte[] res = new byte[type.length + admin_pubkey.length + admin_challenge.length];

        res[type_idx] = type.getValue();
        System.arraycopy(admin_pubkey, 0, res, admin_pubkey_idx, admin_pubkey.length);
        System.arraycopy(admin_challenge, 0, res, admin_challenge_idx, admin_challenge.length);

        return res;
    }
}
