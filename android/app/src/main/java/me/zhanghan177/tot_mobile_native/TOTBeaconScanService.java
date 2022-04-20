package me.zhanghan177.tot_mobile_native;

import static me.zhanghan177.tot_mobile_native.Utilities.bytesToHex;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import org.altbeacon.beacon.Beacon;
import org.altbeacon.beacon.BeaconManager;
import org.altbeacon.beacon.BeaconParser;
import org.altbeacon.beacon.InternalBeaconConsumer;
import org.altbeacon.beacon.RangeNotifier;
import org.altbeacon.beacon.Region;
import org.altbeacon.beacon.utils.UrlBeaconUrlCompressor;

import java.util.Arrays;
import java.util.Collection;

public class TOTBeaconScanService extends Service implements RangeNotifier, InternalBeaconConsumer {

    private final TOTServiceConnection TOTConnection = new TOTServiceConnection();
    protected static final String TAG = "MonitoringActivity";
    private BeaconManager beaconManager;
    byte[] nonceLastSeen = null;

    Handler resetScannerHandler = new Handler();
    int resetScannerInterval = 120_000;
    private Runnable resetScannerRunnable = new Runnable() {
        @Override
        public void run() {
            Log.d(TAG, "repeat scan");
            restartScan();

            resetScannerHandler.postDelayed(this, resetScannerInterval);
        }
    };

    Handler heartbeatHandler = new Handler();
    int heartbeatInterval = 30_000;
    private Runnable heartbeatRunnable = new Runnable() {
        @Override
        public void run() {
            byte[] nonce = getProximityNonce();
            if (nonce != null) {
                TOTConnection.getTOTBinder().sendProximityHeartbeat(nonce);
            }

            heartbeatHandler.postDelayed(this, heartbeatInterval);
        }
    };

    public TOTBeaconScanService() {
    }

    @Override
    public void onBeaconServiceConnect() {
        Region region = new Region("all-beacons-region", null, null, null);
        beaconManager.startRangingBeacons(region);
        beaconManager.addRangeNotifier(this);
    }

    private void updateNonceLastSeen(byte[] nonceIn) {
        if (nonceIn == null || (nonceLastSeen != null && Arrays.equals(nonceLastSeen, nonceIn))) {
            return;
        }

        nonceLastSeen = nonceIn;
    }

    public byte[] getProximityNonce() {
        return nonceLastSeen;
    }

    @Override
    public void didRangeBeaconsInRegion(Collection<Beacon> beacons, Region region) {
        for (Beacon beacon : beacons) {
            if (beacon.getServiceUuid() == 0xfeaa && beacon.getBeaconTypeCode() == 0x10) {
                // This is a Eddystone-URL frame

                // only catch "TOT" devices
                byte[] content = beacon.getId1().toByteArray();
                if (content.length > 4
                        && content[0] == 'T'
                        && content[1] == 'O'
                        && content[2] == 'T'
                        && content[3] == 'B') {
                    String url = UrlBeaconUrlCompressor.uncompress(beacon.getId1().toByteArray());
                    Log.d(TAG, "Raw byte array:" + beacon.getId1().toHexString() + ", length: " + content.length);
                    Log.d(TAG, "I see a beacon transmitting a url: " + url +
                            " approximately " + beacon.getDistance() + " meters away.");

                    byte[] nonce = Arrays.copyOfRange(content, 4, content.length);
                    Log.d(TAG, "Extract nonce: " + bytesToHex(nonce));

                    updateNonceLastSeen(nonce);
                }
            }
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Intent intent = new Intent(this, TOTKeyStoreService.class);
        bindService(intent, TOTConnection, Context.BIND_AUTO_CREATE);

        startScan();

        resetScannerHandler.post(resetScannerRunnable);
        heartbeatHandler.post(heartbeatRunnable);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        stopScan();

        unbindService(TOTConnection);
        TOTConnection.setmBound(false);
    }

    private void startScan() {
        beaconManager = BeaconManager.getInstanceForApplication(this);
        beaconManager.getBeaconParsers().add(new BeaconParser().
                setBeaconLayout(BeaconParser.EDDYSTONE_URL_LAYOUT));
        beaconManager.bindInternal(this);

        beaconManager.setForegroundBetweenScanPeriod(10_000);

        Log.d(TAG, "foreground scan period: " + beaconManager.getForegroundScanPeriod());
        Log.d(TAG, "foreground between scan period: " + beaconManager.getForegroundBetweenScanPeriod());
    }

    private void stopScan() {
        beaconManager.unbindInternal(this);
    }

    private void restartScan() {
        stopScan();
        startScan();
    }
}