//
// Created by chenjidong on 2019/7/10.
// 签名 或 包名 校验
//

#include <string.h>
#include "check_signature.h"

jobject getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass class_activityThread = (*env)->FindClass(env, ACTIVITY_THREAD_CLASS);
    if (class_activityThread != NULL) {
        const char *name = "currentApplication";

        jmethodID methodId_currentApplication = (*env)->GetStaticMethodID(env,
                                                                          class_activityThread,
                                                                          name,
                                                                          "()Landroid/app/Application;");
        if (methodId_currentApplication != NULL) {
            application = (*env)->CallStaticObjectMethod(env, class_activityThread,
                                                         methodId_currentApplication);
        } else {
            LOGE("Cannot find method: currentApplication() in ActivityThread.");
        }
        (*env)->DeleteLocalRef(env, class_activityThread);
    } else {
        LOGE("Cannot find class: android.app.ActivityThread");
    }

    return application;
}


jint check_signature(JNIEnv *env) {
    jobject context = getApplication(env);
    if (context == NULL)
        return JNI_ERR;
    LOGI("校验 hashCode");
    //Context的类
    jclass class_context = (*env)->GetObjectClass(env, context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_getPackageName = (*env)->GetMethodID(env, class_context, "getPackageName",
                                                            "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring string_packageName = (*env)->CallObjectMethod(env, context, methodID_getPackageName);

    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env, class_context,
                                                               "getPackageManager",
                                                               "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject object_packageManager = (*env)->CallObjectMethod(env, context,
                                                             methodID_getPackageManager);
    // 获得 PackageManager 类
    jclass class_packageManager = (*env)->GetObjectClass(env, object_packageManager);
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_getPackageInfo = (*env)->GetMethodID(env, class_packageManager,
                                                            "getPackageInfo",
                                                            "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    // 获得PackageInfo
    jobject object_packageInfo = (*env)->CallObjectMethod(env, object_packageManager,
                                                          methodID_getPackageInfo,
                                                          string_packageName, 64);

    jclass class_packageInfo = (*env)->GetObjectClass(env, object_packageInfo);

    jfieldID fieldID_signatures = (*env)->GetFieldID(env, class_packageInfo,
                                                     "signatures",
                                                     "[Landroid/content/pm/Signature;");
    jobjectArray signature_arr = (jobjectArray) (*env)->GetObjectField(env,
                                                                       object_packageInfo,
                                                                       fieldID_signatures);
    //Signature数组中取出第一个元素
    jobject signature = (*env)->GetObjectArrayElement(env, signature_arr, 0);
    //读signature的hashcode
    jclass class_signature = (*env)->GetObjectClass(env, signature);
    jmethodID methodID_hashcode = (*env)->GetMethodID(env, class_signature,
                                                      "hashCode", "()I");
    jint hashCode = (*env)->CallIntMethod(env, signature, methodID_hashcode);

    (*env)->DeleteLocalRef(env, context);
    (*env)->DeleteLocalRef(env, class_context);

    (*env)->DeleteLocalRef(env, string_packageName);

    (*env)->DeleteLocalRef(env, object_packageManager);
    (*env)->DeleteLocalRef(env, class_packageManager);

    (*env)->DeleteLocalRef(env, object_packageInfo);
    (*env)->DeleteLocalRef(env, class_packageInfo);

    (*env)->DeleteLocalRef(env, signature_arr);
    (*env)->DeleteLocalRef(env, signature);
    (*env)->DeleteLocalRef(env, class_signature);


    return hasSignature(hashCode);
}

jint check_package(JNIEnv *env) {
    jobject context = getApplication(env);
    if (context == NULL)
        return JNI_ERR;
    LOGI("校验 package");
    //Context的类
    jclass class_context = (*env)->GetObjectClass(env, context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_getPackageName = (*env)->GetMethodID(env, class_context, "getPackageName",
                                                            "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring string_packageName = (*env)->CallObjectMethod(env, context, methodID_getPackageName);
    const char *package_name = (*env)->GetStringUTFChars(env, string_packageName, 0);

    //release
    (*env)->DeleteLocalRef(env, context);
    (*env)->DeleteLocalRef(env, class_context);

    (*env)->DeleteLocalRef(env, string_packageName);

    return hasPackage(package_name);
}

jint check_white_list(JNIEnv *env) {
    if (check_package(env) == JNI_OK || check_signature(env) == JNI_OK)
        return JNI_OK;
    return JNI_ERR;
}


jint hasPackage(const char *packageName) {
    for (int i = 0; i < sizeof(APP_PACKAGE_NAME); ++i) {
        if (strcmp(APP_PACKAGE_NAME[i], packageName) == 0) {
            return JNI_OK;
        }
    }
    return JNI_ERR;
}

jint hasSignature(int hashCode) {
    for (int i = 0; i < sizeof(APP_SIGNATURE_HASH_CODE); ++i) {
        if (APP_SIGNATURE_HASH_CODE[i] == hashCode)
            return JNI_OK;
    }
    return JNI_ERR;
}

char *getSrKey() {
    char *key = "C980DAF2E1ABE8329B0335DD419F96E1";
    return key;
}