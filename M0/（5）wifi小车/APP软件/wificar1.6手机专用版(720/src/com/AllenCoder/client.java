package com.AllenCoder;

/**
 * @ Allen 
 * 实现功能：udp发送数据
 */
import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.URL;
import java.util.ArrayList;
import java.util.Hashtable;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.gesture.Gesture;
import android.gesture.GestureLibraries;
import android.gesture.GestureLibrary;
import android.gesture.GestureOverlayView;
import android.gesture.Prediction;
import android.gesture.GestureOverlayView.OnGesturePerformedListener;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.MediaStore.Images;
import android.util.Log;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RatingBar;
import android.widget.RatingBar.OnRatingBarChangeListener;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.view.WindowManager;

import java.net.DatagramSocket;

import android.widget.*;

public class client extends Activity implements OnRatingBarChangeListener {
	private Context mContext = null;
	private ImageView imView = null;
	private Handler messageHandler = null;
	private Boolean isStop = true;
	private String conStr = "http://192.168.11.245:8000/?action=stream";
	private HttpRequest http = null;
	private String cmdPid = "";
	private Button btnSend;
	private ImageButton forward;
	private ImageButton backward;
	private ImageButton left;
	private ImageButton right;
	private ImageButton stop;
	private ToggleButton left_toggleButton = null;
	private ToggleButton right_toggleButton = null;
	ImageView image;
	// ////////////////////////////////
	private DatagramSocket socketUDP = null;
	private int portRemoteNum;
	private int portLocalNum;
	private String addressIP;
	private String revData;
	private boolean flag = false;
	private RatingBar mIndicatorRatingBar;
	private SensorManager sensorMgr;
	GestureOverlayView gestureOverlayView;
	// /////////////////////////////////
	private static final int MENU_ITEM_COUNTER = Menu.FIRST;
	GestureDetector myGestureDetector;

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0, MENU_ITEM_COUNTER, 0, "连接");
		menu.add(0, MENU_ITEM_COUNTER + 1, 0, "设置");
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case MENU_ITEM_COUNTER:
			Conn();
			break;
		case MENU_ITEM_COUNTER + 1:
			Setting();
			break;

		default:
			break;
		}
		return super.onOptionsItemSelected(item);
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// gestureOverlayView = (GestureOverlayView)
		// findViewById(R.id.gestureOverlayView1);
		RadioButton radioButton;
		CheckBox checkBox;
		// checkBox = (CheckBox) findViewById(R.id.checkBox1);
		((RatingBar) findViewById(R.id.ratingBar1))
				.setOnRatingBarChangeListener(this);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
//		imView = (ImageView) findViewById(R.id.imageView1);
		forward = (ImageButton) findViewById(R.id.forward);
		forward.setOnTouchListener(new forward_OnTouchListener());
		stop = (ImageButton) findViewById(R.id.stop);
		stop.setOnTouchListener(new stop_OnTouchListener());

		backward = (ImageButton) findViewById(R.id.backward);
		backward.setOnTouchListener(new backward_OnTouchListener());

		left = (ImageButton) findViewById(R.id.left);
		left.setOnTouchListener(new left_OnTouchListener());

		right = (ImageButton) findViewById(R.id.right);
		right.setOnTouchListener(new right_OnTouchListener());
		left_toggleButton = (ToggleButton) findViewById(R.id.left_toggleButton);
		right_toggleButton = (ToggleButton) findViewById(R.id.right_toggleButton);

		mContext = this;
		http = new HttpRequest();
		Looper looper = Looper.myLooper();
		messageHandler = new MessageHandler(looper);
		left_toggleButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if (left_toggleButton.isChecked()) {
					String str = "a";
					sendData(str);
					Log.i("info",  "a");
				} else {
					String str = "b";
					sendData(str);
					Log.i("info",  "b");
				}
			}
		});

		right_toggleButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if (right_toggleButton.isChecked()) {
					String str = "c";
					sendData(str);
					Log.i("info",  "c");
				} else {
					String str = "d";
					sendData(str);
					Log.i("info",  "d");
				}
			}
		});
		// class myGestureListener extends SimpleOnGestureListener {
		// @Override
		// public boolean onFling(MotionEvent e1, MotionEvent e2,
		// float velocityX, float velocityY) {
		// if (e1.getX() - e2.getX() > 80) {
		// Log.i("info", "左转");
		// String str = "3";
		// sendData(str);
		// } else if (e2.getX() - e1.getX() > 80) {
		// Log.i("info", "右转");
		// String str = "4";
		// sendData(str);
		// } else if (e1.getY() - e2.getY() > 80) {
		// Log.i("info", "上");
		// String str = "1";
		// sendData(str);
		// } else if (e2.getY() - e1.getY() > 80) {
		// Log.i("info", "下");
		// String str = "2";
		// sendData(str);
		// }
		// return super.onFling(e1, e2, velocityX, velocityY);
		// }
		// }
//		imView = (ImageView) findViewById(R.id.imageView1);

		// myGestureDetector = new GestureDetector(new myGestureListener());
		// imView.setOnTouchListener(new OnTouchListener() {
		// @Override
		// public boolean onTouch(View v, MotionEvent event) {
		// myGestureDetector.onTouchEvent(event);
		// return true;
		// // 手势监听器
		// // SimpleOnGestureListener
		// // 手势探测器
		// // GestureDetector
		//
		// // 1、检测出手势是向左滑动
		// // 2、检测出手势是向右滑动
		// }
		// });
		// final GestureLibrary library = GestureLibraries.fromRawResource(this,
		// R.raw.gestures);
		// library.load();// 。。。。。。
		// gestureOverlayView
		// .addOnGesturePerformedListener(new OnGesturePerformedListener() {
		//
		// @Override
		// public void onGesturePerformed(GestureOverlayView overlay,
		// Gesture gesture) {
		// // 读取出手势库中的内容
		// ArrayList<Prediction> mygesture = library
		// .recognize(gesture);
		// // 挨个遍历所有的手势
		// if (!mygesture.isEmpty()) {
		// Prediction prediction = mygesture.get(0);
		//
		// if (prediction.score >= 9.0) {
		// if (prediction.name.equals("up")) {
		// Log.i("info", "up");
		// String str = "1";
		// sendData(str);
		// } else if (prediction.name.equals("down")) {
		// Log.i("info", "left");
		// String str = "3";
		// sendData(str);
		// } else if (prediction.name.equals("left")) {
		// Log.i("info", "down");
		// String str = "3";
		// sendData(str);
		// } else if (prediction.name.equals("right")) {
		// Log.i("info", "right");
		// String str = "4";
		// sendData(str);
		// }
		// }
		// }else{
		// Log.i("info", "未识别");
		// }
		//
		// }
		// });
	}

	private OnCheckedChangeListener listener = new OnCheckedChangeListener() {

		private float x, y, z;

		public void onCheckedChanged(CompoundButton buttonView,
				boolean isChecked) {
			Log.i("info", "x=" + (int) x + "," + "y=" + (int) y + "," + "z="
					+ (int) z);
			Sensor sensor = sensorMgr
					.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			sensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);
			// cBox1被选中
			// if (buttonView.getId()==R.id.checkBox1){
			// if (isChecked){
			// SensorEventListener lsn = new SensorEventListener() {
			// public void onSensorChanged(SensorEvent e) {
			// x = e.values[SensorManager.DATA_X];
			// y = e.values[SensorManager.DATA_Y];
			// z = e.values[SensorManager.DATA_Z];
			// // setTitle("x="+(int)x+","+"y="+(int)y+","+"z="+(int)z);
			// Log.i("info", "x="+(int)x+","+"y="+(int)y+","+"z="+(int)z);
			//
			// }
			//
			// public void onAccuracyChanged(Sensor s, int accuracy) {
			// }
			// };
			// }
			// }
		}
	};

	// /////////////////////////////////////////////////
	class forward_OnTouchListener implements OnTouchListener {
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
			{
				forward.setImageResource(R.drawable.forward2);
				String str = "1";
				Log.i("info", "1");
				sendData(str);
			}
			if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
			{
				forward.setImageResource(R.drawable.forward1);
				String str = "0";
				Log.i("info", "0");
				sendData(str);
			}
			// 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
			return true;
		}
	}

	// //////////////////////////////////
	class backward_OnTouchListener implements OnTouchListener {
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
			{
				backward.setImageResource(R.drawable.backward2);
				String str = "2";
				Log.i("info", "2");
				sendData(str);
			}
			if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
			{
				backward.setImageResource(R.drawable.backward1);
				String str = "0";
				Log.i("info", "0");
				sendData(str);
			}
			// 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
			return true;
		}
	}

	// //////////////////////////////////
	class left_OnTouchListener implements OnTouchListener {
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
			{
				left.setImageResource(R.drawable.left2);
				String str = "3";
				Log.i("info", "3");
				sendData(str);
			}
			if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
			{
				left.setImageResource(R.drawable.left1);
				String str = "0";
				Log.i("info", "0");
				sendData(str);
			}
			// 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
			return true;
		}
	}

	// //////////////////////////////////
	class right_OnTouchListener implements OnTouchListener {
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
			{
				right.setImageResource(R.drawable.right2);
				String str = "4";
				Log.i("info", "4");
				sendData(str);
			}
			if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
			{
				right.setImageResource(R.drawable.right1);
				String str = "0";
				Log.i("info", "0");

				sendData(str);
			}
			// 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
			return true;
		}
	}

	class stop_OnTouchListener implements OnTouchListener {
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
			{
				stop.setImageResource(R.drawable.stop2);
				String str = "0";
				Log.i("info", "stop");
				sendData(str);
			}
			if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
			{
				stop.setImageResource(R.drawable.stop);
				String str = "0";
				Log.i("info", "stop");
				sendData(str);
			}
			// 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
			return true;
		}
	}

	// // //////////////////////////////////
	// class yforward_OnTouchListener implements OnTouchListener {
	// @Override
	// public boolean onTouch(View v, MotionEvent event) {
	// if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
	// {
	// yforward.setImageResource(R.drawable.forward2);
	// String str = "5";
	// sendData(str);
	// }
	// if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
	// {
	// yforward.setImageResource(R.drawable.forward1);
	// String str = "0";
	// sendData(str);
	// }
	// // 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
	// return true;
	// }
	// }
	//
	// // //////////////////////////////////
	// class ybackward_OnTouchListener implements OnTouchListener {
	// @Override
	// public boolean onTouch(View v, MotionEvent event) {
	// if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
	// {
	// ybackward.setImageResource(R.drawable.backward2);
	// String str = "6";
	// sendData(str);
	// }
	// if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
	// {
	// ybackward.setImageResource(R.drawable.backward1);
	// String str = "0";
	// sendData(str);
	// }
	// // 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
	// return true;
	// }
	// }
	//
	// // //////////////////////////////////
	// class yleft_OnTouchListener implements OnTouchListener {
	// @Override
	// public boolean onTouch(View v, MotionEvent event) {
	// if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
	// {
	// yleft.setImageResource(R.drawable.left2);
	// String str = "7";
	// sendData(str);
	// }
	// if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
	// {
	// yleft.setImageResource(R.drawable.left1);
	// String str = "0";
	// sendData(str);
	// }
	// // 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
	// return true;
	// }
	// }
	//
	// // //////////////////////////////////
	// class yright_OnTouchListener implements OnTouchListener {
	// @Override
	// public boolean onTouch(View v, MotionEvent event) {
	// if (event.getAction() == MotionEvent.ACTION_DOWN)// 按下
	// {
	// yright.setImageResource(R.drawable.right2);
	// String str = "8";
	// sendData(str);
	// }
	// if (event.getAction() == MotionEvent.ACTION_UP)// 弹起
	// {
	// yright.setImageResource(R.drawable.right1);
	// String str = "0";
	// sendData(str);
	// }
	// // 返回true 表示事件处理完毕，会中断系统对该事件的处理。false 系统会同时处理对应的事件
	// return true;
	// }
	// }

	// //////////////////////////////////
	void sendData(String str) {
		try {
			flag = true;
			// portRemoteNum=1376;
			// portLocalNum=8080;
			portRemoteNum = 8000;
			portLocalNum = 5000;
			addressIP = "192.168.11.245";
			// addressIP = "192.168.1.103";
			socketUDP = new DatagramSocket(portLocalNum);
			// 绑定到特定端口号，其它不变
			btnSend.setEnabled(true);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {

			InetAddress serverAddress = InetAddress.getByName(addressIP);
			// byte data [] = str.getBytes();
			DatagramPacket packetS = new DatagramPacket(str.getBytes(),
					str.getBytes().length, serverAddress, portRemoteNum);
			// 从本地端口给指定IP的远程端口发数据包
			socketUDP.send(packetS);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		socketUDP.close();

	}

	// //////////////////////////////////
	class MessageHandler extends Handler {
		public MessageHandler(Looper looper) {
			super(looper);
		}

		public void handleMessage(Message msg) {
			switch (msg.arg1) {
			case 0:
				imView.setImageBitmap((Bitmap) msg.obj);
				break;
			default:
				break;
			}

		}
	}

	public void Conn() {
		if (isStop) {
			isStop = false;
			// conBtn.setText("Clo");
			setTitle("连接");
			new Thread() {
				@SuppressWarnings("unchecked")
				public void run() {
					try {
						URL url = new URL(conStr);

						Socket server = new Socket(url.getHost(), url.getPort());
						OutputStream os = server.getOutputStream();
						InputStream is = server.getInputStream();

						StringBuffer request = new StringBuffer();
						request.append("GET " + url.getFile() + " HTTP/1.0\r\n");
						request.append("Host: " + url.getHost() + "\r\n");
						request.append("\r\n");
						os.write(request.toString().getBytes(), 0,
								request.length());

						StreamSplit localStreamSplit = new StreamSplit(
								new DataInputStream(new BufferedInputStream(is)));
						Hashtable localHashtable = localStreamSplit
								.readHeaders();

						String str3 = (String) localHashtable
								.get("content-type");
						int n = str3.indexOf("boundary=");
						Object localObject2 = "--";
						if (n != -1) {
							localObject2 = str3.substring(n + 9);
							str3 = str3.substring(0, n);
							if (!((String) localObject2).startsWith("--"))
								localObject2 = "--" + (String) localObject2;
						}
						if (str3.startsWith("multipart/x-mixed-replace")) {
							localStreamSplit
									.skipToBoundary((String) localObject2);
						}
						Message message1 = Message.obtain();
						message1.arg1 = 1;
						messageHandler.sendMessage(message1);
						do {
							if (localObject2 != null) {
								localHashtable = localStreamSplit.readHeaders();
								if (localStreamSplit.isAtStreamEnd())
									break;
								str3 = (String) localHashtable
										.get("content-type");
								if (str3 == null)
									throw new Exception("No part content type");
							}
							if (str3.startsWith("multipart/x-mixed-replace")) {
								n = str3.indexOf("boundary=");
								localObject2 = str3.substring(n + 9);
								localStreamSplit
										.skipToBoundary((String) localObject2);
							} else {
								byte[] localObject3 = localStreamSplit
										.readToBoundary((String) localObject2);
								if (localObject3.length == 0)
									break;

								Message message = Message.obtain();
								message.arg1 = 0;
								message.obj = BitmapFactory.decodeByteArray(
										localObject3, 0, localObject3.length);
								messageHandler.sendMessage(message);
							}
							try {
								Thread.sleep(10L);
							} catch (InterruptedException localInterruptedException) {

							}
						} while (!isStop);
						server.close();
					} catch (Exception e) {
						e.printStackTrace();
						System.out.println("错误");
						// Toast.makeText(mContext, "无法连接上服务器!",
						// Toast.LENGTH_SHORT).show();
						Message message = Message.obtain();
						message.arg1 = 1;
						messageHandler.sendMessage(message);
					}
				}
			}.start();
		} else {
			isStop = true;
			// conBtn.setText("Con");
			setTitle("断开");
		}
	}

	public void Setting() {
		LayoutInflater factory = LayoutInflater.from(mContext);
		final View v1 = factory.inflate(R.layout.setting, null);
		AlertDialog.Builder dialog = new AlertDialog.Builder(mContext);
		dialog.setTitle("连接地址");
		dialog.setView(v1);
		EditText et = (EditText) v1.findViewById(R.id.connectionurl);
		et.setText(conStr);
		dialog.setPositiveButton("确定", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				EditText qet = (EditText) v1.findViewById(R.id.connectionurl);
				conStr = qet.getText().toString();
				Toast.makeText(mContext, "设置成功!", Toast.LENGTH_SHORT).show();
			}
		});
		dialog.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {

			}
		});
		dialog.show();
	}

	public void onRatingChanged(RatingBar ratingBar, float rating,
			boolean fromUser) {
		final int numStars = ratingBar.getNumStars();
		String s = Integer.toString(numStars);
		mIndicatorRatingBar = (RatingBar) findViewById(R.id.ratingBar1);
		final float number = mIndicatorRatingBar.getRating();

		// Log.i("info", s1);
		int selection = (int) rating;
		switch (selection) {
		case 1:
			String str = "5";
			Log.i("info", str);
			sendData(str);
			break;

		case 2:
			String str1 = "6";
			Log.i("info", str1);
			sendData(str1);
			break;
		case 3:
			String str2 = "7";
			Log.i("info", str2);
			sendData(str2);
			break;
		case 4:
			String str3 = "8";
			Log.i("info", str3);
			sendData(str3);
			break;
		case 5:

			String str4 = "9";
			Log.i("info", str4);
			sendData(str4);
			break;
		}
		// Log.i("info", s);
		// Since this rating bar is updated to reflect any of the other rating
		// bars, we should update it to the current values.
		if (mIndicatorRatingBar.getNumStars() != numStars) {
			mIndicatorRatingBar.setNumStars(numStars);
			// String s =Integer.toString(numStars);

		}

		if (mIndicatorRatingBar.getRating() != rating) {
			mIndicatorRatingBar.setRating(rating);

		}
		final float ratingBarStepSize = ratingBar.getStepSize();

		if (mIndicatorRatingBar.getStepSize() != ratingBarStepSize) {
			mIndicatorRatingBar.setStepSize(ratingBarStepSize);
			Log.i("info", "点击getStepSize");
		}
		// mIndicatorRatingBar.setRating(5);

	}

}