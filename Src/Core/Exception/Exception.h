/**
 *  @file       Exception.h
 *  @brief      Base exception class.
 *  @author     Patrick Girard.
 *  @date       19/01/03.
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
#ifndef     _EXCEPTION_H_
#define     _EXCEPTION_H_


#include "Containers/Containers.h"


namespace Gamedesk {


/**
 *  Helper class used to define a location in the code.
 *  Used mainly by the Exception class.
 */
class CORE_API CodeLocation
{
public:
    /**
     *  Constructor.
     *  @param  pFileName       Name of the file.
     *  @param  pFunctionName   Name of the function.
     *  @param  pLineNumber     Line number.
     */
    CodeLocation( const Char* pFileName, const Char* pFunctionName, UInt32 pLineNumber );

    /**
     *  Get the name of the file.
     *  @return A string, the name of the file pointed by this object.
     */
    INLINE const String& File() const;

    /**
     *  Get the name of the function pointed by this object.
     *  @return A string, the name of the function pointed by this object.
     */
    INLINE const String& Function() const;

    /**
     *  Get the line number.
     *  @return The line number as an integer.
     */
    INLINE UInt32        Line() const;

protected:
    //! Default constructor (disabled)
    CodeLocation();

    String  mFileName;          //!< Name of the file.
    String  mFunctionName;      //!< Name of the function or method.
    UInt32  mLineNumber;        //!< Line number.
};


/**
 *  Macro used to construct a CodeLocation object. Used when throwing an exception.
 */
#define Here CodeLocation(__FILE__, __FUNCTION__, __LINE__)


/**
 *  Exception class used everywhere in gamedesk.
 *  @remark To throw an exception, use the THROW macro defined below, and the Here macro defined above.
 */
class CORE_API Exception
{
public:
    /**
     *  Constructor.
     *  @param  pLocation
     */
    Exception( CodeLocation pLocation );

    /**
     *  Destructor.
     */
    virtual ~Exception() {}


    /**
     *  Get the name of the class of this exception.	
     *  @return The name of the class.
     *  @remark See the DECLARE_EXCEPTION macro below.
     */
    virtual const Char* GetName() const = 0;

    /**
     *  Get the code location where the exception was thrown.
     *  @return the code location where the exception was thrown.
     */
    const CodeLocation& GetLocation() const;

    /**
     *  Get the name of the file where the exception occured.
     *  @return The name of the file.
     */
    const String&  GetFile() const;

    /**
     *  Get the name of the function/method where the exception occured.
     *  @return The name of the function/method.
     *  @remark Not supported by VC++.
     */
    const String&  GetFunction() const;

    /**
     *  Get the line number where the exception occured.
     *  @return The line number where the exception occured.
     */
    UInt32  GetLine() const;

    /**
     *  Get the error message of this exception.
     *  @return The error message string.
     */
    const String&  GetMessage() const;

    /**
     *  Get a description of this exception.
     *  @return A description as a string.
     */
    String  ToString() const;

    /**
     *  Add a function/method name to the stack of this exception.
     *  Should be constructed in the methods using the guard/unguard macros.
     *  @param  pFunc   Name of the function to push on the stack.
     */
    void    BuildStack( const Char* pFunc );

    /**
     *  Output the exception description in the debug window.
     *  @return A reference to this exception.
     *  @see The throw macro.
     */
    Exception& DebugOut();

protected:

    CodeLocation    mLocation;      //!< Location where the exception occured.
    String          mMessage;       //!< Exception message.
    Vector<String>  mCallStack;     //!< Call stack (from the exception creation to the catch).
};


/**
 *  Use this macro to automatically implement the GetName() method for an exception class.
 */
#define DECLARE_EXCEPTION( clsName )    \
    public: \
        virtual const Char* GetName() const { return #clsName; }


class CORE_API NullPointerException : public Exception
{
    DECLARE_EXCEPTION(NullPointerException);
    
public:
    NullPointerException( const String& pNullVar, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = pNullVar;
        mMessage += " is NULL.";
        DebugOut();
    }
};


} // namespace Gamedesk


#endif  //  _EXCEPTION_H_
