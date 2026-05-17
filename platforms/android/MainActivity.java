// ============================================================================
// Dionite — Android entry point (NativeActivity + JNI bridge to C++ core)
// ============================================================================
package com.dionite.shatteredwilds;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.WindowManager;

public class MainActivity extends NativeActivity {
    static { System.loadLibrary("dionite_core"); }

    public static native void onMoveInput(float x, float y);
    public static native void onAimInput(float x, float y);
    public static native void onFire(boolean down);
    public static native void onAbility(int idx, boolean down);
    public static native void onDash(boolean down);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
}
