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


int main( int argc, char **argv ) {
    // Construct the buffer
    static uint8_t floppy_buffer[FK_FLOPPY_SIZE] = { 0 };

    // Generate default headers
    fk_header_t *header = (fk_header_t*)floppy_buffer;
    int8_t signature[] = "FATKATT";
    memcpy( header->signature, signature, FK_SIGNATURE_SIZE );
    header->bytes_per_sector  = FK_FLOPPY_SECTOR_SIZE;
    header->sector_count      = FK_FLOPPY_SECTOR_COUNT;
    header->fat_size          = FK_FAT_SIZE;
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
    fwrite( floppy_buffer, sizeof(uint8_t), FK_FLOPPY_SIZE, fp );

    // Close and exit
    fclose( fp );
    return 0;
}