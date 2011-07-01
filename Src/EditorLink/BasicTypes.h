#pragma once

#include "BasicTypesEditors.h"
#include "Engine.h"

namespace GameEdit
{
	//[ComponentModel::Editor(Vector3fEditor::typeid, Activities::Presentation::PropertyEditing::ExtendedPropertyValueEditor::typeid)]
    //[ComponentModel::TypeConverter(ComponentModel::ExpandableObjectConverter::typeid)]
    [ComponentModel::TypeConverter(ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class Vector3f
	{
	public:
		Vector3f(ref class ObjectLink^ pObject, Int32 propOffset)
			: mObject(pObject)
			, mPropOffset(propOffset)
		{
		}

        Vector3f(Vector3f% pOther)
            : mObject(pOther.mObject)
			, mPropOffset(pOther.mPropOffset)
        {
        }

        virtual bool Equals(Object^ obj) override
        {
            if(obj == nullptr)
                return false;

            if(GetType() != obj->GetType())
                return false;

            // Safe because of the GetType check
            Vector3f^ vec = (Vector3f^) obj;     

            // use this pattern to compare reference members
            if (x == vec->x && y == vec->y && z == vec->z)
                return true;

            return false;
        }

		virtual String^ ToString() override
		{
			String^ toString = "{";
			toString += x;
			toString += ",";
			toString += y;
			toString += ",";
			toString += z;
			toString += "}";
			return toString;
		}
		
		property float x
		{
			float get();
			void set(float value);
		}

		property float y
		{
			float get();
			void set(float value);
		}

		property float z
		{
			float get();
			void set(float value);
		}

	private:
		ref class ObjectLink^ mObject;
		Int32 mPropOffset;		
	};

	public ref struct Quaternionf
	{
	public:
		Quaternionf() {}
		Quaternionf(Quaternionf% copy)
		{
			x = copy.x;
			y = copy.y;
			z = copy.z;
			w = copy.w;
		}

		property float x;
		property float y;
		property float z;
		property float w;
	};

	public ref struct Color3f
	{
	public:
		Color3f() {}
		Color3f(Color3f% copy)
		{
			R = copy.R;
			G = copy.G;
			B = copy.B;
		}

		property float R;
		property float G;
		property float B;
	};

	public ref struct Color4f
	{
	public:
		Color4f() {}
		Color4f(Color4f% copy)
		{
			R = copy.R;
			G = copy.G;
			B = copy.B;
			A = copy.A;
		}

		property float R;
		property float G;
		property float B;
		property float A;
	};

	public ref struct Enum
	{
	public:
		Enum() {}
		Enum(Enum% copy)
		{
			Value = copy.Value;
		}

		property UInt32 Value;
	};
}