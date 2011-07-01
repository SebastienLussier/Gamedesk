/**
 *  @file       ResourceHandle.h
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       19/09/04.
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
#ifndef     _RESOURCE_HANDLE_H_
#define     _RESOURCE_HANDLE_H_

#include "Containers/Containers.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {


class ResourceInfo
{
    const String& GetFile() const
    {
        return mFile;
    }

protected:
    ResourceInfo( const String& pFile )
        : mFile(pFile)
    {}

    String  mFile;
};


template <class T, class I, class F>
class ResourceHandle
{
public:
    ResourceHandle() : 
        mValue(NULL)
    {
    }

    ResourceHandle( const ResourceHandle& pOther )
    {
        *this = pOther;
    }
    
    virtual ~ResourceHandle()
    {
        if( mValue )
            F::Instance().Release( this );
    }
    
    ResourceHandle& operator = ( const ResourceHandle& pOther )
    {
        if( *this == pOther )
            return *this;

        GetHandle( pOther.mInfo );
        return *this;
    }

    Bool operator == ( const ResourceHandle& pOther )
    {
        return mInfo == pOther.mInfo;
    }

    Bool operator != ( const ResourceHandle& pOther )
    {
        return !(*this == pOther);
    }

    T* operator -> () const
    {
        GD_ASSERT( mValue != NULL );
        return mValue;
    }

    T* operator * () const
    {
        GD_ASSERT( mValue != NULL );
        return mValue;
    }

    operator Bool () const
    {
        return mValue != NULL;
    }

    const I& GetInfo() const
    {
        return mInfo;
    }

protected:
    void GetHandle( const I& pInfo )
    {
        try
        {
            if( mValue )
            {
                F::Instance().Release( mInfo );
                mValue = NULL;
            }
            
            mInfo = pInfo;
            mValue = F::Instance().Create( mInfo );
        }
        catch( Exception& e )
        {
            mValue = NULL;
        }
    }

protected:
    I       mInfo;
    T*      mValue;
};


template <class R, class I>
class ResourceFactory
{
public:
    static ResourceFactory& Instance();

    R* Create( const I& pInfo )
    {
        R* res = GetResource( pInfo );

        if( !res )
        {
            ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pInfo->GetFile(), R::StaticClass() );
            res = Cast<R>( importer->Import(pInfo) );
            res->Init();
            mLoadedResources[pInfo] = res;
        }        

        return res;
    }

    void Release( const I& pInfo )
    {
        typename ResourceMap::iterator itFind = mLoadedResources.find( pInfo );
        GD_ASSERT( itFind != mLoadedResources.end() );
                
        if( itFind->second->RemoveRef() )
        {
            itFind->second->Kill();
            GD_DELETE(itFind->second);

            mLoadedResources.erase( itFind );
        }
    }

    R* GetResource( const I& pInfo )
    {
        typename ResourceMap::iterator itFind = mLoadedResources.find( pInfo );
        if( itFind != mLoadedResources.end() )
        {
            itFind->second->AddRef();
            return itFind->second;
        }

        return NULL;
    }

private:
	typedef Map<I,R*> ResourceMap;
    ResourceMap	mLoadedResources;
};


} // namespace Gamedesk


#endif  //  _RESOURCE_HANDLE_H_
