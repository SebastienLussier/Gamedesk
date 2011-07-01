/**
 *  @file       SingletonPattern.h
 *  @brief	    Macro definition for the Singleton design pattern.
 *  @author     Sébastien Lussier.
 *  @date       20/06/02.
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
#ifndef     _SINGLETON_PATTERN_H_
#define     _SINGLETON_PATTERN_H_


/**
 *  Macro used to declare a class as a singleton.
 *  Add necessery methods and members to a class to make it singleton.
 *  @brief  Add necessery methods and members to a class to make it singleton.
 *  @param  aClass      the class name.
 */
#define DECLARE_SINGLETON( aClass )                 \
            public:                                 \
            static aClass* Instance();              \
            private:                                \
            static aClass* mInstance;

#define DECLARE_ABSTRACT_SINGLETON( aClass )        \
            DECLARE_SINGLETON( aClass )
            

/**
 *  Very lame macro for implemeting a singleton.
 *  @brief  Define necessery methods and members to a class to make it singleton.
 *  @param  aClass      the class name.
 */
#define IMPLEMENT_SINGLETON( aClass )               \
    aClass* aClass::mInstance = NULL;               \
    aClass* aClass::Instance()                      \
    {                                               \
        if( mInstance == NULL )                     \
        {                                           \
        mInstance = GD_NEW(aClass, 0, "Singleton::#aClass"); \
        }                                           \
                                                    \
        return mInstance;                           \
    }

/**
 *  Very lame macro for implemeting a singleton.
 *  @brief  Define necessary methods and members to a class to make it singleton.
 *  @param  aClass      the class name.
 */
#define IMPLEMENT_ABSTRACT_SINGLETON( aClass )      \
    aClass* aClass::mInstance = NULL;               \
    aClass* aClass::Instance()                      \
    {                                               \
        return mInstance;                           \
    }

#endif  //  _SINGLETON_PATTERN_H_


