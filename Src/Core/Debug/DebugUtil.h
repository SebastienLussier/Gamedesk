/**
 *  @file       DebugUtil.h
 *  @brief      Utility for debugging and validation (i.e. asserts).
 *  @author     Sébastien Lussier.
 *  @date       20/01/02.
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
#ifndef     _DEBUG_UTIL_H_
#define     _DEBUG_UTIL_H_


namespace Gamedesk {


/**
 *  Class used to add a function name on the stack of function call.
 *  When this object is destroyed, it automatically remove the
 *  function from the stack.  Used by the stack tracer.
 *  @brief  Add and remove a function name from the call stack.
 *  @author Sébastien Lussier.
 *  @date   20/01/02.
 */
class CORE_API CallStacker
{
public:
    /**
     *  Constructor taking the name of the function to stack as a parameter.
     *  @brief  Constructor.
     *  @param  strFunctionName the name of the function to add to the stack.
     */
    CallStacker( const Char* pFunctionName );

    /**
     *  Simple destructor. Will remove the function call from the call stack.
     *  @brief  Destructor.
     */
    ~CallStacker();
};


} // namespace Gamedesk


///////////////////////////////////////////////////////////////////////////////
// Debug macros.

///////////////////////////////////////////////////////////////////////////////
// guard()
#if GD_CFG_USE_GUARD == GD_ENABLED
    #if GD_CFG_USE_STACK_TRACING == GD_ENABLED
        //! When stack tracing is enabled, guard() add a call on the stack.
        #define guard(func)         {Gamedesk::CallStacker callStacker(#func);

        //! Closing brace, will cause the ClallStacker object created in the
        //! associated guard() to be destroyed, thus removing the call from the stack.
        #define unguard             }
    #else
        //! Stack tracing not enabled, does nothing.
        #define guard(func)         static const Gamedesk::Char* pFunctionName__ = #func; \
                                    try                                     \
                                    {

        //! Stack tracing not enabled, does nothing.
        #define unguard             }                                       \
                                    catch( Gamedesk::Exception* pEx )                 \
                                    {                                       \
                                        pEx->BuildStack( pFunctionName__ );     \
                                        throw;                              \
                                    }

    #endif
#else
    //! Guard not enabled, does nothing.
    #define guard(func)

    //! Guard not enabled, does nothing.
    #define unguard
#endif


///////////////////////////////////////////////////////////////////////////////
// guardDebug()
#if GD_CFG_USE_GUARD == GD_ENABLED && defined(GD_DEBUG)
    //! Will only be enabled in debug mode.  In speed critical functions, or in
    //! functions called very often (like containers or streams function)
    //! use this instead of guard()
    #define guardDebug(func)        guard(func)
    #define unguardDebug            unguard
#else
    //! Guard Debug not enabled, does nothing.
    #define guardDebug(func)        {

    //! Guard Debug not enabled, does nothing.
    #define unguardDebug            }
#endif


///////////////////////////////////////////////////////////////////////////////
// GD_ASSERT()
#if GD_CFG_USE_ASSERT == GD_ENABLED
    //! Show an GD_ASSERT if expression fails.
    #define GD_ASSERT_M(expression,description) \
                { \
                    static Gamedesk::Bool ignoreAlways = false; \
                    if( !ignoreAlways && !(expression) ) \
                    { \
                        Gamedesk::AssertManager assertMngr( (description), __LINE__, __FILE__, __FUNCTION__, ignoreAlways ); \
                        if( assertMngr.ShowAssert() == Gamedesk::AssertManager::ASSERT_RETRY ) \
                        { \
                            Gamedesk::debugBreak(); \
                        } \
                    } \
                }

    #define GD_ASSERT(expression)                       GD_ASSERT_M(expression, (#expression)) 
#else
    //! Asserts not enabled.
    #define GD_ASSERT(expression)    
    #define GD_ASSERT_M(expression,description)
#endif


#if GD_CFG_USE_ASSERT_DEBUG == GD_ENABLED
    #define GD_ASSERT_EX(expression)                    GD_ASSERT(expression)
    #define GD_ASSERT_M_EX(expression,description)      GD_ASSERT_M(expression,description)
#endif


#endif  //  _DEBUG_UTIL_H_
