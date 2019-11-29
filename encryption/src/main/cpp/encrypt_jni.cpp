//
// Created by chenjidong on 2019/7/10.
//
#include <jni.h>
#include <cstring>
#include <sstream>

extern "C" {
#include "check_signature.h"
#include "aes.h"
#include "md5.h"
}

/**
 * 获取 android Contxt
 * @param env
 * @return
 */
static const char *const className = "android/app/ActivityThread";

static jobject getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = env->FindClass(className);
    if (activity_thread_clz != NULL) {
        const char *name = "currentApplication";
        jmethodID currentApplication = env->GetStaticMethodID(
                activity_thread_clz, name, "()Landroid/app/Application;");
        if (currentApplication != NULL) {
            application = env->CallStaticObjectMethod(activity_thread_clz, currentApplication);
        }
    }
    return application;
}

/**
 * 校验签名
 * @param env
 * @return  1 成功
 */
static int checkSignature(JNIEnv *env) {
    return check_signature(env, getApplication(env));
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    //配合 验证包名 可以防止二次打包 和非法调用
    if (check_white_list(env,getApplication(env)) != 1) {
        return JNI_ERR;//签名校验不通过直接 闪退
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_encode(JNIEnv *env, jclass type, jbyteArray jbArr) {


    char *str = NULL;
    jsize alen = env->GetArrayLength(jbArr);
    jbyte *ba = env->GetByteArrayElements(jbArr, JNI_FALSE);
    str = (char *) malloc(alen + 1);
    memcpy(str, ba, alen);
    str[alen] = '\0';
    env->ReleaseByteArrayElements(jbArr, ba, 0);

    char *result = AES_ECB_PKCS7_Encrypt(str, AES_KEY);//AES ECB PKCS7Padding加密
//    char *result = AES_CBC_PKCS7_Encrypt(str, AES_KEY, AES_IV);//AES CBC PKCS7Padding加密
    return env->NewStringUTF(result);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_cjd_encryption_EncryptJNI_decode(JNIEnv *env, jclass type, jstring out_str) {
    const char *str = env->GetStringUTFChars(out_str, 0);
    char *result = AES_ECB_PKCS7_Decrypt(str, AES_KEY);//AES ECB PKCS7Padding解密
//    char *result = AES_CBC_PKCS7_Decrypt(str, AES_KEY, AES_IV);//AES CBC PKCS7Padding解密
    env->ReleaseStringUTFChars(out_str, str);

    jsize len = (jsize) strlen(result);
    jbyteArray jbArr = env->NewByteArray(len);
    env->SetByteArrayRegion(jbArr, 0, len, (jbyte *) result);
    return jbArr;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkPackage(JNIEnv *env, jclass type) {
    return check_package(env, getApplication(env));
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkWhileList(JNIEnv *env, jclass type) {
    return check_white_list(env, getApplication(env));
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_pwdMD5(JNIEnv *env, jclass type, jstring out_str) {

    const char *str = env->GetStringUTFChars(out_str, 0);
    string result = MD5(MD5(PWD_MD5_KEY + string(str)).toStr()).toStr();//加盐后进行两次md5
    env->ReleaseStringUTFChars(out_str, str);
    return env->NewStringUTF(("###" + result).data());//最后再加三个#
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_getSrKey(JNIEnv *env, jclass type) {

    return env->NewStringUTF(getSrKey());
}