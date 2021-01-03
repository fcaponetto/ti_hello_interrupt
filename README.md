# TI Launchpad F28069M interrupt hello world example

**Note: this project does not use the HAL**

Import the css folder. The result should be like the bottom image:

![project](./docs/files.jpg)

The files outside of src folder should be linked from C2000Ware package.

Example: *~/ti/C2000Ware_3_01_00_00_Software/device_support/f2806x/common/include*

Remember also to link the linker file:

![linker](./docs/linker.jpg)

Example *~/ti/C2000Ware_3_01_00_00_Software/device_support/f2806x/common/cmd*

## Load function from Flash to RAM

[Speeding up flash-based embedded applications](https://www.embedded.com/speeding-up-flash-based-embedded-applications)

Specify what functions put on RAM:
```
#pragma CODE_SECTION(Flash_Init, “ramfuncs”);
```

Copying the functions into RAM at start-up
```
/* Copy time critical code and Flash setup code to RAM
*  The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
*  symbols are created by the linker. Refer to the project.cmd file.
*/
memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
```
