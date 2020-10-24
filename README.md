# PIO + GD32F130C6

Test project for the GigaDevices GD32F130C6 using PlatformIO

# Installation **(UPDATE)**

Previously, this project required users to install a custom `framework-spl` package from a `.zip` file in this repository. This is **not necessary anymore** since the package was uploaded by my in the PIO trusted registry. A project can specify usage of my package by adding 

```ini
[env]
; globally override framework-spl for all environments.
platform_packages = 
    maxgerhardt/framework-spl@2.10300.0
```

to the `platformio.ini`. 

This `framework-spl` version contains `GD32F1x0_Firmware_Library_v3.1.0` (to support htis `GD32F130`) as well as `GD32F10x_Firmware_Library_V2.1.2` (to support e.g., `GD32F103`).

To compile this project, simply clone it and execute `pio run` un it. 

# IDE export 

Use `pio init --ide=<ide here>`. See [docs](https://docs.platformio.org/en/latest/userguide/project/cmd_init.html).

# Flashing

Flash the compiled `.pio\build\GD32F130C6T6\firmware.bin` after compiling using some program or an STLink (default). Run `pio run -t upload` to attempt an OpenOCD upload via an STLink with a STM32F1 target setting (no idea if this works).

# Clock init 

Starts from internal 8MHz oscillator and then uses its PLL to go to 72MHz. Selection is done statically in `system_gd32f1x0.c` in the `framework-spl` folder.

Note: The claimed system speed is a paradox. Datasheet states 48MHz max, but the GD32F1x0 device code uses up 72 MHz with a PLL multiplier of 18...

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