package com.globallogic.gl_smart;

import android.app.Application;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

/**
 * @author eugenii.samarskyi.
 */
public class App extends Application {

	private static final String TAG = App.class.getSimpleName();

	private static App self;

	private SharedPreferences mPreferences;

	private Handler mHandler = new Handler();
	private Executor mExecutor = Executors.newSingleThreadExecutor();
	private Gson mGson = new GsonBuilder().serializeNulls().create();

	@Override
	public void onCreate() {
		super.onCreate();

		mPreferences = getSharedPreferences("settings", MODE_PRIVATE);

		self = this;
	}

	public static App self() {
		return self;
	}

	public static Handler getHandler() {
		return self.mHandler;
	}

	public static Executor getExecutor() {
		return self.mExecutor;
	}

	public static void runOnMainThread(Runnable runnable) {
		if (runnable == null) {
			return;
		}
		if (Looper.getMainLooper().getThread() == Thread.currentThread()) {
			runnable.run();
		} else {
			getHandler().post(runnable);
		}
	}

	public static SharedPreferences getPreferences() {
		return self.mPreferences;
	}

	public static Gson getGson() {
		return self.mGson;
	}
}
