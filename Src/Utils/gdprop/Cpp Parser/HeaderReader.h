/**
 *  @file      HeaderReader.h
 *  @brief	   Cpp file reader.
 *  @author    Sébastien Lussier.
 *  @date      28/02/06.
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
#ifndef     _CPP_HEADER_READER_H_
#define     _CPP_HEADER_READER_H_


#include "Cpp Parser/Reader.h"
#include "Cpp Parser/ClassDesc.h"
#include "Cpp Parser/EnumDesc.h"
#include "Cpp Parser/PropertyDescFactory.h"


namespace Cpp
{
    class Context
    {
    public:
        enum Type
        {
            CTX_Class,
            CTX_Namespace,
            CTX_Block
        };

        Context( const String& pName, Type pType, ClassDesc* pClassDesc = NULL )
            : mName(pName)
            , mType(pType)
            , mClassDesc(pClassDesc)
        {
        }

        Context( const Context& pContext )
            : mName(pContext.mName)
            , mType(pContext.mType)
            , mClassDesc(pContext.mClassDesc)
        {
        }

        Context& operator = ( const Context& pContext )
        {
            mName = pContext.mName;
            mType = pContext.mType;
            mClassDesc = pContext.mClassDesc;
        }

    public:
        String      mName;
        Type        mType;
        ClassDesc*  mClassDesc;
    };

    class HeaderReader : public Reader
    {
    public:
        HeaderReader()
        {
        }

        virtual ~HeaderReader()
        {
            for( List<ClassDesc*>::iterator it(mClassDesc.begin()); it != mClassDesc.end(); ++it )
                GD_DELETE(*it);

            for( List<EnumDesc*>::iterator it(mEnumDesc.begin()); it != mEnumDesc.end(); ++it )
                GD_DELETE(*it);
        }

        void Read( const String& pFileName )
        {
            mCurrentFilename = pFileName;

            Reader::Read( pFileName );
            ReadBlock();

            mCurrentFilename = "";
        }

        String GetContextString() const
        {
            String contextString;
            for( List<Context>::const_iterator it(mContexts.begin()); it != mContexts.end(); ++it )
            {
                if( it->mType == Context::CTX_Class || it->mType == Context::CTX_Namespace )
                {
                    contextString += it->mName;
                    contextString += "::";
                }
            }

            return contextString;
        }

        void ReadBlock()
        {
            String token;

            while( !mTokenizer.AtEnd() )
            {
                strcpy_s(mLastComment, COMMENT_BUFFER_SIZE, "");

                ReadToken(token);

                if( strcmp(token.c_str(),"class") == 0 )
                {
                    String className;

                    // Read class name, watch for CORE_API or ENGINE_API... (ignore all _API)
                    ReadToken(className);
                    token = className;
                    ToUpper(token);
                    if( className.length() > 4 && token == className )
                        ReadToken(className);

                    Char next = NextChar();

                    if( next == ';' || next == '*' || next == '&' )
                        continue;   // This is a forward declaration... ignore it!

                    ClassDesc* newClassDesc = GD_NEW(ClassDesc, this, "ClassDesc")(mCurrentFilename.c_str(),GetContextString());
                    newClassDesc->mName = className;
                    mClassDesc.push_back( newClassDesc );

                    while( !mTokenizer.AtEnd() && strcmp(token.c_str(), "{") != 0 )
                        ReadToken(token);

                    mContexts.push_back( Context(className, Context::CTX_Class, newClassDesc) );
                    ReadBlock();
                    mContexts.pop_back();

                    MustRead(";");
                }
                else if( strcmp(token.c_str(), "namespace") == 0 )
                {
                    ReadToken(token);

                    MustRead("{");

                    mContexts.push_back( Context(token, Context::CTX_Namespace) );
                    ReadBlock();
                    mContexts.pop_back();

                }
                else if( strcmp(token.c_str(), "template") == 0 )
                {
                    MustRead("<");

                    UInt32 countBrace = 1;
                    while( !mTokenizer.AtEnd() && countBrace != 0 )
                    {
                        ReadToken(token);
                        if( strcmp(token.c_str(), "<") == 0 )
                            countBrace++;
                        else if( strcmp(token.c_str(), ">") == 0 )
                            countBrace--;
                    }
                }
                else if( strcmp(token.c_str(), "enum") == 0 )
                {
                    String enumName;
                    ReadToken(enumName);

                    String keyStr;
                    String valueStr = "0";
                    UInt32 value = Number<UInt32>::Max;
                    EnumDesc* enumDesc = 0;

                    if( strlen(mLastComment) != 0 )
                    {
                        enumDesc = GD_NEW(EnumDesc, this, "EnumDesc")(enumName,GetContextString(), mLastComment);
                        mEnumDesc.push_back( enumDesc );

                        // Add include
                        mEnumIncludes.push_back( mCurrentFilename );

                        strcpy_s(mLastComment, COMMENT_BUFFER_SIZE, "");
                    }

                    MustRead("{");

                    while(!mTokenizer.AtEnd())
                    {
                        if( NextChar() == ',' )
                        {
                            ReadToken();
                        }

                        if( NextChar() == '}' )
                        {
                            ReadToken();
                            MustRead(";");
                            break;
                        }

                        ReadToken(keyStr);

                        if( NextChar() == '=' )
                        {
                            ReadToken();
                            valueStr = "";

                            while( NextChar() != ',' && NextChar() != '}' && !mTokenizer.AtEnd() )
                            {
                                ReadToken(token);
                                valueStr += token;
                            }

                            if( enumDesc )
                                enumDesc->mValues[keyStr] = valueStr;
                        }
                        else
                        {
                            value++;
                            valueStr = ToString<UInt32>(value);

                            if( enumDesc )
                                enumDesc->mValues[keyStr] = valueStr;
                        }                        
                    }
                }
                else if( strcmp(token.c_str(), "{") == 0 )
                {
                    ReadBlock();
                }
                else if( strcmp(token.c_str(), "}") == 0 )
                {
                    return;
                }
                else if( strcmp(token.c_str(), "properties") == 0 )
                {
                    ReadToken(token);
                    if( strcmp(token.c_str(), ":") == 0 )
                    {
                        Bool blockEnd = ReadProperties();
                        if( blockEnd )
                            return;
                    }
                }
            }
        }

        /**
        *  Name
        *  This is a simple description for a property
        *  Range  = [0,10]
        *  MinLen = 5
        *  MaxLen = 20
        */
        Bool ReadProperties()
        {
            String propType;
            String propName;
            String token;

            ReadToken(token);

            ClassDesc* clsDesc = mContexts.back().mType == Context::CTX_Class ? mContexts.back().mClassDesc : NULL;
            GD_ASSERT(clsDesc != NULL);

            // Read properties until we hit "public", "private", "protected" or "}"
            do
            {
                UInt32 typeID;

                while( !mTokenizer.IsWhitespace(mTokenizer.NextChar()) )
                    token += mTokenizer.GetChar();

                // Token must be a supported property type, else we assume it's an enum
                if( !IsASupportedPropertyType(token, typeID) )
                    typeID = PropertyEnum::ID();

                propType = token;

                // Read name
                ReadToken(token);
                propName = token;
                MustRead(";");

                // Create property 
                PropertyDesc* propertyDesc = CreatePropertyDesc(typeID, GetContextString() + String(propType), propName);

                // Store infos from comments...
                if( strlen(mLastComment) != 0 )
                {
                    propertyDesc->ParseComments( mLastComment );
                    strcpy_s(mLastComment, COMMENT_BUFFER_SIZE, "");
                }

                // Add that new property to the class description.
                clsDesc->AddProperty(propertyDesc);

                ReadToken(token);
            } while( strcmp(token.c_str(), "}") != 0         && 
                strcmp(token.c_str(), "public") != 0    &&
                strcmp(token.c_str(), "private") != 0   &&
                strcmp(token.c_str(), "protected") != 0 );

            return strcmp(token.c_str(), "}") == 0;
        }

    public:
        String              mCurrentFilename;
        List<Context>       mContexts;
        List<ClassDesc*>    mClassDesc;
        List<EnumDesc*>     mEnumDesc;
        List<String>        mEnumIncludes;
    };
}


#endif  //  _CPP_HEADER_READER_H_
