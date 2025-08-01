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

#include "stdint.h"

/* Specificiation
*     Layout
*         Header - sector 0
*         Top level directory - sector 1
*         File allocation table - sectors 2-3
*         Free - sectors 4-2880
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

#define FATKATT_SIGNATURE_SIZE 7
#define FATKATT_FAT_SIZE       16

// Header section as a struct
typedef struct {
    uint16_t jmp;
    int8_t signature[FATKATT_SIGNATURE_SIZE];
    uint16_t bytes_per_sector;
    uint16_t sector_count;
    uint16_t fat_size;
    uint16_t sectors_per_track;

} __attribute__((packed)) header_t;



#endif