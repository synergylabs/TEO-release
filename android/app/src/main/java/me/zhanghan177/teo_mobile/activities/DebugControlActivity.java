package me.zhanghan177.teo_mobile.activities;

import androidx.appcompat.app.AppCompatActivity;

import me.zhanghan177.teo_mobile.GlobalConfig;
import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOAdminService;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class DebugControlActivity extends AppCompatActivity {
    TEOServiceConnection TOTConnection = new TEOServiceConnection();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_control);
    }

    @Override
    protected void onStart() {
        super.onStart();

        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TOTConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();

        unbindService(TOTConnection);
        TOTConnection.setmBound(false);
    }

    public void btnFlushKeypairOnClick(View view) {
        if (TOTConnection.ismBound()) {
            TOTConnection.getTEOBinder().flushKeyPair();
        }
    }

    public void btnDisplayDeviceSecretOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getDeviceSecretB64();
        }
        Toast.makeText(this, "Debug secret: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowAdminPubkeyOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getAdminPubkeyB64();
        }
        Toast.makeText(this, "Admin Pubkey: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowPreAuthTokenOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getPreAuthTokenB64();
        }
        Toast.makeText(this, "User Pre Auth Token: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowClaimedDeviceOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getClaimedDeviceB64();
        }
        Toast.makeText(this, "User Claimed Device: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowStorageInfoOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getStoragePubkeyB64();
        }
        Toast.makeText(this, "Storage Info: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowKeypairOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTEOBinder().getClientPubkeyB64();
        }
        Toast.makeText(this, "User Keypair Pubkey: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnSendAdminNotificationOnClick(View view) {
        Intent intent = new Intent(this, TEOAdminService.class);
        intent.putExtra(GlobalConfig.INTENT_EXTRA_TYPE, GlobalConfig.INTENT_EXTRA_SEND_NOTIFICATION);
        startService(intent);
    }
}