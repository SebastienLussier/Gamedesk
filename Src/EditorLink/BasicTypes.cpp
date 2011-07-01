#include "stdafx.h"
#include "BasicTypes.h"
#include "EditorLink.h"

namespace GameEdit
{
	float Vector3f::x::get()	{ return mObject->GetProperty_Float(mPropOffset); }
	void Vector3f::x::set(float value) { mObject->SetProperty(mPropOffset, value); }

	float Vector3f::y::get()	{ return mObject->GetProperty_Float(mPropOffset + 4); }
	void Vector3f::y::set(float value) { mObject->SetProperty(mPropOffset + 4, value); }

	float Vector3f::z::get()	{ return mObject->GetProperty_Float(mPropOffset + 8); }
	void Vector3f::z::set(float value) { mObject->SetProperty(mPropOffset + 8, value); }
}
