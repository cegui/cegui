package sample_framework.cegui;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.os.Environment;

public class Renderer implements GLSurfaceView.Renderer {
    private boolean d_isInitialised = false;
    private MainActivity d_activity = null;
    
    public Renderer(MainActivity activity)
    {
        d_activity = activity;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        if (!d_isInitialised)
        {
            d_isInitialised = true;
            final String cegui_dir = Environment.getExternalStorageDirectory()
              .getAbsolutePath() +"/CEGUI";
            MainActivity.init(d_activity, cegui_dir +"/CEGUI.log",
                              cegui_dir +"/datafiles");
        }
        MainActivity.render(d_activity);
    }
}
