/**
 *  @file       ResourceManager.h
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       21/06/03.
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
#ifndef     _RESOURCE_MANAGER_H_
#define     _RESOURCE_MANAGER_H_


namespace Gamedesk {


class Resource;


class ENGINE_API ResourceImporter : public Object
{
    DECLARE_ABSTRACT_CLASS(ResourceImporter,Object);

public:
    Bool IsSupportedType( const String& pFilename ) const;
    
    virtual Class*    GetResourceClass() = 0;
    virtual Resource* Import( const String& pFilename, const String& pParams = "" ) = 0;

protected:
    ResourceImporter()
    {
    }
    
    void AddSupportedType( const String& pExtension );
    
private:
    Vector<String>  mSupportedTypes;
};


class ENGINE_API ResourceExporter : public Object
{
    DECLARE_ABSTRACT_CLASS(ResourceExporter,Object);

public:
    Bool IsSupportedType( const String& pFilename ) const;

    virtual Class*    GetResourceClass() = 0;
    virtual void      Export( Resource* resource, const String& pFilename ) = 0;

protected:
    ResourceExporter()
    {
    }
    
    void AddSupportedType( const String& pExtension );   

private:
    Vector<String>  mSupportedTypes;
};


class ENGINE_API ResourceManager : public Object
{
    DECLARE_CLASS(ResourceManager, Object);

public:
    virtual ~ResourceManager();
    
    static ResourceManager* Instance()
    {
        return &mInstance;
    }
    
    void Init();
    void Kill();

    template <typename T> T* Import( const String& pFilename, Object* pOwner = NULL, const String& pObjectName = "" )
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pFilename, T::StaticClass() );
        
        T* obj = NULL;

        try
        {
            obj = Cast<T>(importer->Import( pFilename ));
        }
        catch( Exception& /*e*/ )
        {
        }

        GD_ASSERT_M( obj != NULL, "[ResourceManager::Import] Import failed!" );

        if( pObjectName.size() == 0 )
        {
            String name;
            UInt32 pos, posStart, posEnd;

            pos = pFilename.rfind( "." );
            if( pos == String::npos )
                posEnd = pFilename.size();
            else
                posEnd = pos;

            pos = pFilename.rfind( "/" );
            if( pos == String::npos )
                posStart = 0;
            else
                posStart = pos;

            pos = pFilename.rfind( "\\" );
            if( pos != String::npos && pos > posStart )
                posStart = pos;

            name = pFilename.substr( posStart + 1, posEnd - posStart - 1 );

            obj->SetName( name );
        }
        else
            obj->SetName( pObjectName );
        

        obj->Init();
        obj->SetOwner( pOwner );

        return obj;
    }

    template <typename T> void Export( const String& pFilename, const T* pObject )
    {
        ResourceExporter* exporter = ResourceManager::Instance()->GetExporterForFile( pFilename, T::StaticClass() );
        exporter->Export( pFilename, pObject );
    }

    ResourceImporter* GetImporterForFile( const String& pFilename, Class* pResourceClassWanted );
    ResourceExporter* GetExporterForFile( const String& pFilename, Class* pResourceClassWanted );

private:
    ResourceManager();
    
    Bool                        mInitialized;
    
    Vector<ResourceImporter*>   mImporters;
    Vector<ResourceExporter*>   mExporters;

    static ResourceManager      mInstance;
};

class ENGINE_API ResourceImportException : public Exception
{
    DECLARE_EXCEPTION(ResourceImportException);

public:
    ResourceImportException( const String& pMessage, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage = pMessage;
        DebugOut();
    }
};

class ENGINE_API ResourceExportException : public Exception
{
    DECLARE_EXCEPTION(ResourceExportException);

public:
    ResourceExportException( const String& pMessage, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage = pMessage;
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _RESOURCE_MANAGER_H_
