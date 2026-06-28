#pragma once

#include "Reflection/Type.h"

class XmlElement;

namespace WBX
{
	class WebParser 
	{
	private:
		static boost::mutex JSONmutex;
	public:

		typedef enum 
		{
			FailOnNonJSON,
			SkipNonJSON
		} NonJSONBehavior;

		static bool parseWebGenericResponse(std::istream& stream, wbx::Reflection::Variant& result);
		static bool parseWebGenericResponse(const XmlElement* root, wbx::Reflection::Variant& result);
		static bool parseWebListResponse(std::istream& stream, wbx::Reflection::ValueArray& result);
		static bool legacyParseWebJSONResponse(std::stringstream& rawWebResponse, shared_ptr<const Reflection::ValueTable>& valueTable);
		static bool ptreeParseWebJSONResponse(std::stringstream& rawWebResponse, shared_ptr<const Reflection::ValueTable>& valueTable);

		static bool parseJSONTable(const std::string& rawWebResponse, shared_ptr<const Reflection::ValueTable>& valueTable);
		static bool parseJSONArray(const std::string& rawWebResponse, shared_ptr<const Reflection::ValueArray>& valueArray);
		static bool parseJSONObject(const std::string& rawWebResponse, Reflection::Variant& result);
		
		static bool writeJSON(const Reflection::Variant& value, std::string& result, NonJSONBehavior skip = SkipNonJSON);
	
	protected:
		static bool loadTable(const XmlElement* tableElement, wbx::Reflection::ValueMap& result);
		static bool loadList(const XmlElement*  listElement, wbx::Reflection::ValueArray& result);
		static bool loadEntry(const XmlElement* entryElement, std::string& key, wbx::Reflection::Variant& value);
		static bool loadValue(const XmlElement* valueElement, wbx::Reflection::Variant& value);
	};
}
