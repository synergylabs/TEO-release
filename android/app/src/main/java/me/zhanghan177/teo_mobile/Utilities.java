package me.zhanghan177.teo_mobile;

import static android.content.Context.MODE_PRIVATE;

import static me.zhanghan177.teo_mobile.GlobalConfig.SHARED_PREF_NAME;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Base64;
import android.widget.Toast;

public class Utilities {
    static String keyClientName = "clientName";

    public static String stripLineBreak(String in) {
        return in.replace("\n", "").replace("\r", "");
    }

    public static String base64EncodeStrip(byte[] in, int flags) {
        return stripLineBreak(Base64.encodeToString(in, flags));
    }

    public static void notImplementWarning(Context context) {
        Toast.makeText(context, "Not Yet Implemented!", Toast.LENGTH_SHORT).show();
    }

    private static final char[] HEX_ARRAY = "0123456789ABCDEF".toCharArray();

    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = HEX_ARRAY[v >>> 4];
            hexChars[j * 2 + 1] = HEX_ARRAY[v & 0x0F];
        }
        return new String(hexChars);
    }

    public static void saveClientName(Context context, String name) {
        // Creating an Editor object to edit(write to the file)
        SharedPreferences sp = context.getSharedPreferences(SHARED_PREF_NAME, MODE_PRIVATE);
        SharedPreferences.Editor myEdit = sp.edit();

        // Storing the key and its value as the data fetched from edittext
        myEdit.putString(keyClientName, name);

        // Once the changes have been made,
        // we need to commit to apply those changes made,
        // otherwise, it will throw an error
        myEdit.apply();
    }

    public static String getClientName(Context context) {
        SharedPreferences sp = context.getSharedPreferences(SHARED_PREF_NAME, MODE_PRIVATE);
        return sp.getString(keyClientName, context.getString(R.string.default_client_name));
    }
}
