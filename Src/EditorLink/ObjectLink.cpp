#include "Stdafx.h"
#include "ObjectLink.h"

#include "EditorLink.h"

namespace GameEdit
{
    void ObjectLink::PropertyChanged::add(ComponentModel::PropertyChangedEventHandler^ handler)
    {
        m_myEvent += handler;
    }

    void ObjectLink::PropertyChanged::remove(ComponentModel::PropertyChangedEventHandler^ handler)
    {
        m_myEvent -= handler;
    }

    void ObjectLink::PropertyChanged::raise(Object^ sender, ComponentModel::PropertyChangedEventArgs^ e)
    {
        // Check if there is any event handler registered.
        if (m_myEvent != nullptr) 
        {
            m_myEvent->Invoke(sender, e);
        }
    }

    ObjectLink^ ObjectLink::Create(Gamedesk::Object* pGDObject)
    {
        return EditorLink::Instance->CreateLink(pGDObject); 
    }

    Boolean ObjectLink::GetProperty_Bool(Int32 propOffset)
    {
	    bool* pVal = (bool*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Boolean newValue)
    {
	    bool* pVal = (bool*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Char ObjectLink::GetProperty_Char(Int32 propOffset)
    {
	    char* pVal = (char*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Char newValue)
    {
	    char* pVal = (char*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Int16 ObjectLink::GetProperty_Int16(Int32 propOffset)
    {
	    Int16* pVal = (Int16*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Int16 newValue)
    {
	    Int16* pVal = (Int16*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Int32 ObjectLink::GetProperty_Int32(Int32 propOffset)
    {
	    Int32* pVal = (Int32*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Int32 newValue)
    {
	    Int32* pVal = (Int32*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Int64 ObjectLink::GetProperty_Int64(Int32 propOffset)
    {
	    Int64* pVal = (Int64*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Int64 newValue)
    {
	    Int64* pVal = (Int64*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Byte ObjectLink::GetProperty_Byte(Int32 propOffset)
    {
	    Byte* pVal = (Byte*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Byte newValue)
    {
	    Byte* pVal = (Byte*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    UInt16 ObjectLink::GetProperty_UInt16(Int32 propOffset)
    {
	    UInt16* pVal = (UInt16*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, UInt16 newValue)
    {
	    UInt16* pVal = (UInt16*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    UInt32 ObjectLink::GetProperty_UInt32(Int32 propOffset)
    {
	    UInt32* pVal = (UInt32*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, UInt32 newValue)
    {
	    UInt32* pVal = (UInt32*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    UInt64 ObjectLink::GetProperty_UInt64(Int32 propOffset)
    {
	    UInt64* pVal = (UInt64*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, UInt64 newValue)
    {
	    UInt64* pVal = (UInt64*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Single ObjectLink::GetProperty_Float(Int32 propOffset)
    {
	    float* pVal = (float*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    Single ObjectLink::GetProperty_Single(Int32 propOffset)
    {
	    return GetProperty_Float(propOffset);
    }

    void ObjectLink::SetProperty(Int32 propOffset, Single newValue)
    {
	    float* pVal = (float*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    Double ObjectLink::SetProperty_Double(Int32 propOffset)
    {
	    double* pVal = (double*)(((Byte*)mGDObject) + propOffset);
	    return *pVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, Double newValue)
    {
	    double* pVal = (double*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue;

	    NotifyPropertyChanged("");
    }

    GameEdit::Vector3f^ ObjectLink::GetProperty_Vector3f(Int32 propOffset)
    {
	    return gcnew GameEdit::Vector3f(this, propOffset);
    }

    void ObjectLink::SetProperty(Int32 propOffset, GameEdit::Vector3f^ newValue)
    {
	    Gamedesk::Vector3f* pVal = (Gamedesk::Vector3f*)(((Byte*)mGDObject) + propOffset);
	    pVal->x = newValue->x;
	    pVal->y = newValue->y;
	    pVal->z = newValue->z;

	    NotifyPropertyChanged("");
    }

    GameEdit::Quaternionf^ ObjectLink::GetProperty_Quaternionf(Int32 propOffset)
    {
	    Gamedesk::Quaternionf* pVal = (Gamedesk::Quaternionf*)(((Byte*)mGDObject) + propOffset);
	    GameEdit::Quaternionf^ retVal = gcnew GameEdit::Quaternionf();
	    retVal->x = pVal->x;
	    retVal->y = pVal->y;
	    retVal->z = pVal->z;
	    retVal->w = pVal->w;
	    return retVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, GameEdit::Quaternionf^ newValue)
    {
	    Gamedesk::Quaternionf* pVal = (Gamedesk::Quaternionf*)(((Byte*)mGDObject) + propOffset);
	    pVal->x = newValue->x;
	    pVal->y = newValue->y;
	    pVal->z = newValue->z;
	    pVal->w = newValue->w;

	    NotifyPropertyChanged("");
    }

    GameEdit::Color3f^ ObjectLink::GetProperty_Color3f(Int32 propOffset)
    {
	    Gamedesk::Color3f* pVal = (Gamedesk::Color3f*)(((Byte*)mGDObject) + propOffset);
	    GameEdit::Color3f^ retVal = gcnew GameEdit::Color3f();
	    pVal->R = retVal->R;
	    pVal->G = retVal->G;
	    pVal->B = retVal->B;
	    return retVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, GameEdit::Color3f^ newValue)
    {
	    Gamedesk::Color3f* pVal = (Gamedesk::Color3f*)(((Byte*)mGDObject) + propOffset);
	    pVal->R = newValue->R;
	    pVal->G = newValue->G;
	    pVal->B = newValue->B;

	    NotifyPropertyChanged("");
    }

    GameEdit::Color4f^ ObjectLink::GetProperty_Color4f(Int32 propOffset)
    {
	    Gamedesk::Color4f* pVal = (Gamedesk::Color4f*)(((Byte*)mGDObject) + propOffset);
	    GameEdit::Color4f^ retVal = gcnew GameEdit::Color4f();
	    pVal->R = retVal->R;
	    pVal->G = retVal->G;
	    pVal->B = retVal->B;
	    pVal->A = retVal->A;
	    return retVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, GameEdit::Color4f^ newValue)
    {
	    Gamedesk::Color4f* pVal = (Gamedesk::Color4f*)(((Byte*)mGDObject) + propOffset);
	    pVal->R = newValue->R;
	    pVal->G = newValue->G;
	    pVal->B = newValue->B;
	    pVal->A = newValue->A;

	    NotifyPropertyChanged("");
    }

    String^ ObjectLink::GetProperty_String(Int32 propOffset)
    {
	    Gamedesk::String* pVal = (Gamedesk::String*)(((Byte*)mGDObject) + propOffset);
	    return Runtime::InteropServices::Marshal::PtrToStringAnsi( (IntPtr)(char*)pVal->c_str() );
    }

    void ObjectLink::SetProperty(Int32 propOffset, String^ newValue)
    {
	    Gamedesk::String* pVal = (Gamedesk::String*)(((Byte*)mGDObject) + propOffset);
		
	    const char* chars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(newValue)).ToPointer();
	    *pVal = chars;
	    Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)chars));

	    NotifyPropertyChanged("");
    }

    GameEdit::Enum^ ObjectLink::GetProperty_Enum(Int32 propOffset)
    {
	    Gamedesk::UInt32* pVal = (Gamedesk::UInt32*)(((Byte*)mGDObject) + propOffset);
	    GameEdit::Enum^ retVal = gcnew GameEdit::Enum();
	    retVal->Value = *pVal;
	    return retVal;
    }

    void ObjectLink::SetProperty(Int32 propOffset, GameEdit::Enum^ newValue)
    {
	    Gamedesk::UInt32* pVal = (Gamedesk::UInt32*)(((Byte*)mGDObject) + propOffset);
	    *pVal = newValue->Value;

	    NotifyPropertyChanged("");
    }

    void ObjectLink::NotifyPropertyChanged(String^ info)
    {
	    PropertyChanged(this, gcnew ComponentModel::PropertyChangedEventArgs(info));
    }
}