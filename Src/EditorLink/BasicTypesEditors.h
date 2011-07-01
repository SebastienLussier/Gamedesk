#pragma once

#include "GlobalResources.h"

namespace GameEdit
{
	ref class Vector3fEditor : Activities::Presentation::PropertyEditing::ExtendedPropertyValueEditor
	{
	public:
		Vector3fEditor()
		{
			this->InlineEditorTemplate = safe_cast<Windows::DataTemplate^>(GlobalResources::Resources["DefaultInlineEditor"]);
			this->ExtendedEditorTemplate = safe_cast<Windows::DataTemplate^>(GlobalResources::Resources["Vector3fExtendedEditor"]);
		}
	};
}