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
        String text = "{\"中文\":\"汉字\",\"key\":\"value\"}";
        Log.d("--->", "normal: " + text);
        String encode = EncryptJNI.encrypt(text);
        Log.d("--->", "encode: " + encode);
        String decode = EncryptJNI.decrypt(encode);
        Log.d("--->", "decode: " + decode);
        int hashCode = EncryptJNI.getSignature(this);
        Log.d("--->", "signature: " + hashCode);
        tv.setText("hashCode:" + hashCode);
        tv.setText("key:" + EncryptJNI.getSrKey());

        Log.d("--->", "md5: " + EncryptJNI.pwdMD5("123456"));

    }

}
