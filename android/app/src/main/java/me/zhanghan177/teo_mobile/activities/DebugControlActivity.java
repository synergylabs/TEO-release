package me.zhanghan177.teo_mobile.activities;

import androidx.appcompat.app.AppCompatActivity;
import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TOTAdminService;
import me.zhanghan177.teo_mobile.TOTKeyStoreService;
import me.zhanghan177.teo_mobile.TOTServiceConnection;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class DebugControlActivity extends AppCompatActivity {
    TOTServiceConnection TOTConnection = new TOTServiceConnection();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug_control);
    }

    @Override
    protected void onStart() {
        super.onStart();

        Intent intent = new Intent(this, TOTKeyStoreService.class);
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
            TOTConnection.getTOTBinder().flushKeyPair();
        }
    }

    public void btnDisplayDeviceSecretOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getDeviceSecretB64();
        }
        Toast.makeText(this, "Debug secret: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowAdminPubkeyOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getAdminPubkeyB64();
        }
        Toast.makeText(this, "Admin Pubkey: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowPreAuthTokenOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getPreAuthTokenB64();
        }
        Toast.makeText(this, "User Pre Auth Token: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowClaimedDeviceOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getClaimedDeviceB64();
        }
        Toast.makeText(this, "User Claimed Device: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowStorageInfoOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getStoragePubkeyB64();
        }
        Toast.makeText(this, "Storage Info: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnShowKeypairOnClick(View view) {
        String toastText = "TOT service not connected!";
        if (TOTConnection.ismBound()) {
            toastText = TOTConnection.getTOTBinder().getClientPubkeyB64();
        }
        Toast.makeText(this, "User Keypair Pubkey: " + toastText, Toast.LENGTH_SHORT).show();
    }

    public void btnSendAdminNotificationOnClick(View view) {
        Intent intent = new Intent(this, TOTAdminService.class);
        intent.putExtra(TOTAdminService.EXTRA_TYPE, TOTAdminService.SEND_NOTIFICATION);
        startService(intent);
    }
}