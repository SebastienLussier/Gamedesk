/**
 *  @file       ConfigFile.cpp
 *  @brief      Definition of classed needed to use Config Files.
 *  @author     Sébastien Lussier.
 *  @date       02/08/03.
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
#include "Core.h"
#include "ConfigFile.h"
#include "FileManager/FileManager.h"


namespace Gamedesk {
	
	
///////////////////////////////////////////////////////////////////////////////
// ConfigSection
ConfigSection::ConfigSection( const String& pSectionName, const ConfigFile& pConfigFile ) :
    mConfigFile( pConfigFile ),
    mSectionName( pSectionName )
{
}

ConfigSection::~ConfigSection()
{
    Clear();
}

void ConfigSection::Clear()
{
    Variant*                          variant = NULL;
    Map<String,Variant*>::iterator    itMap;

    for( itMap = mVars.begin(); itMap != mVars.end(); itMap++ )
    {
        variant = (*itMap).second;
        GD_DELETE(variant);
    }

    mVars.clear();
}

void ConfigSection::Save( std::ostream& pOut )
{
    Variant*                          variant = NULL;
    Map<String,Variant*>::iterator    itMap;
    String                            value;
    String                            name;

    for( itMap = mVars.begin(); itMap != mVars.end(); itMap++ )
    {
        variant    = (*itMap).second;

        name       = (*itMap).first;
        value      = (String)(*variant);
        
        pOut << name.c_str() << " = " << value.c_str() << std::endl;
    }
}

void ConfigSection::Load( std::istream& pIn )
{
    String varName;
    String equalSign;
    String varValue;
    String nextWord;

    char              buffer[512];
    std::stringstream lineStream;

    while( pIn.good() )
    {
        varName    = "";
        equalSign  = "";
        varValue   = "";

        if( pIn.peek() == '[' )
            break;
        
        pIn.getline( buffer, 512 );

        lineStream.clear();
        lineStream.str(buffer);
                
        lineStream >> varName;
        lineStream >> equalSign;

        // Put the rest of the line in the value.
        lineStream >> varValue;
        while( !lineStream.fail() )
        {
            lineStream >> nextWord;
            if( !lineStream.fail() )
            {
                varValue += " ";
                varValue += nextWord;
            }
        }

        if( varName.empty() )
            continue;   // Line is empty.

        if( equalSign.empty() || varValue.empty() )
            throw InvalidConfigFileException( mConfigFile.GetFileName(), Here );

        // Create a new variable.
        (*this)[varName] = varValue;
    }
}

Variant& ConfigSection::operator [] ( const String& pVarName )
{
    Variant*                          variant = NULL;
    Map<String,Variant*>::iterator    itMap = mVars.find( pVarName );
        
    // If we've found the section in our map, return it immediately.
    if( itMap != mVars.end() )
    {
        variant = (*itMap).second;
    }
    // Otherwise we need to create it.
    else
    {
        variant = GD_NEW(Variant, this, "Core::Config::ConfigSection::Variant");
        mVars[pVarName] = variant;
    }

    return *variant;
}

Variant& ConfigSection::Get( const String& pVarName, Variant pDefaultValue )
{
    Variant*                          variant = NULL;
    Map<String,Variant*>::iterator    itMap = mVars.find( pVarName );
        
    // If we've found the section in our map, return it immediately.
    if( itMap != mVars.end() )
    {
        variant = (*itMap).second;
    }
    // Otherwise we need to create it.
    else
    {
        variant = GD_NEW(Variant, this, "Core::Config::ConfigSection::Variant")( pDefaultValue );
        mVars[pVarName] = variant;
    }

    return *variant;
}

const String& ConfigSection::GetName() const
{
    return mSectionName;
}

///////////////////////////////////////////////////////////////////////////////
// ConfigFile
///////////////////////////////////////////////////////////////////////////////
ConfigFile::ConfigFile( const String& pFileName, const String& pFolder )
{
    mFileName = pFolder + pFileName;
}

ConfigFile::~ConfigFile()
{
    Clear();
}

void ConfigFile::Clear()
{
    ConfigSection*                          section = NULL;
    Map<String,ConfigSection*>::iterator    itMap;

    for( itMap = mSections.begin(); itMap != mSections.end(); itMap++ )
    {
        section = (*itMap).second;
        GD_DELETE(section);
    }

    mSections.clear();
}

void ConfigFile::Load()
{
    std::ifstream   inFile( mFileName.c_str() );

    if( inFile.fail() )
    {
        inFile.close();
        throw FileNotFoundException( mFileName, Here );
    }

    String              sectionName;
    String              nextWord;
    char                buffer[512];
    std::stringstream   lineStream;
    
    inFile.getline(buffer, 512);
    while( inFile.good() )
    {        
        lineStream.clear();
        lineStream.str(buffer);
                
        // Read section name ("[Section Name]").
        lineStream >> sectionName;
        while( !lineStream.fail() )
        {
            lineStream >> nextWord;
            if( !lineStream.fail() )
            {
                sectionName += " ";
                sectionName += nextWord;
            }
        }        
        
        // Find opening and closing bracket (must be at start and end of section name).
        if( sectionName.at(0) != '[' || sectionName.at( sectionName.size() -1 ) != ']' )
        {
            inFile.close();
            throw InvalidConfigFileException( mFileName, Here );
        }

        // Extract the section name from the string (remove "[]")
        sectionName = sectionName.substr( 1, sectionName.length() - 2 );

        // Load this section.
        (*this)[sectionName].Load(inFile);

        // Read another section...
        inFile.getline(buffer, 512);
    }
}

void ConfigFile::Save()
{
    std::ofstream   outFile( mFileName.c_str() );

    if( outFile.fail() )
    {
        outFile.close();
        throw FileNotFoundException( mFileName, Here );
    }

    ConfigSection*                          section = NULL;
    Map<String,ConfigSection*>::iterator    itMap;

    for( itMap = mSections.begin(); itMap != mSections.end(); itMap++ )
    {
        section = (*itMap).second;

        outFile << "[" << (*itMap).first.c_str() << "]" << std::endl;
        section->Save( outFile );
        outFile << std::endl;
    }

    outFile.close();
}

ConfigSection& ConfigFile::operator [] ( const String& pSectionName )
{
    ConfigSection*                          section = NULL;
    Map<String,ConfigSection*>::iterator    itMap = mSections.find( pSectionName );
        
    // If we've found the section in our map, return it immediately.
    if( itMap != mSections.end() )
    {
        section = (*itMap).second;
    }
    // Otherwise we need to create it.
    else
    {
        section = GD_NEW(ConfigSection, this, "Core::Config::ConfigFile::ConfigSection")( pSectionName, *this );
        mSections[pSectionName] = section;
    }

    return *section;
}

Variant& ConfigFile::Get( const String& pSectionName, const String& pVarName, Variant pDefaultValue )
{
    return operator[]( pSectionName ).Get( pVarName, pDefaultValue );
}

const String& ConfigFile::GetFileName() const
{
    return mFileName;
}

ConfigFile::Iterator::Iterator( ConfigFile& pConfigFile ) :
    mConfigFile( pConfigFile ),
    mIterator( pConfigFile.mSections.begin() )
{
}

ConfigFile::Iterator& ConfigFile::Iterator::operator ++ ()
{
    ++mIterator;
    return *this;
}

Bool ConfigFile::Iterator::IsValid() const
{
    return mIterator != mConfigFile.mSections.end();
}

ConfigFile::Iterator::operator Bool() const
{
    return IsValid();
}

ConfigSection& ConfigFile::Iterator::operator* () const
{
    return *(*mIterator).second;
}


#ifdef GD_DEBUG
std::ostream& ConfigFile::DebugPrint( std::ostream& pOut )
{
    ConfigSection*                          section = NULL;
    Map<String,ConfigSection*>::iterator    itMap;

    for( itMap = mSections.begin(); itMap != mSections.end(); itMap++ )
    {
        section = (*itMap).second;

        pOut << "[" << (*itMap).first.c_str() << "]" << std::endl;
        section->Save( pOut );
        pOut << std::endl;
    }

    return pOut;
}
#endif // DEBUG


} // namespace Gamedesk
