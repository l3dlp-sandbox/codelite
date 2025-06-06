//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : archive.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "archive.h"

#include "clFontHelper.h"
#include "serialized_object.h"
#include "xmlutils.h"

#include <wx/colour.h>
#include <wx/xml/xml.h>

namespace
{
const wxChar breakpointName[] = wxT("Breakpoint");
const wxChar breakpointArrayName[] = wxT("BreakpointArray");
} // namespace

// class Tab info
TabInfo::TabInfo()
    : m_firstVisibleLine(0)
    , m_currentLine(0)
{
}

TabInfo::~TabInfo() {}

void TabInfo::DeSerialize(Archive& arch)
{
    arch.Read(wxT("FileName"), m_fileName);
    arch.Read(wxT("FirstVisibleLine"), m_firstVisibleLine);
    arch.Read(wxT("CurrentLine"), m_currentLine);
    arch.Read(wxT("Bookmarks"), m_bookmarks);
    arch.Read(wxT("CollapsedFolds"), m_folds);
}

void TabInfo::Serialize(Archive& arch)
{
    arch.Write(wxT("FileName"), m_fileName);
    arch.Write(wxT("FirstVisibleLine"), m_firstVisibleLine);
    arch.Write(wxT("CurrentLine"), m_currentLine);
    arch.Write(wxT("Bookmarks"), m_bookmarks);
    arch.Write(wxT("CollapsedFolds"), m_folds);
}

// class Archive
Archive::Archive()
    : m_root(NULL)
{
}

Archive::~Archive() {}

bool Archive::Write(const wxString& name, SerializedObject* obj)
{
    Archive arch;
    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("SerializedObject"), name);
    if(node) {
        m_root->RemoveChild(node);
        delete node;
    }

    node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("SerializedObject"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    arch.SetXmlNode(node);
    obj->Serialize(arch);
    return true;
}

bool Archive::Read(const wxString& name, SerializedObject* obj)
{
    Archive arch;
    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("SerializedObject"), name);
    if(node) {
        arch.SetXmlNode(node);
        obj->DeSerialize(arch);
        return true;
    }
    return false;
}

bool Archive::Write(const wxString& name, const wxArrayString& arr)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxArrayString"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each wxString in the array
    for(size_t i = 0; i < arr.GetCount(); i++) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxString"));
        node->AddChild(child);
        child->AddAttribute(wxT("Value"), arr.Item(i));
    }
    return true;
}

bool Archive::Write(const wxString& name, std::vector<TabInfo>& _vTabInfoArr)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("TabInfoArray"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each wxString in the array
    for(size_t i = 0; i < _vTabInfoArr.size(); i++) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("TabInfo"));
        Archive arch;
        arch.SetXmlNode(child);
        _vTabInfoArr[i].Serialize(arch);
        node->AddChild(child);
    }
    return true;
}

bool Archive::Write(const wxString& name, std::vector<int>& _vInt)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("IntVector"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each int in the vector
    for(size_t i = 0; i < _vInt.size(); ++i) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("IntVectorItem"));
        node->AddChild(child);
        child->AddAttribute(wxT("Value"), wxString::Format(wxT("%i"), _vInt.at(i)));
    }
    return true;
}

bool Archive::Write(const wxString& name, const StringMap& str_map)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("StringMap"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each wxString in the array
    for (const auto& [key, value] : str_map) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("MapEntry"));
        node->AddChild(child);
        child->AddAttribute(wxT("Key"), key);
        child->AddAttribute(wxT("Value"), value);
    }
    return true;
}

#if wxUSE_GUI
bool Archive::Write(const wxString& name, wxSize size)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxSize"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    wxString xstr, ystr;
    xstr << size.x;
    ystr << size.y;

    node->AddAttribute(wxT("x"), xstr);
    node->AddAttribute(wxT("y"), ystr);
    return true;
}
bool Archive::Write(const wxString& name, wxPoint pt)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxPoint"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    wxString xstr, ystr;
    xstr << pt.x;
    ystr << pt.y;

    node->AddAttribute(wxT("x"), xstr);
    node->AddAttribute(wxT("y"), ystr);
    return true;
}
#endif

bool Archive::Read(const wxString& name, wxArrayString& arr)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("wxArrayString"), name);
    if(node) {
        // fill the output array with the values
        arr.Clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("wxString")) {
                wxString value;
                value = child->GetAttribute(wxT("Value"), wxEmptyString);
                arr.Add(value);
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, std::vector<TabInfo>& _vTabInfoArr)
{
    if(!m_root) {
        return false;
    }

    Archive arch;
    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("TabInfoArray"), name);
    if(node) {
        // fill the output array with the values
        _vTabInfoArr.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("TabInfo")) {
                arch.SetXmlNode(child);
                TabInfo oTabInfo;
                oTabInfo.DeSerialize(arch);
                _vTabInfoArr.push_back(oTabInfo);
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, std::vector<int>& _vInt)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("IntVector"), name);
    if(node) {
        // fill the output array with the values
        _vInt.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("IntVectorItem")) {
                long value;
                wxString stringvalue = child->GetAttribute(wxT("Value"), wxEmptyString);
                if(stringvalue.ToLong(&value)) {
                    _vInt.push_back(value);
                }
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, StringMap& str_map)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("StringMap"), name);
    if(node) {
        // fill the output array with the values
        str_map.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("MapEntry")) {
                wxString value;
                wxString key;
                key = child->GetAttribute(wxT("Key"), wxEmptyString);
                value = child->GetAttribute(wxT("Value"), wxEmptyString);
                str_map[key] = value;
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}
#if wxUSE_GUI
bool Archive::Read(const wxString& name, wxSize& size)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("wxSize"), name);
    if(node) {
        long v;
        wxString value;
        value = node->GetAttribute(wxT("x"), wxEmptyString);
        value.ToLong(&v);
        size.x = v;

        value = node->GetAttribute(wxT("y"), wxEmptyString);
        value.ToLong(&v);
        size.y = v;
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, wxPoint& pt)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("wxPoint"), name);
    if(node) {
        long v;
        wxString value;
        value = node->GetAttribute(wxT("x"), wxEmptyString);
        value.ToLong(&v);
        pt.x = v;

        value = node->GetAttribute(wxT("y"), wxEmptyString);
        value.ToLong(&v);
        pt.y = v;
        return true;
    }
    return false;
}
#endif

bool Archive::Write(const wxString& name, int value) { return WriteSimple(value, wxT("int"), name); }

bool Archive::Read(const wxString& name, int& value)
{
    if(!m_root) {
        return false;
    }

    long v;
    bool res = ReadSimple(v, wxT("int"), name);
    if(res) {
        value = v;
    }
    return res;
}

bool Archive::Write(const wxString& name, long value) { return WriteSimple(value, wxT("long"), name); }

bool Archive::Read(const wxString& name, long& value) { return ReadSimple(value, wxT("long"), name); }

bool Archive::Write(const wxString& name, bool value) { return WriteSimple(value ? 1 : 0, wxT("bool"), name); }

bool Archive::Read(const wxString& name, bool& value)
{
    long v;
    bool res = ReadSimple(v, wxT("bool"), name);
    if(res) {
        v == 0 ? value = false : value = true;
    }
    return res;
}

bool Archive::Write(const wxString& name, const wxString& str)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxString"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Value"), str);
    node->AddAttribute(wxT("Name"), name);
    return true;
}

bool Archive::WriteCData(const wxString& name, const wxString& str)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("CData"));
    m_root->AddChild(node);

    XmlUtils::SetCDATANodeContent(node, str);
    node->AddAttribute(wxT("Name"), name);
    return true;
}

bool Archive::ReadCData(const wxString& name, wxString& value)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("CData"), name);
    if(node) {
        // get the content node
        value = node->GetNodeContent();
        value.Trim().Trim(false);
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, wxString& value)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("wxString"), name);
    if(node) {
        value = node->GetAttribute(wxT("Value"), value);
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, size_t& value)
{
    long v = 0;
    bool res = Read(name, v);
    if(res) {
        value = v;
    }
    return res;
}

bool Archive::Read(const wxString& name, wxFileName& fileName)
{
    wxString value;
    bool res = Read(name, value);
    if(res) {
        fileName = wxFileName(value);
    }
    return res;
}

bool Archive::Write(const wxString& name, size_t value) { return Write(name, (long)value); }

bool Archive::Write(const wxString& name, const wxFileName& fileName) { return Write(name, fileName.GetFullPath()); }

void Archive::SetXmlNode(wxXmlNode* node) { m_root = node; }

bool Archive::WriteSimple(long value, const wxString& typeName, const wxString& name)
{
    if(!m_root)
        return false;

    wxString propValue;
    propValue << value;

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, typeName);
    m_root->AddChild(node);
    node->AddAttribute(wxT("Value"), propValue);
    node->AddAttribute(wxT("Name"), name);
    return true;
}

bool Archive::ReadSimple(long& value, const wxString& typeName, const wxString& name)
{
    if(!m_root)
        return false;

    value = 0;
    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, typeName, name);
    if(node) {
        wxString val = node->GetAttribute(wxT("Value"), wxEmptyString);
        val.ToLong(&value);
        return true;
    }
    return false;
}

#if wxUSE_GUI
bool Archive::Read(const wxString& name, wxColour& colour)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("wxColour"), name);
    wxString value;
    if(node) {
        value = node->GetAttribute(wxT("Value"), wxEmptyString);
    }

    if(value.IsEmpty()) {
        return false;
    }

    colour = wxColour(value);
    return true;
}
bool Archive::Write(const wxString& name, const wxColour& colour)
{
    if(!m_root) {
        return false;
    }
    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("wxColour"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Value"), colour.GetAsString());
    node->AddAttribute(wxT("Name"), name);
    return true;
}
#endif

bool Archive::Write(const wxString& name, const wxStringMap_t& stringMap)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("std_string_map"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each wxString in the array
    for (const auto& [key, value] : stringMap) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("MapEntry"));
        node->AddChild(child);
        child->AddAttribute(wxT("Key"), key);
        XmlUtils::SetNodeContent(child, value);
    }
    return true;
}

bool Archive::Read(const wxString& name, wxStringMap_t& stringMap)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("std_string_map"), name);
    if(node) {
        // fill the output array with the values
        stringMap.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("MapEntry")) {
                wxString value;
                wxString key;
                key = child->GetAttribute(wxT("Key"), wxEmptyString);
                value = child->GetNodeContent();
                stringMap[key] = value;
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool Archive::Read(const wxString& name, wxStringSet_t& s)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = XmlUtils::FindNodeByName(m_root, wxT("std_string_set"), name);
    if(node) {
        // fill the output array with the values
        s.clear();
        wxXmlNode* child = node->GetChildren();
        while(child) {
            if(child->GetName() == wxT("SetEntry")) {
                wxString value;
                value = child->GetNodeContent();
                s.insert(value);
            }
            child = child->GetNext();
        }
        return true;
    }
    return false;
}

bool Archive::Write(const wxString& name, const wxStringSet_t& s)
{
    if(!m_root) {
        return false;
    }

    wxXmlNode* node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("std_string_set"));
    m_root->AddChild(node);
    node->AddAttribute(wxT("Name"), name);

    // add an entry for each wxString in the array
    for (const auto text : s) {
        wxXmlNode* child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("SetEntry"));
        node->AddChild(child);
        XmlUtils::SetNodeContent(child, text);
    }
    return true;
}
#if wxUSE_GUI
bool Archive::Read(const wxString& name, wxFont& font, const wxFont& defaultFont)
{
    wxString strFont;
    bool res = Read(name, strFont);
    if(!res || strFont.IsEmpty()) {
        font = defaultFont;
        return false;
    }
    font = clFontHelper::FromString(strFont);
    return true;
}

bool Archive::Write(const wxString& name, const wxFont& font) { return Write(name, clFontHelper::ToString(font)); }
#endif
