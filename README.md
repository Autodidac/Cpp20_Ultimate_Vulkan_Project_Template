[![wakatime](https://wakatime.com/badge/user/17eacdf5-0954-4233-89e5-cf09a2e230eb/project/f493991c-890e-48ab-9625-4bdd1e9e68c7.svg)](https://wakatime.com/badge/user/17eacdf5-0954-4233-89e5-cf09a2e230eb/project/f493991c-890e-48ab-9625-4bdd1e9e68c7)

# CPP20_Ultimate_Project_Template
The Ultimate Hello World - Cross Platform, Multi Editor Enabled Application/Library Project Template For C++20
Contains scripts and build systems to build with virtually any Compiler on any IDE, Code Editor, or Terminal


## NOTICE : You'll want to immediately change the license info upon use of this template repo, thank you!



## Multiplatform Build System
Including CMake, vscode, msvc, builds.





### On Windows

You'll need to set this in console for vcpkg manifest, it's required for cmake
```batch
$env:VCPKG_ROOT="C:\path\to\vcpkg"
$env:PATH="$env:VCPKG_ROOT;$env:PATH"
```




#### With MSVC - Microsoft Visual C/C++ Studio 2022 Community

Project.sln - Solution File in the Main Directory



#### With VSCode
tasks.json

It will actually build and use vcpkg manifest despite some of the build system errors, that's because it uses ninja or cl on windows,
but it will use gcc or clang on linux just fine. it also generates full msvc projects for some reason, which seems useful. The Projects
can stay but I will try to remove these issues in the future, for now vscode is linux focused because we have msvc on windows anyway.



### On Linux




#### With VSCode or VSCodium on Linux!
Uses a CMake Build system you can run tasks.json task, or build buttons at the bottom of the CMake Extension
Or better yet Build And Run From Command-Line Terminal below!




#### Built-in Fully Autonomous Build Scripts - Install VulkanSDK, Dependencies, and Compile Directly from Commandline in Linux Terminal
you will need to chmod -x the following to use:

./build.sh gcc Release
./install.sh gcc Release
./run.sh

or:

./build.sh clang Debug
./install.sh clang Debug


Notice: the difference in compiler in the Debug builds, clang vs gcc which ever compiler you prefer to use.
