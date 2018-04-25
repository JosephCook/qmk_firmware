/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _MCUCONF_H_
#define _MCUCONF_H_

#define KL6x_MCUCONF

/*
 * HAL driver system settings.
 */
#if 1
#define KINETIS_MCG_MODE            KINETIS_MCG_MODE_PEE
#define KINETIS_PLLCLK_FREQUENCY    96000000UL

#define KINETIS_SYSCLK_FREQUENCY    48000000UL
//#define KINETIS_CLKDIV1_OUTDIV1 2

#define KINETIS_BUSCLK_FREQUENCY    48000000UL
//#define KINETIS_CLKDIV1_OUTDIV2 2

#define KINETIS_FLASHCLK_FREQUENCY  24000000
//#define KINETIS_CLKDIV1_OUTDIV4 4
#endif

#if 0
/* crystal-less FEI mode - 48 MHz with internal 32.768 kHz crystal */
#define KINETIS_MCG_MODE            KINETIS_MCG_MODE_FEI
#define KINETIS_MCG_FLL_DMX32       1           /* Fine-tune for 32.768 kHz */
#define KINETIS_MCG_FLL_DRS         1           /* 1464x FLL factor */
#define KINETIS_SYSCLK_FREQUENCY    59998208    /* 32.768 kHz * 1831 (~48 MHz) */
#define KINETIS_CLKDIV1_OUTDIV1     1           /* do not divide system clock */
#endif

/*
 * SERIAL driver system settings.
 */
#define KINETIS_SERIAL_USE_UART0              TRUE

/*
 * USB driver settings
 */
#define KINETIS_USB_USE_USB0                  TRUE

#endif
