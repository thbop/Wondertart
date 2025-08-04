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

#include "fatkatt.h"

// System file handler
typedef struct {
    fk_dir_entry_t dir_entry;
} fk_file_handler_t;

// Just manipulate the entire floppy within memory
uint8_t g_fk_floppy[FK_FLOPPY_SIZE];
fk_file_handler_t g_fk_file_handles[FK_FILE_HANDLE_COUNT];

// Initialize the FATKATT floppy and file system
// Returns false upon failure
bool fk_initialize( const char *floppy_name ) {
    // Ensure that all file handles are set to zero
    memset( g_fk_file_handles, 0, sizeof(g_fk_file_handles) );

    FILE *fp = fopen( floppy_name, "rb" );
    if ( fp == NULL ) {
        fprintf( stderr, "Unable to load floppy file \"%s\"!\n", floppy_name );
        return false;
    }

    fread( g_fk_floppy, sizeof(uint8_t), FK_FLOPPY_SIZE, fp );


    fclose( fp );
    return true;

}

// Reads a given number of sectors into a given buffer
void _fk_floppy_read( uint8_t *dst, uint32_t lba, uint32_t sector_count ) {
    // Actual implementation will be different
    memcpy(
        dst,
        g_fk_floppy + ( lba * FK_FLOPPY_SECTOR_SIZE ),
        sector_count * FK_FLOPPY_SECTOR_SIZE
    );
}

// Writes a 512 byte block of memory onto the disk
void _fk_floppy_write_sector( uint32_t lba, uint8_t *src ) {
    // Actual implementation will be different
    memcpy(
        g_fk_floppy + ( lba * FK_FLOPPY_SECTOR_SIZE ),
        src,
        FK_FLOPPY_SECTOR_SIZE
    );
}

// https://cplusplus.com/reference/cstdio/fopen/
FK_FILE *fk_fopen( const char *filename, const char *mode ) {

}