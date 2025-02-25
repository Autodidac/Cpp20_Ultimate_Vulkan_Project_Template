25 hours + [![wakatime](https://wakatime.com/badge/user/17eacdf5-0954-4233-89e5-cf09a2e230eb/project/7a9b5c7e-57a9-4eb8-a0bb-24268da337cc.svg)](https://wakatime.com/badge/user/17eacdf5-0954-4233-89e5-cf09a2e230eb/project/7a9b5c7e-57a9-4eb8-a0bb-24268da337cc)

# CPP20_Ultimate_Vulkan_Project_Template
The Ultimate Vulkan Hello World - Cross Platform, Multi Editor Enabled Application/Library Project Template For C++20
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






### On Windows





#### With MSVC (Microsoft Visual C/C++ Studio 2022 Community)
- Open the `Project.sln` file located in the main directory.






#### With VSCode
- Use the provided `tasks.json` file to manage your build tasks.
- You can `configure gcc` or `configure clang` and `build gcc` or `build all` there is also a `clean` and `clean all` task options
- The CMake vscode extension is primarily used and will be configured for Linux, but may also work with the build options for that at the bottom of vscode or vscode clones like cursor
- You can `configure CMake` and `build CMake` or `install CMake` using those tasks, this uses the Ninja build system.

---

It will actually build and use vcpkg manifest despite some of the build system errors, that's because it uses ninja or cl on windows,
but it will use gcc or clang on linux just fine. it also generates full msvc projects for some reason, which seems useful. The Projects
can stay but I will try to remove these issues in the future, for now vscode is linux focused because we have msvc on windows anyway.

update: You can fix the manifest errors tinkering and building multiple times until it links, editor restarts also may be required
ultimately the debugger seems to stop here regardless atm :     
message(FATAL_ERROR "Required packages (glfw3 and glm) not found. Please install them via your package manager or vcpkg.")








### On Linux








#### With VSCode or VSCodium on Linux!
Uses a CMake Build system you can run tasks.json task, or build buttons at the bottom of the CMake Extension
Or better yet Build And Run From Command-Line Terminal below!








#### Built-in Fully Autonomous Build Scripts - Install VulkanSDK, Dependencies, and Compile Directly from Commandline in Linux Terminal
you will need to chmod -x the following to use:

The repository includes scripts that install the Vulkan SDK, manage dependencies, and compile the project directly from the terminal.

Before using these scripts, make sure they are executable:

```bash
chmod +x ./VulkanSDK/install_vulkansdk_debian.sh
chmod +x ./VulkanSDK/uninstall_vulkansdk_debian.sh
chmod +x ./build.sh
chmod +x ./install.sh
chmod +x ./run.sh
chmod +x ./clean.sh
```

To install the Vulkan SDK on Debian-based systems, run:

```bash
./VulkanSDK/install_vulkansdk_debian.sh
```

To uninstall the Vulkan SDK, run:

```bash
./VulkanSDK/uninstall_vulkansdk_debian.sh
```

```bash
./build.sh gcc Release
./install.sh gcc Release
./run.sh
./clean.sh
```

or:

```bash
./build.sh clang Debug
./install.sh clang Debug
./clean.sh
```

Notice: the difference in compiler in the Debug builds, clang vs gcc which ever compiler you prefer to use.
