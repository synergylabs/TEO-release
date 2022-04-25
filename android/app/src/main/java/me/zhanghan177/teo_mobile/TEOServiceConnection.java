package me.zhanghan177.teo_mobile;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

import me.zhanghan177.teo_mobile.activities.TEOBinderClass;

public class TEOServiceConnection implements ServiceConnection {
    private TEOBinderClass TOTBinder;
    private boolean mBound = false;

    @Override
    public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
        setTOTBinder((TEOBinderClass) iBinder);
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

    public TEOBinderClass getTOTBinder() {
        return TOTBinder;
    }

    private void setTOTBinder(TEOBinderClass TOTBinder) {
        this.TOTBinder = TOTBinder;
    }
}
