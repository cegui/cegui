/***************************************************************************
 * This file have been borowed from the Ogre project 
 * @LICENSE CHECK@
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
          Logger* logger = Logger::getSingletonPtr();
          
          if(logger)
          {
               logger->logEvent("---- Beginning exe bundle loading ----");
               logger->logEvent(name);
          
               logger->logEvent("Get reference to base bundle", Insane);
          }
          CFBundleRef baseBundle = CFBundleGetBundleWithIdentifier(CFSTR("net.sourceforge.crayzedsgui.CEGUIBase"));

          if(logger) logger->logEvent("Get reference to main bundle", Insane);
          CFBundleRef mainBundle = CFBundleGetMainBundle();
          
          if(logger) logger->logEvent("Create name", Insane);
          CFStringRef nameRef = CFStringCreateWithCString(0, name, kCFStringEncodingASCII);
          CFURLRef bundleURL = 0; //URL of bundle to load
          CFBundleRef bundle = 0; //bundle to load
          
          // Cut off .bundle if present.
          if(logger) logger->logEvent("Check if .bundle suffix is on name", Insane);
          if(CFStringHasSuffix(nameRef, CFSTR(".bundle"))) 
          {
               if(logger) logger->logEvent("Create temporary name reference", Insane);
               CFStringRef nameTempRef = nameRef;
               int end = CFStringGetLength(nameTempRef) - CFStringGetLength(CFSTR(".bundle"));
               nameRef = CFStringCreateWithSubstring(0, nameTempRef, CFRangeMake(0, end));

               if(logger) logger->logEvent("Release temporary name reference", Insane);
               CFRelease(nameTempRef);
          }
          
          // Assume relative to Resources/ directory of application's bundle.
          if(logger) logger->logEvent("Create bundle URL", Insane);
          bundleURL = CFBundleCopyResourceURL(mainBundle, nameRef, CFSTR("bundle"), 0);
          if(bundleURL)
          {
               if(logger) logger->logEvent("Create bundle from URL", Insane);
               bundle = CFBundleCreate(0, bundleURL);
               
               if(logger) logger->logEvent("Release bundle URL", Insane);
               CFRelease(bundleURL);
          }
          
          // Otherwise, try Resources/ directory of CEGUI Framework bundle.
          if(!bundle) 
          {
               if(logger) logger->logEvent("Couldn't get bundle from main bundle reference; try base");
               bundleURL = CFBundleCopyResourceURL(baseBundle, nameRef, CFSTR("bundle"), 0);
               if(bundleURL) 
               {
                    if(logger) logger->logEvent("Create bundle from URL", Insane);
                    bundle = CFBundleCreate(0, bundleURL);
                    
                    if(logger) logger->logEvent("Release bundle URL", Insane);
                    CFRelease(bundleURL);
               }
          }
          if(logger) logger->logEvent("Release name reference", Insane);
          CFRelease(nameRef);
          
          if(bundle) 
          {
               if(logger) logger->logEvent("Load the bundle executable.", Insane);
               if(CFBundleLoadExecutable(bundle)) 
               {
                    if(logger) logger->logEvent("Bundle loaded successfully.");
                    return bundle;
               }
               else 
               {
                    if(logger) logger->logEvent("Bundle loading failed!");
                    CFRelease(bundle);
               }
          }
          
          if(logger) logger->logEvent("Failure; return 0", Insane);
          return 0;
     }
     
     void* mac_getBundleSym(CFBundleRef bundle, const char* name) 
     {
          Logger* logger = Logger::getSingletonPtr();
          
          if(logger) logger->logEvent("---- Getting bundle symbol ----", Insane);
          CFStringRef nameRef = CFStringCreateWithCString(0, name, kCFStringEncodingASCII);
          
          if(logger) 
          {
               logger->logEvent("Find function pointer for name: ", Insane);
               logger->logEvent(name, Insane);
          }
          void* sym = CFBundleGetFunctionPointerForName(bundle, nameRef);
          
          if(logger) logger->logEvent("Release bundle name", Insane);
          CFRelease(nameRef);
          
          if(logger) logger->logEvent("---- Done getting bundle symbol ----", Insane);
          return sym;
     }
          
     // Returns 1 on error, 0 otherwise.
     bool mac_unloadExeBundle(CFBundleRef bundle) 
     {
          Logger* logger = Logger::getSingletonPtr();
          
          if(logger) logger->logEvent("---- Beginning exe bundle unloading ----");
          
          if(bundle) 
          {
               if(logger) logger->logEvent("Bundle unloaded.", Insane);

               // No-op, can't unload Obj-C bundles without crashing.
               return 0;
          }
          
          if(logger) logger->logEvent("---- Ending exe bundle unloading ----");
          return 1;
     }
     
     const char* mac_errorBundle() 
     {
          return "Unknown Error";
     }

}
