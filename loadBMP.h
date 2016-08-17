/**************************************************************************//**
* @file
*****************************************************************************/

#ifndef _BMPLOAD_H_
#define _BMPLOAD_H_

#include <string>

typedef unsigned char byte;

/*!Load a bitmap file into memory*/
bool loadBMP( std::string filename, int &Rows, int &Cols, byte* &imgPtr );

/*!Get the number of bytes per row in a file*/
inline int BytesPerRow( int NumCols );

#endif

