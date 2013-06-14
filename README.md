AT90USB64x/128x Support Files for Arduino
=========================================

This package provides everything you need to build software for the AT90USB64x and AT90USB128x series of microcontrollers from Atmel.  The AT90 series microcontrollers have built-in USB and USB OTG support (647 and 1287 processors), with the 64x having 64KB of flash space, 4K of RAM and the 128x having 128KB of flash space, 8K of RAM.  These low-cost microcontrollers are an excellent substition for those projects that need additional flash and SRAM at a lower cost than the ATMega2560.

With this package, you also get full access to using the LUFA libraries directly in your Arduino sketches, without having to write and compile your applications in another IDE system or using command-line Makefiles.  All of the current LUFA libraries are included as part of the core and can be used directly. LUFA is an excellent USB control (host and device) set from Dean Camera that provides nearly complete control of most USB specifications on AVR hardware.  Nearly every kind of Device and Host mode is supported - from Audio, to PTP, and even basics like Mice and Keyboards.

In addition to the standard LUFA libraries, we have also started to create a series of easier, Arduino-oriented libraries that don't require any low-level knowledge of how to use the LUFA libraries, and provide specific set of activities that can be used in as little as two to three lines of code.  See the KeyboardHost example to show how easy it can be to read an attached USB keyboard.
 
Arduino > 1.5.2
---------------
You must be using a version of Arduino greater than 1.5.2 to use this package.  Versions older than 1.5.x do not support the 3rd party hardware extensions at all, and 1.5.2 has several issues with the 3rd party hardware extensions.  We reccommend using the latest nightly build of the arduino 1.5 series.

Installation Instructions (Arduino > 1.5.2)
-------------------------

1. Copy the contents of the Arduino-1.5/hardware directory into the hardware/ directory inside of the Arduino application folder
2. Re-start Arduino if already running
3. Select correct board type
 

