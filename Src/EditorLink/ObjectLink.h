#pragma once

#include "BasicTypes.h"

namespace GameEdit
{
    public ref class ObjectLink : ComponentModel::INotifyPropertyChanged
	{
	public:
		virtual event ComponentModel::PropertyChangedEventHandler^ PropertyChanged
		{
			void add(ComponentModel::PropertyChangedEventHandler^ handler);
			void remove(ComponentModel::PropertyChangedEventHandler^ handler);
			void raise(Object^ sender, ComponentModel::PropertyChangedEventArgs^ e);
		}

        static ObjectLink^ Create(Gamedesk::Object* pGDObject);

	public:
		Boolean GetProperty_Bool(Int32 propOffset);
		void SetProperty(Int32 propOffset, Boolean newValue);

		Char GetProperty_Char(Int32 propOffset);
		void SetProperty(Int32 propOffset, Char newValue);

		Int16 GetProperty_Int16(Int32 propOffset);
		void SetProperty(Int32 propOffset, Int16 newValue);

		Int32 GetProperty_Int32(Int32 propOffset);
		void SetProperty(Int32 propOffset, Int32 newValue);

		Int64 GetProperty_Int64(Int32 propOffset);
		void SetProperty(Int32 propOffset, Int64 newValue);

		Byte GetProperty_Byte(Int32 propOffset);
		void SetProperty(Int32 propOffset, Byte newValue);

		UInt16 GetProperty_UInt16(Int32 propOffset);
		void SetProperty(Int32 propOffset, UInt16 newValue);

		UInt32 GetProperty_UInt32(Int32 propOffset);
		void SetProperty(Int32 propOffset, UInt32 newValue);

		UInt64 GetProperty_UInt64(Int32 propOffset);
		void SetProperty(Int32 propOffset, UInt64 newValue);

		Single GetProperty_Float(Int32 propOffset);
		Single GetProperty_Single(Int32 propOffset);
		void SetProperty(Int32 propOffset, Single newValue);

		Double SetProperty_Double(Int32 propOffset);
		void SetProperty(Int32 propOffset, Double newValue);

		GameEdit::Vector3f^ GetProperty_Vector3f(Int32 propOffset);
		void SetProperty(Int32 propOffset, GameEdit::Vector3f^ newValue);

		GameEdit::Quaternionf^ GetProperty_Quaternionf(Int32 propOffset);
		void SetProperty(Int32 propOffset, GameEdit::Quaternionf^ newValue);

		GameEdit::Color3f^ GetProperty_Color3f(Int32 propOffset);
		void SetProperty(Int32 propOffset, GameEdit::Color3f^ newValue);

		GameEdit::Color4f^ GetProperty_Color4f(Int32 propOffset);
		void SetProperty(Int32 propOffset, GameEdit::Color4f^ newValue);

		String^ GetProperty_String(Int32 propOffset);
		void SetProperty(Int32 propOffset, String^ newValue);

		GameEdit::Enum^ GetProperty_Enum(Int32 propOffset);
		void SetProperty(Int32 propOffset, GameEdit::Enum^ newValue);

		Gamedesk::Object* mGDObject;

	private:
		void NotifyPropertyChanged(String^ info);

		ComponentModel::PropertyChangedEventHandler^ m_myEvent;
	};
}
