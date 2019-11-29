# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
# 设置CMake构造本地库所需要的最低版本
cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
# 创建并命名库，可以将库的类型设置为静态
# 或共享，并提供该库源代码的相对路径。
# 你可以定义多个library库，并使用CMake来构建。
# Gradle会自动打包库集成到apk中。

#设置生成的so动态库最后输出的路径 注意：（声明输出路径必须配置 jniLibs.srcDirs 到 build中 否则冲突）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

add_library( # Sets the name of the library.
        #库的名称
        encrypt-lib

        #库的分享类型
        # Sets the library as a shared library.
        SHARED

        # 库所在位置的相对路径
        # Provides a relative path to your source file(s).
        encrypt_jni.cpp
        aes.c
        aes.h
        base64.c
        base64.h
        check_signature.h
        check_signature.c
        md5.h
        md5.cpp
        )
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# NDK提供了一套实用的原生API和库，可以使用find_library搜索NDK中存在的库，只需要加入所需要使用库的名称即可，如下面的日志库log-lib。
# 更多的库可参考https://developer.android.com/ndk/guides/stable_apis
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
# 指定CMake连接到目标库中，可以链接多个库，如上面的encrypt-lib库以及NDK中自带的log-lib库

target_link_libraries( # Specifies the target library.
        encrypt-lib
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})