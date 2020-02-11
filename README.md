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

Flash the compiled `.pio\build\GD32F130C6T6\firmware.bin` after compiling using some program or an STLink (default). Run `pio run -t upload` to attempt an OpenOCD upload via an STLink with a STM32F1 target setting (no idea if this works).

# Clock init 

Starts from internal 8MHz oscillator and then uses its PLL to go to 72MHz. Selection is done statically in `system_gd32f1x0.c` in the `framework-spl` folder.

Note: The claimed system speed is a paradox. Datasheet states 42MHz max, but the GD32F1x0 device code uses up 72 MHz with a PLL multiplier of 18...

```c
static void system_clock_72m_irc8m(void)
{
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    /* PLL = (IRC8M/2) * 18 = 72 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL18);
    
    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0 == (RCU_CTL0 & RCU_CTL0_PLLSTB));

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while(0 == (RCU_CFG0 & RCU_SCSS_PLL));
}
``` 

vs "The GD32F130xx device incorporates the ARM® Cortex®-M3 32-bit processor core operating at 48 MHz frequency"

¯\_(ツ)_/¯