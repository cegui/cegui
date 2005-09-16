/*
-----------------------------------------------------------------------------
This source file is part of CrazyEddiesGUI
    (but borrowed from OGRE, see http://www.ogre3d.org/)

Copyright � 2000-2002 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General  License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General  License for more details.

You should have received a copy of the GNU Lesser General  License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#include <CoreFoundation/CoreFoundation.h>

#include "macPlugins.h"

#include "CEGUILogger.h"

int main(void)
{
}

namespace CEGUI 
{
     
     CFBundleRef mac_loadExeBundle(const char* name) 
     {
          Logger::getSingleton().logEvent("---- Beginning exe bundle loading ----");
          Logger::getSingleton().logEvent(name);
          
          Logger::getSingleton().logEvent("Get reference to base bundle", Insane);
          CFBundleRef baseBundle = CFBundleGetBundleWithIdentifier(CFSTR("net.sourceforge.crayzedsgui.CEGUIBase"));

          Logger::getSingleton().logEvent("Get reference to main bundle", Insane);
          CFBundleRef mainBundle = CFBundleGetMainBundle();
          
          Logger::getSingleton().logEvent("Create name", Insane);
          CFStringRef nameRef = CFStringCreateWithCString(0, name, kCFStringEncodingASCII);
          CFURLRef bundleURL = 0; //URL of bundle to load
          CFBundleRef bundle = 0; //bundle to load
          
          // Cut off .bundle if present.
          Logger::getSingleton().logEvent("Check if .bundle suffix is on name", Insane);
          if(CFStringHasSuffix(nameRef, CFSTR(".bundle"))) 
          {
               Logger::getSingleton().logEvent("Create temporary name reference", Insane);
               CFStringRef nameTempRef = nameRef;
               int end = CFStringGetLength(nameTempRef) - CFStringGetLength(CFSTR(".bundle"));
               nameRef = CFStringCreateWithSubstring(0, nameTempRef, CFRangeMake(0, end));

               Logger::getSingleton().logEvent("Release temporary name reference", Insane);
               CFRelease(nameTempRef);
          }
          
          // Assume relative to Resources/ directory of application's bundle.
          Logger::getSingleton().logEvent("Create bundle URL", Insane);
          bundleURL = CFBundleCopyResourceURL(mainBundle, nameRef, CFSTR("bundle"), 0);
          if(bundleURL)
          {
               Logger::getSingleton().logEvent("Create bundle from URL", Insane);
               bundle = CFBundleCreate(0, bundleURL);
               
               Logger::getSingleton().logEvent("Release bundle URL", Insane);
               CFRelease(bundleURL);
          }
          
          // Otherwise, try Resources/ directory of CEGUI Framework bundle.
          if(!bundle) 
          {
               Logger::getSingleton().logEvent("Couldn't get bundle from main bundle reference; try base");
               bundleURL = CFBundleCopyResourceURL(baseBundle, nameRef, CFSTR("bundle"), 0);
               if(bundleURL) 
               {
                    Logger::getSingleton().logEvent("Create bundle from URL", Insane);
                    bundle = CFBundleCreate(0, bundleURL);
                    
                    Logger::getSingleton().logEvent("Release bundle URL", Insane);
                    CFRelease(bundleURL);
               }
          }
          Logger::getSingleton().logEvent("Release name reference", Insane);
          CFRelease(nameRef);
          
          if(bundle) 
          {
               Logger::getSingleton().logEvent("Load the bundle executable.", Insane);
               if(CFBundleLoadExecutable(bundle)) 
               {
                    Logger::getSingleton().logEvent("Bundle loaded successfully.");
                    return bundle;
               }
               else 
               {
                    Logger::getSingleton().logEvent("Bundle loading failed!");
                    CFRelease(bundle);
               }
          }
          
          Logger::getSingleton().logEvent("Failure; return 0", Insane);
          return 0;
     }
     
     void* mac_getBundleSym(CFBundleRef bundle, const char* name) 
     {
          Logger::getSingleton().logEvent("---- Getting bundle symbol ----", Insane);
          CFStringRef nameRef = CFStringCreateWithCString(0, name, kCFStringEncodingASCII);
          
          Logger::getSingleton().logEvent("Find function pointer for name: ", Insane);
          Logger::getSingleton().logEvent(name, Insane);
          void* sym = CFBundleGetFunctionPointerForName(bundle, nameRef);
          
          Logger::getSingleton().logEvent("Release bundle name", Insane);
          CFRelease(nameRef);
          
          Logger::getSingleton().logEvent("---- Done getting bundle symbol ----", Insane);
          return sym;
     }
          
     // Returns 1 on error, 0 otherwise.
     bool mac_unloadExeBundle(CFBundleRef bundle) 
     {
          Logger::getSingleton().logEvent("---- Beginning exe bundle unloading ----");
          
          if(bundle) 
          {
               Logger::getSingleton().logEvent("Bundle unloaded.", Insane);

               // No-op, can't unload Obj-C bundles without crashing.
               return 0;
          }
          
          Logger::getSingleton().logEvent("---- Ending exe bundle unloading ----");
          return 1;
     }
     
     const char* mac_errorBundle() 
     {
          return "Unknown Error";
     }

}
