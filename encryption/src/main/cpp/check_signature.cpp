//
// Created by chenjidong on 2019/12/5.
//

#include <ostream>
#include "check_signature.h"

jobject getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass class_activityThread = env->FindClass(ACTIVITY_THREAD_CLASS);
    if (class_activityThread != NULL) {
        const char *name = "currentApplication";

        jmethodID methodId_currentApplication = env->GetStaticMethodID(
                class_activityThread,
                name,
                "()Landroid/app/Application;");
        if (methodId_currentApplication != NULL) {
            application = env->CallStaticObjectMethod(class_activityThread,
                                                      methodId_currentApplication);
        } else {
            LOGE("Cannot find method: currentApplication() in ActivityThread.");
        }
        env->DeleteLocalRef(class_activityThread);
    } else {
        LOGE("Cannot find class: android.app.ActivityThread");
    }

    return application;
}


jint checkSignature(JNIEnv *env) {
    jobject context = getApplication(env);
    if (context == NULL)
        return JNI_ERR;
    LOGI("校验 hashCode");
    //Context的类
    jclass class_context = env->GetObjectClass(context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_getPackageName = env->GetMethodID(class_context, "getPackageName",
                                                         "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring string_packageName = (jstring) env->CallObjectMethod(context, methodID_getPackageName);

    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = env->GetMethodID(class_context,
                                                            "getPackageManager",
                                                            "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject object_packageManager = env->CallObjectMethod(context,
                                                          methodID_getPackageManager);
    // 获得 PackageManager 类
    jclass class_packageManager = env->GetObjectClass(object_packageManager);
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_getPackageInfo = env->GetMethodID(class_packageManager,
                                                         "getPackageInfo",
                                                         "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    // 获得PackageInfo
    jobject object_packageInfo = env->CallObjectMethod(object_packageManager,
                                                       methodID_getPackageInfo,
                                                       string_packageName, 64);

    jclass class_packageInfo = env->GetObjectClass(object_packageInfo);

    jfieldID fieldID_signatures = env->GetFieldID(class_packageInfo,
                                                  "signatures",
                                                  "[Landroid/content/pm/Signature;");
    jobjectArray signature_arr = (jobjectArray) env->GetObjectField(
            object_packageInfo,
            fieldID_signatures);
    //Signature数组中取出第一个元素
    jobject signature = env->GetObjectArrayElement(signature_arr, 0);
    //读signature的hashcode
    jclass class_signature = env->GetObjectClass(signature);
    jmethodID methodID_hashcode = env->GetMethodID(class_signature,
                                                   "hashCode", "()I");
    jint hashCode = env->CallIntMethod(signature, methodID_hashcode);

    env->DeleteLocalRef(context);
    env->DeleteLocalRef(class_context);

    env->DeleteLocalRef(string_packageName);

    env->DeleteLocalRef(object_packageManager);
    env->DeleteLocalRef(class_packageManager);

    env->DeleteLocalRef(object_packageInfo);
    env->DeleteLocalRef(class_packageInfo);

    env->DeleteLocalRef(signature_arr);
    env->DeleteLocalRef(signature);
    env->DeleteLocalRef(class_signature);


    return hasSignature(hashCode);
}

jint checkPackage(JNIEnv *env) {
    jobject context = getApplication(env);
    if (context == NULL)
        return JNI_ERR;
    LOGI("校验 package");
    //Context的类
    jclass class_context = env->GetObjectClass(context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_getPackageName = env->GetMethodID(class_context, "getPackageName",
                                                         "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring string_packageName = (jstring) env->CallObjectMethod(context, methodID_getPackageName);
    const char *package_name = env->GetStringUTFChars(string_packageName, JNI_FALSE);

    //release
    env->DeleteLocalRef(context);
    env->DeleteLocalRef(class_context);

    env->DeleteLocalRef(string_packageName);

    return hasPackage(package_name);
}

jint checkWhiteList(JNIEnv *env) {
    if (checkPackage(env) == JNI_OK || checkSignature(env) == JNI_OK)
        return JNI_OK;
    return JNI_ERR;
}


jint hasPackage(const char *packageName) {
    int len = sizeof(APP_PACKAGE_NAME) / sizeof(APP_PACKAGE_NAME[0]);
    for (int i = 0; i < len; ++i) {
        if (strcmp(APP_PACKAGE_NAME[i], packageName) == 0) {
            return JNI_OK;
        }
    }
    return JNI_ERR;
}

jint hasSignature(int hashCode) {
    int len = sizeof(APP_SIGNATURE_HASH_CODE) / sizeof(APP_SIGNATURE_HASH_CODE[0]);
    for (int i = 0; i < len; ++i) {
        if (APP_SIGNATURE_HASH_CODE[i] == hashCode)
            return JNI_OK;
    }
    return JNI_ERR;
}

char *getSrKey() {
    char *key = "C980DAF2E1ABE8329B0335DD419F96E1";
    return key;
}

jstring md5(JNIEnv *env, jstring outStr) {

    const char *str = env->GetStringUTFChars(outStr, JNI_FALSE);
    string result = MD5(str).toStr();
    env->ReleaseStringUTFChars(outStr, str);
    jstring value = env->NewStringUTF(result.data());

    checkException(env);

    return value;
}

jstring aesEcbPkcs7Encrypt(JNIEnv *env, jbyteArray outArr) {
    jsize length = env->GetArrayLength(outArr);
    jbyte *byte = env->GetByteArrayElements(outArr, JNI_FALSE);
    char *str = (char *) malloc(length + 1);
    memcpy(str, byte, length);
    str[length] = '\0';
    env->ReleaseByteArrayElements(outArr, byte, 0);
    char *result = AES_ECB_PKCS7_Encrypt(str, AES_KEY);

    checkException(env);

    return env->NewStringUTF(result);
}

jbyteArray aesEcbPkcs7Decrypt(JNIEnv *env, jstring inStr) {
    const char *str = env->GetStringUTFChars(inStr, JNI_FALSE);

    char *result = AES_ECB_PKCS7_Decrypt(str, AES_KEY);

    env->ReleaseStringUTFChars(inStr, str);

    jsize len = strlen(result);
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, (jbyte *) result);

    checkException(env);

    return array;
}

jint checkException(JNIEnv *env) {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return JNI_ERR;
    }
    return JNI_OK;
}

jint checkPackageName(JNIEnv *env, jstring packageName) {
    const char *str = env->GetStringUTFChars(packageName, JNI_FALSE);

    jint result = hasPackage(str);

    env->ReleaseStringUTFChars(packageName, str);
    checkException(env);

    return result;
}

jint checkSignature(JNIEnv *env, jint hashCode) {
    int code = hashCode;
    jint result = hasSignature(code);

    checkException(env);

    return result;
}
