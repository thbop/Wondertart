/*
* Copyright © 2025 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* SOFTWARE.
*/

// Make File System FAT KATT

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "fatkatt.h"

#define FLOPPY_SECTORS_PER_TRACK 18
#define FLOPPY_SECTOR_SIZE       512
#define FLOPPY_SECTOR_COUNT      2880
#define FLOPPY_SIZE              ( FLOPPY_SECTOR_SIZE * FLOPPY_SECTOR_COUNT )


int main( int argc, char **argv ) {
    // Construct the buffer
    static uint8_t floppy_buffer[FLOPPY_SIZE] = { 0 };

    // Generate default headers
    header_t *header = (header_t*)floppy_buffer;
    int8_t signature[] = "FATKATT";
    memcpy( header->signature, signature, FATKATT_SIGNATURE_SIZE );
    header->bytes_per_sector  = FLOPPY_SECTOR_SIZE;
    header->sector_count      = FLOPPY_SECTOR_COUNT;
    header->fat_size          = FATKATT_FAT_SIZE;
    header->sectors_per_track = 18;

    // Attempt to open the provided file
    if ( argc != 2 ) {
        fprintf_s( stderr, "Invalid argument! Please supply a file.\n" );
        return 1;
    }
    FILE *fp = fopen( argv[1], "wb" );
    if ( fp == NULL ) {
        fprintf_s( stderr, "Unable to open file \"%s\"!\n", argv[1] );
        return 1;
    }

    // Write
    fwrite( floppy_buffer, sizeof(uint8_t), FLOPPY_SIZE, fp );

    // Close and exit
    fclose( fp );
    return 0;
}