#ifndef CHECKSUM_H
#define CHECKSUM_H

#pragma once

// Crunch the adjustment byte for this ROM / PROM combination to match checksum
unsigned char calculateChecksum(unsigned char * ROM, int size, unsigned char * PROM, int pSize);

// Inject a signature
void addSignature(unsigned char * &ROM, int size);

// Trim ROM to eprom size - NO$NES makes them too large!
void fitToEprom(unsigned char * &ROM, int &size);

#endif