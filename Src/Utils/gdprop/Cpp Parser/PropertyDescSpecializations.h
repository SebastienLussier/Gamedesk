/**
 *  @file      PropertyDescSpecializations.h
 *  @brief	   Specializations of the base PropertyDesc class.
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
#ifndef     _CPP_PROPERTY_DESC_SPEC_H_
#define     _CPP_PROPERTY_DESC_SPEC_H_


namespace Cpp
{
    template <typename T>
    class NumberPropertyDesc : public PropertyDesc
    {
    public:
        NumberPropertyDesc( const String& pVariableName )
            : PropertyDesc(pVariableName)
            , mMin(Number<T>::Min)
            , mMax(Number<T>::Max)
            , mUseRangeValidation(false)
            , mUseSlider(false)
        {
        }

        virtual Bool AddInfo( const String& pInfoType, StringTokenizer& tokenizer )
        {
            if( PropertyDesc::AddInfo(pInfoType, tokenizer) )
            {
                return true;
            }
            else if( strcmp(pInfoType.c_str(), "@min") == 0 )
            {
                tokenizer >> mMin;
                mUseRangeValidation = true;
                return true;
            }
            else if( strcmp(pInfoType.c_str(), "@max") == 0 )
            {
                tokenizer >> mMax;
                mUseRangeValidation = true;
                return true;
            }
            else if( strcmp(pInfoType.c_str(), "@range") == 0 )
            {
                tokenizer >> mMin;
                tokenizer >> mMax;
                mUseRangeValidation = true;
                return true;
            }

            return false;
        }

        virtual void OutputPropertyDescription( TextOutputStream& out, const Char* pClassName )
        {
            PropertyDesc::OutputPropertyDescription(out, pClassName);

            out << "    ";
            out << mVariableName;
            out << "Property.UseRangeValidation(";
            out << mUseRangeValidation;
            out << ");\n";

            out << "    ";
            out << mVariableName;
            out << "Property.SetMinimum((";
            out << Number<T>::Name();
            out << ")";
            out << mMin;
            out << ");\n";

            out << "    ";
            out << mVariableName;
            out << "Property.SetMaximum((";
            out << Number<T>::Name();
            out << ")";
            out << mMax;
            out << ");\n";
        }

    public:
        T    mMin;
        T    mMax;
        Bool mUseRangeValidation;
        Bool mUseSlider;
    };

    class BoolPropertyDesc : public PropertyDesc
    {
    public:
        BoolPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}  
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyBool"); return PROPERTY_TYPE_NAME; }
    };

    class CharPropertyDesc : public NumberPropertyDesc<Char>
    {
    public:
        CharPropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Char>(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyChar"); return PROPERTY_TYPE_NAME; }

        virtual void OutputPropertyDescription( TextOutputStream& out, const Char* pClassName )
        {
            PropertyDesc::OutputPropertyDescription(out, pClassName);

            Int32 min = mMin;
            Int32 max = mMax;

            out << "    ";
            out << mVariableName;
            out << "Property.UseRangeValidation(";
            out << mUseRangeValidation;
            out << ");\n";

            out << "    ";
            out << mVariableName;
            out << "Property.SetMinimum((";
            out << Number<Char>::Name();
            out << ")";
            out << min;
            out << ");\n";

            out << "    ";
            out << mVariableName;
            out << "Property.SetMaximum((";
            out << Number<Char>::Name();
            out << ")";
            out << max;
            out << ");\n";
        }
    };

    class Int16PropertyDesc : public NumberPropertyDesc<Int16>
    {
    public:
        Int16PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Int16>(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyInt16"); return PROPERTY_TYPE_NAME; }
    };

    class Int32PropertyDesc : public NumberPropertyDesc<Int16>
    {
    public:
        Int32PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Int16>(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyInt32"); return PROPERTY_TYPE_NAME; }
    };

    class Int64PropertyDesc : public NumberPropertyDesc<Int64>
    {
    public:
        Int64PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Int64>(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyInt64"); return PROPERTY_TYPE_NAME; }
    };

    class BytePropertyDesc : public NumberPropertyDesc<Byte>
    {
    public:
        BytePropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Byte>(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyByte"); return PROPERTY_TYPE_NAME; }
    };

    class UInt16PropertyDesc : public NumberPropertyDesc<UInt16>
    {
    public:
        UInt16PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<UInt16>(pVariableName) {}   
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyUInt16"); return PROPERTY_TYPE_NAME; }
    };

    class UInt32PropertyDesc : public NumberPropertyDesc<UInt32>
    {
    public:
        UInt32PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<UInt32>(pVariableName) {}    
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyUInt32"); return PROPERTY_TYPE_NAME; }
    };

    class UInt64PropertyDesc : public NumberPropertyDesc<UInt64>
    {
    public:
        UInt64PropertyDesc( const String& pVariableName ) : NumberPropertyDesc<UInt64>(pVariableName) {}    
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyUInt64"); return PROPERTY_TYPE_NAME; }
    };

    class FloatPropertyDesc : public NumberPropertyDesc<Float>
    {
    public:
        FloatPropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Float>(pVariableName) {}    
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyFloat"); return PROPERTY_TYPE_NAME; }
    };

    class DoublePropertyDesc : public NumberPropertyDesc<Double>
    {
    public:
        DoublePropertyDesc( const String& pVariableName ) : NumberPropertyDesc<Double>(pVariableName) {}   
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyDouble"); return PROPERTY_TYPE_NAME; }
    };

    class Vector3fPropertyDesc : public PropertyDesc
    {
    public:
        Vector3fPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}  
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyVector3f"); return PROPERTY_TYPE_NAME; }
    };

    class QuaternionfPropertyDesc : public PropertyDesc
    {
    public:
        QuaternionfPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyQuaternionf"); return PROPERTY_TYPE_NAME; }
    };

    class Color3fPropertyDesc : public PropertyDesc
    {
    public:
        Color3fPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}  
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyColor3f"); return PROPERTY_TYPE_NAME; }
    };

    class Color4fPropertyDesc : public PropertyDesc
    {
    public:
        Color4fPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}    
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyColor4f"); return PROPERTY_TYPE_NAME; }
    };

    class StringPropertyDesc : public PropertyDesc
    {
    public:
        StringPropertyDesc( const String& pVariableName ) : PropertyDesc(pVariableName) {}     
        const String& GetPropertyTypeName() const { static const String PROPERTY_TYPE_NAME("PropertyString"); return PROPERTY_TYPE_NAME; }
    };

    class EnumPropertyDesc : public PropertyDesc
    {
    public:
        EnumPropertyDesc( const String& pEnumType, const String& pVariableName ) 
            : PropertyDesc(pVariableName) 
            , mPropertyTypeName("TPropertyEnum<EnumInfo::")
        {
            mPropertyTypeName += pEnumType;
            mPropertyTypeName += ">";
        }

        const String& GetPropertyTypeName() const 
        {
            return mPropertyTypeName;
        }

    private:
        String mPropertyTypeName;
    };
}


#endif  //  _CPP_PROPERTY_DESC_SPEC_H_
