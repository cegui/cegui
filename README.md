# Crazy Eddie's GUI System Mk-2
[![CEGUI logo](https://bitbucket.org/cegui/cegui-promo/raw/default/logo/ceguilogo.png)](http://www.cegui.org.uk)
http://www.cegui.org.uk

Copyright ©2004 - 2014 Paul D Turner, The CEGUI Development Team and Contributing Authors

The majority of the auxiliary files for CEGUI, that used to be plain text, are now kept in a "doxygenised" format within the doc/doxygen directory - please see those files, or generate the documentation for a more friendly format. Alternativelly, visit http://static.cegui.org.uk/docs for all your documentation needs!

What follows is just a quick-start guide, go to our doxygen docs for more detailed documentation. 

## Contributing

We are happiest with **clean pull requests** containing conscise commits with **proper commit messages**. We also accept **plain patches** but making it easier for us to just accept your contribution with one click greatly speeds up the review process.

Here is an explanation on how to fork from our repository, commit changes to your fork, and create a pull request targeting the right branch:
https://confluence.atlassian.com/display/BITBUCKET/Fork+a+Repo,+Compare+Code,+and+Create+a+Pull+Request

- Make sure you target the appropriate branch! If in doubt, ask us on IRC.
  - `default` if your change **breaks API**
  - *API branch* (e.g.: `v0`) if it is a minor change that **doesn't break API but does break ABI**
  - *ABI branch* (e.g.: `v0-8`) for bugfixes or very small changes that **don't break API and ABI**
- Join the `#cegui` and `#cegui-devel` channels on `irc.freenode.net`
  - We are fun and rarely on-topic.

## Building CEGUI
The following script is more or less universal for *NIX systems and Windows. Minor changes may be required.

```bash
cd $cegui_folder
# you can call the folder differently but "build" is customary
mkdir build/
cd build/
# run the configure step
cmake-gui ../
# fix any issues pointed out by cmake
# not all dependencies are required so if some are not found, don't panic and carry on!
# alternative (if you are a command line pro)
# cmake ../
```

At this point, Makefiles, project files or something else will be generated. Next step depends on which that is.

For Makefiles, just run
```bash
cd $cegui_folder
cd build/
make
```

For Visual Studio solutions, double click, change Build Mode accordingly (Release, Debug, ...) and press Build.

## Installing CEGUI
This section only makes sense on *NIX-like systems.

Make sure you have the correct CMAKE_INSTALL_PREFIX set at configure time. Alternativelly rerun cmake and set it. By default it should be `/usr/local/` but you may want `/usr/`.

```bash
cd $cegui_folder
cd build/
sudo make install
```

## Running CEGUI samples
If you installed CEGUI system-wide just call:
```bash
CEGUISampleFramework-0
```

If is preferable to call it from the command line because it will ask you to select a renderer in case you have more than 1 available.

If you have no installed system-wide, it's a bit more involved and complicated.
```bash
cd $cegui_folder
cd build/bin/
CEGUI_SAMPLE_DATAFILES=../../datafiles ./CEGUISampleFramework-0 ../datafiles/samples/samples.xml
```

## We want to clarify some common misconceptions about CEGUI:

### "It causes a DLL hell" or "Oh no, so many dll files!"
First off, the term "DLL hell" is used wrongly by users in this context. It does not mean "I see many DLL files, this must be hell!". Dynamically linking the CEGUI library is the best way to have things working as they are supposed to and guaranteeing good compatibility and a low chance of issues arising with dependencies. On Windows we heavily recommend to never use static linking with CEGUI and past experience has proven us right. A short summary of static vs dynamic linking can be found here: http://stackoverflow.com/questions/1993390/static-linking-vs-dynamic-linking

### "It has useless version numbers in the dlls and executables"
This numbering system does actually serve a very important purpose! Please let us keep them. It allows Linux distributions (and others) to install multiple CEGUI API versions alongside which eases migration and speeds up adoption of new CEGUI versions. On Windows this will allow us to provide you with precompiled CEGUI dependencies using Nuget in the future.

### "It has a killion dependencies"
CEGUI has relatively few **required** dependencies (currently only glm) and many optional dependencies. The fact that it supports many different rendering libraries and engines, many different image loaders/codecs (with pass through options) and many different xml parsers is a good thing and only an uninformed person would tell you otherwise.

### "CMake tells me something was not found"
If CMake tells you that **something** was not found, you **shall not panic** ;) ! Most probably it's a harmless message. You should only worry if not a single dependency was found. On Windows and Mac OS X, you should however make sure you placed the dependency folder correctly before running Cmake.

### "It is slow"
Whenever users complained in the forums about CEGUI's speed it turned out to be that they either ran the application in Debug configuration (which, clearly, the compiler does not compile with full optimisations) or did something wrong (such as updating CEGUI the wrong way or causing unnecessary amounts of events or creating event handling functions that cause the issues on the user's side). Only occasionally it can be tracked down to a bug, but mostly that is connected to specific usage. 

While we agree that **some** areas of the CEGUI codebase are not as optimised as they could be yet, most of our code-base is very fast. CEGUI can easily compete with other GUI libraries in speed. This is true for the computations on the CPU as well as the rendering speed on the GPU. It even runs optimally still if hudreds of windows are opened and rendered at the same time. 

The best proof that CEGUI is fast is that big proprietary games, which displays hundreds of widgets and use complex hierarchies, have been made using CEGUI (Torchlight 1, Torchlight 2, Venetica, etc.). CEGUI is definitely not *too slow to use*. It could be too slow if you are doing something wrong - like loading layout files every frame or causing unnecessary updates and events. In this case it is best to do a forum search and - if you do not find anything helpful - to describe your setup in detail and what issues you have.

We expect CEGUI to perform very well, and in a lot of cases faster than other libraries. This is epecially true for GUI libraries and toolkits that depend on Flash or similar techniques for rendering and do not access OpenGL or Direct3D directly. Most of our samples, if started in Release mode, will render at speeds above 3000 frames per second on a modern CPU and GPU. As an additional note for people who like to cite or look at benchmarks, we want you to keep in mind that benchmarks are often very situation-dependent and could easily misrepresent a library's actual speed by wrong or unusual usage.