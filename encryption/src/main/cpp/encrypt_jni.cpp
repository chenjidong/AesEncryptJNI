//
// Created by chenjidong on 2019/7/10.
//

#include "check_signature.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    //配合 验证包名 可以防止二次打包 和非法调用
    if (checkWhiteList(env) != JNI_OK) {
        return JNI_ERR;//签名校验不通过直接 闪退
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_encode(JNIEnv *env, jclass type, jbyteArray jbArr) {
    return aesEcbPkcs7Encrypt(env, jbArr);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_cjd_encryption_EncryptJNI_decode(JNIEnv *env, jclass type, jstring out_str) {
    return aesEcbPkcs7Decrypt(env,out_str);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkPackage(JNIEnv *env, jclass type) {
    return checkPackage(env);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkWhileList(JNIEnv *env, jclass type) {
    return checkWhiteList(env);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_pwdMD5(JNIEnv *env, jclass type, jstring out_str) {
    return md5(env, out_str);//最后再加三个#
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_getSrKey(JNIEnv *env, jclass type) {

    return env->NewStringUTF(getSrKey());
}