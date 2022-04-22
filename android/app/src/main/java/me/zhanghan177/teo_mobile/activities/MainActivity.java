package me.zhanghan177.teo_mobile.activities;

import static me.zhanghan177.teo_mobile.Utilities.getClientName;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import me.zhanghan177.teo_mobile.R;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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
}