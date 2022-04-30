package me.zhanghan177.teo_mobile.activities;

import static java.lang.Thread.sleep;

import static me.zhanghan177.teo_mobile.Utilities.displayDialog;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import me.zhanghan177.teo_mobile.GlobalConfig;
import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOAdminService;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

public class AdminDashboardActivity extends AppCompatActivity {
    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();
    private ExecutorService executor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_admin_dashboard);

        executor = Executors.newFixedThreadPool(2);
    }

    @Override
    protected void onStart() {
        super.onStart();
        // Start background services
        Intent intent = new Intent(this, TEOKeyStoreService.class);
        bindService(intent, TEOConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        updateManagedDevicePubkeyDisplay();
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(TEOConnection);
        TEOConnection.setmBound(false);
    }

    private void updateManagedDevicePubkeyDisplay() {
        // Create background task to update client public key content on UI
        executor.execute(() -> {
            while (!TEOConnection.ismBound()) {
                try {
                    sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            runOnUiThread(() -> {
                TextView tvPubkey = findViewById(R.id.textViewAdminManagedDevicePubkey);
                tvPubkey.setText(TEOConnection.getTEOBinder().getAdminManagedDevicePubkeyB64());
            });
        });
    }

    public void btnAdminInitDeviceOnClick(View view) {
        if (TEOConnection.ismBound()) {
            int err = TEOConnection.getTEOBinder().initDevice(this);
            if (err == 0) {
                Toast.makeText(this, "Success init device", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this,
                        "Error initializing device! Please try again.",
                        Toast.LENGTH_SHORT
                ).show();
            }
            updateManagedDevicePubkeyDisplay();
        } else {
            displayDialog(this, "TEO backend service not yet connected!");
        }
    }

    public void btnQRScanOnClick(View view) {
        Intent intent = new Intent(this, QRScanActivity.class);
        startActivity(intent);
    }

    public void btnTestNotificationOnClick(View view) {
        Intent intent = new Intent(this, TEOAdminService.class);
        intent.putExtra(GlobalConfig.INTENT_EXTRA_TYPE, GlobalConfig.INTENT_EXTRA_SEND_NOTIFICATION);
        startService(intent);
    }
}