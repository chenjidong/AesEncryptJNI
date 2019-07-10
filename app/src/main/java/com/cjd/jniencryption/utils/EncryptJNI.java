package com.cjd.jniencryption.utils;

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
     * 检查 打包签名是否 是正确的 防止被二次打包
     *
     * @param context
     * @return 1 : pass ， -1 or  -2 : error.
     */
    public static native int checkSignature(Object context);


    public static native int checkPackage(Object context);

    public static native int checkWhileList(Object context);

    /**
     * AES加密 AES128_ECB
     *
     * @param context
     * @param str
     * @return
     */
    public static native String encode(Object context, String str);


    /**
     * AES 解密 AES128_ECB
     *
     * @param context
     * @param str
     * @return UNSIGNATURE ： sign not pass .
     */
    public static native String decode(Object context, String str);
}
