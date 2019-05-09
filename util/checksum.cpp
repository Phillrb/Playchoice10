
// BSD 3-Clause License

// Copyright (c) 2019, Phillip Riscombe-Burton
// All rights reserved.

#include "rp5h01_device.h"
#include "util.h"
#include <iostream>

using namespace std;

// Calculate the checksum for this combination of ROM and PROM bytes
void add_chksum(int prom_byte, unsigned char &a, int i, unsigned char &e, unsigned char ROM_byte) {

    // This is puled from the PC-10 bios
    a = ((((unsigned char)i & 0xff) + 0x01) & 0xff) ^ prom_byte;
    printByte_debug( "xor: ", a);

    printByte_debug( "rom: ", ROM_byte);
    if ((e + a) > 0xff)
        a = ((a + e) - ROM_byte) - 0x01;
    else
        a = (a + e) - ROM_byte;
    e = a;
    printByte_debug( "e: ", a);
}

// Go through every byte of the ROM and calulate a rolling checksum with the assigned PROM value
unsigned char calculateChecksum(unsigned char * ROM, int size, unsigned char * PROM, int pSize) {

    // The last byte in the ROM can be adjusted to make the checksum balance
    // Useful if you want to adjust the PC-10 Instruction ROM contents
    unsigned char lastByte = 0x00; 

    // Create a PROM instance
    bool isPromReplacement = (pSize == 0);
    rp5h01_device m_rp5h01 = rp5h01_device(PROM);
    m_rp5h01.device_reset();

    unsigned char a, e;
    unsigned char ROM_byte = 0x00;
    unsigned char prom_byte = 0x00;

    unsigned char seed_byte = ROM[0x01];
    printByte_debug("Seed byte: ", seed_byte);

    // Checksum must equal 0xc9 at the end - taken from PC-10 bios
    unsigned char checksum = 0xc9;

    // Reset e to seed byte from ROM
    e = seed_byte;
    
    // Playchoice bios writes 0xCC to prom at start
    prom_w(&m_rp5h01, 0xCC);
    
    // Playchoice bios writes 0xCF to 'a'
    a = 0xCF;

    // Loop through every byte
    for (unsigned int i = 0; i < size; i++)
    {
        // Get this ROM byte
        ROM_byte = ROM[i];
        
        // Configure PROM for this checksum
        printByte_debug( "a: ", a );
        prom_w(&m_rp5h01, a);

        // Get PROM data
        prom_byte = prom_r(&m_rp5h01, isPromReplacement);
        printByte_debug( "prom: ", prom_byte );

        // Last byte ? Let's brute-force it
        if (i == (size - 1)) {

            unsigned char a2 = 0;
            unsigned char e2 = 0;

            #ifdef DEBUG_LOGGING
                cout << "***** LAST BYTE ***** " << endl;
            #endif  
            for (int r = 0; r <= 0xff; r++) {
                a2 = a;
                e2 = e;
                add_chksum(prom_byte, a2, i , e2 , r);
                if (e2 == checksum) {
                    printByte_debug("ADJUSTMENT BYTE: ", r);
                    e = e2;
                    a = a2;
                    lastByte = r;
                    break;
                }
                #ifdef DEBUG_LOGGING
                    cout << "=========" << endl; 
                #endif  
            } 
        }
            else {
            // calculate the checksum for this ROM byte and PROM byte combination
            add_chksum(prom_byte, a, i , e , ROM_byte);
        }

        // Move on - taken from bios
        a&= 0xee;
        a = (a + 0x01) & 0xff;

        #ifdef DEBUG_LOGGING
            cout << "---------" << endl; 
        #endif          
    }

    cout << "Checksum: " << ((e == checksum) ? "ok" : "!! BAD !!") << endl;
    
    return lastByte;
}

void fitToEprom(unsigned char * &ROM, int &size) {
    // If the ROM is too long then clip it to the right size!
    if (size > 0x2000) {
        size = 0x2000;
        cout << "Trimmed .BIN file to fit eprom: 0x2000" << endl;
    }
}

void addSignature(unsigned char * &ROM, int size) {
    // Replace end of NO$NES signature with '@nes4life'
    unsigned char sig_NO_NES[] = {0x61, 0x73, 0x73, 0x65, 0x6D, 0x62, 0x6C, 0x65, 0x72};
    unsigned char sig_NES4Life[] = {0x40, 0x6E, 0x65, 0x73, 0x34, 0x6C, 0x69, 0x66, 0x65};
    const int sig_size = 9;

    bool injected = false;

    // Loop through all bytes in ROM
    for (int i = 0; i < size - sig_size && !injected; i++) {
        // Overwrite signature match
        for (int j = 0; j < sig_size && ROM[i + j] == sig_NO_NES[j]; j++) {
            if(j == sig_size - 1) {
                for (int k = 0; k < sig_size; k++) {
                    ROM[i + k] = sig_NES4Life[k];
                }
                injected = true;
            }
        }
    }
    if(injected) {
        cout << "Signature: ok" << endl;
    } else {
        cout << "Signature: no" << endl;
    }
}
