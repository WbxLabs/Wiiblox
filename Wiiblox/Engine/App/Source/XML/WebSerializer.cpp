#include "stdafx.h"

#include "V8Xml/WebSerializer.h"
#include "V8Xml/Serializer.h"
#include "wbx/make_shared.h"

namespace WBX
{
	XmlElement* WebSerializer::writeTable(const wbx::Reflection::ValueMap& result)
	{
		XmlElement* table = new XmlElement(tag_WebTable);

		wbx::Reflection::ValueMap::const_iterator end = result.end();
		for(wbx::Reflection::ValueMap::const_iterator iter = result.begin(); iter != end; ++iter)
		{
			if(XmlElement* child = writeEntry(iter->first, iter->second))
			{
				table->addChild(child);
			}
		}
		return table;
	}
	XmlElement* WebSerializer::writeList(const wbx::Reflection::ValueArray& result)
	{
		XmlElement* list = new XmlElement(tag_WebList);

		wbx::Reflection::ValueArray::const_iterator end = result.end();
		for(wbx::Reflection::ValueArray::const_iterator iter = result.begin(); iter != end; ++iter)
		{
			if(XmlElement* child = writeValue(*iter))
				list->addChild(child);
		}
		return list;
	}
	XmlElement* WebSerializer::writeEntry(const std::string& key, const wbx::Reflection::Variant& value)
	{
		if(XmlElement* valueElement = writeValue(value))
		{
			XmlElement* entry = new XmlElement(tag_WebEntry);
			{
				XmlElement* keyElement = new XmlElement(tag_WebKey);
				keyElement->setValue(key);
				entry->addChild(keyElement);
			}
			entry->addChild(valueElement);
			return entry;
		}
		return NULL;
	}
	XmlElement* WebSerializer::writeValue(const wbx::Reflection::Variant& value)
	{
		if(value.isType<double>() || value.isType<float>())
		{
			XmlElement* result = new XmlElement(tag_WebValue);
			result->addAttribute(tag_WebType, "number");
			result->setValue(value.get<std::string>());
			return result;
		}
		if(value.isType<std::string>())
		{
			XmlElement* result = new XmlElement(tag_WebValue);
			result->addAttribute(tag_WebType, "string");
			result->setValue(value.get<std::string>());
			return result;
		}
		if(value.isType<bool>())
		{
			XmlElement* result = new XmlElement(tag_WebValue);
			result->addAttribute(tag_WebType, "boolean");
			result->setValue(value.get<bool>() ? "true" : "false");
			return result;
		}
		if(value.isType<shared_ptr<wbx::Instance> >())
		{
			shared_ptr<Instance> instance = value.get<shared_ptr<Instance> >();
			if(instance)
			{
				if(XmlElement* instanceRoot = Instance::toNewXmlRoot(instance.get(), SerializationCreator))
				{
					XmlElement* result = new XmlElement(tag_WebValue);
					result->addAttribute(tag_WebType, "instance");
					result->addChild(instanceRoot);
					return result;
				}
			}
			return NULL;
		}
		if(value.isType<shared_ptr<const wbx::Reflection::ValueMap> >())
		{
			shared_ptr<const wbx::Reflection::ValueMap> valueMap = value.get<shared_ptr<const wbx::Reflection::ValueMap> >();
			if(XmlElement* valueMapRoot = writeTable(*valueMap))
			{
				XmlElement* result = new XmlElement(tag_WebValue);
				result->addAttribute(tag_WebType, "table");
				result->addChild(valueMapRoot);
				return result;
			}
			return NULL;
		}
		if(value.isType<shared_ptr<const wbx::Reflection::ValueArray> >())
		{
			shared_ptr<const wbx::Reflection::ValueArray> valueList = value.get<shared_ptr<const wbx::Reflection::ValueArray> >();
			if(XmlElement* valueListRoot = writeList(*valueList))
			{
				XmlElement* result = new XmlElement(tag_WebValue);
				result->addAttribute(tag_WebType, "list");
				result->addChild(valueListRoot);
				return result;
			}
			return NULL;
		}
		return NULL;		
	}
	

}