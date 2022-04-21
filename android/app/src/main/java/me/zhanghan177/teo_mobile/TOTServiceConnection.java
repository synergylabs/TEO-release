package me.zhanghan177.teo_mobile;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

public class TOTServiceConnection implements ServiceConnection {
    private TOTKeyStoreService.TOTLocalBinder TOTBinder;
    private boolean mBound = false;

    @Override
    public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
        setTOTBinder((TOTKeyStoreService.TOTLocalBinder) iBinder);
        setmBound(true);
    }

    @Override
    public void onServiceDisconnected(ComponentName componentName) {
        setmBound(false);
    }

    public boolean ismBound() {
        return mBound;
    }

    public void setmBound(boolean mBound) {
        this.mBound = mBound;
    }

    public TOTKeyStoreService.TOTLocalBinder getTOTBinder() {
        return TOTBinder;
    }

    private void setTOTBinder(TOTKeyStoreService.TOTLocalBinder TOTBinder) {
        this.TOTBinder = TOTBinder;
    }
}
