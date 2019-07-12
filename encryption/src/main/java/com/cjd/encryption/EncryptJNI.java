package com.cjd.encryption;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

/**
 * @Author chenjidong
 * @email 374122600@qq.com
 * created 2019/7/10
 * description
 */
public class EncryptJNI {
    static {
        System.loadLibrary("encrypt-lib");
    }

    /**
     * 检查包名
     *
     * @return 1 成功
     */
    public static native int checkPackage();

    /**
     * 检查 包名和签名是否正确
     *
     * @return 1成功
     */
    public static native int checkWhileList();

    /**
     * AES加密 AES128_ECB
     *
     * @param bytes
     * @return
     */
    public static native String encode(byte[] bytes);

    /**
     * AES 解密 AES128_ECB
     *
     * @param str
     * @return UNSIGNATURE ： sign not pass .
     */
    public static native byte[] decode(String str);

    /**
     * md5 加密  2次
     *
     * @param str
     * @return
     */
    public static native String pwdMD5(String str);


    public static String encrypt(String str) {
        return encode(str.getBytes());
    }

    public static String decrypt(String str) {
        return new String(decode(str));
    }


    /**
     * 获取签名 hashCode
     */
    public static int getSignature(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);

            Signature sign = packageInfo.signatures[0];
            return sign.hashCode();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }

}
