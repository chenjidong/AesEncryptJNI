package com.cjd.jniencryption;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.cjd.encryption.EncryptJNI;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        int value = EncryptJNI.checkPackage(getApplicationContext());
        String text = "sakfjlskfjlaskdf";
        Log.d("--->", "onCreate: " + text);
        String encode = EncryptJNI.encode(getApplicationContext(), text);
        Log.d("--->", "onCreate: " + encode);
        String decode = EncryptJNI.decode(getApplicationContext(), encode);
        Log.d("--->", "onCreate: " + decode);

        tv.setText(value + "");
    }

}
