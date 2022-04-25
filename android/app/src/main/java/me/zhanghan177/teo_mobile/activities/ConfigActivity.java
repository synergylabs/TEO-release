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
                tvPubkey.setText(TEOConnection.getTOTBinder().getClientPubkeyB64());
            });
        });
    }

    public void btnResetKeypairOnClick(View view) {
        if (TEOConnection.ismBound()) {
            TEOConnection.getTOTBinder().flushKeyPair();
            updateClientPubkeyDisplay();
        }
    }
}