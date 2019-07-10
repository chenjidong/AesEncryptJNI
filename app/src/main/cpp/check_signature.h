//
// Created by chenjidong on 2019/7/10.
//
#include "base64.h"

#define   LOG_TAG    "native_log"
#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#ifndef AESJNIENCRYPT_SIGNACTURECHECK_H
#define AESJNIENCRYPT_SIGNACTURECHECK_H


//合法的APP包名
static const char *app_package_names[] = {"com.cjd.jniencryption"};
//合法的hashcode -625644214:这个值是我生成的这个可以store文件的hash值
static const int app_signature_hash_code[] = {-625644214};

/**
 * 校验APP 签名是否合法
 * @param env
 * @param context app 上下文
 * @return 1 合法
 */
jint check_signature(JNIEnv *env, jobject context);

/**
 * 检查包名是否合法
 * @param env
 * @param context 上下文
 * @return  1合法
 */
jint check_package(JNIEnv *env, jobject context);

/**
 * 检查白名单
 * @param env
 * @param context 上下文
 * @return 1 校验通过 -1 包名不匹配 -2 签名错误
 */
jint check_white_list(JNIEnv *env, jobject context);

/**
 * 特别的获取aes key 加大 反编译获取难度
 * @return
 */
char *getAesKey();

int hasPackages(char *packageName);

int hasSignature(int hashCode);

#endif //AESJNIENCRYPT_SIGNACTURECHECK_H
