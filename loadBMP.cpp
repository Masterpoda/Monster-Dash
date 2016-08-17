#include <iostream>
#include <fstream>
#include "loadBMP.h"

typedef unsigned char byte;

/**************************************************************************//**
 * @author Dr. Hinker
 *
 * Read into memory an RGB image from an uncompressed BMP file.\n
 * Return true for success, false for failure.
 *
 * @param[in]  filename - The name of the file to load
 * @param[out] Rows - How many rows are in the loaded image
 * @param[out] Cols - How many columns are in the loaded image
 * @param[out] iPtr - An pointer to where the image is stored in memory
 *
 * @returns     bool - Whether the image was loaded successfuly or not
 *****************************************************************************/

bool loadBMP( std::string filename, int &Rows, int &Cols, byte* &iPtr )
{
    std::ifstream infile(filename, std::ios_base::binary);
    if ( !infile )
    {
        std::cerr << "Unable to open file: " << filename << "\n";
        return false;
    }

    // process BMP file header
    bool fileFormatOK = false;
    char bChar, mChar, temp[50];
    int bitsPerPixel;

    infile.get(bChar);
    infile.get(mChar);
    int fileOffsetToPixels;
    if ( bChar == 'B' && mChar == 'M' ) // should start with "BM" for "BitMap"
    {
        infile.get(temp, 9);
        infile.read((char*)&fileOffsetToPixels, 4);
        infile.get(temp, 5);       // Skip 4 fields we don't care about
        infile.read((char*)&Cols, 4);
        infile.read((char*)&Rows, 4);
        infile.get(temp, 3);
        infile.read((char*)&bitsPerPixel, 2);
//      byte low = bitsPerPixel >> 8;
//      byte high = bitsPerPixel & 0x000F;
//      bitsPerPixel = (low << 8) + high;

        if ( Cols > 0 && Cols <= 4000 &&
                Rows > 0 && Rows <= 4000 &&
                bitsPerPixel == 24 &&
                !infile.eof())
        {
            fileFormatOK = true;
        }
    }

    if ( !fileFormatOK )
    {
        infile.close();
        std::cerr << "Not a valid 24-bit bitmap file: " << filename << std::endl;
        return false;
    }

    // allocate memory

    iPtr = new unsigned char[ Rows * BytesPerRow( Cols ) ];
    if ( !iPtr )
    {
        infile.close();
        std::cerr << "Unable to allocate memory for " << Rows << " x " << Cols << " bitmap: " << filename << std::endl;
        return false;
    }

    // seek to the right place in the file
    infile.seekg(fileOffsetToPixels, std::ios_base::beg);

    // read image RGB data
    unsigned char* cPtr = iPtr;
    for ( int i = 0; i < Rows; i++ )
    {
        int j;
        char red, green, blue;
        for ( j = 0; j < Cols; j++ )
        {
            infile.get(blue);	// Blue color value
            infile.get(green);	// Green color value
            infile.get(red);	// Red color value
            cPtr[2] = blue;
            cPtr[1] = green;
            cPtr[0] = red;
            cPtr += 3;
        }
        int k = 3 * Cols;			// number of bytes already read
        for ( ; k < BytesPerRow( Cols ); k++ )
        {
            infile.get(temp[0]);			// ignore padding
            *( cPtr++ ) = 0;
        }
    }

    // and... we should be done
    if (infile.eof())
    {
        infile.close();
        std::cerr << "Corrupt input file, premature EOF found: " << filename << std::endl;
        return false;
    }

    infile.close();	// Close the file
    return true;
}

/**************************************************************************//**
 * @author Dr. Weiss
 *
 * Get the number of bytes in a row
 *
 * @param[in]  Cols - The number of columns in the file
 *
 * @returns     int - The number of bytes in a row
 *****************************************************************************/
inline int BytesPerRow( int Cols )
{
    return ( ( 3 * Cols + 3 ) >> 2 ) << 2;
}

