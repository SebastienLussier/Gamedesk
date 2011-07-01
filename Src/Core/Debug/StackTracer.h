/**
 *  @file       StackTracer.h
 *  @brief      Used for displaying call stack, can be useful for debugging.
 *  @author     Sébastien Lussier.
 *  @date       18/02/02.
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
#ifndef     _STACK_TRACER_H_
#define     _STACK_TRACER_H_


#include "Patterns/Singleton.h"


namespace Gamedesk {
	

/**
 *  Used to display a function call stack.  Can be very useful when debugging,
 *  but it is very slow.  So enable only when necessary.  To enable it, look in
 *  BuildOptions.h for the GD_CFG_USE_STACK_TRACING define.
 *  @brief  Used to do call stack tracing.
 *  @author Sebastien Lussier.
 *  @date   18/02/02.
 */
class CORE_API StackTracer
{
    DECLARE_SINGLETON( StackTracer );

    friend class CallStacker;

public:
    /**
     *  Will output the stack trace to the stream passed in parameter.
     *  This method will only work if stack tracing is enabled.
     *  See the BuildOptions.h file for more details on enabling it.
     *  @brief  Print the stack trace to an output stream.
     *  @param  pOut    a reference to an output stream, where the stack will be printed.
     */
    void PrintStackTrace( std::ostream& pOut )
    {
        if( mPrintingStack )
            return;
        else
            mPrintingStack = true;

        String strStack;

        pOut << "\n\nPrinting stack trace (depth=" << mFunctionStack.size() << "):\n";

        int i = 0;
        for( Vector<String>::const_iterator it = mFunctionStack.begin();
             it != mFunctionStack.end(); ++it )
        {
            strStack.append(2*i++,' ');
            strStack += *it;
            strStack += "\n";
        }

        pOut << strStack;

        mPrintingStack = false;
    }

private:
    /**
     *  Constructor. Private due to the fact that this class is a singleton.
     *  @brief  Constructor.
     */
    StackTracer()
    {
        mPrintingStack= false;
    }

    /**
     *  Add a call on the call stack. Only the friend class CallStacker is
     *  allowed to access this method.
     *  @brief  Add a call on the call stack.
     *  @param  strFuncName     a string, the name of the function to be pushed on top of the stack.
     */
    void PushCall( const Char* strFuncName )
    {
        // Don't add anything to the stack while printing it
        if( mPrintingStack )
            return;

        // Avoid infinite recursion by using no guard version of PushItem
        // Not using the PrintStackTrace for speed issues.
        mFunctionStack.push_back( String(strFuncName) );
    }

    /**
     *  Remove the top call from the call stack. Only the friend class CallStacker is
     *  allowed to access this method.
     *  @brief  Remove the top call from the call stack.
     */
    void PopCall()
    {
        // Don't remove anything from the stack while printing it
        if( mPrintingStack )
            return;

        // Avoid infinite recursion by using no guard version of PopItem
        // Not using the PrintStackTrace for speed issues.
        mFunctionStack.pop_back();
    }

private:
    Bool                mPrintingStack;     //!< Lock mechanism to avoid infinite recursion when printing stack.
    Vector<String>      mFunctionStack;     //!< A list of function name, the call stack.
};


} // namespace Gamedesk


#endif  //  _STACK_TRACER_H_
