# TI Launchpad F28069M interrupt hello world example

**Note: this project does not use the HAL**

This simple example set a CPU timer of 1 sec and prints on the serial common interface (SCI) something like:
```
Hello World! 1 
Hello World! 2 
Hello World! 3 
Hello World! 4 
Hello World! 5 
...
```

## Setup
Import the css folder. The result should be like the bottom image:

![project](./docs/files.jpg)

The files outside of src folder should be linked from C2000Ware package.

Example: *~/ti/C2000Ware_3_01_00_00_Software/device_support/f2806x/common/include*

## Write program into Flash or RAM (not permanet)

The linker file (*cmd*) will determine address locations on your board.

To write the program into FLASH use the **xxx_boardName_xxx.cmd**

Instead, to write the program into RAM use the **xxx_boardName_xxx_ram_lnk.cmd**

Example on Flash:

![linker](./docs/linker.jpg)

The path location is:
```
 ~/ti/C2000Ware_3_01_00_00_Software/device_support/f2806x/common/cmd
 ```


## Load function from Flash to RAM

[C28x Compiler - Understanding Linking](https://processors.wiki.ti.com/index.php/C28x_Compiler_-_Understanding_Linking)

[Speeding up flash-based embedded applications](https://www.embedded.com/speeding-up-flash-based-embedded-applications)

If running from Flash, specify what function to compy on RAM:
```C
#pragma CODE_SECTION(Flash_Init, “ramfuncs”);
```

Copying the functions into RAM at start-up
```C
/* Copy time critical code and Flash setup code to RAM
*  The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
*  symbols are created by the linker. Refer to the project.cmd file.
*/
memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
```
