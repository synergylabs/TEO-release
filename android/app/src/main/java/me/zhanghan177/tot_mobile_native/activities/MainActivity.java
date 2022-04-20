package me.zhanghan177.tot_mobile_native.activities;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import me.zhanghan177.tot_mobile_native.GlobalConfig;
import me.zhanghan177.tot_mobile_native.TOTBeaconScanService;
import me.zhanghan177.tot_mobile_native.TOTKeyStoreService;
import me.zhanghan177.tot_mobile_native.TOTServiceConnection;
import me.zhanghan177.tot_mobile_native.databinding.ActivityMainBinding;

import static me.zhanghan177.tot_mobile_native.NetworkUtils.getIPAddress;
import static me.zhanghan177.tot_mobile_native.Utilities.notImplementWarning;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private final TOTServiceConnection TOTConnection = new TOTServiceConnection();

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

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent startTOTKeyStore = new Intent(this, TOTKeyStoreService.class);
        startService(startTOTKeyStore);

        ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        /*
          JNI exploration
         */
        // Example of a call to a native method
        TextView tv = binding.sampleText;
        String ip = getIPAddress(true);
        tv.setText("My IP: " + ip);

        TextView userGreeting = binding.textView2;
        String[] ipParts = ip.split("[.]");
        userGreeting.setText("User " + ipParts[ipParts.length - 1]);


        if (GlobalConfig.ENABLE_BEACON_SCAN) {
            Intent startTOTBeaconScan = new Intent(this, TOTBeaconScanService.class);
            startService(startTOTBeaconScan);
        }

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        Intent stopTOTKeyStore = new Intent(this, TOTKeyStoreService.class);
        stopService(stopTOTKeyStore);

    }

    /*
     * Java Native Interface
     */
    public native String stringFromJNI();

    public native int integerFromJNI();

    public native Integer integerObjectFromJNI();

    public void btnQrScanOnClick(View view) {
        Intent intent = new Intent(this, QRScanActivity.class);
        startActivity(intent);
    }

    public void btnDebugOnClick(View view) {
        Intent intent = new Intent(this, DebugControlActivity.class);
        startActivity(intent);
    }

    public void btnAdminInitDeviceOnClick(View view) {
        if (TOTConnection.ismBound()) {
            int err = TOTConnection.getTOTBinder().initDevice();
            if (err == 0) {
                Toast.makeText(this, "Success init device", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void btnAcquirePreAuthOnClick(View view) {
        if (TOTConnection.ismBound()) {
            int err = TOTConnection.getTOTBinder().acquirePreAuthToken();
            if (err == 0) {
                Toast.makeText(this, "Acquire Pre Auth Token Success!", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void btnClaimDeviceOnClick(View view) {
        if (TOTConnection.ismBound()) {
            int err = TOTConnection.getTOTBinder().claimDevice();

            if (err == 0) {
                Toast.makeText(this, "Claimed device success!", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void btnReencryptOnClick(View view) {
        // TODO
        notImplementWarning(this);
    }

    public void btnEvalOnClick(View view) {
        Intent intent = new Intent(this, EvaluationActivity.class);
        startActivity(intent);
    }

    public void btnReleaseDeviceOnClick(View view) {
        if (TOTConnection.ismBound()) {
            int err = TOTConnection.getTOTBinder().releaseDevice();

            if (err == 0) {
                Toast.makeText(this, "Release device success!", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void btnRemoveRealTimeAccessOnClick(View view) {
        if (TOTConnection.ismBound()) {
            int err = TOTConnection.getTOTBinder().removeRealTimeAccess();

            if (err == 0) {
                Toast.makeText(this, "Remove access success!", Toast.LENGTH_SHORT).show();
            }
        }
    }
}