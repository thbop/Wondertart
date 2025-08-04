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

// Just manipulate the entire floppy within memory
uint8_t g_fk_floppy[FK_FLOPPY_SIZE];
FK_SYS_FILE g_fk_file_handles[FK_FILE_HANDLE_COUNT];

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