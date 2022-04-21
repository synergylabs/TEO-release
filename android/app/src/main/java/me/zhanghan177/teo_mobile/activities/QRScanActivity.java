package me.zhanghan177.teo_mobile.activities;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.ImageProxy;
import androidx.camera.core.Preview;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.camera.view.PreviewView;
import androidx.lifecycle.ViewModelProvider;
import me.zhanghan177.teo_mobile.CameraXViewModel;
import me.zhanghan177.teo_mobile.R;
import me.zhanghan177.teo_mobile.TOTKeyStoreService;
import me.zhanghan177.teo_mobile.TOTServiceConnection;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.util.Size;
import android.widget.Toast;

import com.google.android.gms.common.internal.Preconditions;
import com.google.mlkit.vision.barcode.Barcode;
import com.google.mlkit.vision.barcode.BarcodeScanner;
import com.google.mlkit.vision.barcode.BarcodeScanning;
import com.google.mlkit.vision.common.InputImage;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Objects;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static me.zhanghan177.teo_mobile.GlobalConfig.*;

public class QRScanActivity extends AppCompatActivity {

    private static final String TAG = "QR Scan Activity";
    private CameraSelector cameraSelector;
    private PreviewView previewView;

    @Nullable
    private ProcessCameraProvider cameraProvider;
    @Nullable
    private Preview previewUseCase;
    @Nullable
    private ImageAnalysis analysisUseCase;

    private final int lensFacing = CameraSelector.LENS_FACING_BACK;

    TOTServiceConnection TOTConnection = new TOTServiceConnection();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qrscan);

        setupCamera();

        Intent intent = new Intent(this, TOTKeyStoreService.class);
        bindService(intent, TOTConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(TOTConnection);
        TOTConnection.setmBound(false);
    }

    private void setupCamera() {
        previewView = findViewById(R.id.preview_view);
        cameraSelector = new CameraSelector.Builder().requireLensFacing(CameraSelector.LENS_FACING_BACK).build();

        new ViewModelProvider(this, ViewModelProvider.AndroidViewModelFactory.getInstance(getApplication()))
                .get(CameraXViewModel.class)
                .getProcessCameraProvider()
                .observe(
                        this,
                        provider -> {
                            cameraProvider = provider;
                            bindCameraUseCases();

                        });
    }

    private void bindCameraUseCases() {
        if (cameraProvider != null) {
            // As required by CameraX API, unbinds all use cases before trying to re-bind any of them.
            cameraProvider.unbindAll();
            bindPreviewUseCase();
            bindAnalyseUseCase();
        }
    }

    private void bindPreviewUseCase() {
        if (cameraProvider == null) {
            return;
        }
        if (previewUseCase != null) {
            cameraProvider.unbind(previewUseCase);
        }

        Preview.Builder builder = new Preview.Builder();
        Size targetResolution = getCameraXTargetResolution(this, lensFacing);
        if (targetResolution != null) {
            builder.setTargetResolution(targetResolution);
        }
        previewUseCase = builder.build();
        previewUseCase.setSurfaceProvider(previewView.getSurfaceProvider());
        cameraProvider.bindToLifecycle(/* lifecycleOwner= */ this, cameraSelector, previewUseCase);
    }


    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    @Nullable
    private static android.util.Size getCameraXTargetResolution(Context context, int lensfacing) {
        Preconditions.checkArgument(
                lensfacing == CameraSelector.LENS_FACING_BACK
                        || lensfacing == CameraSelector.LENS_FACING_FRONT);
        String prefKey =
                lensfacing == CameraSelector.LENS_FACING_BACK
                        ? context.getString(R.string.pref_key_camerax_rear_camera_target_resolution)
                        : context.getString(R.string.pref_key_camerax_front_camera_target_resolution);
        SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);
        try {
            return android.util.Size.parseSize(sharedPreferences.getString(prefKey, null));
        } catch (Exception e) {
            return null;
        }
    }


    private void bindAnalyseUseCase() {
        BarcodeScanner barcodeScanner = BarcodeScanning.getClient();

        if (cameraProvider == null) {
            return;
        }
        if (analysisUseCase != null) {
            cameraProvider.unbind(analysisUseCase);
        }

        ImageAnalysis.Builder builder = new ImageAnalysis.Builder();
        Size targetResolution = getCameraXTargetResolution(this, lensFacing);
        if (targetResolution != null) {
            builder.setTargetResolution(targetResolution);
        }
        analysisUseCase = builder.build();

        Executor cameraExecutor = Executors.newSingleThreadExecutor();

        analysisUseCase.setAnalyzer(cameraExecutor, imageProxy -> {
            processImageProxy(barcodeScanner, imageProxy);
        });

        cameraProvider.bindToLifecycle(this, cameraSelector, analysisUseCase);
    }

    @SuppressLint("UnsafeOptInUsageError")
    private void processImageProxy(BarcodeScanner barcodeScanner, ImageProxy imageProxy) {
        InputImage inputImage = InputImage.fromMediaImage(Objects.requireNonNull(imageProxy.getImage()),
                imageProxy.getImageInfo().getRotationDegrees());

        barcodeScanner.process(inputImage)
                .addOnSuccessListener(barcodes -> {
                    for (Barcode barcode : barcodes) {
                        String rawValue = barcode.getRawValue();
                        Log.d(TAG, rawValue);

                        processTOTQRCode(rawValue);
                    }
                })
                .addOnFailureListener(e -> {
                    Log.e(TAG, e.getMessage());
                })
                .addOnCompleteListener(res -> {
                    imageProxy.close();
                })
        ;
    }

    private String getBase64FixSpecial(String input) {
        // Special weird bug: my browser translates "%2B" into "+" in the URL address.
        // As a result, the B64 encoding content has spaces " " instead of "+"
        return input.replace(" ", "+");
    }

    public void processTOTQRCode(String rawValue) {
        if (rawValue == null || !TOTConnection.ismBound()) {
            return;
        }

        try {
            JSONObject obj = new JSONObject(rawValue);

            String issuer = obj.getString(JSON_MESSAGE_ISSUER_TAG);

            if (issuer.equals(JSON_MESSAGE_ISSUER_DATA_BLOCK)) {
                String dataBlockUUID = obj.getString(JSON_DATA_BLOCK_UUID_TAG);
                String encMetaBlockUUID = obj.getString(JSON_ENC_META_BLOCK_UUID_TAG);
                TOTConnection.getTOTBinder().setDataUUID(dataBlockUUID);
                TOTConnection.getTOTBinder().setEncMetaUUID(encMetaBlockUUID);

                Toast.makeText(this, "Data blocks scan successful.", Toast.LENGTH_SHORT).show();
            } else {

                String type = obj.getString(JSON_MESSAGE_TYPE_TAG);

                String issuerPubkeyB64 = getBase64FixSpecial(obj.getString(JSON_ISSUER_PUBKEY_TAG));
                String issuerIP = obj.getString(JSON_ISSUER_IP_TAG);
                String issuerPort = obj.getString(JSON_ISSUER_PORT_TAG);

                if (issuer.equals(JSON_MESSAGE_ISSUER_DEVICE)) {
                    if (type.equals(JSON_MESSAGE_TYPE_ADMIN)) {
                        String secretB64 = getBase64FixSpecial(obj.getString(JSON_DEVICE_SECRET_TAG));
                        int err = TOTConnection.getTOTBinder().setDeviceSecret(secretB64);
                        if (err == 0) {
                            Toast.makeText(this,
                                    "Set Device Secret to be: "
                                            + TOTConnection.getTOTBinder().getDeviceSecretB64(),
                                    Toast.LENGTH_SHORT)
                                    .show();
                        }
                    } else if (type.equals(JSON_MESSAGE_TYPE_USER)) {
                        Log.d(TAG, "User QR code");

                        String deviceAdminB64 = getBase64FixSpecial(obj.getString(JSON_DEVICE_ADMIN_TAG));
                        TOTConnection.getTOTBinder().setAdminInfo(deviceAdminB64, "", "");

                        Toast.makeText(this, "Scan Device QR for User Success!", Toast.LENGTH_SHORT).show();
                    } else {
                        Log.e(TAG, "Unknown QR code message type!");
                        Toast.makeText(this, "Unknown QR code message type!", Toast.LENGTH_SHORT).show();
                    }

                    TOTConnection.getTOTBinder().setDeviceInfo(issuerPubkeyB64, issuerIP, issuerPort);
                } else if (issuer.equals(JSON_MESSAGE_ISSUER_ADMIN)) {
                    TOTConnection.getTOTBinder().setAdminInfo(issuerPubkeyB64, issuerIP, issuerPort);
                    Toast.makeText(this, "Successfully load Admin information!", Toast.LENGTH_SHORT).show();
                } else if (issuer.equals(JSON_MESSAGE_ISSUER_STORAGE)) {
                    boolean notify = true;
                    if (issuerPubkeyB64.equals(TOTConnection.getTOTBinder().getStoragePubkeyB64()))
                        notify = false;
                    TOTConnection.getTOTBinder().setStorageInfo(issuerPubkeyB64, issuerIP, issuerPort);
                    if (notify)
                        Toast.makeText(this, "Successfully load Storage information!", Toast.LENGTH_SHORT).show();
                } else {
                    Log.d(TAG, "Unknown issuer!");
                }
            }
        } catch (JSONException e) {
            e.printStackTrace();
            Log.e(TAG, "Failed JSON conversion");
        }
    }

}