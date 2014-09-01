This ReadMe gives a short introduction to the structure of the samples folder and explains how to create new samples.

The structure of the samples folder is very simple:   
- a "common" folder containing all files that are used by more than one sample  
- a "browser" folder containing all files that are used by the SamplesBrowser. You should not use    ANY of the sources as base code for creating own apps with CEGUI. Look into the "tests/app" folder instead.   
- and a lot of other folders containing a sample corresponding to the name   
- a CMakeLists.txt.template, a SampleTemplate.h, and a SampleTemplate.cpp - this files might be used as base files for your own sample   

If you want to create a new sample you simply have to add a new folder and rename it to what you want.
After that you have to create a source and a header file within that folder with exact the same name and you have to create a CMakeLists.txt file.
It is strongly recommended to use the prototype files that are in this folder!

Due to the last major changes to the samples browser you have to follow some naming conventions:
- name your folder to what you want but choose a short, concise, and precise name regarding to what your sample wants to demonstrate. Every first letter of a word has to be capitalised!
- your core source and header files have to have the same name with the same capitalisation
- your sample's class name has to have the same name with the same capitalisation too
- append "Sample" to your class name

If you followed these rules your folder structure should look like this (if you decided to name your sample "Widgets"):
```
/Widgets
  |- CMakeLists.txt
  |- Widgets.cpp
  |- Widgets.h
```
And the name of your class would have to be "WidgetsSample".

Right now you can start to implement your sample. Put all additional imagesets, layouts, etc. into the corresponding subfolder of the "datafiles" folder.