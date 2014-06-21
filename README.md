# Crazy Eddie's GUI System Mk-2
[![CEGUI logo](https://bitbucket.org/cegui/cegui-promo/raw/default/logo/ceguilogo.png)](http://www.cegui.org.uk)
http://www.cegui.org.uk

Copyright ©2004 - 2014 CEGUI Team & Contributing Authors

The majority of the auxiliary files for CEGUI, that used to be plain text, are now kept in a "doxygenised" format within the doc/doxygen directory - please see those files, or generate the documentation for a more friendly format. Alternativelly, visit http://static.cegui.org.uk/docs for all your documentation needs!

What follows is just a quick-start guide, go to our doxygen docs for more detailed documentation. 

## Contributing

We are happiest with **clean pull requests** containing conscise commits with **proper commit messages**. We also accept **plain patches** but making it easier for us to just accept your contribution with one click greatly speeds up the review process.

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

## Common Issues / Complaints

Please do not tell us about these. We are quite tired of them :-)

### DLL hell
Is not what you are claiming it to be, research it! It does not mean "I see many DLL files, this must be hell!".

### The version suffixes
These were called stupid numbering but they do actually serve a purpose! Please let us keep them. It allows Linux distributions (and others) to install multiple CEGUI API versions alongside which eases migration and speeds up adoption of new CEGUI versions.

### CEGUI has zillion dependencies
CEGUI has **very few** required dependencies (IIRC it's just glm), many optional dependencies. The fact that it supports many different rendering situations, many different image loaders (with pass through options) and many different xml parsers is a good thing and only an uninformed person would tell you otherwise.

If cmake tells you that something was not found you **shall not panic**! Most probably it's a harmless message. It could even be that the dependency makes no sense on your platform!

### CEGUI is slow because ...
Obsession with speed is common, especially with newcomers. They have the impression that they want the fastest framework available so that their hyper AAA game can run with high FPS. If somebody on the internetz claims that something is 5% faster, they want it, they want it now! While we agree that there are areas in the CEGUI codebase that could be optimised, in practice this is rarely an issue.

People run CEGUI in bigger proprietary games and it displays hundreds of widgets for them in complex hierarchies. CEGUI is definitely not *too slow to use*. It could be too slow if you are doing something wrong - like loading layout files every frame.

And please give me a break with benchmarks comparing one toolkit to another. Such benchmarks can be tailored to make any of the toolkits win.
