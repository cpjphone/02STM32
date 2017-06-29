    package com.AllenCoder;  
    import android.app.Activity;  
import android.os.Bundle;  
import android.util.Log;
import android.hardware.SensorManager;  
import android.hardware.Sensor;  
import android.hardware.SensorEventListener;  
import android.hardware.SensorEvent;  
      
      
   public class SensorTest extends Activity {  
       private SensorManager sensorMgr;  
       Sensor sensor = sensorMgr.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);  
       private float x, y, z;  
       protected void onCreate(Bundle savedInstanceState) {  
       super.onCreate(savedInstanceState);  
       sensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);  
           SensorEventListener lsn = new SensorEventListener() {  
               public void onSensorChanged(SensorEvent e) {  
                 x = e.values[SensorManager.DATA_X];     
                 y = e.values[SensorManager.DATA_Y];     
                 z = e.values[SensorManager.DATA_Z];  
               //  setTitle("x="+(int)x+","+"y="+(int)y+","+"z="+(int)z);  
             Log.i("info", "x="+(int)x+","+"y="+(int)y+","+"z="+(int)z);
               
               }  
                
               public void onAccuracyChanged(Sensor s, int accuracy) {  
               }  
           };  
          //注册listener，第三个参数是检测的精确度  
           sensorMgr.registerListener(lsn, sensor, SensorManager.SENSOR_DELAY_GAME);  
       }  
         
   }  

