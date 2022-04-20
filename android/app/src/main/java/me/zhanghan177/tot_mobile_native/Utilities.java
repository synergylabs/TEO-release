package me.zhanghan177.tot_mobile_native;

import android.content.Context;
import android.util.Base64;
import android.widget.Toast;

import java.nio.charset.StandardCharsets;

public class Utilities {
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
}
