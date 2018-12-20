//
//  xmlhelper.hpp
//  OpenVPN
//
//  Copyright (C) 2012-2017 OpenVPN Technologies, Inc.
//  All rights reserved.
//

#pragma once

#include <string>

#include <tinyxml2.h>

namespace openvpn {

  class Xml
  {
  public:
    OPENVPN_EXCEPTION(xml_parse);

    struct Document : public tinyxml2::XMLDocument
    {
      Document(const std::string& str,
	       const std::string& title)
      {
	if (tinyxml2::XMLDocument::Parse(str.c_str()))
	  OPENVPN_THROW(xml_parse, title << " : " << format_error(*this));
      }
    };

    static std::string to_string(const tinyxml2::XMLDocument& doc)
    {
      tinyxml2::XMLPrinter printer;
      doc.Print(&printer);
      return printer.CStr();
    }

    static std::string format_error(const tinyxml2::XMLDocument& doc)
    {
      std::string ret = doc.ErrorName();
#if OVPN_TINYXML2_VERSION_MAJOR < 6
      const char *es1 = doc.GetErrorStr1();
      const char *es2 = doc.GetErrorStr2();
      if (es1)
	{
	  ret += ' ';
	  ret += es1;
	}
      if (es2)
	{
	  ret += ' ';
	  ret += es2;
	}
#else
      const char *es = doc.ErrorStr();
      if (es)
	{
	  ret += ' ';
	  ret += es;
	}
#endif
      return ret;
    }

    template<typename T, typename... Args>
    static std::string find_text(const tinyxml2::XMLNode* node,
				 const T& first,
				 Args... args)
    {
      const tinyxml2::XMLElement* e = find(node, first, args...);
      if (e)
	return e->GetText();
      else
	return std::string();
    }

    template<typename T, typename... Args>
    static const tinyxml2::XMLElement* find(const tinyxml2::XMLNode* node,
					    const T& first,
					    Args... args)
    {
      const tinyxml2::XMLElement *e = find(node, first);
      if (e)
	e = find(e, args...);
      return e;
    }

    static const tinyxml2::XMLElement* find(const tinyxml2::XMLNode* node,
					    const std::string& first)
    {
      return node->FirstChildElement(first.c_str());
    }

    static const tinyxml2::XMLElement* find(const tinyxml2::XMLNode* node,
					    const char *first)
    {
      return node->FirstChildElement(first);
    }

    static const tinyxml2::XMLElement* find(const tinyxml2::XMLElement* elem)
    {
      return elem;
    }

    static const tinyxml2::XMLElement* next_sibling(const tinyxml2::XMLNode* node,
						    const std::string& name)
    {
      return node->NextSiblingElement(name.c_str());
    }

    static const tinyxml2::XMLElement* next_sibling(const tinyxml2::XMLNode* node,
						    const char *name)
    {
      return node->NextSiblingElement(name);
    }

    static const tinyxml2::XMLElement* next_sibling(const tinyxml2::XMLNode* node)
    {
      return node->NextSiblingElement();
    }

  };
}
