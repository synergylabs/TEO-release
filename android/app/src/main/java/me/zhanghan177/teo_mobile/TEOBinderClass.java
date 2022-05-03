package me.zhanghan177.teo_mobile;

import android.content.Context;
import android.os.Binder;
import android.util.Base64;
import android.util.Log;

import androidx.annotation.NonNull;

import me.zhanghan177.teo_mobile.TEOKeyStoreService;

public class TEOBinderClass extends Binder {
    private final TEOKeyStoreService teoKeyStoreService;
    final static String TAG = "TOT Service Binder";

    public TEOBinderClass(TEOKeyStoreService enclosingService) {
        teoKeyStoreService = enclosingService;
    }

    public void flushKeyPair() {
        teoKeyStoreService.flushKeyPair();
    }

    public String getDeviceSecretB64() {
        return teoKeyStoreService.getDeviceSecretB64();
    }

    public int setDeviceSecret(@NonNull String secretB64) {
        try {
            teoKeyStoreService.setDeviceSecret(Base64.decode(secretB64, Base64.DEFAULT));
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            Log.e(TAG, "Incorrect padding for Base64 string");
            return -1;
        }
        return 0;
    }

    public void setDeviceInfo(String devicePubkeyB64, String deviceIp, String devicePort) {
        teoKeyStoreService.setDevicePubkey(Base64.decode(devicePubkeyB64, Base64.DEFAULT));
        teoKeyStoreService.setDeviceIp(deviceIp);
        teoKeyStoreService.setDevicePort(Integer.parseInt(devicePort));
    }

    public int initDevice(Context context) {
        return teoKeyStoreService.initDevice(context);
    }

    public void setAdminInfo(String adminPubkeyB64, String adminIp, String adminPort) {
        teoKeyStoreService.setAdminPubkey(Base64.decode(adminPubkeyB64, Base64.DEFAULT));
        if (adminIp.equals("") || adminPort.equals("")) {
//                teoKeyStoreService.resolveAdminIpPort();
        } else {
            teoKeyStoreService.setAdminIp(adminIp);
            teoKeyStoreService.setAdminPort(Integer.parseInt(adminPort));
        }

//            Log.v(TAG, "Admin IP: " + getAdminIp());
    }

    public String getAdminPubkeyB64() {
        return teoKeyStoreService.getAdminPubkeyB64();
    }

//    public int acquirePreAuthToken() {
////            return teoKeyStoreService.acquirePreAuthToken();
//        return 0;
//    }

//    public String getPreAuthTokenB64() {
//            return teoKeyStoreService.getPreAuthTokenB64();
////        return "";
//    }

    public int claimDevice(Context context) {
        return teoKeyStoreService.claimDevice(context);
    }

    public String getClaimedDeviceB64() {
        return teoKeyStoreService.getClaimedDeviceB64();
    }

    public void setStorageInfo(String issuerPubkeyB64, String issuerIP, String issuerPort) {
        teoKeyStoreService.setStoragePubkey(Base64.decode(issuerPubkeyB64, Base64.DEFAULT));
        teoKeyStoreService.setStorageIp(issuerIP);
        teoKeyStoreService.setStoragePort(Integer.parseInt(issuerPort));
    }

    public String getStoragePubkeyB64() {
        return teoKeyStoreService.getStoragePubkeyB64();
    }

    public String getClientPubkeyB64() {
        return teoKeyStoreService.getClientPubkeyB64();
    }

    public byte[] getClientPubkey() {
        return teoKeyStoreService.getClientPubkey();
    }

    public byte[] getClientPrivkey() {
        return teoKeyStoreService.getClientPrivkey();
    }

    public void setSieveKey(byte[] in) {
        teoKeyStoreService.setSieveKey(in);
    }

    public void setSieveKeyNonce(byte[] in) {
        teoKeyStoreService.setSieveKeyNonce(in);
    }

    public byte[] getSieveKey() {
        return teoKeyStoreService.getSieveKey();
    }

    public byte[] getSieveKeyNonce() {
        return teoKeyStoreService.getSieveKeyNonce();
    }

    public int reEncrypt(Context context) {
            return teoKeyStoreService.reEncrypt(context);
    }

    public void setMetadataUUID(byte[] uuid_in) {
        teoKeyStoreService.setMetadataUUID(uuid_in);
    }

    public byte[] getMetadataUUID() {
        return teoKeyStoreService.getMetadataUUID();
    }

    public void setSieveDataUUID(byte[] uuid_in) {
        teoKeyStoreService.setSieveDataUUID(uuid_in);
    }

    public byte[] getSieveDataUUID() {
        return teoKeyStoreService.getSieveDataUUID();
    }

    public int releaseDevice() {
//            return teoKeyStoreService.releaseDevice();
        return 0;
    }

    public int removeRealTimeAccess() {
//            return teoKeyStoreService.removeRealTimeAccess();
        return 0;
    }

    public void sendProximityHeartbeat(byte[] nonce) {
//            teoKeyStoreService.sendProximityHeartbeat(nonce);
    }

    public String getAdminManagedDevicePubkeyB64() {
        return teoKeyStoreService.getAdminManagedDevicePubkeyB64();
    }

    public byte[] getClaimedDevice() {
        return teoKeyStoreService.getClaimedDevice();
    }
}
