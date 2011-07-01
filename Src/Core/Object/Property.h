/**
 *  @file       Property.h
 *  @brief	    Gamedesk property system.
 *  @author     Sébastien Lussier.
 *  @date       30/03/04.
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
#ifndef     _PROPERTY_H_
#define     _PROPERTY_H_


#include "Containers/Containers.h"

#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/Number.h"
#include "Graphic/Color3.h"
#include "Graphic/Color4.h"


namespace Gamedesk {


class ObjectProperties;

/**
 *  Used to generate unique id for property classes (see DECLARE_PROPERTY_CLASS)
 */
class CORE_API PropertyTypeManager
{
public:
    PropertyTypeManager();
    static UInt32 GetNextID();

private:
    static PropertyTypeManager mInstance;
    UInt32 mNextId;
};

/**
 *  Use this macro to create a new property class with run time type identification.
 */
#define DECLARE_PROPERTY_CLASS(T,C)                                         \
    public:                                                                 \
        virtual UInt32 GetID() const                                        \
        {                                                                   \
            return ID();                                                    \
        }                                                                   \
        static UInt32 ID()                                                  \
        {                                                                   \
            static const UInt32 mID = PropertyTypeManager::GetNextID();     \
            return mID;                                                     \
        }                                                                   \
        virtual const Char* GetTypeName() const                             \
        {                                                                   \
            static const Char* TYPE = #T;                                   \
            return TYPE;                                                    \
        }                                                                   \
        virtual const Char* GetComponentTypeName() const                    \
        {                                                                   \
            static const Char* COMPONENT_TYPE = #C;                         \
            return COMPONENT_TYPE;                                          \
        }                                                                   \

/**
 *  Base property class.	
 */
class Property
{
public:
    enum { InvalidComponent = 0xFFFFFFFF };

    Property( const Char* pName, const Char* pDescription, UInt32 pOffset ) 
        : mName(pName)
        , mDescription(pDescription)
        , mOffset(pOffset)
    {
    }   

    const Char* GetName() const
    {
        return mName;
    }

    const Char* GetDescription() const
    {
        return mDescription;
    }

    UInt32 GetOffset() const
    {
        return mOffset;
    }

	virtual UInt32 GetID() const
	{	
		return 0; 
	}
	
	static UInt32 ID()
	{
		return 0;
	}

    virtual const Char* GetTypeName() const = 0;
    virtual const Char* GetComponentTypeName() const = 0;
    
    virtual UInt32      GetComponentCount() const = 0;
    virtual const Char* GetComponentName( UInt32 /*pIndex*/ ) const 
    {
        GD_ASSERT_M_EX(false, "Should never get here!"); 
        return NULL; 
    }

    virtual void GetValueString(const void* pData, String& pString ) const = 0;
    virtual void GetComponentValueString(const void* /*pData*/, UInt32 /*pIndex*/, String& pString ) const { pString = ""; }
    
protected:
    const char*         mName;
    const char*         mDescription;
    UInt32              mOffset;
};


/**
 *  Template property class offering most of the facility for a property to update itself.
 */
template <class T, class C=T, int N=0>
class TProperty : public Property
{
public:
    typedef      T          Type;
    typedef      C          ComponentType;

    TProperty( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : Property( pName, pDescription, pOffset )
    {
    }

    UInt32 GetComponentCount() const
    {
        return N;
    }

    const C& GetComponentValue( const void* pData, UInt32 pIndex ) const
    {
        GD_ASSERT_EX( pIndex < N );
        return ((const C*)&GetValue(pData))[pIndex];
    }

    C& GetComponentValue( void* pData, UInt32 pIndex ) const
    {
        GD_ASSERT_EX( pIndex < N );
        return ((C*)&GetValue(pData))[pIndex];
    }

    void SetComponentValue( void* pData, UInt32 pIndex, const C& pValue ) const
    {
        GD_ASSERT_EX( pIndex < N );
        ((C*)&GetValue(pData))[pIndex] = pValue;
    }

    /**
    *  Get the value of the property.
    */
    const T& GetValue(const void* pData) const
    {
        return *(const T*)(((const Byte*)(pData))+mOffset);
    }

    T& GetValue(void* pData) const
    {
        return *(T*)(((Byte*)(pData))+mOffset);
    }

    /**
     *  Set the value of the property.
     */
    void SetValue(void* pData, const T& pValue ) const
    {
        *(T*)(((Byte*)(pData))+mOffset) = pValue;
    }
};

/**
 *  Template class used by numbers (float, double and all type of int).
 */
template <class T>
class TNumberProperty : public TProperty<T>
{
public:
    TNumberProperty( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<T>(pName, pDescription, pOffset)
        , mUseRangeValidation(false)
        , mUseSlider(false)  
    {
    }

    void UseRangeValidation( Bool pUseValidation )
    {
        mUseRangeValidation = pUseValidation;
    }

    void UseSlider( Bool pUseSlider, UInt32 pTickSubdivision = 1, UInt32 pSliderStep = 1 )
    {
        mUseSlider = pUseSlider;
        mSliderTickSubdivision = pTickSubdivision;
        mSliderStep = pSliderStep;
    }

    Bool    UseRangeValidation() const  {   return mUseRangeValidation;     }
    Bool    UseSlider() const           {   return mUseSlider;              }

    UInt32  GetSliderTickSubdiv() const {   return mSliderTickSubdivision;  }
    UInt32  GetSliderStep() const       {   return mSliderStep;             }
    
    T       GetMinimum() const          {   return mValidator.GetMin();     }
    void    SetMinimum( T pMin )        {   mValidator.SetMin( pMin );      }
    
    T       GetMaximum() const          {   return mValidator.GetMax();     }
    void    SetMaximum( T pMax )        {   mValidator.SetMax( pMax );      }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        ToString( this->GetValue(pData), pString );
    }

private:
    Bool                mUseRangeValidation;
    Bool                mUseSlider;
    UInt32              mSliderTickSubdivision;
    UInt32              mSliderStep;
    NumberValidator<T>  mValidator;    
};




class CORE_API PropertyBool : public TProperty<Bool>                  
{   
    DECLARE_PROPERTY_CLASS(Bool, Bool);  

    PropertyBool( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<Bool>(pName, pDescription, pOffset)    {}

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = GetValue(pData) == true ? "True" : "False";
    }
};

class CORE_API PropertyChar : public TNumberProperty<Char>           
{
    DECLARE_PROPERTY_CLASS(Char,  Char); 

    PropertyChar( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Char>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyInt16 : public TNumberProperty<Int16>           
{   
    DECLARE_PROPERTY_CLASS(Int16,  Int16);  

    PropertyInt16( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Int16>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyInt32 : public TNumberProperty<Int32>           
{
    DECLARE_PROPERTY_CLASS(Int32,  Int32); 

    PropertyInt32( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Int32>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyInt64 : public TNumberProperty<Int64>           
{
    DECLARE_PROPERTY_CLASS(Int64,  Int64); 

    PropertyInt64( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Int64>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyByte : public TNumberProperty<Byte>           
{
    DECLARE_PROPERTY_CLASS(Byte,  Byte); 

    PropertyByte( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Byte>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyUInt16 : public TNumberProperty<UInt16>           
{
    DECLARE_PROPERTY_CLASS(UInt16,  UInt16); 

    PropertyUInt16( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<UInt16>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyUInt32 : public TNumberProperty<UInt32>           
{
    DECLARE_PROPERTY_CLASS(UInt32,  UInt32); 

    PropertyUInt32( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<UInt32>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyUInt64 : public TNumberProperty<UInt64>           
{
    DECLARE_PROPERTY_CLASS(UInt64,  UInt64); 

    PropertyUInt64( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<UInt64>(pName, pDescription, pOffset) {}
};


class CORE_API PropertyFloat : public TNumberProperty<Float>           
{
    DECLARE_PROPERTY_CLASS(Float,  Float); 

    PropertyFloat( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Float>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyDouble : public TNumberProperty<Double>           
{
    DECLARE_PROPERTY_CLASS(Double, Double); 

    PropertyDouble( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TNumberProperty<Double>(pName, pDescription, pOffset) {}
};

class CORE_API PropertyVector3f : public TProperty<Vector3f, Float, 3> 
{  
    DECLARE_PROPERTY_CLASS(Vector3f, Float); 

    PropertyVector3f( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<Vector3f, Float, 3>(pName, pDescription, pOffset) {}

    const Char* GetComponentName( UInt32 pIndex ) const 
    {
        static const Char* COMPONENTS[] = { "X", "Y", "Z" };
        return COMPONENTS[pIndex]; 
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = "{ ";
        pString += ToString( GetComponentValue(pData, 0) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 1) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 2) );
        pString += " }";
    }

    virtual void GetComponentValueString(const void* pData, UInt32 pIndex, String& pString ) const
    {
        ToString( GetComponentValue(pData, pIndex), pString );
    }
};

class CORE_API PropertyQuaternionf : public TProperty<Quaternionf, Float, 4>      
{
    DECLARE_PROPERTY_CLASS(Quaternionf, Float); 

    PropertyQuaternionf( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<Quaternionf, Float, 4>(pName, pDescription, pOffset) {}

    const Char* GetComponentName( UInt32 pIndex ) const 
    {
        static const Char* COMPONENTS[] = { "X", "Y", "Z", "W" };
        return COMPONENTS[pIndex]; 
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = "{ ";
        pString += ToString( GetComponentValue(pData, 0) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 1) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 2) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 3) );
        pString += " }";
    }

    virtual void GetComponentValueString(const void* pData, UInt32 pIndex, String& pString ) const
    {
        ToString( GetComponentValue(pData, pIndex), pString );
    }
};

class CORE_API PropertyColor3f : public TProperty<Color3f, Float, 3>              
{  
    DECLARE_PROPERTY_CLASS(Color3f, Float);  

    PropertyColor3f( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<Color3f, Float, 3>(pName, pDescription, pOffset) {}

    const Char* GetComponentName( UInt32 pIndex ) const 
    {
        GD_ASSERT_EX(pIndex < 3);
        static const Char* COMPONENTS[3] = { "Red", "Green", "Blue" };
        return COMPONENTS[pIndex];  
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = "{ ";
        pString += ToString( GetComponentValue(pData, 0) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 1) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 2) );
        pString += " }";
    }

    virtual void GetComponentValueString(const void* pData, UInt32 pIndex, String& pString ) const
    {
        ToString( GetComponentValue(pData, pIndex), pString );
    }
};

class CORE_API PropertyColor4f : public TProperty<Color4f, Float, 4>              
{  
    DECLARE_PROPERTY_CLASS(Color4f, Float);  

    PropertyColor4f( const Char* pName, const Char* pDescription, UInt32 pOffset )
        : TProperty<Color4f, Float, 4>(pName, pDescription, pOffset) {}

    const Char* GetComponentName( UInt32 pIndex ) const 
    {
        GD_ASSERT_EX(pIndex < 4);
        static const Char* COMPONENTS[4] = { "Red", "Green", "Blue", "Alpha" };
        return COMPONENTS[pIndex];  
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = "{ ";
        pString += ToString( GetComponentValue(pData, 0) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 1) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 2) );
        pString += ", ";
        pString += ToString( GetComponentValue(pData, 3) );
        pString += " }";
    }

    virtual void GetComponentValueString(const void* pData, UInt32 pIndex, String& pString ) const
    {
        ToString( GetComponentValue(pData, pIndex), pString );
    }
};

class CORE_API PropertyString : public TProperty<String>
{
    DECLARE_PROPERTY_CLASS(String, String);

    PropertyString( const Char* pName, const Char* pDescription, UInt32 pOffset ) 
        : TProperty<String>( pName, pDescription, pOffset )
        , mMaxLen(Number<UInt32>::Max)  
    {
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = GetValue(pData);
    }
    
    UInt32  GetMaxLength() const                    {   return mMaxLen;     }
    void    SetMaxLength( const UInt32& pMaxLen )   {   mMaxLen = pMaxLen;  }

private:
    UInt32 mMaxLen;
};


class CORE_API PropertyEnum : public TProperty<UInt32>    
{
public:
    PropertyEnum( const Char* pName, const Char* pDescription, UInt32 pOffset ) 
        : TProperty<UInt32>( pName, pDescription, pOffset )
    {
    }

    virtual UInt32 GetID() const
    {
        return ID();
    }

    static UInt32 ID()
    {
        static const UInt32 mID = PropertyTypeManager::GetNextID();
        return mID;
    }

    virtual const Char* GetComponentTypeName() const
    {
        return 0;
    }

    virtual const Char* GetTypeName() const
    {
        return "Enum";
    }

    virtual void GetValueString(const void* pData, String& pString ) const
    {
        pString = GetStrings()[GetValue(pData)];
    }

    virtual const char**  GetStrings() const = 0;
    virtual const UInt32* GetValues() const = 0;
    virtual UInt32        GetCount() const = 0;

    UInt32 GetIndexFromString(const char* pString) const
    {
        GD_ASSERT_M(pString != 0, "Invalid string specified!");

        UInt32 count = GetCount();
        const Char** strings = GetStrings();

        for( UInt32 i = 0; i < count; ++i )
        {
            if( strcmp(pString, strings[i]) == 0 )
                return i;
        }

        GD_ASSERT_M(0 && pString, "Invalid string specified!");
        return 0;
    }

    UInt32 GetIndexFromValue(UInt32 pValue) const
    {
        UInt32 count = GetCount();
        const UInt32* values = GetValues();

        for( UInt32 i = 0; i < count; ++i )
        {
            if( pValue == values[i] )
                return i;
        }

        GD_ASSERT_M(0 && pValue, "Invalid value specified!");
        return 0;
    }

    UInt32 GetValueFromIndex(UInt32 pIndex) const
    {
        GD_ASSERT_M(pIndex < GetCount(), "Invalid index specified!");
        return GetValues()[pIndex];
    }

    UInt32 GetValueFromString(const char* pString) const
    {
        return GetValues()[GetIndexFromString(pString)];
    }

    const Char* GetStringFromValue(UInt32 pValue) const
    {
        return GetStrings()[GetIndexFromValue(pValue)];
    }

    const Char* GetStringFromIndex(UInt32 pIndex) const
    {
        GD_ASSERT_M(pIndex < GetCount(), "Invalid index specified!");
        return GetStrings()[pIndex];
    }
};


template <typename T>
class TPropertyEnum : public PropertyEnum, public T
{
public:
    TPropertyEnum<T>( const Char* pName, const Char* pDescription, UInt32 pOffset ) 
        : PropertyEnum( pName, pDescription, pOffset )
    {
    }

    virtual const char** GetStrings() const
    {
        return this->mStrings;
    }

    virtual const UInt32* GetValues() const
    {
        return this->mValues;
    }

    virtual UInt32 GetCount() const
    {
        return this->mSize;
    }
};

#if GD_CFG_USE_PROPERTIES == GD_ENABLED
    #define properties                                                          \
            static void StaticRegisterProperties(Gamedesk::Class* pClass);      \
        protected
#else
    #define properties                                                          \
            static void StaticRegisterProperties(Gamedesk::Class*) {}           \
        protected
#endif


} // namespace Gamedesk


#endif  //  _PROPERTY_H_
