//
// Created by chenjidong on 2019/7/10.
//
#include "android/log.h"
#include <string.h>
#include <jni.h>

#include "aes.h"
#include "md5.h"
#include <cstring>
#include <sstream>


#ifndef JNIENCRYPTION_CHECK_SIGNATURE_H
#define JNIENCRYPTION_CHECK_SIGNATURE_H


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "jniencryption", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "jniencryption", __VA_ARGS__))

//合法的APP包名
static const char *APP_PACKAGE_NAME[] = {"com.cjd.jniencryption", "com.cjd.gxrta"};
//合法的jks hashcode
static const int APP_SIGNATURE_HASH_CODE[] = {820593836, 1167693483};

static const uint8_t AES_KEY[] = "abcdef0123456789";
static const uint8_t AES_IV[] = "KXTUDEdBs9zGlvy7";
static const char *PWD_MD5_KEY = "4J9lKuR2c8OuDPBAniEy5USFQdSM0An4";
static const char *ACTIVITY_THREAD_CLASS = "android/app/ActivityThread";

/**
 * 获取当前 application
 * @param env
 * @return
 */
jobject getApplication(JNIEnv *env);


/**
 * 校验APP 签名是否合法
 * @param env
 * @return JNI_OK
 */
jint checkSignature(JNIEnv *env);

/**
 * 检查包名是否合法
 * @param env
 * @return  JNI_OK
 */
jint checkPackage(JNIEnv *env);

/**
 * 检查白名单
 * @param env
 * @param context 上下文
 * @return JNI_OK
 */
jint checkWhiteList(JNIEnv *env);

/**
 * 判断是否匹配内置 包名
 * @param packageName
 * @return JNI_OK
 */
jint hasPackage(const char *packageName);

/**
 * 判断是否匹配内置 签名
 * @param hashCode
 * @return JNI_OK
 */
jint hasSignature(int hashCode);

/**
 * 内置密钥
 * @return
 */
const char *getSrKey();

/**
 * md5 加密
 * @param env
 * @param outStr
 * @return
 */
jstring md5(JNIEnv *env, jstring outStr);

/**
 * aes 不定长加密，根据密钥自动选择长度
 * @param env
 * @param outArr
 * @return
 */
jstring aesEcbPkcs7Encrypt(JNIEnv *env, jbyteArray outArr);

/**
 * aes 解密
 * @param env
 * @param inStr
 * @return
 */
jbyteArray aesEcbPkcs7Decrypt(JNIEnv *env, jstring inStr);

/**
 * 检查是否有 异常 避免闪退
 * @param env
 * @return  JNI_OK
 */
jint checkException(JNIEnv *env);

/**
 * 检查包名是否匹配
 * @param env
 * @param packageName
 * @return
 */
jint checkPackageName(JNIEnv *env, jstring packageName);

/**
 * 检查签名hashCode 是否匹配
 * @param env
 * @param hashCode
 * @return
 */
jint checkSignature(JNIEnv *env, jint hashCode);


#endif //JNIENCRYPTION_CHECK_SIGNATURE_H
