# Crazy Eddie's GUI System Mk-2
[![CEGUI logo](https://bitbucket.org/cegui/cegui-promo/raw/default/logo/ceguilogo.png)](http://www.cegui.org.uk)
http://www.cegui.org.uk

Copyright ©2004 - 2015 Paul D Turner, The CEGUI Development Team and Contributing Authors

The majority of the auxiliary files for CEGUI, that used to be plain text, are now kept in a "doxygenised" format within the doc/doxygen directory - please see those files, or generate the documentation for a more friendly format. Alternatively, visit http://static.cegui.org.uk/docs for all your documentation needs!

What follows is just a quick-start guide, go to our doxygen docs for more detailed documentation.

## Available branches and versions

- The CEGUI repository consists of multiple branches:
    - `v0-8` provides the latest stable ABI compatible (to 0.8.X releases) version of CEGUI. Based on the C++03 standard and compatible with most common compilers, including Visual Studio 2008-2015. As this branch is ABI compatible, it is possible to replace CEGUI dynamic libraries of version 0.8.X with newer 0.8.X versions, or vice-versa, without having to recompile project. This branch is also the base for new 0.8.X releases.
    - `v0` Provides the latest stable API compatible version of CEGUI and contains changes that break the ABI. Based on the C++03 standard and compatible with most common compilers, including Visual Studio 2008-2015. Versions from this branch will be used for the next minor version release.
    - `default` contains changes that will only be used in the next major version. Based on the C++11 standard and compatible with most common up-to date compilers, including Visual Studio 2013 or newer. This branch is highly unstable, will introduce fundamental changes and **breaks ABI and API** compatibility. We do not recommend you to use this in production, unless you heavily depend on a feature and discussed this with a developer of CEGUI before: This is recommended so that you are aware of all potential risks. In the general case you are advised to use one of the stable branches, to save you a lot of headache. 

The `v0-8` and `v0` branches are considered stable but do undergo bug-fixes and small changes, which do not break ABI and API respectively. These changes of course introduce a small risk that there might be temporary issues for the moment in the branches. If you notice any bugs in those branches, please report them to us as soon as possible - use the [forum](http://cegui.org.uk/forum/index.php) and/or our IRC channels `#cegui` and `#cegui-devel` on `irc.freenode.net` to inform us. Please consider that we are not available on IRC 24 hours per day, but feel free to idle there until we respond. If in doubt which branch to use, also feel free to ask us this way. For production usage we generally recommend to use a stable release version. A list of releases can be found on the [on our website](cegui.org.uk/download).

## Contributing

We are happiest with **clean pull requests** containing conscise commits with **proper commit messages**. We also accept **plain patches** but making it easier for us to just accept your contribution with one click greatly speeds up the review process.

Here is an explanation on how to fork from our repository, commit changes to your fork, and create a pull request targeting the right branch:
https://confluence.atlassian.com/display/BITBUCKET/Fork+a+Repo,+Compare+Code,+and+Create+a+Pull+Request

Please also keep in mind to target the right repository. We prefer to target the ABI compatible branch if possible. Otherwise the API compatible ones. For information about ABI/API compatibility, please read this page: https://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C++

If in doubt which branch to target, please contact us!


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
CEGUI_SAMPLE_DATAPATH=../../datafiles ./CEGUISampleFramework-0 ../datafiles/samples/samples.xml
```

## We want to answer some questions and misconceptions:

### Why are there so many dependencies?
CEGUI has relatively few **required** dependencies (currently only glm) and many **optional** dependencies. The fact that it supports many different rendering libraries and engines, many different image loaders/codecs (with pass through options) and many different xml parsers is a good thing and only an uninformed person would tell you otherwise.

### CMake tells me something was not found, is this a problem?
If CMake tells you that **something** was not found, you **shall not panic** ;) ! Most probably it's a harmless message. You should only worry if a dependency you know you need is not found or if no dependencies are found at all. In the latter case, on Windows and Mac OS X, you probably did not put the "dependencies" folder (including the dependencies compiled in Debug/Release/whatever-else-you-need) into the folder containing all CEGUI files and folders. You can also specify another folder in CMake using the variable CMAKE_PREFIX_PATH.

### What are the version numbers in the dlls and executables good for? How to remove them?
This numbering system does actually serve a very important purpose! Please let us keep them. It allows Linux distributions (and others) to install multiple CEGUI API versions alongside which eases migration and speeds up adoption of new CEGUI versions. On Windows this will allow us to provide you with precompiled CEGUI dependencies using Nuget in the future.

### In my application, the OS cursors that is shown above the CEGUI cursor is a lot faster, why?
This is expected behaviour. First of all you should always test performance in Release mode, but even there the cursor will be slower. The reason is simply that it is very unlikely any application will have a cursor as fast as the OS cursor. Also remember that the speed is closely tied to your frame rate, so if you run the HelloWorld demo at 5000 FPS, the difference will be less but still noticable. Any game, simulation or other application out there that renders its own cursor via OpenGL/Direct3D functions similarly. However, the cursor speed is not an issue for users if your application runs at rasonable frame rates (>60 FPS) without frame drops and will not be perceived as such. Once you hide the OS cursor then the delay will probably not be noticable to you anymore.

### So many DLLs, why does CEGUI cause a "DLL hell"? What about static linking?
First off, the term "DLL hell" is used wrongly in this context. It does not mean "I see many DLL files, this must be hell!". Dynamically linking the CEGUI library is the best way to have things working as they are supposed to and guaranteeing good compatibility and a low chance of issues arising with dependencies. On Windows we recommend to use dynamic linking with CEGUI rather than static linking, as past experience (some of the users ran into technical issues) has shown us that this is safer. However, if you know what you are doing you can definitely use static linking. Be aware though that we only test dynamic linking regularly, so the CMake files might be outdated and you might have to add linked libraries to your IDE yourself etc. On a positive note: in the upcoming 1.0 version we will reduce the amount of DLLs CEGUI creates by merging some of them into the Base library. A short but definitely not complete summary of advantages and disadvantages of static vs dynamic linking can be found here: http://stackoverflow.com/questions/1993390/static-linking-vs-dynamic-linking

### Is CEGUI slower/faster than other libraries? Why is it slow in my case?
Mostly when users complained in the forums about CEGUI's speed, it turned out to be that they either ran the application in Debug config or did something wrong: It can be slow if you are loading layout resources/files every frame or causing unnecessary updates and events. Or it can be slow if you are unnecessarily updating CEGUI multiple times per frame in your program. If you cannot find the issue then it is best to perform a forum/google search and - if you do not find anything helpful - to describe your setup in detail and what issues you have. When CEGUI gets slow it can also be due to a very specific usage of specific features, which we did not expect or test. In this case we would like you to describe your use-case in the forum so we can find a solution or, if you are capable of solving the issue yourself, create a pull-request on bitbucket.

In general CEGUI is very fast and can easily compete with other GUI libraries in speed (especially Flash-based ones, since they do not access OpenGL or Direct3D directly). While no complex library out there will ever be perfectly optimised, CEGUI can be considered highly performant. This is true for the computations done on the CPU as well as those on the GPU. It still runs optimally when hundreds of windows are opened and rendered at the same time.

The best proof that CEGUI is fast is that big proprietary games, which displays hundreds of widgets and use complex hierarchies, have been made using CEGUI (Torchlight 1, Torchlight 2, Venetica, etc.).

Most of our samples, if started in Release mode, will render at speeds above 3000 frames per second on a modern CPU and GPU. As an additional note for some people who liked to cite dubious benchmarks regarding such speed comparisons: benchmarks are situation-dependent and could easily misrepresent a library's actual speed by wrong, inefficient or unusual usage. If used correctly and inside the bounds of expected usage, CEGUI performs extremely well.