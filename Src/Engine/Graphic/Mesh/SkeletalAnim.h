 /**
 *  @file       SkeletalAnim.h
 *  @brief      Definition of an animation for a skeletal mesh.
 *  @author     Sébastien Lussier.
 *  @date       29/08/04.
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
#ifndef     _SKELETAL_ANIM_H_
#define     _SKELETAL_ANIM_H_


namespace Gamedesk {


template <class T>
class Keyframe
{
public:
    Float   mTime;
    T       mValue;
};


template <class T>
class Channel
{
public:
    T GetValue( Float pTime )
    {
        UInt32 keyIndex = 0;
        while( (keyIndex+1) < mKeys.size() && mKeys[keyIndex].mTime < pTime )
            keyIndex++;
        return mKeys[keyIndex].mValue;
    }

    UInt32                  mCurrentKey;
    Vector< Keyframe<T> >   mKeys;
};


class BoneAnimation
{
public:
    enum BoneChannel
    {
        ChannelTransX,
        ChannelTransY,
        ChannelTransZ,
        ChannelRotX,
        ChannelRotY,
        ChannelRotZ,
        NUM_CHANNELS
    };

    Channel<Float>          mChannels[NUM_CHANNELS];
    String                  mName;
};


class SkeletalAnim
{
public:
    SkeletalAnim( const String& pName )
        : mName(pName)
    {
    }

    String                  mName;
    Float                   mAnimLength;
    Vector<BoneAnimation>   mBoneAnims;
};


} // namespace Gamedesk


#endif  //  _SKELETAL_ANIM_H_
