# PIO + GD32F130C6

Test project for the GigaDevices GD32F130C6 using PlatformIO

# Installation

Go to your PlatformIO home folder (Windows: `C:\Users\<user>\.platformio`, Unix/Max: `/home/<user>/.platformio`). Then go into `packages`. If the folder `framework-spl` exists, delete it. 

Unpack the `framework-spl.zip` in the `packages` folder so that the directory structure is now 

```
packages/
| - framework-spl/
| |--  gd32/
| |--  platformio/
| |--  stm32/
| |--  package.json
```

(This folder contains the `GD32F1x0_Firmware_Library_v3.1.0` files)

Then attempt compilation using `pio run`. 

# IDE export 

Use `pio init --ide=<ide here>`. See [docs](https://docs.platformio.org/en/latest/userguide/project/cmd_init.html).

# Flashing

Flash the compiled `C:\Users\Maxi\Desktop\pio-gd32f130c6\.pio\build\GD32F130C6T6\firmware.bin` using some program or an STLink (default). Run `pio run -t upload` to attempt an OpenOCD upload via an STLink with a STM32F1 target setting (no idea if this works).

# Clock init 

Starts from internal 8MHz oscillator and then uses its PLL to go to 72MHz. Selection is done statically in `system_gd32f1x0.c` in the `framework-spl` folder. 