/**
 *  @file       Sound3D.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       23/02/04.
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
#ifndef     _SOUND3D_H_
#define     _SOUND3D_H_


#include "Entity.h"
#include "Sound/SoundHdl.h"


namespace Gamedesk {


/**
 *  Sound3D class used to represent a sound in a 3d world.
 *  @brief  Sound3D class for a sound in a 3d world.
 *  @author Marco Arsenault.
 *  @date   23/02/04.
 */
class ENGINE_API Sound3D : public Entity
{
    DECLARE_CLASS(Sound3D, Entity)

public:
    //! Default constructor.
	Sound3D();
  
    //! Destructor.
    virtual ~Sound3D();

  	//! Update the sound3D.
	virtual void Update(Double pElapsedTime);
  
    //! Render the sound3D.
    virtual void Render() const;

    //! Set the sound handle.
    SoundHdl& GetSoundHandle();

    //! Set the sound file.
    void SetSound( const String& pSoundFileName );

private:
    SoundHdl    mSound;

	Vector3f	mLastSoundPosition;
};


} // namespace Gamedesk


#endif  //  _CAMERA3D_H_