/********************************************************************
 *
 *  PropertyBag.cs
 *  --------------
 *  Copyright (C) 2002  Tony Allowatt
 *  Last Update: 12/14/2002
 * 
 *  THE SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS", WITHOUT WARRANTY
 *  OF ANY KIND, EXPRESS OR IMPLIED. IN NO EVENT SHALL THE AUTHOR BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OF THIS
 *  SOFTWARE.
 * 
 *  Public types defined in this file:
 *  ----------------------------------
 *  namespace Flobbster.Windows.Forms
 *     class PropertySpec
 *     class PropertySpecEventArgs
 *     delegate PropertySpecEventHandler
 *     class PropertyBag
 *        class PropertyBag.PropertySpecCollection
 *     class PropertyTable
 *
 ********************************************************************/
#include "stdafx.h"

#pragma unmanaged

#include "PropertyBag.h"

#pragma managed

using namespace System;
using namespace Collections;
using namespace Collections::Generic;
using namespace ComponentModel;
using namespace Drawing::Design;

namespace GameEdit
{
	PropertyBag::PropertyBag()
	{
		defaultProperty = nullptr;
		mProperties = gcnew List<PropertySpec^>();
	}

	AttributeCollection^ PropertyBag::GetAttributes()
	{
		return TypeDescriptor::GetAttributes(this, true);
	}

	String^ PropertyBag::GetClassName()
	{
		return TypeDescriptor::GetClassName(this, true);
	}

	String^ PropertyBag::GetComponentName()
	{
		return TypeDescriptor::GetComponentName(this, true);
	}

	TypeConverter^ PropertyBag::GetConverter()
	{
		return TypeDescriptor::GetConverter(this, true);
	}

	EventDescriptor^ PropertyBag::GetDefaultEvent()
	{
		return TypeDescriptor::GetDefaultEvent(this, true);
	}

	PropertyDescriptor^ PropertyBag::GetDefaultProperty()
	{
		// This function searches the property list for the property
		// with the same name as the DefaultProperty specified, and
		// returns a property descriptor for it.  If no property is
		// found that matches DefaultProperty, a null reference is
		// returned instead.

		PropertySpec^ propertySpec = nullptr;
		if(defaultProperty != nullptr)
		{
			for each(PropertySpec^ spec in mProperties)
			{
				if(spec->Name == defaultProperty)
				{
					propertySpec = spec;
					break;
				}
			}
		}

		if(propertySpec != nullptr)
			return gcnew PropertySpecDescriptor(propertySpec, this, propertySpec->Name, nullptr);
		else
			return nullptr;
	}

	Object^ PropertyBag::GetEditor(Type^ editorBaseType)
	{
		return TypeDescriptor::GetEditor(this, editorBaseType, true);
	}

	EventDescriptorCollection^ PropertyBag::GetEvents()
	{
		return TypeDescriptor::GetEvents(this, true);
	}

	EventDescriptorCollection^ PropertyBag::GetEvents(array<Attribute^>^ attributes)
	{
		return TypeDescriptor::GetEvents(this, attributes, true);
	}

	PropertyDescriptorCollection^ PropertyBag::GetProperties()
	{
		return GetProperties(gcnew array<Attribute^>{});
	}

	PropertyDescriptorCollection^ PropertyBag::GetProperties(array<Attribute^>^)
	{
		// Rather than passing this function on to the default TypeDescriptor,
		// which would return the actual properties of PropertyBag, I construct
		// a list here that contains property descriptors for the elements of the
		// Properties list in the bag.

		ArrayList^ props = gcnew ArrayList();

		for each(PropertySpec^ prop in mProperties)
		{
			ArrayList^ attrs = gcnew ArrayList();

			// If a category, description, editor, or type converter are specified
			// in the PropertySpec, create attributes to define that relationship.
			if(prop->Category != nullptr)
				attrs->Add(gcnew CategoryAttribute(prop->Category));

			if(prop->Description != nullptr)
				attrs->Add(gcnew DescriptionAttribute(prop->Description));

			if(prop->EditorTypeName != nullptr)
				attrs->Add(gcnew EditorAttribute(prop->EditorTypeName, UITypeEditor::typeid));

			if(prop->ConverterTypeName != nullptr)
				attrs->Add(gcnew TypeConverterAttribute(prop->ConverterTypeName));

			// Additionally, append the custom attributes associated with the
			// PropertySpec, if any.
			if(prop->Attributes != nullptr)
				attrs->AddRange(prop->Attributes);

			array<Attribute^>^ attrArray = (array<Attribute^>^)attrs->ToArray(Attribute::typeid);

			// Create a new property descriptor for the property item, and add
			// it to the list.
			PropertySpecDescriptor^ pd = gcnew PropertySpecDescriptor(prop, this, prop->Name, attrArray);
			props->Add(pd);
		}

		// Convert the list of PropertyDescriptors to a collection that the
		// ICustomTypeDescriptor can use, and return it.
		array<PropertyDescriptor^>^ propArray = (array<PropertyDescriptor^>^)props->ToArray(PropertyDescriptor::typeid);
		return gcnew PropertyDescriptorCollection(propArray);
	}

	Object^ PropertyBag::GetPropertyOwner(PropertyDescriptor^)
	{
		return this;
	}
					
	///////////////////////////////////////////////////////////////////////////

	PropertyBag::PropertySpecDescriptor::PropertySpecDescriptor(PropertySpec^ i, PropertyBag^ b, String^ name, array<Attribute^>^ attrs) : PropertyDescriptor(name, attrs)
	{
		bag = b;
		item = i;
	}
	
	bool PropertyBag::PropertySpecDescriptor::CanResetValue(Object^ component)
	{
		if(item->DefaultValue == nullptr)
			return false;
		else
			return !GetValue(component)->Equals(item->DefaultValue);
	}

	Object^ PropertyBag::PropertySpecDescriptor::GetValue(Object^)
	{
		// Have the property bag raise an event to get the current value
		// of the property.

		PropertySpecEventArgs^ e = gcnew PropertySpecEventArgs(item, nullptr);
		bag->OnGetValue(e);
		return e->Value;
	}

	void PropertyBag::PropertySpecDescriptor::ResetValue(Object^ component)
	{
		SetValue(component, item->DefaultValue);
	}

	void PropertyBag::PropertySpecDescriptor::SetValue(Object^, Object^ value)
	{
		// Have the property bag raise an event to set the current value
		// of the property.

		PropertySpecEventArgs^ e = gcnew PropertySpecEventArgs(item, value);
		bag->OnSetValue(e);
	}

	bool PropertyBag::PropertySpecDescriptor::ShouldSerializeValue(Object^ component)
	{
		Object^ val = GetValue(component);

		if(item->DefaultValue == nullptr && val == nullptr)
			return false;
		else
			return !val->Equals(item->DefaultValue);
	}
}
