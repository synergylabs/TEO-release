package me.zhanghan177.teo_mobile.activities;

import static me.zhanghan177.teo_mobile.GlobalConfig.SHARED_PREF_NAME;
import static me.zhanghan177.teo_mobile.Utilities.getClientName;
import static me.zhanghan177.teo_mobile.Utilities.saveClientName;

import androidx.appcompat.app.AppCompatActivity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;

import me.zhanghan177.teo_mobile.R;

public class ConfigActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_config);
    }

    @Override
    protected void onResume() {
        super.onResume();

        EditText editTextClientName = findViewById(R.id.editTextClientName);
        editTextClientName.setText(getClientName(this));
        editTextClientName.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                if (actionId == EditorInfo.IME_ACTION_SEARCH ||
                actionId == EditorInfo.IME_ACTION_DONE ||
                keyEvent == null ||
                keyEvent.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                    // User finished typing
                    String clientName = textView.getText().toString();
                    saveClientName(getApplicationContext(), clientName);
                    return true;
                }
                return false;
            }
        });
    }

}