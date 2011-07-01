/**
 *  @file       World.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       01/02/04.
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
#ifndef     _WORLD_H_
#define     _WORLD_H_


#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"


#include "Containers/Containers.h"


namespace Gamedesk {


class Entity;
class Camera;
class SpacePartition;


/**
 *  World class used to contain the representation of the 3d world.
 *  @brief  World class for a 3d world.
 *  @author Marco Arsenault.
 *  @date   01/02/04.
 */
class ENGINE_API World : public Object
{
    DECLARE_CLASS(World, Object);

public:
    //! Default Constructor.
	World();

    //! Destructor.
	virtual ~World();

    virtual void Init(SpacePartition* pSpacePartition);
    virtual void Kill();

    //! Update the world.
    void Update( Double pElapsedTime );

    //! Render the world.
    void Render();
    
    //! Add a camera.
    void AddCamera( Camera* pCamera );

    //! Remove a camera;
    void RemoveCamera( Camera* pCamera );

    //! Set the current camera with the camera at index pCameraIndex.
    virtual void SetCurrentCamera( UInt32 pCameraIndex );

    //! Go to the next camera.
    virtual void NextCamera();

    //! Return the current camera.
    Camera* GetCurrentCamera();

    //! Returns the list of cameras.
    const List<Camera*>& GetCameras() const;

    virtual Entity* SpawnEntity( Class* pEntityClass, const Vector3f& pPosition = Vector3f(0,0,0), const Quaternionf& pOrientation = Quaternionf(1,0,0,0), const String& pName = "" );
    virtual void    DestroyEntity( Entity* pEntity );

    const List<Entity*>& GetEntities() const;

    UInt32 GetNbRenderedEntities() const
    {
        return mNbRenderedEntities;
    }

    Entity* LineTrace( const Vector3f& pOrigin, const Vector3f& pDir );

    void SaveWorld( const String& pFilename );
    void LoadWorld( const String& pFilename );

    virtual void Serialize( Stream& pStream );

    void InsertEntity( Entity* pEntity );
    void RemoveEntity( Entity* pEntity );

private:
    List<Camera*>			mCameras;
    Camera*					mCurrentCamera;

    class SpacePartition*	mSpacePartition;
    List<Entity*>			mEntities;

    UInt32                  mNbRenderedEntities;
    
    Bool                    mWorldInitialized;
};


} // namespace Gamedesk


#endif  //  _WORLD_H_
