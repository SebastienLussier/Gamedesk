/**
 *  @file       ResourceManager.cpp
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#include "Engine.h"
#include "ResourceManager.h"
#include "Module/ModuleManager.h"
#include "Object/ObjectIterator.h"
#include "FileManager/FileManager.h"
#include "Debug/PerformanceMonitor.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS( ResourceImporter );
IMPLEMENT_ABSTRACT_CLASS( ResourceExporter );

IMPLEMENT_CLASS( ResourceManager );


Bool ResourceImporter::IsSupportedType( const String& pFilename ) const
{
    Vector<String>::const_iterator it;
    it = std::find( mSupportedTypes.begin(), mSupportedTypes.end(), pFilename ); 
    return it != mSupportedTypes.end();
}

void ResourceImporter::AddSupportedType( const String& pExtension )
{
    if( !IsSupportedType(pExtension) )
        mSupportedTypes.push_back( pExtension );
}

Bool ResourceExporter::IsSupportedType( const String& pFilename ) const
{
    Vector<String>::const_iterator it;
    it = std::find( mSupportedTypes.begin(), mSupportedTypes.end(), pFilename ); 
    return it == mSupportedTypes.end();
}

void ResourceExporter::AddSupportedType( const String& pExtension )
{
    if( !IsSupportedType(pExtension) )
        mSupportedTypes.push_back( pExtension );
}


ResourceManager ResourceManager::mInstance;

ResourceManager::ResourceManager() :
    mInitialized(false)
{
}

ResourceManager::~ResourceManager()
{
    
}

void ResourceManager::Init()
{
    if( !mInitialized )
    {
        mInitialized = true;

        ModuleManager::Instance()->LoadModulesInFolder( "Plugins/ImportExport/" );

        Class::Iterator itImpClasses( ResourceImporter::StaticClass() );
        for( ; itImpClasses.IsValid(); ++itImpClasses )
        {
            Profile( (*itImpClasses)->GetName() );
            mImporters.push_back( Cast<ResourceImporter>((*itImpClasses)->AllocateNew()) );
        }

        Class::Iterator itExpClasses( ResourceExporter::StaticClass() );
        for( ; itExpClasses.IsValid(); ++itExpClasses )
        {
            Profile( (*itExpClasses)->GetName() );
            mExporters.push_back( Cast<ResourceExporter>((*itExpClasses)->AllocateNew()) );
        }
    }
}


void ResourceManager::Kill()
{
    Vector<ResourceImporter*>::iterator     itImporters;
    Vector<ResourceExporter*>::iterator     itExporters;

    for( itImporters = mImporters.begin(); itImporters != mImporters.end(); ++itImporters )
        GD_DELETE(*itImporters);

    for( itExporters = mExporters.begin(); itExporters != mExporters.end(); ++itExporters )
        GD_DELETE(*itExporters);
}


ResourceImporter* ResourceManager::GetImporterForFile( const String& pFilename, Class* pResourceClassWanted )
{
    Init();

    if( !FileManager::FileExist( pFilename ) )
        throw FileNotFoundException( pFilename, Here );

    String strExtension = FileManager::GetFileExtension( pFilename );

    for( Vector<ResourceImporter*>::const_iterator it = mImporters.begin(); it != mImporters.end(); ++it )
    {
        if( (*it)->IsSupportedType( strExtension ) && (*it)->GetResourceClass() == pResourceClassWanted )
        {
            return *it;
        }
    }

    return NULL;
}

ResourceExporter* ResourceManager::GetExporterForFile( const String& pFilename, Class* pResourceClassWanted )
{
    Init();

    String strExtension = FileManager::GetFileExtension( pFilename );

    for( Vector<ResourceExporter*>::const_iterator it = mExporters.begin(); it != mExporters.end(); ++it )
    {
        if( (*it)->IsSupportedType( strExtension ) && (*it)->GetResourceClass() == pResourceClassWanted )
        {
            return *it;
        }
    }

    return NULL;
}


} // namespace Gamedesk
