package me.zhanghan177.teo_mobile.activities;

import static java.lang.Thread.sleep;
import static me.zhanghan177.teo_mobile.Utilities.getClientName;
import static me.zhanghan177.teo_mobile.Utilities.saveClientName;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

public class ConfigActivity extends AppCompatActivity {

    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();
    private ExecutorService executor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_config);

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
    protected void onStop() {
        super.onStop();

        unbindService(TEOConnection);
        TEOConnection.setmBound(false);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // Add listener for changing client name
        EditText editTextClientName = findViewById(R.id.editTextClientName);
        editTextClientName.setText(getClientName(this));
        editTextClientName.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                // User finished typing
                String clientName = editable.toString();
                saveClientName(getApplicationContext(), clientName);
            }
        });

        updateClientPubkeyDisplay();
        updateBLEBeaconDisplay();

        RadioGroup radioGroupBLEBeacon = findViewById(R.id.radioGroupBLEBeacon);
        radioGroupBLEBeacon.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int checkedId) {
                boolean newValue = (checkedId == R.id.radioBeaconEnabled);

                executor.execute(() -> {
                    while (!TEOConnection.ismBound()) {
                        try {
                            sleep(100);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                    TEOConnection.getTEOBinder().setBLEBeaconEnabled(newValue);
                });
            }
        });
    }

    private void updateClientPubkeyDisplay() {
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
                TextView tvPubkey = findViewById(R.id.textViewDisplayClientPubkeyContent);
                tvPubkey.setText(TEOConnection.getTEOBinder().getClientPubkeyB64());
            });
        });
    }

    private void updateBLEBeaconDisplay() {
        executor.execute(() -> {
            while (!TEOConnection.ismBound()) {
                try {
                    sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            runOnUiThread(() -> {
                boolean enabled = TEOConnection.getTEOBinder().getBLEBeaconEnabled();
                int checkedButton = enabled ? R.id.radioBeaconEnabled : R.id.radioBeaconDisabled;
                RadioButton button = findViewById(checkedButton);
                button.setChecked(true);
            });
        });
    }

    public void btnResetKeypairOnClick(View view) {
        if (TEOConnection.ismBound()) {
            TEOConnection.getTEOBinder().flushKeyPair();
            updateClientPubkeyDisplay();
        }
    }
}