package me.zhanghan177.teo_mobile;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

public class TEOServiceConnection implements ServiceConnection {
    private TEOBinderClass TEOBinder;
    private boolean mBound = false;

    @Override
    public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
        setTEOBinder((TEOBinderClass) iBinder);
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

    public TEOBinderClass getTEOBinder() {
        return TEOBinder;
    }

    private void setTEOBinder(TEOBinderClass TEOBinder) {
        this.TEOBinder = TEOBinder;
    }
}
