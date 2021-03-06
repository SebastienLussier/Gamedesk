/**
 *  @file       BSPReader.h
 *  @brief      Read a BSP file into it's corresponding data structures
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
 */
/*
 *  Copyright (C) 2004 Gamedesk
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
#ifndef     _BSP_READER_H_
#define     _BSP_READER_H_


#include "FileManager/MemoryFile.h"

#include "BSPFile.h"

namespace BSP
{

class BSPReader
{
    CLASS_DISABLE_COPY(BSPReader);

public:
    BSPReader( BSPFile& pBSP );
	virtual ~BSPReader();
    
    void Read( const String& pFileName );

private:
    BSPFile&       mBSPFile;
	MemoryFile*    mMemoryFile; 
};


}

#endif  //  _BSP_READER_H_
