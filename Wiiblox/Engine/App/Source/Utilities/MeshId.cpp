#include "stdafx.h"

#include "V8DataModel/ContentProvider.h"
#include "Util/MeshId.h"


#include "V8DataModel/Workspace.h"
#include "V8DataModel/Camera.h"

#include "util/standardout.h"

namespace WBX
{
	// TODO: Refactor: It is a little ugly to have to implement these 6 functions for each "ContentID" derived class
	//  Potentially we can template this a little. Maybe define a templated ContentIDPropDescriptor or something.
	template<>
	std::string StringConverter<MeshId>::convertToString(const MeshId& value)
	{
		return value.toString();
	}

	template<>
	bool StringConverter<MeshId>::convertToValue(const std::string& text, MeshId& value)
	{
		value = text;
		return true;
	}

	namespace Reflection
	{
		template<>
		const Reflection::Type& Reflection::Type::getSingleton<MeshId>()
		{
			return Reflection::Type::singleton<ContentId>();
		}


		template<>
		void TypedPropertyDescriptor<wbx::MeshId>::readValue(DescribedBase* instance, const XmlElement* element, IReferenceBinder& binder) const
		{
			if (!element->isXsiNil())
			{
				ContentId value;
				if (element->getValue(value))
					setValue(instance, value);
			}
		}

		template<>
		void TypedPropertyDescriptor<wbx::MeshId>::writeValue(const DescribedBase* instance, XmlElement* element) const
		{
			element->setValue(ContentId(getValue(instance)));
		}



		template<>
		wbx::MeshId& Variant::convert<MeshId>(void)
		{
			if (_type->isType<std::string>())
			{
				value = wbx::MeshId(cast<std::string>());
				_type = &Type::singleton<MeshId>();
			}
			return genericConvert<wbx::MeshId>();
		}

        template<>
        int TypedPropertyDescriptor<wbx::MeshId>::getDataSize(const DescribedBase* instance) const 
        {
            return sizeof(wbx::MeshId) + getValue(instance).toString().size();
        }

		template<>
		bool TypedPropertyDescriptor<wbx::MeshId>::hasStringValue() const {
			return true;
		}

		template<>
		std::string TypedPropertyDescriptor<wbx::MeshId>::getStringValue(const DescribedBase* instance) const{
			return StringConverter<wbx::MeshId>::convertToString(getValue(instance));
		}

		template<>
		bool TypedPropertyDescriptor<wbx::MeshId>::setStringValue(DescribedBase* instance, const std::string& text) const {
			wbx::MeshId value;
			if (StringConverter<wbx::MeshId>::convertToValue(text, value))
			{
				setValue(instance, value);
				return true;
			}
			else
				return false;
		}

	} // namespace Reflection
}// namespace WBX