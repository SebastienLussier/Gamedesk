/**
*  @file       DDSImporter.cpp
*  @brief      DDS Texture Importer, because DevIL is crap with DDS.
*  @author     Sébastien Lussier.
*  @date       24/04/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  Gamedesk
*  http://gamedesk.type-cast.com
*
*/
#include "DevILImpExp.h"
#include "DDSImporter.h"
#include "FileManager/MemoryFile.h"
#include "Graphic/Image/Image.h"
#include "Graphic/Texture/Texture.h"


IMPLEMENT_CLASS(DDSImporter);


const UInt32 DDS_FOURCC  = 0x00000004;
const UInt32 DDS_RGB     = 0x00000040;
const UInt32 DDS_RGBA    = 0x00000041;
const UInt32 DDS_DEPTH   = 0x00800000;

const UInt32 DDS_COMPLEX = 0x00000008;
const UInt32 DDS_CUBEMAP = 0x00000200;
const UInt32 DDS_VOLUME  = 0x00200000;

const UInt32 FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
const UInt32 FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
const UInt32 FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

struct DDS_PIXELFORMAT
{
    UInt32  dwSize;
    UInt32  dwFlags;
    UInt32  dwFourCC;
    UInt32  dwRGBBitCount;
    UInt32  dwRBitMask;
    UInt32  dwGBitMask;
    UInt32  dwBBitMask;
    UInt32  dwABitMask;
};

struct DXTColBlock
{
    UInt16  col0;
    UInt16  col1;

    Byte    row[4];
};

struct DXT3AlphaBlock
{
    UInt16  row[4];
};

struct DXT5AlphaBlock
{
    Byte    alpha0;
    Byte    alpha1;

    Byte    row[6];
};

struct DDS_HEADER
{
    UInt32  dwSize;
    UInt32  dwFlags;
    UInt32  dwHeight;
    UInt32  dwWidth;
    UInt32  dwPitchOrLinearSize;
    UInt32  dwDepth;
    UInt32  dwMipMapCount;
    UInt32  dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    UInt32  dwCaps1;
    UInt32  dwCaps2;
    UInt32  dwReserved2[3];
};


DDSImporter::DDSImporter()
{
    AddSupportedType( "dds" );
}


Class* DDSImporter::GetResourceClass()
{
    return Texture::StaticClass();
}

///////////////////////////////////////////////////////////////////////////////
// Swap the bytes in a 32 bit value
inline void swap_endian(void *val)
{
#ifdef GD_BIG_ENDIAN
    unsigned int *ival = (unsigned int *)val;

    *ival = ((*ival >> 24) & 0x000000ff) |
            ((*ival >>  8) & 0x0000ff00) |
            ((*ival <<  8) & 0x00ff0000) |
            ((*ival << 24) & 0xff000000);
#else
    val;
#endif
}


Resource* DDSImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    Bool texture1d  = false;
    Bool texture2d  = false;
    Bool texture3d  = false;
    Bool cubemap    = false;
    Bool compressed = false;

    Image::Format format;

    DDS_HEADER  ddsh;

    MemoryFile   memFile(pFilename.c_str(), true);
    Byte*        memory = memFile.GetMemory();
    UInt32       fileSize = memFile.GetSize();
    UInt32       filePointer = 0;
    
    if( fileSize == 0 || memory == 0 )
        return NULL;

    // read in file marker, make sure its a DDS file
    GD_ASSERT(filePointer+4 <= fileSize);
    if(strncmp((const char*)&memory[filePointer], "DDS ", 4) != 0)
        return NULL;
    filePointer += 4;
    

    // read in DDS header
    GD_ASSERT(filePointer+sizeof(ddsh) <= fileSize);
    memcpy(&ddsh, &memory[filePointer], sizeof(ddsh));
    filePointer += sizeof(ddsh);

    swap_endian(&ddsh.dwSize);
    swap_endian(&ddsh.dwFlags);
    swap_endian(&ddsh.dwHeight);
    swap_endian(&ddsh.dwWidth);
    swap_endian(&ddsh.dwPitchOrLinearSize);
    swap_endian(&ddsh.dwDepth);
    swap_endian(&ddsh.dwMipMapCount);
    swap_endian(&ddsh.dwCaps1);
    swap_endian(&ddsh.dwCaps2);
    swap_endian(&ddsh.ddspf.dwSize);
    swap_endian(&ddsh.ddspf.dwFlags);
    swap_endian(&ddsh.ddspf.dwFourCC);
    swap_endian(&ddsh.ddspf.dwRGBBitCount);
    swap_endian(&ddsh.ddspf.dwRBitMask);
    swap_endian(&ddsh.ddspf.dwGBitMask);
    swap_endian(&ddsh.ddspf.dwBBitMask);
    swap_endian(&ddsh.ddspf.dwABitMask);    

    // Check if image is a cubemap
    if (ddsh.dwCaps2 & DDS_CUBEMAP)
        cubemap = true;
    else if ((ddsh.dwCaps2 & DDS_VOLUME) && (ddsh.dwDepth > 0))
        texture3d = true;
    else if( ddsh.dwHeight == 1 || ddsh.dwWidth == 1 )
        texture1d = true;
    else 
        texture2d = true;

    // figure out what the image format is
    if( ddsh.ddspf.dwFlags & DDS_FOURCC )
    {
        switch(ddsh.ddspf.dwFourCC)
        {
        case FOURCC_DXT1:
            format = Image::Format_DXT1;
            compressed = true;
            break;
        case FOURCC_DXT3:
            format = Image::Format_DXT3;
            compressed = true;
            break;
        case FOURCC_DXT5:
            format = Image::Format_DXT5;
            compressed = true;
            break;
        default:
            return NULL;
        }
    }
    else if( ddsh.ddspf.dwFlags == DDS_RGBA && ddsh.ddspf.dwRGBBitCount == 32 )
    {
        format = Image::Format_B8G8R8A8;
        compressed = false;
    }
    else if( ddsh.ddspf.dwFlags == DDS_RGB  && ddsh.ddspf.dwRGBBitCount == 32 )
    {
        format = Image::Format_B8G8R8A8;
        compressed = false;
    }
    else if( ddsh.ddspf.dwFlags == DDS_RGB  && ddsh.ddspf.dwRGBBitCount == 24 )
    {
        format = Image::Format_B8G8R8; 
        compressed = false;
    }
    else 
    {
        return NULL;
    }

    if( ddsh.dwDepth == 0 )
        ddsh.dwDepth = 1;

    if( ddsh.dwMipMapCount == 0 )
        ddsh.dwMipMapCount = 1;

    Image img[6];

    // load all surfaces for the image (6 surfaces for cubemaps)
    for( Int32 i = 0; i < (cubemap ? 6 : 1); i++ )
    {
        img[i].Create( ddsh.dwWidth, ddsh.dwHeight, format, ddsh.dwMipMapCount, ddsh.dwDepth );

        GD_ASSERT(filePointer+img[i].GetDataSize() <= fileSize);
        memcpy( img[i].GetData(), &memory[filePointer], img[i].GetDataSize() );
        filePointer += img[i].GetDataSize();
    }

    Texture* texture = NULL;
    if( texture1d )
    {
        Texture1D* tex1D = Cast<Texture1D>(Texture1D::StaticClass()->AllocateNew( pFilename ));
        tex1D->Create( img[0], false );
        texture = tex1D;
    }
    else if( texture2d )
    {
        Texture2D* tex2D = Cast<Texture2D>(Texture2D::StaticClass()->AllocateNew( pFilename ));
        tex2D->Create( img[0], false );
        texture = tex2D;
    }
    else if( texture3d )
    {
        Texture3D* tex3D = Cast<Texture3D>(Texture3D::StaticClass()->AllocateNew( pFilename ));
        tex3D->Create( img[0], false );
        texture = tex3D;
    }
    else if( cubemap )
    {
        Cubemap* cubemap = Cast<Cubemap>(Cubemap::StaticClass()->AllocateNew( pFilename ));

        Vector<Image*>   vimg;
        vimg.push_back( &img[0] );
        vimg.push_back( &img[1] );
        vimg.push_back( &img[2] );
        vimg.push_back( &img[3] );
        vimg.push_back( &img[4] );
        vimg.push_back( &img[5] );
        cubemap->Create( vimg, false );
        texture = cubemap;
    }

    return texture;
}