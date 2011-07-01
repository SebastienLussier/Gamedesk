/**
 *  @file       ConfigFile.h
 *  @brief      Declaration of classed needed to use Config Files.
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
#ifndef     _CONFIG_FILE_H_
#define     _CONFIG_FILE_H_


#include "Variant.h"


namespace Gamedesk {
	

/**
 *  Define the content of a config file section. A config file section is
 *  defined this way :
 *      [MySection]
 *      myVar1 = myValue1
 *      myVar2 = myValue2
 *      ...
 *  This class can be used to add or get the value of a variable in the section.
 *  @brief  Manage a config file section.
 *  @author Sébastien Lussier.
 */
class CORE_API ConfigSection
{
    CLASS_DISABLE_COPY(ConfigSection);

    //! ConfigFile has access to this class.
    friend class ConfigFile;
    
public:
    //! Constructor.
    ConfigSection( const String& pSectionName, const class ConfigFile& pConfigFile );

    //! Destructor.
    virtual ~ConfigSection();

    /**
     *  Access to variable using their name as the key with operator []
     *  @param  pVarName        Name of the variable to access.
     *  @return The variable as a variant. If not found then a new variable is created with the given name.
     */
    Variant& operator [] ( const String& pVarName );

    /**
     *  Access a variable in the section. Similar to the [] operator but more secure because
     *  you can specifie a default value in case the variable is not found.
     *  @param  pVarName        Name of the variable to get.
     *  @param  pDefaultValue   Default value that will be returned if the variable is not found.
     *  @return A variant containing the variable's value.
     */
    Variant& Get( const String& pVarName, Variant pDefaultValue );

    /**
     *  Remove all variables in this section.
     */
    void Clear();

    /**
     *  Get this section's name.
     *  @return This section's name.
     */
    const String& GetName() const;
    
private:
    /**
     *  Save this section to a stream.
     *  @param  pOut    Output stream.
     */
    void Save( std::ostream& pOut );

    /**
     *  Load this section from a stream.
     *  @param  pInt    Input stream.
     */
    void Load( std::istream& pIn );

private:
    Map<String,Variant*>        mVars;          //!< Map of variants in this section.
    const class ConfigFile&     mConfigFile;    //!< Config file associated to this section.
    const String                mSectionName;   //!< Name of this section. 
};


/**
 *  Define a config file.  A config file usually contains one or more config section, that 
 *  in turn contains variables.
 *  @author Sébastien Lussier.
 */
class CORE_API ConfigFile
{
public:
    
    /**
     *  Iterator class used to iterate through all the sections in the config file.
     */
    class CORE_API Iterator
    {
    public:
        Iterator( ConfigFile& pConfigFile );
        Iterator& operator ++ ();
        Bool IsValid() const;
        operator Bool() const;
        ConfigSection& operator* () const;

    private:
        Iterator( const Iterator& pOther );
        Bool operator = ( const Iterator& pOther );

        ConfigFile&                             mConfigFile;
        Map<String,ConfigSection*>::iterator    mIterator;
    };
    friend class Iterator;

public:
    /**
     *  Constructor.
     *  @param  pFileName   Name of the config file.
     *  @param  pFolder     Folder where to find this config file (by default, "config/").
     */
    ConfigFile( const String& pFileName, const String& pFolder = "Config/" );

    //! Destructor.
    virtual ~ConfigFile();

    //! Save the content of the config file on disk.
    void Save();   
    
    //! Load the content of the config file from disk.
    void Load();

    //! Empty the config file of all it's sections and variables.
    void Clear();

    /**
     *  Access to config sections using their name as the index with the operator [].
     *  @param  pSectionName    Name of the section to get.
     *  @return The section with the given name. If it was not found, then it will be created.
     */
    ConfigSection& operator [] ( const String& pSectionName );

    /**
     *  Access a variable in a section. Similar to the [][] operators but more secure because
     *  you can specifie a default value in case the variable is not found.
     *  @param  pSectionName    Name of the section to get.
     *  @param  pVarName        Name of the variable to get.
     *  @param  pDefaultValue   Default value that will be returned if the variable is not found.
     *  @return A variant containing the variable's value.
     */
    Variant& Get( const String& pSectionName, const String& pVarName, Variant pDefaultValue );

    //!< Get the filename.
    const String& GetFileName() const;

#ifdef GD_DEBUG
    //!< Print the content of the config file.
    std::ostream& DebugPrint( std::ostream& pOut );
#endif

private:
    String                      mFileName;      //!< Config filename.
    Map<String,ConfigSection*>  mSections;      //!< Map of sections associated with their names.
};


class CORE_API InvalidConfigFileException : public Exception
{
    DECLARE_EXCEPTION(InvalidConfigFileException);

public:
    InvalidConfigFileException( const String& pFileName, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "This file is not a valid config file : ";
        mMessage += pFileName;
        mMessage += ".";
        DebugOut();
    }
};


class CORE_API ConfigFileMissingValueException : public Exception
{
    DECLARE_EXCEPTION(ConfigFileMissingValueException);

public:
    ConfigFileMissingValueException( const String& pConfigFilename, const String& pSection, const String& pVariable, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Undefined variable [";
        mMessage += pSection;
        mMessage += "][";
        mMessage += pVariable;
        mMessage += "] in config file \"";
        mMessage += pConfigFilename;
        mMessage += ".";
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _CONFIG_FILE_H_
