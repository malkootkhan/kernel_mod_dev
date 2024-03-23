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


