package me.zhanghan177.teo_mobile.activities;

import static me.zhanghan177.teo_mobile.Utilities.getClientName;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private final TEOServiceConnection TEOConnection = new TEOServiceConnection();


    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Request permissions
        // Check bluetooth permission
        if (checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 33);
        }

        // Check camera permission
        if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 34);
        }
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

    @SuppressLint("SetTextI18n")
    @Override
    protected void onResume() {
        super.onResume();

        TextView helloPrompt = findViewById(R.id.tvHelloPrompt);
        helloPrompt.setText("Hello " + getClientName(this));

        // Test native library integration
        TextView placeholder = findViewById(R.id.textViewPlaceholder);
        placeholder.setText(teoIntegrationTestStringJNI());
    }

    public void btnConfigOnClick(View view) {
        Intent intent = new Intent(this, ConfigActivity.class);
        startActivity(intent);
    }

    public void btnQRScanOnClick(View view) {
        Intent intent = new Intent(this, QRScanActivity.class);
        startActivity(intent);
    }

    /*
    JNI declaration
     */
    public native String teoIntegrationTestStringJNI();

    public void btnAdminDashboardOnClick(View view) {
        Intent intent = new Intent(this, AdminDashboardActivity.class);
        startActivity(intent);
    }

    public void btnUserDashboardOnClick(View view) {
        Intent intent = new Intent(this, UserDashboardActivity.class);
        startActivity(intent);
    }
}