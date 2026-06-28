#include "stdafx.h"

#include "V8DataModel/ContentProvider.h"
#include "Util/AnimationId.h"


#include "V8DataModel/Workspace.h"
#include "V8DataModel/Camera.h"

#include "util/standardout.h"

namespace WBX {

// TODO: Refactor: It is a little ugly to have to implement these 6 functions for each "ContentID" derived class
//  Potentially we can template this a little. Maybe define a templated ContentIDPropDescriptor or something.
template<>
std::string StringConverter<AnimationId>::convertToString(const AnimationId& value)
{
	return value.toString();
}

template<>
bool StringConverter<AnimationId>::convertToValue(const std::string& text, AnimationId& value)
{
	value = text;
	return true;
}

namespace Reflection {

template<>
const Type& Type::getSingleton<wbx::AnimationId>()
{
	return Type::singleton<ContentId>();
}


template<>
void TypedPropertyDescriptor<wbx::AnimationId>::readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
{
	if (!element->isXsiNil())
	{
		ContentId value;
		if (element->getValue(value))
			setValue(instance, value);
	}
}

template<>
void TypedPropertyDescriptor<wbx::AnimationId>::writeValue(const DescribedBase* instance, XmlElement* element) const
{
	element->setValue(ContentId(getValue(instance)));
}



template<>
wbx::AnimationId& Variant::convert<AnimationId>(void)
{
	if (_type->isType<std::string>())
	{
		value = wbx::AnimationId(cast<std::string>());
		_type = &Type::singleton<AnimationId>();
	}
	return genericConvert<wbx::AnimationId>();
}

template<>
int TypedPropertyDescriptor<wbx::AnimationId>::getDataSize(const DescribedBase* instance) const 
{
    return sizeof(wbx::AnimationId) + getValue(instance).toString().size();
}

template<>
bool TypedPropertyDescriptor<wbx::AnimationId>::hasStringValue() const {
	return true;
}

template<>
std::string TypedPropertyDescriptor<wbx::AnimationId>::getStringValue(const DescribedBase* instance) const{
	return StringConverter<wbx::AnimationId>::convertToString(getValue(instance));
}

template<>
bool TypedPropertyDescriptor<wbx::AnimationId>::setStringValue(DescribedBase* instance, const std::string& text) const {
	wbx::AnimationId value;
	if (StringConverter<wbx::AnimationId>::convertToValue(text, value))
	{
		setValue(instance, value);
		return true;
	}
	else
		return false;
}

} // namespace Reflection
} // namespace WBX 