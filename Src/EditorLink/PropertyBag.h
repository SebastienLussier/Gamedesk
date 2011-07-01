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
#pragma managed


namespace GameEdit
{
	/// <summary>
	/// Represents a single property in a PropertySpec.
	/// </summary>
	public ref class PropertySpec
	{
	public:
		/// <summary>
		/// Gets or sets a collection of additional Attributes for this property.  This can
		/// be used to specify attributes beyond those supported intrinsically by the
		/// PropertySpec class, such as ReadOnly and Browsable.
		/// </summary>
		property array<Attribute^>^ Attributes;

		/// <summary>
		/// Gets or sets the category name of this property.
		/// </summary>
		property String^ Category;

		/// <summary>
		/// Gets or sets the fully qualified name of the type converter
		/// type for this property.
		/// </summary>
		property String^ ConverterTypeName;

		/// <summary>
		/// Gets or sets the default value of this property.
		/// </summary>
		property Object^ DefaultValue;

		/// <summary>
		/// Gets or sets the help text description of this property.
		/// </summary>
		property String^ Description;

		/// <summary>
		/// Gets or sets the fully qualified name of the editor type for
		/// this property.
		/// </summary>
		property String^ EditorTypeName;

		/// <summary>
		/// Gets or sets the name of this property.
		/// </summary>
		property String^ Name;

		/// <summary>
		/// Gets or sets the fully qualfied name of the type of this
		/// property.
		/// </summary>
		property String^ TypeName;
	};

	/// <summary>
	/// Provides data for the GetValue and SetValue events of the PropertyBag class.
	/// </summary>
	public ref class PropertySpecEventArgs : EventArgs
	{
	public:
		/// <summary>
		/// Initializes a new instance of the PropertySpecEventArgs class.
		/// </summary>
		/// <param name="property">The PropertySpec that represents the property whose
		/// value is being requested or set.</param>
		/// <param name="val">The current value of the property.</param>
		PropertySpecEventArgs(PropertySpec^ propertySpec, Object^ val)
		{
			Property = propertySpec;
			Value = val;
		}

		/// <summary>
		/// Gets the PropertySpec that represents the property whose value is being
		/// requested or set.
		/// </summary>
		property PropertySpec^ Property;

		/// <summary>
		/// Gets or sets the current value of the property.
		/// </summary>
		property Object^ Value;
	};

	/// <summary>
	/// Represents the method that will handle the GetValue and SetValue events of the
	/// PropertyBag class.
	/// </summary>
	public delegate void PropertySpecEventHandler(Object^ sender, PropertySpecEventArgs^ e);

	/// <summary>
	/// Represents a collection of custom properties that can be selected into a
	/// PropertyGrid to provide functionality beyond that of the simple reflection
	/// normally used to query an object's properties.
	/// </summary>
	public ref class PropertyBag : ComponentModel::ICustomTypeDescriptor
	{
	public:
		/// <summary>
		/// Initializes a new instance of the PropertyBag class.
		/// </summary>
		PropertyBag();

		/// <summary>
		/// Gets or sets the name of the default property in the collection.
		/// </summary>
		property String^ DefaultProperty;

		/// <summary>
		/// Gets the collection of properties contained within this PropertyBag.
		/// </summary>
		property Collections::Generic::List<PropertySpec^>^ Properties
		{
			Collections::Generic::List<PropertySpec^>^ get()
			{
				return mProperties;
			}
		}

		/// <summary>
		/// Occurs when a PropertyGrid requests the value of a property.
		/// </summary>
		virtual event PropertySpecEventHandler^ GetValue
		{
			void add(PropertySpecEventHandler^ handler)
			{
				GetValueHandlers += handler;
			}

			void remove(PropertySpecEventHandler^ handler)
			{
				GetValueHandlers += handler;
			}
		}
		

		/// <summary>
		/// Occurs when the user changes the value of a property in a PropertyGrid.
		/// </summary>
		virtual event PropertySpecEventHandler^ SetValue
		{
			void add(PropertySpecEventHandler^ handler)
			{
				SetValueHandlers += handler;
			}

			void remove(PropertySpecEventHandler^ handler)
			{
				SetValueHandlers += handler;
			}
		}

		// Overiding 
		virtual ComponentModel::AttributeCollection^ GetAttributes();
		virtual String^ GetClassName();
		virtual String^ GetComponentName();
		virtual ComponentModel::TypeConverter^ GetConverter();
		virtual ComponentModel::EventDescriptor^ GetDefaultEvent();
		virtual ComponentModel::PropertyDescriptor^ GetDefaultProperty();
		virtual Object^ GetEditor(Type^ editorBaseType);
		virtual ComponentModel::EventDescriptorCollection^ GetEvents();
		virtual ComponentModel::EventDescriptorCollection^ GetEvents(array<Attribute^>^ attributes);
		virtual ComponentModel::PropertyDescriptorCollection^ GetProperties();
		virtual ComponentModel::PropertyDescriptorCollection^ GetProperties(array<Attribute^>^ attributes);
		virtual Object^ GetPropertyOwner(ComponentModel::PropertyDescriptor^ pd);
				
	protected:
		/// <summary>
		/// Raises the GetValue event.
		/// </summary>
		/// <param name="e">A PropertySpecEventArgs that contains the event data.</param>
		virtual void OnGetValue(PropertySpecEventArgs^ e)
		{
			if(GetValueHandlers != nullptr)
				GetValueHandlers(this, e);
		}

		/// <summary>
		/// Raises the SetValue event.
		/// </summary>
		/// <param name="e">A PropertySpecEventArgs that contains the event data.</param>
		virtual void OnSetValue(PropertySpecEventArgs^ e)
		{
			if(SetValueHandlers != nullptr)
				SetValueHandlers(this, e);
		}

	private:
		PropertySpecEventHandler^ GetValueHandlers;
		PropertySpecEventHandler^ SetValueHandlers;

		ref class PropertySpecDescriptor : ComponentModel::PropertyDescriptor
		{
		public:
			PropertySpecDescriptor(PropertySpec^ item, PropertyBag^ bag, String^ name, array<Attribute^>^ attrs);

			virtual property Type^ ComponentType
			{
				Type^ get() override
				{
					return item->GetType();
				}
			}

			virtual property bool IsReadOnly
			{
				bool get() override
				{
					return (Attributes->Matches(ComponentModel::ReadOnlyAttribute::Yes));
				}
			}

			virtual property Type^ PropertyType
			{
				Type^ get() override
				{
					return Type::GetType(item->TypeName);
				}
			}

			virtual bool CanResetValue(Object^ component) override;

			virtual Object^ GetValue(Object^ component) override;

			virtual void ResetValue(Object^ component) override;

			virtual void SetValue(Object^ component, Object^ value) override;

			virtual bool ShouldSerializeValue(Object^ component) override;

		private:
			PropertyBag^ bag;
			PropertySpec^ item;
		};

		String^ defaultProperty;
		Collections::Generic::List<PropertySpec^>^ mProperties;
	};
}
