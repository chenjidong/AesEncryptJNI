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
     * tips：生产环境移除
     *
     * @return 1 成功
     */
    public static native int checkPackageName(String packageName);

    /**
     * 检查签名是否合法
     * tips：生产环境移除
     *
     * @param hashCode
     * @return
     */
    public static native int checkSignature(int hashCode);

    /**
     * AES加密 AES128_ECB
     *
     * @param bytes
     * @return
     */
    public static native String encodeAes(byte[] bytes);

    /**
     * AES 解密 AES128_ECB
     *
     * @param str
     * @return UNSIGNATURE ： sign not pass .
     */
    public static native byte[] decodeAes(String str);

    /**
     * md5 加密
     *
     * @param str
     * @return
     */
    public static native String md5(String str);

    /**
     * 自定义获取key
     *
     * @return
     */
    public static native String getSrKey();


    public static String encryptAes(String str) {
        return encodeAes(str.getBytes());
    }

    public static String decryptAes(String str) {
        return new String(decodeAes(str));
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
