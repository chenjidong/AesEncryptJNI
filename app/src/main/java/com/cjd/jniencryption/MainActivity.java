package com.cjd.jniencryption;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.cjd.encryption.EncryptJNI;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    Spinner spinner;
    Button btnEncrypt, btnDecrypt, btnPackage;
    EditText etEncrypt;
    int type;
    TextView tvResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        spinner = findViewById(R.id.spinner);
        btnDecrypt = findViewById(R.id.btn_decrypt);
        btnEncrypt = findViewById(R.id.btn_encrypt);
        etEncrypt = findViewById(R.id.et_aes);
        btnPackage = findViewById(R.id.btn_package);

        tvResult = findViewById(R.id.tv_result);

        btnEncrypt.setOnClickListener(this);
        btnDecrypt.setOnClickListener(this);
        btnPackage.setOnClickListener(this);


        // Example of a call to a native method

        String text = "{\"中文\":\"汉字\",\"key\":\"value\"}";
        Log.d("--->", "normal: " + text);
        String encode = EncryptJNI.encryptAes(text);
        Log.d("--->", "encode: " + encode);
        String decode = EncryptJNI.decryptAes(encode);
        Log.d("--->", "decode: " + decode);
        int hashCode = EncryptJNI.getSignature(this);
        Log.d("--->", "signature: " + hashCode);


        Log.d("--->", "md5: " + EncryptJNI.md5("123456"));

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                type = position;
                switch (position) {
                    case 0:
                        setEnabled(true, true, false);
                        break;
                    case 1:
                        setEnabled(false, true, false);
                        break;
                    case 2:
                        setEnabled(false, false, true);

                        break;
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private void setEnabled(boolean isDEnabled, boolean isEEnabled, boolean isCEnabled) {
        btnDecrypt.setEnabled(isDEnabled);
        btnEncrypt.setEnabled(isEEnabled);
        btnPackage.setEnabled(isCEnabled);
    }

    @Override
    public void onClick(View v) {
        String value = etEncrypt.getText().toString();
        StringBuilder builder = new StringBuilder("结果：");
        switch (v.getId()) {
            case R.id.btn_decrypt:
                if (type == 1) {
                    builder.append(EncryptJNI.decryptAes(value));
                }
                break;
            case R.id.btn_encrypt:
                if (type == 0) {
                    builder.append(EncryptJNI.encryptAes(value));
                } else if (type == 1) {
                    builder.append(EncryptJNI.md5(value));
                }
                break;
            case R.id.btn_package:
                int result = EncryptJNI.checkPackageName(value);
                if (result == 0) {
                    builder.append("包名通过");
                } else
                    builder.append("非法包名");
                break;

        }
        tvResult.setText(builder.toString());
    }
}
