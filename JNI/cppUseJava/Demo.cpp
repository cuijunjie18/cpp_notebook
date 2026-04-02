#include "Demo.h"
#include "jni.h"
#include <iostream>

// 静态方法，cls是类对象，obj是对象实例
JNIEXPORT void JNICALL Java_Demo_showInCpp(JNIEnv * env, jclass cls) {
    std::cout << "show in cpp" << std::endl;

    // 1. 获取静态整数字段的 ID
    // 参数：类对象，字段名，字段签名 (I 代表 int)
    jfieldID countFieldID = env->GetStaticFieldID(cls, "b", "I");
    if (countFieldID != nullptr) {
        // 获取静态整数值
        jint count = env->GetStaticIntField(cls, countFieldID);
        std::cout << "b = " << count << std::endl;
    } else {
        std::cout << "未找到字段 b" << std::endl;
    }
}

JNIEXPORT void JNICALL Java_Demo_showInCpp2(JNIEnv * env, jobject obj) {
    std::cout << "show in cpp2" << std::endl;
    // 1. 从对象获取类 (jclass)
    jclass clazz = env->GetObjectClass(obj);
    if (clazz == nullptr) return;

    // 2. 读取实例字段 (实例字段用 GetFieldID，不是 GetStaticFieldID)
    jfieldID valueFieldID = env->GetFieldID(clazz, "a", "I");
    if (valueFieldID != nullptr) {
        // 读取实例字段值 (需要传入 obj)
        jint value = env->GetIntField(obj, valueFieldID);
        std::cout << "实例字段 a = " << value << std::endl;
    }

    // 3. 调用实例方法
    jmethodID methodID = env->GetMethodID(clazz, "show", "()V");
    if (methodID != nullptr) {
        env->CallVoidMethod(obj, methodID);
    }
}