/**
*  @file       TestProperties.h
*  @author     Sébastien Lussier.
*  @date       09/03/06.
*/
/*
*  Copyright (C) 2006 Gamedesk
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
#include "World/Entity.h"


namespace Gamedesk {


class ENGINE_API TestProperties : public Entity
{
    DECLARE_CLASS(TestProperties, Entity);

public:
    TestProperties() 
    {
        mBool   = false;
        mChar   = 'a';
        mInt16  = -16;
        mInt32  = -32;
        mInt64  = -64;
        mByte   = 8;
        mUInt16 = 16;
        mUInt32 = 32;
        mUInt64 = 64;
        mFloat  = 0.33f;
        mDouble = 0.6666;
        mColor3 = Color3f(0.50f, 0.75f, 1.0f);
        mColor4 = Color4f(1.0f, 0.75f, 0.50f, 1.0f);
        mVector3 = Vector3f(1.0f, 2.0f, 3.0f);
        mQuaternion.SetIdentity();
        mString = "String";
        mEnum = First;
    }
    
    virtual ~TestProperties() {}

    //! MyEnum
    enum MyEnum
    {
        First,      //!< First item
        Second,     //!< Second item
        Third,      //!< Third item
        MAX = 0xFFFFFFFF
    };

properties:
    Bool        mBool;
    Char        mChar;
    Int16       mInt16;
    Int32       mInt32;
    Int64       mInt64;
    Byte        mByte;
    UInt16      mUInt16;
    UInt32      mUInt32;
    UInt64      mUInt64;
    Float       mFloat;
    Double      mDouble;
    Color3f     mColor3;
    Color4f     mColor4;
    Vector3f    mVector3;
    Quaternionf mQuaternion;
    String      mString;
    MyEnum      mEnum;
};


} // namespace Gamedesk
