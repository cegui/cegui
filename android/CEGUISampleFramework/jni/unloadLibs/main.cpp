#include "CEGUI/Base.h"

#include <jni.h>
#include <android/api-level.h>
#include <dlfcn.h>

extern "C"
{

/*!
\brief
    A function for unloading a set of native libraries in an Android
    application.

    This is useful in the following scenario: say that our application needs to
    use a version of e.g. the PCRE library built especially for it. Now say,
    that Android already contains a system version of this library (e.g.
    "/system/lib/libpcre.so"), and say that this system version is already
    loaded by the application. If we don't unload the system version of PCRE,
    then our version (which is included in the APK) will not be used.

    So at the beginning of our Android activity we load this helper library (the
    one that contains this source file), and use the following function to
    unload any version of any native library included in our APK, which is not
    the version included in our APK. We can find the set of full paths to the
    loaded libraries by examining the file "/proc/self/maps".
*/

JNIEXPORT void JNICALL Java_sample_1framework_cegui_MainActivity_unloadLibs
  (JNIEnv* env, jclass /*class_*/, jobjectArray libs)
{
    jsize libs_n(env->GetArrayLength(libs));
    for (jsize lib_idx(0);  lib_idx < libs_n;  ++lib_idx)
    {
        jstring lib_java(static_cast<jstring>(env->GetObjectArrayElement(libs, lib_idx)));
        const char* lib(env->GetStringUTFChars(lib_java, 0));
        void* lib_handle(0);
        
        /* You'll probably notice, in the folliwing code, that we call "dlclose"
           twice. This is not a mistake. If "lib" is already loaded, "dlopen"
           increases its reference count by 1. Therefore, now to eventually
           decrease the reference count by 1, we need to decrease it by 2. Each
           call to "dlclose" decreases it by 1, so we need to call it twice. */
#if defined __ANDROID_API__  &&  __ANDROID_API__ >= 21
        for ( int lib_unload_iteration(0);  lib_unload_iteration < 0x100;
              ++lib_unload_iteration)
        {
            if (      !(lib_handle = dlopen(lib, RTLD_LAZY |RTLD_NOLOAD))
                  ||  dlclose(lib_handle)
                  ||  dlclose(lib_handle))
                break;
        }
#else
        // No, this is not a mistake.
        (lib_handle = dlopen(lib, RTLD_LAZY))
          &&  !dlclose(lib_handle)
          &&  !dlclose(lib_handle);
#endif
        env->ReleaseStringUTFChars(lib_java, lib);
        env->DeleteLocalRef(lib_java);
    }
}

} // extern "C"
