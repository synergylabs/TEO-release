package me.zhanghan177.tot_mobile_native;

import java.util.HashMap;
import java.util.Map;

/**
 * Bind service interfaces
 **/
public enum BindAction {
    UNKNOWN(0),
    FLUSH_KEYPAIR(1);

    private final int value;
    private static final Map map = new HashMap<>();

    BindAction(int value) {
        this.value = value;
    }

    static {
        for (BindAction bindAction : BindAction.values()) {
            assert (!map.containsKey(bindAction.value)) : "Duplicated bind action values!";
            map.put(bindAction.value, bindAction);
        }
    }

    public static BindAction valueOf(int ba) {
        return (BindAction) map.get(ba);
    }

    public int getValue() {
        return value;
    }
}