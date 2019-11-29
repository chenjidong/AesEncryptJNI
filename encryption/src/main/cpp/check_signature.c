//
// Created by chenjidong on 2019/7/10.
//

#include <string.h>
#include <jni.h>
#include "check_signature.h"


jint check_signature(JNIEnv *env, jobject context) {
    //Context的类
    jclass context_clazz = (*env)->GetObjectClass(env, context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_packageName = (*env)->GetMethodID(env, context_clazz, "getPackageName",
                                                         "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring application_package = (*env)->CallObjectMethod(env, context, methodID_packageName);

    // 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env, context_clazz,
                                                               "getPackageManager",
                                                               "()Landroid/content/pm/PackageManager;");
    // 获得PackageManager对象
    jobject packageManager = (*env)->CallObjectMethod(env, context,
                                                      methodID_getPackageManager);
    // 获得 PackageManager 类
    jclass pm_clazz = (*env)->GetObjectClass(env, packageManager);
    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = (*env)->GetMethodID(env, pm_clazz, "getPackageInfo",
                                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    // 获得PackageInfo
    jobject packageInfo = (*env)->CallObjectMethod(env, packageManager,
                                                   methodID_pm, application_package, 64);

    jclass packageInfo_clazz = (*env)->GetObjectClass(env, packageInfo);
    jfieldID fieldID_signatures = (*env)->GetFieldID(env, packageInfo_clazz,
                                                     "signatures",
                                                     "[Landroid/content/pm/Signature;");
    jobjectArray signature_arr = (jobjectArray) (*env)->GetObjectField(env,
                                                                       packageInfo,
                                                                       fieldID_signatures);
    //Signature数组中取出第一个元素
    jobject signature = (*env)->GetObjectArrayElement(env, signature_arr, 0);
    //读signature的hashcode
    jclass signature_clazz = (*env)->GetObjectClass(env, signature);
    jmethodID methodID_hashcode = (*env)->GetMethodID(env, signature_clazz,
                                                      "hashCode", "()I");
    jint hashCode = (*env)->CallIntMethod(env, signature, methodID_hashcode);

    return hasSignature(hashCode);
}

jint check_package(JNIEnv *env, jobject context) {
    //Context的类
    jclass context_clazz = (*env)->GetObjectClass(env, context);

    // 得到 getPackageName 方法的 ID
    jmethodID methodID_packageName = (*env)->GetMethodID(env, context_clazz, "getPackageName",
                                                         "()Ljava/lang/String;");
    // 获得当前应用的包名
    jstring application_package = (*env)->CallObjectMethod(env, context, methodID_packageName);
    const char *package_name = (*env)->GetStringUTFChars(env, application_package, 0);

    return hasPackages(package_name);
}

jint check_white_list(JNIEnv *env, jobject context) {

    jint package_result = check_package(env, context);
    if (package_result != 1)
        return package_result;
    jint signature_result = check_signature(env, context);
    if (signature_result != 1)
        return package_result;
    return 1;
}

char *getAesKey() {
    int n = 0;
    char s[23];//"NMTIzNDU2Nzg5MGFiY2RlZg";

    s[n++] = 'N';
    s[n++] = 'M';
    s[n++] = 'T';
    s[n++] = 'I';
    s[n++] = 'z';
    s[n++] = 'N';
    s[n++] = 'D';
    s[n++] = 'U';
    s[n++] = '2';
    s[n++] = 'N';
    s[n++] = 'z';
    s[n++] = 'g';
    s[n++] = '5';
    s[n++] = 'M';
    s[n++] = 'G';
    s[n++] = 'F';
    s[n++] = 'i';
    s[n++] = 'Y';
    s[n++] = '2';
    s[n++] = 'R';
    s[n++] = 'l';
    s[n++] = 'Z';
    s[n++] = 'g';
    char *encode_str = s + 1;
    return (char *) b64_decode(encode_str, strlen(encode_str));
}

int hasPackages(char *packageName) {
    for (int i = 0; i < sizeof(app_package_names); ++i) {
        if (strcmp(app_package_names[i], packageName) == 0) {
            return 1;
        }
    }
    return -1;
}

int hasSignature(int hashCode) {
    for (int i = 0; i < sizeof(app_signature_hash_code); ++i) {
        if (app_signature_hash_code[i] == hashCode)
            return 1;
    }
    return -2;
}

char *getSrKey(){
    char *key = "C980DAF2E1ABE8329B0335DD419F96E1";
    return key;
}