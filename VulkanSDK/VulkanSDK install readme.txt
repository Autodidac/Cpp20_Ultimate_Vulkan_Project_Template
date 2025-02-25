 
On Linux you have to allow execution of scripts
 chmod +x install_vulkan_debian.sh
 chmod +x uninstall_vulkan_debian.sh


 These are provided for convience to keep a consistent version for building the engine,
 they can also help to keep user varients of the engine more consistent with changed or,
 updated versions of VulkanSDK

 build.sh is a scripted version that runs cmake from commandline
 cmake can also be run from vscode or vscodium, cursor, etc

 They all have their seperate output directories atm I'm trying to unify them, but different compilers require different output paths over they all just overwrite each other and that's no fun.

 after you chmod your scripts you can run them from their current directory in terminal

 ./install_vulkan_debian.sh
 and
 ./uninstall_vulkan_debian.sh

 These will install version 1.3.275.0 the same version running in the basic installion of Ubuntu Linux from the microsoft store and Kubutu Linux installed at the same time. Giving a strong unified starting point, this should be able to handle Cyberpunk 2077 level gfx. In Feb 2025. - Almond Engine


 after that you can run build, run, and run from the Bin directory
 ./build.sh clang Debug
./run.sh
./Bin/Configure preset using toolchain file/run.sh
