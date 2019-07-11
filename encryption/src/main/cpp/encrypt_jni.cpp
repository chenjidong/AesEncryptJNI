//
// Created by chenjidong on 2019/7/10.
//
#include <jni.h>
#include <cstring>
#include <sstream>

extern "C" {
#include "check_signature.h"
#include "aes.h"
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkSignature(JNIEnv *env, jclass type,
                                                           jobject context) {

    return check_signature(env, context);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_encode(JNIEnv *env, jclass type, jobject context,
                                                   jstring str_) {
    uint8_t *AES_KEY = (uint8_t *) getAesKey();
    const char *in = env->GetStringUTFChars(str_, JNI_FALSE);
    char *baseResult = AES_128_ECB_PKCS5Padding_Encrypt(in, AES_KEY);
    env->ReleaseStringUTFChars(str_, in);

    jstring result = env->NewStringUTF(baseResult);
    free(baseResult);
    free(AES_KEY);
    return result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjd_encryption_EncryptJNI_decode(JNIEnv *env, jclass type, jobject context,
                                                   jstring str_) {
    uint8_t *AES_KEY = (uint8_t *) getAesKey();
    const char *str = env->GetStringUTFChars(str_, JNI_FALSE);
    char *desResult = AES_128_ECB_PKCS5Padding_Decrypt(str, AES_KEY);
    env->ReleaseStringUTFChars(str_, str);
    jstring result = env->NewStringUTF(desResult);
    free(desResult);
    free(AES_KEY);
    return result;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkPackage(JNIEnv *env, jclass type,
                                                         jobject context) {
    return check_package(env, context);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_cjd_encryption_EncryptJNI_checkWhileList(JNIEnv *env, jclass type,
                                                           jobject context) {
    return check_white_list(env, context);
}