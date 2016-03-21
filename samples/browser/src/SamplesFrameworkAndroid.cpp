/***********************************************************************
    created:    27/7/2015
    author:     Yaron Cohen-Tal
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/

#if !defined __ANDROID__
#error This file should only be used in Android.
#endif

#include "SamplesFramework.h"

#include "CEGUI/RendererModules/OpenGL/GL.h"
#include <jni.h>
#include <cassert>
#include <android/log.h>

typedef jobject AndroidActivity;

SamplesFramework* G_sampleFramework(0);

void finishAndroidActivity(JNIEnv* env, AndroidActivity activity)
{
    jclass activity_class(env->FindClass("android/app/Activity"));
    assert(env->IsSameObject(activity_class, 0) == (jboolean)JNI_FALSE);
    jmethodID finish_method(env->GetMethodID(activity_class, "finish", "()V"));
    assert(finish_method);
    env->CallVoidMethod(activity, finish_method);
}

extern "C"
{

JNIEXPORT void JNICALL
  Java_sample_1framework_cegui_MainActivity_nativeFinish
  (JNIEnv* env, jclass /*class_*/, AndroidActivity activity)
{
    bool caught_exception(false);
    CEGUI_TRY
    {
        assert(G_sampleFramework);
        G_sampleFramework->cleanup();
        CEGUI_DELETE_AO G_sampleFramework;
        G_sampleFramework = 0;
    }
    CEGUI_CATCH(const std::exception& exception)
    {
        G_sampleFramework->outputExceptionMessage(exception.what());
        finishAndroidActivity(env, activity);
    }
}

JNIEXPORT void JNICALL
  Java_sample_1framework_cegui_MainActivity_init (JNIEnv* env,
  jclass /*class_*/, AndroidActivity activity, jstring log_file_java,
  jstring data_path_prefix_java)
{
    bool caught_exception(false);
    CEGUI_TRY
    {
        assert(!G_sampleFramework);
        G_sampleFramework = new SamplesFramework("");
        const char* log_file(env->GetStringUTFChars(log_file_java, 0));
        const char* data_path_prefix
          (env->GetStringUTFChars(data_path_prefix_java, 0));
        G_sampleFramework->initialise(
          CEGUI::String(reinterpret_cast<const CEGUI::utf8*>(log_file)),
          CEGUI::String(
            reinterpret_cast<const CEGUI::utf8*>(data_path_prefix)));
        env->ReleaseStringUTFChars(log_file_java, log_file);
        env->ReleaseStringUTFChars(data_path_prefix_java, data_path_prefix);
    }
    CEGUI_CATCH(const std::exception& exception)
    {
        G_sampleFramework->outputExceptionMessage(exception.what());
        finishAndroidActivity(env, activity);
    }
}

JNIEXPORT void JNICALL
  Java_sample_1framework_cegui_MainActivity_render
  (JNIEnv* env, jclass /*class_*/, AndroidActivity activity)
{
    bool caught_exception(false);
    CEGUI_TRY
    {
        glClear(GL_COLOR_BUFFER_BIT);
        G_sampleFramework->renderSingleFrame(0);
    }
    CEGUI_CATCH(const std::exception& exception)
    {
        G_sampleFramework->outputExceptionMessage(exception.what());
        finishAndroidActivity(env, activity);
    }
}

} // extern "C"
