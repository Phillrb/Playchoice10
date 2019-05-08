# Playchoice10
Tools for the Playchoice 10 arcade machine

## PC10
v1.0.0

Port .a22 NO$NES assembled '.BIN' files to work with specific host Playchoice carts.
With this tool you can alter a custom instructions ROM to work with specific Playchoice carts.
This allows the new ROM to work with the existing PROM on the host carts.
No 'Replacement Security IC PCB' is required and you do not need to burn a new Ricoh PROM.

This tool is particularly useful for converting lackluster titles such as 'Golf' and 'Tennis' to more exciting or classic games such as 'Balloon Fight' and 'Mario Bros.' 

Steps:
* Create .a22 file (as described in KLOV thread or NO$NES site)
* Use NO$NES to assemble a '.bin' ROM file
* Copy the '.bin' file in the 'files' directory
* Source the official ROM file for the hosting cart (dump it yourself or be careful when downloading it)
  * Copy the ROM ZIP to the 'files' directory
* Run ./PC10.out from the command line (tested on macOS)
  * ./PC10.out -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip
* A new '.bin' file will be created in the 'converted' directory
* Burn the image to a 27c64 eprom and place in U3
* Replace U1 and U2 with a suitable target game

## Background
Supported Playchoice 10 games have 3 Eproms and a serial prom. U1 and U2 are the game code eproms, U3 is the instruction eprom, and the serial prom is a tiny security IC. The Playchoice 10 bios calculates a checksum between the data held in U3 and the prom. The checksum can be calculated for any combination or prom and instruction rom.

The KLOV thread outlines a method to address the problem by means of eliminating the prom entirly with a 'Replacement Security IC PCB'. This PCB contains 2 buffers that simply pull up the data and counter lines on the prom socket when data is requested. The checksum was then calculated by the NO$NES assembler for this arrangement. The downside to this arrangement is that the replacement PCBs are fiddly to create but the positive is that there is no need to source any physical prom.

This PC10 tool was created to enable conversion of some games to others without removing the existing prom.

## Usage

Command line arguments:
```./PC10.out -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip```

Pass '-h' on the command line:
```
PC10 v1.0.0
@NES4Life - https://github.com/Phillrb/Playchoice10

Usage: -b bin_file -c cart_rom_zip
* Pass path to bin file as '-b' param
* Pass path to zipped ROM file for hosting cart as '-c' param
  * Pass "" as the '-c' param if using the security IC replacement PCB

e.g. Making a 'Balloon Fight' conversion for a hosting 'Golf' cartridge:
./PC10.out -b BALLOON_FIGHT.BIN -c pc_golf.zip

A new .BIN file will be created called 'files/converted/BALLOON_FIGHT.BIN' 
```

## Links
* KLOV thread: https://forums.arcade-museum.com/showthread.php?t=350798
* NO$NES: https://problemkaputt.de/nes.htm

## 3rd Party Software
Mame implementation of RP5H01 - Ricoh prom:
// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
https://github.com/mamedev/mame/blob/master/src/devices/machine/rp5h01.h
https://github.com/mamedev/mame/blob/master/src/devices/machine/rp5h01.cpp

Mame implementation of Playchoice 10:
// license:BSD-3-Clause
// copyright-holders:Ernesto Corvi,Brad Oliver
https://github.com/mamedev/mame/blob/master/src/mame/machine/playch10.cpp

Miniz (used for unzip):
Copyright 2013-2014 RAD Game Tools and Valve Software
Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
https://github.com/richgel999/miniz

NO$NES:
NO$NES (c) Martin Korth
https://problemkaputt.de/nes.htm
