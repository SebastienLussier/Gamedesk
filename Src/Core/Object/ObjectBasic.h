/**
 *  @file       ObjectBasic.h
 *  @brief      Macro definition for the Reflective factory implementation.
 *  @author     Sébastien Lussier.
 *  @date       19/01/02.
 *  @note
 *
 *  Based on the article by Chris Hargrove on Reflective factory that can be
 *  found on GameDev.
 *
 *  References:
 *      http://www.gamedev.net/reference/articles/article1415.asp
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
#ifndef     _OBJECT_BASIC_H_
#define     _OBJECT_BASIC_H_


#include "Core.h"


// Internal macro, do not use.
#define DECLARE_BASE_CLASS( clsName )                           \
    private:                                                    \
        static Gamedesk::Class mClass;                          \
    public:                                                     \
        typedef clsName Self;                                   \
        virtual Gamedesk::Class* GetClass() const               \
        {                                                       \
            return StaticClass();                               \
        }

// Internal macro, do not use.
#define CLASS_DEFINE_STATIC( clsName, clsSuper )                \
        DECLARE_BASE_CLASS( clsName );                          \
        typedef clsSuper Super;                                 \
        static Gamedesk::Class* StaticClass()                   \
        {                                                       \
             return &mClass;                                    \
        }                                                       \
        friend Gamedesk::Stream& operator <<( Gamedesk::Stream& s, clsName*& c ) \
        {                                                       \
            return s << *(Object**)&c;                          \
        }

// Internal macro, do not use.
// Allocator for an abstract class.
#define CLASS_DEFINE_ABSTRACTNEW( clsName )                     \
    private:                                                    \
        static clsName* StaticNew() { GD_ASSERT_M(false, "Calling AllocateNew() on an abstract class is an error!"); return 0; }

// Internal macro, do not use.
// Allocator for a normal class
#define CLASS_DEFINE_CONCRETENEW( clsName )                     \
    public:                                                     \
        static clsName* StaticNew() { return GD_NEW(clsName, 0, "StaticNew::#clsName"); }

// Internal macro, do not use.
// Allocator for a class allocated through a factory.
#define CLASS_DEFINE_FACTORYNEW( clsName, factoryClsName )      \
    public:                                                     \
    static clsName* StaticNew()                                 \
    {                                                           \
        return Gamedesk::Cast<clsName>( factoryClsName::Instance()->Create( clsName::StaticClass() ) ); \
    }


/**
 * Disable object copy
 */
#define CLASS_DISABLE_COPY( clsName )                           \
    private:                                                    \
    clsName( const clsName& pOther );                           \
    const clsName& operator = ( const clsName& pOther );

/**
 *  Declare a class as inheriting from another class.
 *  Use this macro just after the opening bracket of your class.
 *  @brief  Declare a class as inheriting from another class.
 *  @param  clsName     the class name.
 *  @param  clsSuper    the parent class name.
 */
#define DECLARE_CLASS( clsName, clsSuper )                      \
    CLASS_DEFINE_CONCRETENEW( clsName );                        \
    CLASS_DEFINE_STATIC( clsName, clsSuper );                   \
    CLASS_DISABLE_COPY( clsName );

/**
*  Declare a class as inheriting from another class.
*  Use this macro just after the opening bracket of your class.
*  @brief  Declare a class as inheriting from another class.
*  @param  clsName     the class name.
*  @param  clsSuper    the parent class name.
*/
#define DECLARE_RESOURCE_CLASS( clsName, clsSuper )                 \
    CLASS_DEFINE_FACTORYNEW( clsName, clsFactory, clsFactoryClass );\
    CLASS_DEFINE_STATIC( clsName, clsSuper );                       \
    CLASS_DISABLE_COPY( clsName );

/**
 *  Declare a class as inheriting from another class.
 *  Use this macro just after the opening bracket of your abstract class.
 *  @brief  Declare an abstract class as inheriting from another class.
 *  @param  clsName     the abstract class name.
 *  @param  clsSuper    the parent class name.
 */
#define DECLARE_ABSTRACT_CLASS( clsName, clsSuper )             \
    CLASS_DEFINE_ABSTRACTNEW( clsName );                        \
    CLASS_DEFINE_STATIC( clsName, clsSuper );                   \
    CLASS_DISABLE_COPY( clsName );

/**
*  Declare a class as inheriting from another class.
*  Use this macro just after the opening bracket of your abstract class.
*  @brief  Declare an abstract class as inheriting from another class.
*  @param  clsName     the abstract class name.
*  @param  clsSuper    the parent class name.
*  @param  clsFactory  the factory class that will be used to allocate objects of this class.
*/
#define DECLARE_ABSTRACT_RESOURCE_CLASS( clsName, clsSuper, clsFactory )    \
    CLASS_DEFINE_FACTORYNEW( clsName, clsFactory );                         \
    CLASS_DEFINE_STATIC( clsName, clsSuper );                               \
    CLASS_DISABLE_COPY( clsName );                                          \
    virtual Gamedesk::Class* SerializeAs()                                  \
    {                                                                       \
        return clsName::StaticClass();                                      \
    }

/**
 *  Macro used by the Object class only.
 *  @brief  Macro used by the Object class only.
 *  @param  clsName     the class name.
 */
#define DECLARE_OBJECT_ABSTRACT_CLASS( clsName )                \
    CLASS_DEFINE_ABSTRACTNEW( clsName );                        \
    DECLARE_BASE_CLASS( clsName );                              \
    static Gamedesk::Class* StaticClass()                       \
    {                                                           \
        return &mClass;                                         \
    }                                                           \
    static void StaticRegisterProperties(Class*)                \
    {                                                           \
    }                                                           \
    CLASS_DISABLE_COPY( clsName );

/**
 *  This macro has to be used for every DECLARE_CLASS() macro
 *  used.  It will initialize static data members.
 *  @brief  Implement class by initializing static members.
 *  @param  clsName     the class name.
 */
#define IMPLEMENT_CLASS( clsName )                              \
    Gamedesk::Class clsName::mClass( #clsName,                  \
                           Super::StaticClass(),                \
                           sizeof( clsName ),                   \
                           (Object*(*)())clsName::StaticNew,    \
                           clsName::StaticRegisterProperties);  \
    Gamedesk::Class* autoRegisterClass##clsName = clsName::StaticClass();

/**
 *  This macro has to be used for every DECLARE_ABSTRACT_CLASS() macro
 *  used.  It will initialize static data members.
 *  @brief  Implement class by initializing static members.
 *  @param  clsName     the abstract class name.
 */
#define IMPLEMENT_ABSTRACT_CLASS( clsName )                     \
    Gamedesk::Class clsName::mClass( #clsName,                  \
                           Super::StaticClass(),                \
                           sizeof( clsName ),                   \
                           (Object*(*)())clsName::StaticNew,    \
                           clsName::StaticRegisterProperties,   \
                           true );                              \
    Gamedesk::Class* autoRegisterClass##clsName = clsName::StaticClass();

/**
 *  Macro used by the Object class only.
 *  @brief  Macro used by the Object class only.
 *  @param  clsName     the name of the class.
 */
#define IMPLEMENT_OBJECT_CLASS( clsName )                       \
    Gamedesk::Class clsName::mClass( #clsName,                  \
                           NULL,                                \
                           sizeof( clsName ),                   \
                           (Gamedesk::Object*(*)())clsName::StaticNew);   \
    Gamedesk::Class* autoRegisterClass##clsName = clsName::StaticClass();


#define DECLARE_ENUM_INFO()                                             \
    public:                                                             \
        static Gamedesk::UInt32 GetSize();                              \
        static Gamedesk::UInt32 GetIndex(Gamedesk::UInt32 pValue);      \
        static Gamedesk::UInt32 GetIndex(const char* pString);          \
        static Gamedesk::UInt32 GetValue(Gamedesk::UInt32 pIndex);      \
        static Gamedesk::UInt32 GetStringOfValue(const char* pString);  \
        static const char* GetString(Gamedesk::UInt32 pIndex);          \
        static const char* GetValueOfString(Gamedesk::UInt32 pValue);   \
        static const Gamedesk::UInt32 mValues[];                        \
        static const char*  mStrings[];                                 \
        static const Gamedesk::UInt32 mSize;                              

#endif  //  _OBJECT_BASIC_H_
