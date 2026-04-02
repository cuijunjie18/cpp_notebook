#include "Demo.h"
#include <iostream>

JNIEXPORT jint JNICALL Java_Demo_add(JNIEnv *, jclass, jint a, jint b) {
    std::cout << "a: " << a << ", b: " << b << std::endl;
    return a + b;
}
