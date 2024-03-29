# Writing the inTree module for linux kernel we have to write the following necessary files   

## Kconfig:  
This is local Kconfig should be in the way shown here with basic syntax it is responsible to add the new module into the kernel menuconfig and this Kconfig should be added to the upper level Kconfig  
`source "inTree_custom_module/Kconfig"`

## Makefile:  
This Makefile is local to the upper level Makefile and that triggers this one  

## chrDrv.c:  
This is the main source file  

## Kbuild:  
Kbuild is linux build system it adds inTree module directory into main linux Kbuild   
`obj-y			+= inTree_custom_module/`

The first version of character device creation and link with the system is fully working and will be updating gradually with time
This code for character driver in wihtin kernel in my and build and run with kernel you can enable or disable in kernel config menu or .config file but any one can use as outTree module no change need in c source but the change required is in make file may be

[dvice driver](https://embetronicx.com/tutorials/linux/device-drivers/linux-device-driver-part-1-introduction/)
