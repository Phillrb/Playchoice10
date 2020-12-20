// BSD 3-Clause License

// Copyright (c) 2019, Phillip Riscombe-Burton
// All rights reserved.

#include <iostream>
#include "../util/util.h"
#include "../util/checksum.h"
#include "../util/xgetopt/xgetopt.h"

using namespace std;

#define PC10_VERSION "1.0.1"

// Main flow of program

void run(char * rom, char * prom, bool skip_signature) {
    // Select ROM and PROM
    const char * nameROM = rom;
    // PROMs can be extracted from ROM zip files automatically
    const char * namePROM = prom;

    // To use PROM replacement PCB just pass an empty string "" as the name of the PROM
    // const char * namePROM = ""; 
    
    int size = 0;
    int pSize = 0;   

    // Read in the ROM / PROM data
    cout << "ROM: " << nameROM << endl;
    unsigned char * ROM = getROM(nameROM, size);
    cout << "ROM size: " << size << endl;

    cout << "PROM: " << namePROM << endl;
    unsigned char * PROM = getROM(namePROM, pSize);
    if(pSize > 0) cout << "PROM size: " << pSize << endl;

    cout << "+++++++++++++++" << endl;

    // NO$NES .BIN files are too large
    fitToEprom(ROM, size);

    // Add signature?
    if(!skip_signature) {
        addSignature(ROM, size);
    }

    // Calculate checksum & crunch last byte
    unsigned char lastByte = calculateChecksum(ROM, size, PROM, pSize);
    printByte( "Adjustment byte: ", lastByte );

    // Make the adjustment byte change to balance the checksum
    ROM[size - 1] = lastByte;

    // Make the new file
    createConvertedROM(nameROM, ROM, size);
}





// Usage message
static void show_usage()
{
    std::cerr << "PC10 v" << PC10_VERSION << std::endl
              << "@NES4Life - https://github.com/Phillrb/Playchoice10" << std::endl << std::endl
              << "Usage: "
              << "-b bin_file "
              << "-c cart_rom_zip" 
              << std::endl
              << "* Pass path to bin file as '-b' param"
              << std::endl
              << "* Pass path to zipped ROM file for hosting cart as '-c' param" << std::endl
              << "   * Pass \"\" as the '-c' param if using the security IC replacement PCB"
              << std::endl << std::endl
              << "e.g. Making a 'Balloon Fight' conversion for a hosting 'Golf' cartridge:"
              << std::endl
#ifdef _WIN32
              << "./PC10.exe -b BALLOON_FIGHT.BIN -c pc_golf.zip"
#else
              << "./PC10.out -b BALLOON_FIGHT.BIN -c pc_golf.zip"
#endif
              << std::endl << std::endl
              << "A new .BIN file will be created called 'converted_BALLOON_FIGHT.BIN'"
              << std::endl << std::endl;
}


// Command line argument handling
int main(int argc, char* argv[])
{
    char *bvalue = NULL;
    char *cvalue = NULL;
    int index;
    bool skip_signature = false;
    int c;

    xopterr = 0;

    if (argc < 3) {
        show_usage();
        return 1;
    }

    while ((c = xgetopt (argc, argv, "b:c:hsv")) != -1)
        
        switch (c)
        {
            case 'b':
                bvalue = xoptarg;
                break;
            case 'c':
                cvalue = xoptarg;
                break;
            case 'h':
                show_usage();
                return 0;
            case 's':
                skip_signature = true;
                break;
            case 'v':
                cout << "PC10 v" << PC10_VERSION << endl;
                return 0;
                break;
            case '?':
                if (xoptopt == 'b' || xoptopt == 'c')
                fprintf (stderr, "Option -%c requires an argument.\n", xoptopt);
                else if (isprint (xoptopt))
                fprintf (stderr, "Unknown option `-%c'.\n", xoptopt);
                else
                fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        xoptopt);
                return 1;
            default:
                abort ();
        }

    for (index = xoptind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    // Go with the names / params supplied
    run(bvalue, cvalue, skip_signature);

    return 0;
}

