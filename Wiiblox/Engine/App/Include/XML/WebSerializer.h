#pragma once

#include "Reflection/Type.h"

class XmlElement;

namespace WBX
{
	class WebSerializer {
	public:
		static XmlElement* writeTable(const wbx::Reflection::ValueMap& result);
		static XmlElement* writeList(const wbx::Reflection::ValueArray& result);
		static XmlElement* writeEntry(const std::string& key, const wbx::Reflection::Variant& value);
		static XmlElement* writeValue(const wbx::Reflection::Variant& value);
	};
}
