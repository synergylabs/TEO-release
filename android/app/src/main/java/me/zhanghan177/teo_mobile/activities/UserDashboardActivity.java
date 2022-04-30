package me.zhanghan177.teo_mobile.activities;

import static java.lang.Thread.sleep;

import static me.zhanghan177.teo_mobile.Utilities.displayDialog;
import static me.zhanghan177.teo_mobile.Utilities.uuidBytesToString;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

public class UserDashboardActivity extends AppCompatActivity {
    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();
    private ExecutorService executor;

    public static String BROADCAST_ACTION_UPDATE_USER_OWNED_DATA = "me.zhanghan177.teo.broadcast.UPDATE_USER_OWNED_DATA";

    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            updateOwnedDataDisplay();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_dashboard);

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

        IntentFilter filter = new IntentFilter();
        filter.addAction(BROADCAST_ACTION_UPDATE_USER_OWNED_DATA);
        registerReceiver(receiver, filter);

        updateOwnedDevicePubkeyDisplay();
        updateOwnedDataDisplay();
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(receiver);
    }

    private void updateOwnedDevicePubkeyDisplay() {
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
                TextView tvPubkey = findViewById(R.id.textViewUserOwnedDevicePubkey);
                tvPubkey.setText(TEOConnection.getTEOBinder().getClaimedDeviceB64());
            });
        });
    }

    private void changeOwnedDataVisibility(int vis) {
        int[] dataGroup = {R.id.spaceUserOwnedData,
                R.id.textViewUserOwnedDataPrompt,
                R.id.textViewUserOwnedDataUUID,
                R.id.viewUserOwnedDataDivider,
                R.id.spaceUserReEncrypt,
                R.id.buttonUserReEncrypt};
        for (int id : dataGroup) {
            View v = findViewById(id);
            v.setVisibility(vis);
        }
    }

    private void updateOwnedDataDisplay() {
        runOnUiThread(() -> {
            changeOwnedDataVisibility(View.GONE);
        });

        executor.execute(() -> {
            while (!TEOConnection.ismBound()) {
                try {
                    sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            runOnUiThread(() -> {
                String uuid_str = uuidBytesToString(TEOConnection.getTEOBinder().getMetadataUUID());
                if (!uuid_str.equals("NULL")) {
                    TextView textViewUUID = findViewById(R.id.textViewUserOwnedDataUUID);
                    textViewUUID.setText(uuid_str);

                    changeOwnedDataVisibility(View.VISIBLE);
                }
            });
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(TEOConnection);
        TEOConnection.setmBound(false);
    }

    public void btnQRScanOnClick(View view) {
        Intent intent = new Intent(this, QRScanActivity.class);
        startActivity(intent);
    }

    public void btnUserClaimDeviceOnClick(View view) {
        if (TEOConnection.ismBound()) {
            int err = TEOConnection.getTEOBinder().claimDevice(this);

            if (err == 0) {
                Toast.makeText(this, "Claim device success!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this,
                        "Error claiming device! Please try again.",
                        Toast.LENGTH_SHORT
                ).show();
            }
            updateOwnedDevicePubkeyDisplay();
        } else {
            displayDialog(this, "TEO backend service not yet connected!");
        }
    }

    public void btnReEncryptOnClick(View view) {
        if (TEOConnection.ismBound()) {
            int err = TEOConnection.getTEOBinder().reEncrypt(this);
            if (err == 0) {
                Toast.makeText(this, "Data re-encryption success!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this,
                        "Error re-encrypting the data block!",
                        Toast.LENGTH_SHORT
                ).show();
            }
        } else {
            displayDialog(this, "TEO backend service not yet connected!");
        }
    }
}