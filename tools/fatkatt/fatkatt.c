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
    int8_t mode;  // File open mode
} fk_file_handler_t;

// Just manipulate the entire floppy within memory
// Treat this as an actual storage device... not as memory
uint8_t g_fk_floppy[FK_FLOPPY_SIZE];

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

uint8_t g_fk_fat[FK_FAT_SIZE * FK_FLOPPY_SECTOR_SIZE];

uint16_t _fk_index_fat( uint16_t fat_index ) {
    return *(uint16_t*)( g_fk_fat + fat_index * 3 ) & 0x0FFF;
}

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

    // Load the FAT into memory
    _fk_floppy_read( g_fk_fat, FK_START_FAT_SECTOR, FK_FAT_SIZE );

    return true;

}

// Returns the index of a free (closed) file handle
// Returns -1 if no free file is found
int _fk_get_file_handle() {
    for ( int i = 0; i < FK_FILE_HANDLE_COUNT; i++ )
        if ( g_fk_file_handles[i].mode == FK_FILE_CLOSED )
            return i;
    
    return -1;
}

// Gets the path filename from the raw filename
// 'grass   txt' -> 'grass.txt'
char *_fk_path_filename_from_raw( const char *raw_filename ) {
    static char out[FK_DIR_ENTRY_NAME_SIZE + 1];

    // States
    enum {
        STATE_BASE,
        STATE_SPACE,
        STATE_EXTENSION,
    };
    int state = STATE_BASE;
    int out_i = 0;

    for ( int i = 0; i < FK_DIR_ENTRY_NAME_SIZE; i++ ) {
        switch ( state ) {
            case STATE_BASE:
                if ( raw_filename[i] != ' ' ) 
                    out[out_i++] = raw_filename[i];
                else
                    state = STATE_SPACE;
                break;
            case STATE_SPACE:
                if ( raw_filename[i] != ' ' ) {
                    state = STATE_EXTENSION;
                    out[out_i++] = '.';
                    goto l_state_extension;
                }
                break;

            case STATE_EXTENSION:
                l_state_extension:
                out[out_i++] = raw_filename[i];
                break;
        }
    }

    return out;
}

// Given (specifically) a directory, searches for a sub-directory entry
// Set the search_dir to NULL if you want to search the root directory
// Returns true upon success
bool _fk_scan_directory(
    const char *dir_name,       // Which could be a file
    fk_dir_entry_t *search_dir, // If NULL, root directory
    fk_dir_entry_t *out_entry
) {
    uint8_t open_sector[FK_FLOPPY_SECTOR_SIZE];
    uint16_t fat_entry = FK_FAT_ENTRY_END;

    // Set initial fat_entry
    if ( search_dir != NULL ) {
        fat_entry = search_dir->fat_index;
        _fk_floppy_read( open_sector, search_dir->fat_index, 1 );
    } else
        _fk_floppy_read( open_sector, FK_ROOT_DIR_SECTOR, 1 );

    // Iterate through FAT sectors
    while ( true ) {
        // Iterate through entries
        for ( int i = 0; i < FK_DIR_ENTRIES_IN_SECTOR; i++ ) {
            fk_dir_entry_t *entry = ( (fk_dir_entry_t*)open_sector ) + i;
            if (
                ( entry->properties & FK_DIR_ENTRY_PROPERTY_IS_DIR &&
                strncmp(
                    dir_name,
                    (char*)entry->name,
                    FK_DIR_ENTRY_NAME_SIZE
                ) == 0 )
                ||
                ( !(entry->properties & FK_DIR_ENTRY_PROPERTY_IS_DIR) &&
                strncmp(
                    dir_name,
                    _fk_path_filename_from_raw( (char*)entry->name ),
                    FK_DIR_ENTRY_NAME_SIZE + 1
                ) == 0 )
            ) {
                memcpy( out_entry, entry, sizeof(fk_dir_entry_t) );
                return true;
            }
        }

        if ( fat_entry == FK_FAT_ENTRY_END )
            break;
        else {
            fat_entry = _fk_index_fat( fat_entry );
        }
    }

    return false;
}

// Processes a file path and returns a directory entry (via arg ptr)
// Returns true upon success
bool _fk_process_path( const char *path, fk_dir_entry_t *dir ) {
    

    return false;
}

// https://cplusplus.com/reference/cstdio/fopen/
FK_FILE *fk_fopen( const char *filename, const char *mode ) {
    return NULL;
}