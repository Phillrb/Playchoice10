# Playchoice10

Tools for the Playchoice 10 arcade machine

## PC10

A tool for converting lackluster Playchoice 10 titles such as 'Golf' and 'Tennis' to more exciting games such as 'Balloon Fight', 'Mario Bros.' and 'Duck Hunt' without removing the existing security prom.
Also useful for porting NES games to the Playchoice 10.

Port '.a22' NO$NES assembled '.BIN' files to work with specific host Playchoice carts. With this tool you can alter a custom instructions ROM to work with specific Playchoice carts. This allows the new ROM to work with the existing PROM on the host cart. No 'Replacement Security IC PCB' is required and you do not need to burn a new Ricoh PROM.

## Download

Download the latest release for Windows, macOS or Linux:
<https://github.com/Phillrb/Playchoice10/releases>

## Code

This repo using git submodules to pull in dependencies so it's best to clone recursively:
`git clone --recursive https://github.com/Phillrb/Playchoice10.git`

## Usage

* Create '.a22' file (as described in KLOV thread or NO$NES site - see links below)
  * _Example '.a22' files are already in 'files/a22'_
* Use NO$NES to assemble a '.bin' Playchoice Instruction ROM
* Copy the '.bin' file to the 'files' directory
  * _Example assembled '.bin' file is 'files/BALLOON_FIGHT.BIN'_
* Source the official ROM file for the hosting cart (dump it yourself or be careful when downloading it)
  * Copy the ROM ZIP to the 'files' directory
  * _You will find no Playchoice 10 ROMs in this repository_
* Run `./PC10.out` or `./PC10.exe` from the command line
  * `./PC10.out -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip`
* A new '.bin' file will be created in the 'files/converted' directory
  * _Example converted '.bin' file is 'files/converted/BALLOON_FIGHT.BIN'_
* Burn the converted '.bin' to a 27c64 eprom and place in U3 on the cart
* Replace U1 and U2 with suitable ROMs for the new game
* Place / remove solder at solder points SL1, SL2 & SL3 as necessary (see compatibility list)

## Background

Typically Playchoice 10 games have 3 eproms and a serial prom. U1 and U2 are the game code eproms, U3 is the instruction eprom and the serial prom is a tiny security IC. The Playchoice 10 bios calculates a checksum between the data held in U3 and the prom. The checksum can be calculated for any combination or prom and instruction rom using the PC10 tool here.

The KLOV thread (_see link below_) outlines a method to address the problem by means of eliminating the prom entirly with a 'Replacement Security IC PCB'. This PCB contains 2 buffers that simply pull up the data and counter lines on the prom socket when data is requested. The checksum was then calculated for this arrangement by the NO$NES assembler (_see link below_). The downside to this arrangement is that the replacement PCBs are fiddly to create but the positive is that there is no need to source any physical prom. The PC10 tool still supports the use of these replacement secuirty ICs by passing an empty string ("") as the '-c' param. 

This PC10 tool was created to enable conversion of some games to others without removing the existing prom.

## Building

_This section is for those that want to build the tool themselves (rather than download the binary / executable from 
<https://github.com/Phillrb/Playchoice10/releases>)_

To make the build process easier on Linux & macOS you can now use [CMake](https://cmake.org/install/)

First create and move to a build directory:

```bash
mkdir build
cd build
```

Then for Linux / macOS Terminal:

```bash
cmake ..
make
```

or for Windows Command Prompt open the Visual Studio solution.

## Usage

To create a new 'Balloon Fight' instruction ROM that would only work in a 'Golf' cartridge:

```./PC10.out -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip```

For help with arguments pass '-h' on the command line:

```
PC10 v1.0.1
@NES4Life - https://github.com/Phillrb/Playchoice10

Usage: -b bin_file -c cart_rom_zip
* Pass path to bin file as '-b' param
* Pass path to zipped ROM file for hosting cart as '-c' param
  * Pass "" as the '-c' param if using the security IC replacement PCB

e.g. Making a 'Balloon Fight' conversion for a hosting 'Golf' cartridge:
Mac / Linux:
./PC10.out -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip
Windows:
./PC10.exe -b files/BALLOON_FIGHT.BIN -c files/pc_golf.zip

A new .BIN file will be created at 'converted_BALLOON_FIGHT.BIN' 
```

## Steps taken by PC10

PC10 will:

* Copy '.BIN' file to memory
* Extract 'security.prom' from zipped host ROM
* Trim '.BIN' data to fit a 27c64 eprom
* Inject a '@NES4LIFE' signature for vanity
* Calculate the checksum for the eprom & host prom combination
* Alter the last byte in the '.BIN' data to make the checksum balance
* Write the '.BIN' data to a new 'converted' file

## Compatibility list

_Notes in brackets are the points that need soldering on the cart_

### Original Playchoice games

_These can be the host cart or the conversion game:_

* Golf (SL2)
* Tennis (SL2)
* Balloon Fight (SL2)
* Mario Bros (SL2)
* Baseball (SL2)
* Duck Hunt (SL3)
* Excitebike (SL3)
* Hogans Alley (SL3)
* Wild Gunman (SL3)
* Kung Fu (SL1,SL3)
* Super Mario Bros. (SL1, SL3)
* Volleyball (SL1, SL3)
* 1942 (SL1, SL2)

## Links

* KLOV thread: <https://forums.arcade-museum.com/showthread.php?t=350798>
* NO$NES: <https://problemkaputt.de/nes.htm>

## 3rd Party Software

MAME implementation of RP5H01 - Ricoh prom:
// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
<https://github.com/mamedev/mame/blob/master/src/devices/machine/rp5h01.h>
<https://github.com/mamedev/mame/blob/master/src/devices/machine/rp5h01.cpp>

MAME implementation of Playchoice 10:
// license:BSD-3-Clause
// copyright-holders:Ernesto Corvi,Brad Oliver
<https://github.com/mamedev/mame/blob/master/src/mame/machine/playch10.cpp>

Miniz (used for unzip):
Copyright 2013-2014 RAD Game Tools and Valve Software
Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
<https://github.com/richgel999/miniz>

xgetopt (for Windows compatibility):
Copyright (c) 2016 hklo.tw@gmail.com
<https://github.com/matthklo/xgetopt>

NO$NES:
NO$NES (c) Martin Korth
<https://problemkaputt.de/nes.htm>

## Thanks

A big thanks to @GadgetFreak on UKVAC for his MAME debugging skills and initial code submissions
