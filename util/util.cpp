
#include "util.h"
#include "../zip/miniz.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


// Unzip prom from a ROM
bool extractPromFromZip(const char * filepath, unsigned char * &prom, int &size) {

    mz_zip_archive zip_archive;
    mz_bool status;

    // Now try to open the archive.
    memset(&zip_archive, 0, sizeof(zip_archive));

    // You can provide the zip data in memory as I did...
    status = mz_zip_reader_init_file(&zip_archive, filepath, 0);

    if (!status)
    {
        cout << "zip file appears invalid..." << endl;
        return false;
    }

    // Get number of file in zip
    int noFilesInZip = mz_zip_reader_get_num_files(&zip_archive);

    // Look for prom
    for (int i = 0; i < noFilesInZip; i++) {
        
        // Check this zipped file is ok
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
        {
            cout << "zip file read error..." << endl;
            mz_zip_reader_end(&zip_archive);
            return false;
        }

        // Only interested in prom
        if(string(file_stat.m_filename) != "security.prm") {
            continue;
        }

        // Unzip the file to heap.
        size_t uncompressed_size = file_stat.m_uncomp_size;
        void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncompressed_size, 0);
        if (!p)
        {
            cout << "mz_zip_reader_extract_file_to_heap() failed..." << endl;
            mz_zip_reader_end(&zip_archive);
            return false;
        }

        prom = (unsigned char*)p;
        size = uncompressed_size;

        mz_zip_reader_end(&zip_archive);
        return true;
    }

   return false;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() < ending.length()) return false;
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
}

// Open ROM / PROM and read the bytes into array
unsigned char * getROM(const char* filepath, int &romsize) {

    // Support empty PROM file
    if ((filepath == NULL) || (filepath[0] == '\0')) {
        romsize = 0;
        cout << "* Building for Security IC Replacement PCB *" << endl;
        return {};
    }

    // unzip PROM?
    if (hasEnding(string(filepath), string(".zip"))) {
        unsigned char * prom;
        bool success = extractPromFromZip(filepath, prom, romsize);

        return prom;
    } 
    else 
    {
        // Reading size of file
        FILE * file = fopen(filepath, "r+");
        if (file == NULL) return 0;
        fseek(file, 0, SEEK_END);
        long int size = ftell(file);
        fclose(file);
        // Reading data to array of unsigned chars
        file = fopen(filepath, "r+");
        unsigned char * ROM = (unsigned char *) malloc(size);
        romsize = fread(ROM, sizeof(unsigned char), size, file);
        fclose(file);
        return ROM;
    }
}

std::string getfilename(std::string path)
{
    path = path.substr(path.find_last_of("/\\") + 1);
    size_t dot_i = path.find_last_of('.');
    return path.substr(0, dot_i);
}

void createConvertedROM(const char * nameROM, unsigned char *ROM, int size) {
    // Write to file
    std::string out_file = string("converted_") + getfilename(string(nameROM)) + string(".BIN");
    std::ofstream out;
    out.open(out_file, std::ios_base::binary);
    out.write(reinterpret_cast<char*>(ROM) , size );
    out.close();
    cout << "New file created at: " << out_file << endl;
}

// Utility to print a byte with a particular message
void printByte(const char * message, unsigned char byte, LogType log_type) {
#ifndef DEBUG_LOGGING
    if (log_type == log_always)
#endif
    {
        std::cout << message << "0x" << std::hex << (0xFF & byte) << std::endl;
    }
}  
void printByte(const char * message, unsigned char byte) {
    printByte(message, byte, log_always);
} 
void printByte_debug(const char * message, unsigned char byte) {
    printByte(message, byte, log_debug);
}