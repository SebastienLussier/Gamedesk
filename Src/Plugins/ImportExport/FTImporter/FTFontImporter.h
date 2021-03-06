/**
 *  @file       FTFontImporter.h
 *  @brief      FreeType font importer for Gamedesk..
 *  @author     Sébastien Lussier.
 *  @date       24/01/04.
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
#ifndef     _FTFONTIMPORTER_H_
#define     _FTFONTIMPORTER_H_


#include "Resource/ResourceManager.h"


class FTIMPORTER_API FTFontImporter : public ResourceImporter
{
    DECLARE_CLASS(FTFontImporter,ResourceImporter);
public:
    FTFontImporter();
    virtual ~FTFontImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

private:
    FT_Library  mFTLibrary;
};


#endif  //  _FTFONTIMPORTER_H_
