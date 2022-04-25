package me.zhanghan177.teo_mobile.activities;

import androidx.appcompat.app.AppCompatActivity;

import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TEOKeyStoreService;
import me.zhanghan177.teo_mobile.TEOServiceConnection;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.BatteryManager;
import android.os.Bundle;
import android.view.View;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Toast;

import java.io.UnsupportedEncodingException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class EvaluationActivity extends AppCompatActivity {

    private int eval_repetition;
    private String data_size;
    private static final int PROGRESS_REPORT_INTERVAL = 20;
    private static final int SLEEP_INTERVAL = 100;

    private static final int DEVICE_EVAL_PORT = 9010;

    private static final String TAG = "TOT Evaluation";
    private final TEOServiceConnection TOTConnection = new TEOServiceConnection();

//    private static final String SPREADSHEET_ID = BuildConfig.SPREADSHEET_ID;
//    private static final String SPREADSHEET_POST_URL = BuildConfig.SPREADSHEET_POST_URL;

    private RadioGroup.OnCheckedChangeListener _evalRepsGroupCheckedChangeListener = new RadioGroup.OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(RadioGroup radioGroup, int i) {
            RadioButton button = radioGroup.findViewById(i);
            eval_repetition = Integer.parseInt((String) button.getText());
        }
    };

    private RadioGroup.OnCheckedChangeListener _dataSizeGroupCheckedChangeListener = new RadioGroup.OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(RadioGroup radioGroup, int i) {
            RadioButton button = radioGroup.findViewById(i);
            data_size = (String) button.getText();
        }
    };


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_evaluation);


        RadioGroup _evalRepsRadioGroup = this.findViewById(R.id.radio_group_eval_reps);
        int defaultReps = _evalRepsRadioGroup.getCheckedRadioButtonId();
        _evalRepsRadioGroup.clearCheck();
        _evalRepsRadioGroup.setOnCheckedChangeListener(_evalRepsGroupCheckedChangeListener);
        _evalRepsRadioGroup.check(defaultReps);

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

    public void btnBatteryReadingOnClick(View view) {
        long chargeCounter = getBatteryLevel();

        Toast.makeText(this, "BATTERY_PROPERTY_CHARGE_COUNTER (microampere-hours) = " + chargeCounter, Toast.LENGTH_SHORT).show();
    }

    public long getBatteryLevel() {
        BatteryManager mBatteryManager = (BatteryManager) getSystemService(Context.BATTERY_SERVICE);
        Long chargeCounter = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            chargeCounter = mBatteryManager.getLongProperty(BatteryManager.BATTERY_PROPERTY_CHARGE_COUNTER);
        }

        return chargeCounter;
    }

    public class EvalBeaconMeasureTask extends EvalAsyncTask {
        @Override
        protected Long doInBackground(Void... voids) {
            // Keep a copy of the reps global variable.
            long BEACON_DURATION = 300_000; // measurement in milliseconds
            long batteryBefore = getBatteryLevel();

            ArrayList<String> stopwatch = new ArrayList<>();
            stopwatch.add("total");

            try {
                Thread.sleep(BEACON_DURATION);
                long batteryAfter = getBatteryLevel();
                postResult("Beacon wait for * " + String.valueOf(BEACON_DURATION), stopwatch, batteryBefore - batteryAfter, 0);

            } catch (InterruptedException | UnsupportedEncodingException interruptedException) {
                interruptedException.printStackTrace();
            }

            return 0L;
        }
    }

    public void btnBeaconMeasureOnClick(View view) {
        Toast.makeText(this, "Start beacon test", Toast.LENGTH_SHORT).show();

//        Intent startTOTBeaconScan = new Intent(this, TOTBeaconScanService.class);
//        startService(startTOTBeaconScan);

        new EvalBeaconMeasureTask().execute();
    }

    public class EvalReencryptTask extends EvalAsyncTask {
        @Override
        protected Long doInBackground(Void... voids) {
            // Keep a copy of the reps global variable.
            int reps = eval_repetition;

            if (TOTConnection.ismBound()) {
                long init = System.currentTimeMillis();
                long batteryBefore = getBatteryLevel();

                ArrayList<String> stopwatch = new ArrayList<>();
                stopwatch.add("total");

                for (int i = 0; i < reps; i++) {
                    long start = System.currentTimeMillis();
                    TOTConnection.getTOTBinder().reencrypt();
                    long end = System.currentTimeMillis();
                    stopwatch.add(String.valueOf(end - start));
                    progressUpdateHelper(i, reps);
                    try {
                        Thread.sleep(SLEEP_INTERVAL);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                try {
                    long batteryAfter = getBatteryLevel();
                    long finish = System.currentTimeMillis();

                    postResult("Re-encrypt * " + reps, stopwatch, batteryBefore - batteryAfter, finish - init);
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }

            return 0L;
        }
    }


    public void btnEvalReEncryptOnClick(View view) {
        Toast.makeText(this, "Start re-encryption test", Toast.LENGTH_SHORT).show();

        new EvalReencryptTask().execute();

    }


    public class EvalAsyncTask extends AsyncTask<Void, Long, Long> {
        @Override
        protected Long doInBackground(Void... voids) {
            throw new UnsupportedOperationException("Not yet implemented");
        }

        public void progressUpdateHelper(int i, int repeat) {
            if (i % PROGRESS_REPORT_INTERVAL == (PROGRESS_REPORT_INTERVAL - 1)) {
                publishProgress((long) i + 1, (long) repeat);
            }
        }

        @Override
        protected void onProgressUpdate(Long... values) {
            super.onProgressUpdate(values);

            Toast.makeText(EvaluationActivity.this, "Progress: " + values[0] + "/" + values[1], Toast.LENGTH_SHORT).show();
        }

        @Override
        protected void onPostExecute(Long elapse) {
            super.onPostExecute(elapse);

            Toast.makeText(EvaluationActivity.this, "Elapsed time: " + elapse, Toast.LENGTH_SHORT).show();
        }
    }

    private void postResult(String type, List<String> value, long batteryDrain, long timeElapsed) throws UnsupportedEncodingException {

        value.add("BatteryDrain");
        value.add(String.valueOf(batteryDrain));
        value.add("Total time elapsed");
        value.add(String.valueOf(timeElapsed));

        String timeStamp = new SimpleDateFormat("yyyy.MM.dd HH:mm:ss").format(new Date());

//        String scriptUri = Uri.parse(SPREADSHEET_POST_URL)
//                .buildUpon()
//                .appendQueryParameter("timestamp", timeStamp)
//                .appendQueryParameter("type", type + ", buildType: " + BuildConfig.BUILD_TYPE)
//                .appendQueryParameter("id", SPREADSHEET_ID)
//                .build().toString();

        StringBuilder payloadBuilder = new StringBuilder();
        for (String v : value) {
            payloadBuilder.append(v).append("\n");
        }
        final String payload = payloadBuilder.toString();

//        RequestQueue queue = Volley.newRequestQueue(this);
//        StringRequest stringRequest = new StringRequest(Request.Method.POST, scriptUri, new Response.Listener<String>() {
//            @Override
//            public void onResponse(String response) {
//                Log.i(TAG, response);
//            }
//        }, new Response.ErrorListener() {
//            @Override
//            public void onErrorResponse(VolleyError error) {
//                Log.e(TAG, error.toString());
//            }
//        }) {
//            @Override
//            public byte[] getBody() throws AuthFailureError {
//                return payload.getBytes(StandardCharsets.UTF_8);
//            }
//        };
//        queue.add(stringRequest);
    }


    public void btnQRScanOnClick(View view) {
        Intent intent = new Intent(this, QRScanActivity.class);
        startActivity(intent);
    }


    public class EvalInitAsyncTask extends EvalAsyncTask {
        @Override
        protected Long doInBackground(Void... voids) {
            // Keep a copy of the reps global variable.
            int reps = eval_repetition;
            if (TOTConnection.ismBound()) {
                long init = System.currentTimeMillis();
                long batteryBefore = getBatteryLevel();

                ArrayList<String> stopwatch = new ArrayList<>();
                stopwatch.add("total");

                for (int i = 0; i < reps; i++) {
                    long start = System.currentTimeMillis();
//                    TOTConnection.getTOTBinder().initDevice();
                    long end = System.currentTimeMillis();
                    stopwatch.add(String.valueOf(end - start));
                    progressUpdateHelper(i, reps);
                    try {
                        Thread.sleep(SLEEP_INTERVAL);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                try {
                    long batteryAfter = getBatteryLevel();
                    long finish = System.currentTimeMillis();

                    postResult("Device Initialization * " + reps, stopwatch, batteryBefore - batteryAfter, finish - init);
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }

            return 0L;
        }
    }


    public void btnEvalInitOnClick(View view) {
        Toast.makeText(this, "Start initialization test", Toast.LENGTH_SHORT).show();

        new EvalInitAsyncTask().execute();
    }


    public class EvalPreAuthClaimAsyncTask extends EvalAsyncTask {
        @Override
        protected Long doInBackground(Void... voids) {
            int reps = eval_repetition;
            if (TOTConnection.ismBound()) {
                long init = System.currentTimeMillis();
                long batteryBefore = getBatteryLevel();

                ArrayList<String> stopwatch = new ArrayList<>();
                stopwatch.add("total");

                for (int i = 0; i < reps; i++) {
                    long start = System.currentTimeMillis();
                    TOTConnection.getTOTBinder().acquirePreAuthToken();
                    TOTConnection.getTOTBinder().claimDevice();
                    long end = System.currentTimeMillis();
                    stopwatch.add(String.valueOf(end - start));
                    progressUpdateHelper(i, reps);
                    try {
                        Thread.sleep(SLEEP_INTERVAL);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                try {
                    long batteryAfter = getBatteryLevel();
                    long finish = System.currentTimeMillis();

                    postResult("Pre Auth + Claim * " + reps, stopwatch, batteryBefore - batteryAfter, finish - init);
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }
            return 0L;
        }
    }

    public void btnEvalPreAuthClaimOnClick(View view) {
        Toast.makeText(this, "Start pre auth claim test", Toast.LENGTH_SHORT).show();
        new EvalPreAuthClaimAsyncTask().execute();
    }


    public class EvalClaimAsyncTask extends EvalAsyncTask {
        @Override
        protected Long doInBackground(Void... voids) {
            int reps = eval_repetition;
            if (TOTConnection.ismBound()) {
                long init = System.currentTimeMillis();
                long batteryBefore = getBatteryLevel();

                TOTConnection.getTOTBinder().acquirePreAuthToken();

                ArrayList<String> stopwatch = new ArrayList<>();
                stopwatch.add("total");

                for (int i = 0; i < reps; i++) {
                    long start = System.currentTimeMillis();
                    TOTConnection.getTOTBinder().claimDevice();
                    long end = System.currentTimeMillis();
                    stopwatch.add(String.valueOf(end - start));
                    progressUpdateHelper(i, reps);
                    try {
                        Thread.sleep(SLEEP_INTERVAL);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                }
                try {
                    long batteryAfter = getBatteryLevel();
                    long finish = System.currentTimeMillis();

                    postResult("Device Claim * " + reps, stopwatch, batteryBefore - batteryAfter, finish - init);
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            }
            return 0L;
        }
    }

    public void btnEvalClaimOnClick(View view) {
        Toast.makeText(this, "Start claim test", Toast.LENGTH_SHORT).show();
        new EvalClaimAsyncTask().execute();
    }

}

