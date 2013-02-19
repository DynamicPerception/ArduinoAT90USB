AT90USB64x/128x Support Files for Arduino
=========================================

These files provide everything you need to build software for the AT90USB64x and AT90USB128x series of microcontrollers from Atmel.  The AT90 series microcontrollers have built-in USB and USB OTG support, with the 64x having 64KB of flash space and the 128x having 128KB of flash space.  These low-cost microcontrollers are an excellent substition for those projects that need additional flash and SRAM at a lower cost than the ATMega2560.

Also included is a functioning LUFA bootloader for these chips, to be used as an alternative to the Atmel-supplied bootloader.

Installation Instructions
-------------------------

1. Copy all files into the hardware/arduino directory inside of the Arduino application folder
2. Re-start Arduino if already running
3. Select correct board type 

Library Support
---------------

There are no known library incompatibilities, if you find any, please submit them in the issues section

Bootloaders
-----------

Included are modified LUFA CDC bootloaders.  These bootloaders are streamlined to reduce their size (down to 4K), and also feature the ability to more easily trigger the bootloader.  Now, the bootloaders will automatically enter the bootloader, and will automatically exit.  To stay in the bootloader, bring PF0 LOW during the boot cycle, and then the bootloader will remain active for approx. 30 seconds.  During this time, PC6 will be blinked high to indicate the bootloader is active. This bootloader should be considered similar to the Leonardo bootloader.

