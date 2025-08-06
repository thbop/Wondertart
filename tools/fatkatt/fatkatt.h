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

#ifndef FATKATT_H
#define FATKATT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

/* Specificiation
*     Layout
*         Header - sector 0
*         Top level directory - sector 1
*         File allocation table - sectors 2-9
*         Free - sectors 10-2880
*     Header
*         jmp short start
*         db 'FATKATT'     ; Signature
*         dw 512           ; Bytes per sector
*         dw 2880          ; Total sectors
*         dw 16            ; FAT size (in sectors)
*         db 18            ; Sectors per track (for CHS)
*     Example directory entry
*         db 0             ; Properties (1st bit is directory flag)
*         db 'NAME    TXT' ; 11 bytes for name
*         dw 0             ; FAT table pointer
*         dw 1             ; FAT sector count
*/

// Floppy definitions
#define FK_FLOPPY_SECTORS_PER_TRACK 18
#define FK_FLOPPY_SECTOR_SIZE       512
#define FK_FLOPPY_SECTOR_COUNT      2880
#define FK_FLOPPY_SIZE \
    ( FK_FLOPPY_SECTOR_SIZE * FK_FLOPPY_SECTOR_COUNT )

// Layout definitions
#define FK_HEADER_SECTOR            0
#define FK_ROOT_DIR_SECTOR          1
#define FK_START_FAT_SECTOR         2
#define FK_FAT_SIZE                 8
#define FK_START_FREE_SECTORS       ( FK_START_FAT_SECTOR + FK_FAT_SIZE )

// Header definitions
#define FK_SIGNATURE_SIZE           7  // In bytes

// Directory entry definitions
#define FK_DIR_ENTRY_NAME_SIZE      11 // In bytes

// Implementation definitions
#define FK_FILE_HANDLE_COUNT        256
#define FK_DIR_ENTRIES_IN_SECTOR    32 // sector_size / sizeof(fk_dir_entry_t)

// Header section as a struct
typedef struct {
    uint16_t jmp;
    int8_t signature[FK_SIGNATURE_SIZE];
    uint16_t bytes_per_sector;
    uint16_t sector_count;
    uint16_t fat_size;
    uint16_t sectors_per_track;

} __attribute__((packed)) fk_header_t;


// Directory entry properties
enum {
    FK_DIR_ENTRY_PROPERTY_IS_DIR = 0x01,
};

// A directory entry as a struct
// 16 bytes
typedef struct {
    uint8_t properties;
    uint8_t name[FK_DIR_ENTRY_NAME_SIZE];
    uint16_t fat_index;
    uint16_t fat_sector_count;
} __attribute__((packed)) fk_dir_entry_t;

// Special FAT entries
enum {
    FK_FAT_ENTRY_END = 0xFFF,
};

// File open modes
enum {
    FK_FILE_CLOSED = 0x00,
    FK_FILE_READ   = 0x01,
    FK_FILE_WRITE  = 0x02,
    FK_FILE_APPEND = 0x04,

    FK_FILE_BINARY = 0x80,
};

// Front end file handler
typedef struct {
    uint8_t _id;   // File handler identifier
    uint32_t _pos; // Current position in the file
} FK_FILE;


// Standard file access functions

// Initialize the FATKATT floppy and file system
bool fk_initialize( const char *floppy_name );

FK_FILE *fk_fopen( const char *filename, const char *mode );
int fk_fclose( FK_FILE *stream );
size_t fk_fread( void *ptr, size_t size, size_t count, FK_FILE *stream );
size_t fk_fwrite( const void *ptr, size_t size, size_t count, FK_FILE *stream );
char *fk_fgets( char *str, int num, FK_FILE *stream );
long int fk_ftell( FK_FILE *stream );
int fk_fseek( FK_FILE *stream, long int offset, int origin );

#endif