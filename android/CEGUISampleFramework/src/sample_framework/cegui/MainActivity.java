package sample_framework.cegui;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.opengl.GLSurfaceView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;
import java.util.HashSet;

public class MainActivity extends Activity
{
    
    public static String extractFileNameFromPath(String path)
    {
        final int dir_seperator_idx = path.lastIndexOf(File.separator);
        return    dir_seperator_idx >= 0
               ?  path.substring(dir_seperator_idx +File.separator.length())
               :  null;
    }

    public void exceptionOccured(Throwable throwable)
    {
        Log.e ("MainActivity", "Exception thrown; aborting.");
        Throwable throwableI = throwable;
        do
        {
            Log.e ("MainActivity", "Exception type: "
                   + throwableI.getClass().getName());
            Log.e ("MainActivity", "Exception Message: "
                   + throwableI.getMessage());
            throwableI = throwableI.getCause();
        } while (throwableI != null);
        finish();
    }
    
    public static final String s_ceguiVerMajorMinor = "0.8";
    public static final String s_unloadLibsName =
      "unloadPreloadedConflictingNativeLibs";
    public static final String s_loadedLibsFilePath = "/proc/self/maps";
    public static final String s_loadedLibsFileEncoding = "UTF-8";

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        try {
            unloadLibs();
            System.loadLibrary("CEGUISampleFramework-" +s_ceguiVerMajorMinor);
            d_openglSurfaceView = new GLSurfaceView(this);
            d_openglSurfaceView.setEGLContextClientVersion(2);
            d_openglSurfaceView.setRenderer(new Renderer(this));
    	    setContentView(d_openglSurfaceView);
        }
        catch (Throwable throwable)
        {
            exceptionOccured(throwable);
        }
    }
    
    @Override
    protected void onDestroy()
    {
        nativeFinish(this);
        super.onDestroy();
    }
    
    @Override
    protected void onPause()
    {
        d_openglSurfaceView.onPause();
        super.onPause();
    }
     
    @Override
    protected void onResume()
    {
        super.onResume();
        d_openglSurfaceView.onResume();
    }
    
    private static native void unloadLibs(String[] libs);
    private static native void nativeFinish(Activity activity);
    static native void init(Activity activity, String log_file,
                                    String data_path_prefix);
    static native void render(Activity activity);

    /* Please refer to "jni/unloadLibs/main.cpp" under the root of the project
       directory for an explanation what this function does and why it's
       necessary. */
    private void unloadLibs() throws IOException, FileNotFoundException
    {
        final String native_libs_dir = getApplicationContext()
          .getApplicationInfo().nativeLibraryDir;
        final File []native_libs_dir_contents =
          new File(native_libs_dir).listFiles();
        HashSet<String> native_libs = new HashSet<String>();
        HashSet<String> native_libs_canonical_paths = new HashSet<String>();
        for (File file : native_libs_dir_contents)
        {
            final String path = file.getCanonicalPath();
            final String file_name = extractFileNameFromPath(path);
            if (      file_name != null
                  &&  file_name.matches("^lib.*\\.so$")
                  &&  !file_name.equals("lib" +s_unloadLibsName +".so"))
            {
                native_libs.add(file_name);
                native_libs_canonical_paths.add(path);
            }
        }
        System.loadLibrary(s_unloadLibsName);
        for ( int libs_unload_iteration=0;  libs_unload_iteration < 0x100;
              ++libs_unload_iteration)
        {
            HashSet<String> native_libs_loaded = new HashSet<String>();
            Scanner loaded_libs_scanner = new Scanner(new File
              (s_loadedLibsFilePath), s_loadedLibsFileEncoding);
            while (loaded_libs_scanner.hasNext())
            {
                final String full_path = loaded_libs_scanner.next();
                final String canonical_path =
                  new File(full_path) .getCanonicalPath();
                final String file_name =
                  extractFileNameFromPath(canonical_path);
                if (      file_name != null
                      &&  native_libs.contains(file_name)
                      &&  !native_libs_canonical_paths
                             .contains(canonical_path))
                    native_libs_loaded.add(canonical_path);
            }
            loaded_libs_scanner.close();
            if (native_libs_loaded.isEmpty())
                break;
            String[] libs = new String[native_libs_loaded.size()];
            native_libs_loaded.toArray(libs);
            unloadLibs(libs);
        } 
    }
    
    private GLSurfaceView d_openglSurfaceView = null;
    
}
