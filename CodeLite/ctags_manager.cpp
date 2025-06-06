//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : ctags_manager.cpp
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
#include "ctags_manager.h"

#include "CTags.hpp"
#include "Cxx/CxxTemplateFunction.h"
#include "Cxx/CxxVariableScanner.h"
#include "Cxx/cpp_comment_creator.h"
#include "StdToWX.h"
#include "cl_command_event.h"
#include "codelite_events.h"
#include "database/tags_storage_sqlite3.h"
#include "event_notifier.h"
#include "fileextmanager.h"
#include "precompiled_header.h"

#include <algorithm>
#include <set>
#include <sstream>
#include <wx/app.h>
#include <wx/busyinfo.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>
#if wxUSE_GUI
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/xrc/xmlres.h>
#endif
#include <wx/log.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

//#define __PERFORMANCE
#include "performance.h"

//---------------------------------------------------------------------------
// Misc

/// Ascending sorting function
struct SAscendingSort {
    bool operator()(const TagEntryPtr& rStart, const TagEntryPtr& rEnd)
    {
        return rEnd->GetName().CmpNoCase(rStart->GetName()) > 0;
    }
};

//////////////////////////////////////
// Adapter class to TagsManager
//////////////////////////////////////
static TagsManager* gs_TagsManager = NULL;

void TagsManagerST::Free()
{
    if(gs_TagsManager) {
        delete gs_TagsManager;
    }
    gs_TagsManager = NULL;
}

TagsManager* TagsManagerST::Get()
{
    if(gs_TagsManager == NULL)
        gs_TagsManager = new TagsManager();

    return gs_TagsManager;
}

//------------------------------------------------------------------------------
// CTAGS Manager
//------------------------------------------------------------------------------

TagsManager::TagsManager()
    : wxEvtHandler()
    , m_lang(NULL)
{
    m_db = std::make_shared<TagsStorageSQLite>();
    m_db->SetSingleSearchLimit(MAX_SEARCH_LIMIT);
}

TagsManager::~TagsManager() {}

void TagsManager::OpenDatabase(const wxFileName& fileName)
{
    m_dbFile = fileName;
    m_db->OpenDatabase(m_dbFile);
    m_db->SetEnableCaseInsensitive(true);
    m_db->SetSingleSearchLimit(m_tagsOptions.GetCcNumberOfDisplayItems());
}

//-----------------------------------------------------------
// Database operations
//-----------------------------------------------------------

TagTreePtr TagsManager::Load(const wxFileName& fileName, TagEntryPtrVector_t* tags)
{
    TagTreePtr tree;
    TagEntryPtrVector_t tagsByFile;

    if(tags) {
        tagsByFile.insert(tagsByFile.end(), tags->begin(), tags->end());

    } else {
        GetDatabase()->SelectTagsByFile(fileName.GetFullPath(), tagsByFile);
    }

    // Load the records and build a language tree
    TagEntry root;
    root.SetName(wxT("<ROOT>"));
    tree = std::make_shared<TagTree>(wxT("<ROOT>"), root);
    for(size_t i = 0; i < tagsByFile.size(); i++) {
        tree->AddEntry(*(tagsByFile.at(i)));
    }
    return tree;
}

void TagsManager::Delete(const wxFileName& path, const wxString& fileName)
{
    GetDatabase()->DeleteByFileName(path, fileName);
}

void TagsManager::FindSymbol(const wxString& name, std::vector<TagEntryPtr>& tags)
{
    // since we dont get a scope, we better user a search that only uses the
    // name (GetTagsByScopeAndName) is optimized to search the global tags table
    GetDatabase()->GetTagsByName(name, tags, true);
}

void TagsManager::FindByNameAndScope(const wxString& name, const wxString& scope, std::vector<TagEntryPtr>& tags)
{
    wxString _name = DoReplaceMacros(name);
    wxString _scope = DoReplaceMacros(scope);
    DoFindByNameAndScope(_name, _scope, tags);

    // Sort the results base on their name
    std::sort(tags.begin(), tags.end(), SAscendingSort());
}

void TagsManager::DoFindByNameAndScope(const wxString& name, const wxString& scope, std::vector<TagEntryPtr>& tags)
{
    wxString sql;
    if(scope == wxT("<global>")) {
        // try the workspace database for match
        GetDatabase()->GetTagsByNameAndParent(name, wxT("<global>"), tags);
    } else {
        std::vector<std::pair<wxString, int>> derivationList;
        derivationList.push_back({ scope, 0 });
        std::unordered_set<wxString> visited;
        GetDerivationList(scope, NULL, derivationList, visited, 1);
        wxArrayString paths;
        for(size_t i = 0; i < derivationList.size(); i++) {
            wxString path_;
            path_ << derivationList.at(i).first << wxT("::") << name;
            paths.Add(path_);
        }

        // try the workspace database for match
        GetDatabase()->GetTagsByPath(paths, tags);
    }
}

bool TagsManager::IsTypeAndScopeExists(wxString& typeName, wxString& scope)
{
    wxString cacheKey;
    cacheKey << typeName << wxT("@") << scope;

    // we search the cache first, note that the cache
    // is used only for the external database
    std::map<wxString, bool>::iterator iter = m_typeScopeCache.find(cacheKey);
    if(iter != m_typeScopeCache.end()) {
        return iter->second;
    }

    // First try the fast query to save some time
    if(GetDatabase()->IsTypeAndScopeExistLimitOne(typeName, scope)) {
        return true;
    }

    // replace macros:
    // replace the provided typeName and scope with user defined macros as appeared in the PreprocessorMap
    typeName = DoReplaceMacros(typeName);
    scope = DoReplaceMacros(scope);

    return GetDatabase()->IsTypeAndScopeExist(typeName, scope);
}

bool TagsManager::GetDerivationList(const wxString& path, TagEntryPtr derivedClassTag,
                                    std::vector<std::pair<wxString, int>>& derivationList,
                                    std::unordered_set<wxString>& visited, int depth)
{
    bool res = GetDerivationListInternal(path, derivedClassTag, derivationList, visited, depth);
    std::sort(
        derivationList.begin(), derivationList.end(),
        [](const std::pair<wxString, size_t>& a, const std::pair<wxString, size_t>& b) { return a.second < b.second; });
    return res;
}

bool TagsManager::GetDerivationListInternal(const wxString& path, TagEntryPtr derivedClassTag,
                                            std::vector<std::pair<wxString, int>>& derivationList,
                                            std::unordered_set<wxString>& visited, int depth)
{
    std::vector<TagEntryPtr> tags;
    TagEntryPtr tag;
    const wxArrayString kind = StdToWX::ToArrayString({ wxT("class"), wxT("struct") });

    GetDatabase()->GetTagsByKindAndPath(kind, path, tags);

    if(tags.size() == 1) {
        tag = tags.at(0);
    } else {
        return false;
    }

    if(tag && tag->IsOk()) {

        // Get the template instantiation list from the child class
        wxArrayString inheritsList = tag->GetInheritsAsArrayNoTemplates();

        wxString templateInstantiationLine;
        if(derivedClassTag) {
            wxArrayString p_inheritsListT = derivedClassTag->GetInheritsAsArrayWithTemplates();
            wxArrayString p_inheritsList = derivedClassTag->GetInheritsAsArrayNoTemplates();

            for(size_t i = 0; i < p_inheritsList.GetCount(); i++) {
                if(p_inheritsList.Item(i) == tag->GetName()) {
                    templateInstantiationLine = p_inheritsListT.Item(i);
                    templateInstantiationLine = templateInstantiationLine.AfterFirst(wxT('<'));
                    templateInstantiationLine.Prepend(wxT("<"));
                    break;
                }
            }
        }

        for(size_t i = 0; i < inheritsList.GetCount(); i++) {
            wxString inherits = inheritsList.Item(i);
            wxString tagName = tag->GetName();
            wxString tmpInhr = inherits;

            bool isTemplate = (tag->GetPattern().Find(wxT("template")) != wxNOT_FOUND);
            tagName.MakeLower();
            tmpInhr.MakeLower();

            // Make sure that inherits != the current name or we will end up in an infinite loop
            if(tmpInhr != tagName) {
                wxString possibleScope(wxT("<global>"));

                // if the 'inherits' already contains a scope
                // dont attempt to fix it
                if(inherits.Contains(wxT("::")) == false) {

                    // Correct the type/scope
                    bool testForTemplate = !IsTypeAndScopeExists(inherits, possibleScope);

                    // If the type does not exists, check for templates
                    if(testForTemplate && derivedClassTag && isTemplate) {
                        TemplateHelper th;

                        // e.g. template<typename T> class MyClass
                        wxArrayString templateArgs = GetLanguage()->DoExtractTemplateDeclarationArgs(tag);
                        th.SetTemplateDeclaration(templateArgs);                // <typename T>
                        th.SetTemplateInstantiation(templateInstantiationLine); // e.g. MyClass<wxString>

                        wxString newType = th.Substitute(inherits);

                        // Locate the new type by name in the database
                        // this is done to make sure that the new type is not a macro...
                        if(!newType.IsEmpty() && newType != inherits) {

                            // check the user defined types for a replacement token
                            wxString replacement = DoReplaceMacros(newType);
                            if(replacement == newType) {
                                // No match was found in the user defined replacements
                                // try the database
                                replacement = DoReplaceMacrosFromDatabase(newType);
                            }
                            inherits = replacement;
                        }
                    }

                    if(possibleScope != wxT("<global>")) {
                        inherits = possibleScope + wxT("::") + inherits;
                    }
                }

                // Make sure that this parent was not scanned already
                if(visited.count(inherits) == 0) {
                    visited.insert(inherits);
                    derivationList.push_back({ inherits, depth });
                    GetDerivationList(inherits, tag, derivationList, visited, depth + 1);
                }
            }
        }
    }
    return true;
}

void TagsManager::CloseDatabase()
{
    m_dbFile.Clear();
    m_db = nullptr; // Free the current database
    m_db = std::make_shared<TagsStorageSQLite>();
    m_db->SetSingleSearchLimit(m_tagsOptions.GetCcNumberOfDisplayItems());
    m_db->SetUseCache(false);
}

DoxygenComment TagsManager::DoCreateDoxygenComment(TagEntryPtr tag, wxChar keyPrefix)
{
    CppCommentCreator commentCreator(tag, keyPrefix);
    DoxygenComment dc;
    dc.comment = commentCreator.CreateComment();
    dc.name = tag->GetName();
    return dc;
}

void TagsManager::SetCtagsOptions(const TagsOptionsData& options)
{
    m_tagsOptions = options;
}

wxString TagsManager::GetScopeName(const wxString& scope)
{
    Language* lang = GetLanguage();
    return lang->GetScopeName(scope);
}

void TagsManager::GetFiles(const wxString& partialName, std::vector<FileEntryPtr>& files)
{
    if(GetDatabase()) {
        GetDatabase()->GetFiles(partialName, files);
    }
}

void TagsManager::GetFiles(const wxString& partialName, std::vector<wxFileName>& files)
{
    std::vector<FileEntryPtr> f;
    GetFiles(partialName, f);

    for(size_t i = 0; i < f.size(); i++) {
        files.push_back(wxFileName(f.at(i)->GetFile()));
    }
}

TagEntryPtr TagsManager::FunctionFromFileLine(const wxFileName& fileName, int lineno)
{
    if(!GetDatabase()) {
        return NULL;
    }

    if(!IsFileCached(fileName.GetFullPath())) {
        CacheFile(fileName.GetFullPath());
    }

    for (TagEntryPtr t : m_cachedFileFunctionsTags) {
        if (t->GetLine() <= lineno) {
            return t;
        }
    }
    return nullptr;
}

void TagsManager::SetLanguage(Language* lang) { m_lang = lang; }

Language* TagsManager::GetLanguage()
{
    if(!m_lang) {
        // for backward compatibility allows access to the tags manager using
        // the singleton call
        return LanguageST::Get();
    } else {
        return m_lang;
    }
}

void TagsManager::GetClasses(std::vector<TagEntryPtr>& tags)
{
    const wxArrayString kind = StdToWX::ToArrayString({ wxT("class"), wxT("struct"), wxT("union") });

    GetDatabase()->GetTagsByKind(kind, wxT("name"), ITagsStorage::OrderAsc, tags);
}

void TagsManager::TagsByScope(const wxString& scopeName, const wxArrayString& kind, std::vector<TagEntryPtr>& tags)
{
    wxArrayString scopes;
    GetScopesByScopeName(scopeName, scopes);
    // make enough room for max of 500 elements in the vector
    tags.reserve(500);
    GetDatabase()->GetTagsByScopesAndKind(scopes, kind, tags);

    // and finally sort the results
    std::sort(tags.begin(), tags.end(), SAscendingSort());
}

wxString TagsManager::NormalizeFunctionSig(const wxString& sig, size_t flags,
                                           std::vector<std::pair<int, int>>* paramLen)
{
    // FIXME: make the standard configurable
    CxxVariableScanner varScanner(sig, eCxxStandard::kCxx11, {}, true);
    CxxVariable::Vec_t vars = varScanner.ParseFunctionArguments();

    // construct a function signature from the results
    wxString str_output;
    str_output << wxT("(");

    if(paramLen) {
        paramLen->clear();
    }
    if(flags & Normalize_Func_Arg_Per_Line && !vars.empty()) {
        str_output << wxT("\n    ");
    }

    for(const auto& var : vars) {
        int start_offset = str_output.length();

        // FIXME: the standard should be configurable
        size_t toStringFlags = CxxVariable::kToString_None;
        if(flags & Normalize_Func_Name) {
            toStringFlags |= CxxVariable::kToString_Name;
        }
        if(flags & Normalize_Func_Default_value) {
            toStringFlags |= CxxVariable::kToString_DefaultValue;
        }

        str_output << var->ToString(toStringFlags);
        // keep the length of this argument
        if(paramLen) {
            paramLen->push_back({ start_offset, str_output.length() - start_offset });
        }

        str_output << ", ";
        if((flags & Normalize_Func_Arg_Per_Line) && !vars.empty()) {
            str_output << wxT("\n    ");
        }
    }

    if(vars.empty() == false) {
        str_output = str_output.BeforeLast(',');
    }

    str_output << ")";
    return str_output;
}

void TagsManager::CacheFile(const wxString& fileName)
{
    if(!GetDatabase()) {
        return;
    }

    m_cachedFile = fileName;
    m_cachedFileFunctionsTags.clear();

    const wxArrayString kinds = StdToWX::ToArrayString({ wxT("function"), wxT("prototype") });
    // disable the cache
    GetDatabase()->SetUseCache(false);
    GetDatabase()->GetTagsByKindAndFile(kinds, fileName, wxT("line"), ITagsStorage::OrderDesc,
                                        m_cachedFileFunctionsTags);
    // re-enable it
    GetDatabase()->SetUseCache(true);
}

void TagsManager::ClearCachedFile(const wxString& fileName)
{
    if(fileName == m_cachedFile) {
        m_cachedFile.Clear();
        m_cachedFileFunctionsTags.clear();
    }
}

bool TagsManager::IsFileCached(const wxString& fileName) const { return fileName == m_cachedFile; }

wxString TagsManager::DoReplaceMacros(const wxString& name)
{
    // replace macros:
    // replace the provided typeName and scope with user defined macros as appeared in the PreprocessorMap
    wxString _name(name);

    const wxStringTable_t& iTokens = GetCtagsOptions().GetTokensWxMap();
    wxStringTable_t::const_iterator it;

    it = iTokens.find(_name);
    if(it != iTokens.end()) {
        if(it->second.empty() == false) {
            _name = it->second;
        }
    }
    return _name;
}

void TagsManager::FilterNonNeededFilesForRetaging(wxArrayString& strFiles, ITagsStoragePtr db)
{
    std::vector<FileEntryPtr> files_entries;
    db->GetFiles(files_entries);
    std::unordered_set<wxString> files_set;

    for(size_t i = 0; i < strFiles.GetCount(); i++) {
        files_set.insert(strFiles.Item(i));
    }

    for(size_t i = 0; i < files_entries.size(); i++) {
        const FileEntryPtr& fe = files_entries.at(i);

        // does the file exist in both lists?
        std::unordered_set<wxString>::iterator iter = files_set.find(fe->GetFile());
        if(iter != files_set.end()) {
            // get the actual modification time of the file from the disk
            struct stat buff;
            int modified(0);

            const wxCharBuffer cname = _C((*iter));
            if(stat(cname.data(), &buff) == 0) {
                modified = (int)buff.st_mtime;
            }

            // if the timestamp from the database < then the actual timestamp, re-tag the file
            if(fe->GetLastRetaggedTimestamp() >= modified) {
                files_set.erase(iter);
            }
        }
    }

    // copy back the files to the array
    strFiles.Clear();
    strFiles.Alloc(files_set.size());
    for (const auto& s : files_set) {
        strFiles.Add(s);
    }
}

void TagsManager::GetDereferenceOperator(const wxString& scope, std::vector<TagEntryPtr>& tags)
{
    std::vector<std::pair<wxString, int>> derivationList;

    // add this scope as well to the derivation list
    wxString _scopeName = DoReplaceMacros(scope);
    derivationList.push_back({ _scopeName, 0 });
    std::unordered_set<wxString> visited;
    GetDerivationList(_scopeName, NULL, derivationList, visited, 1);

    // make enough room for max of 500 elements in the vector
    for(size_t i = 0; i < derivationList.size(); i++) {
        wxString tmpScope(derivationList.at(i).first);
        tmpScope = DoReplaceMacros(tmpScope);

        GetDatabase()->GetDereferenceOperator(tmpScope, tags);
        if(!tags.empty()) {
            // No need to further check
            break;
        }
    }
}

void TagsManager::GetSubscriptOperator(const wxString& scope, std::vector<TagEntryPtr>& tags)
{
    std::vector<std::pair<wxString, int>> derivationList;

    // add this scope as well to the derivation list
    wxString _scopeName = DoReplaceMacros(scope);
    derivationList.push_back({ _scopeName, 0 });
    std::unordered_set<wxString> visited;
    GetDerivationList(_scopeName, NULL, derivationList, visited, 1);

    // make enough room for max of 500 elements in the vector
    for(size_t i = 0; i < derivationList.size(); i++) {
        wxString tmpScope(derivationList.at(i).first);
        tmpScope = DoReplaceMacros(tmpScope);

        GetDatabase()->GetSubscriptOperator(scope, tags);
        if(!tags.empty()) {

            // No need to further check
            break;
        }
    }
}

bool TagsManager::IsBinaryFile(const wxString& filepath, const TagsOptionsData& tod)
{
    // If the file is a C++ file, avoid testing the content return false based on the extension
    FileExtManager::FileType type = FileExtManager::GetType(filepath);
    if(type == FileExtManager::TypeHeader || type == FileExtManager::TypeSourceC ||
       type == FileExtManager::TypeSourceCpp)
        return false;

    // If this file matches any of the c++ patterns defined in the configuration
    // don't consider it as a binary file
    if(FileUtils::WildMatch(tod.GetFileSpec(), filepath)) {
        return false;
    }

    // examine the file based on the content of the first 4K (max) bytes
    FILE* fp = fopen(filepath.To8BitData(), "rb");
    if(fp) {

        char buffer[1];
        int textLen(0);
        const int maxTextToExamine(4096);

        // examine up to maxTextToExamine first chars in the file and search for '\0'
        while(fread(buffer, sizeof(char), sizeof(buffer), fp) == 1 && textLen < maxTextToExamine) {
            textLen++;
            // if we found a NULL, return true
            if(buffer[0] == 0) {
                fclose(fp);
                return true;
            }
        }

        fclose(fp);
        return false;
    }

    // if we could not open it, return true
    return true;
}

ITagsStoragePtr TagsManager::GetDatabase() { return m_db; }

wxString TagsManager::DoReplaceMacrosFromDatabase(const wxString& name)
{
    std::set<wxString> scannedMacros;
    wxString newName = name;
    while(true) {
        TagEntryPtr matchedTag = GetDatabase()->GetTagsByNameLimitOne(newName);
        if(matchedTag && matchedTag->IsMacro() && scannedMacros.find(matchedTag->GetName()) == scannedMacros.end()) {
            TagEntryPtr realTag = matchedTag->ReplaceSimpleMacro();
            if(realTag) {

                newName = realTag->GetName();
                scannedMacros.insert(newName);
                continue;

            } else {
                break;
            }
        } else {
            break;
        }
    }
    return newName;
}

bool TagsManager::InsertFunctionDecl(const wxString& clsname, const wxString& functionDecl, wxString& sourceContent,
                                     int visibility)
{
    return GetLanguage()->InsertFunctionDecl(clsname, functionDecl, sourceContent, visibility);
}

void TagsManager::GetScopesByScopeName(const wxString& scopeName, wxArrayString& scopes)
{
    std::vector<std::pair<wxString, int>> derivationList;

    // add this scope as well to the derivation list
    wxString _scopeName = DoReplaceMacros(scopeName);
    derivationList.push_back({ _scopeName, 0 });
    std::unordered_set<wxString> visited;
    GetDerivationList(_scopeName, NULL, derivationList, visited, 1);

    for(size_t i = 0; i < derivationList.size(); i++) {
        wxString tmpScope(derivationList.at(i).first);
        tmpScope = DoReplaceMacros(tmpScope);
        scopes.Add(tmpScope);
    }
}

void TagsManager::InsertForwardDeclaration(const wxString& classname, const wxString& fileContent, wxString& lineToAdd,
                                           int& line, const wxString& impExpMacro)
{
    lineToAdd << "class ";
    if(!impExpMacro.IsEmpty()) {
        lineToAdd << impExpMacro << " ";
    }
    lineToAdd << classname << ";";
    line = GetLanguage()->GetBestLineForForwardDecl(fileContent);
}

void TagsManager::GetFilesForCC(const wxString& userTyped, wxArrayString& matches)
{
    GetDatabase()->GetFilesForCC(userTyped, matches);
}

void TagsManager::GetCXXKeywords(wxStringSet_t& words)
{
    wxArrayString arr;
    GetCXXKeywords(arr);
    words.clear();
    words.insert(arr.begin(), arr.end());
}

void TagsManager::GetCXXKeywords(wxArrayString& words)
{
    words = StdToWX::ToArrayString({ "alignas",      "alignof",
                                     "and",          "and_eq",
                                     "asm",          "auto",
                                     "bitand",       "bitor",
                                     "bool",         "break",
                                     "case",         "catch",
                                     "char",         "char16_t",
                                     "char32_t",     "class",
                                     "compl",        "const",
                                     "constexpr",    "const_cast",
                                     "continue",     "decltype",
                                     "default",      "delete",
                                     "do",           "double",
                                     "dynamic_cast", "else",
                                     "enum",         "explicit",
                                     "export",       "extern",
                                     "false",        "final",
                                     "float",        "for",
                                     "friend",       "goto",
                                     "if",           "inline",
                                     "int",          "long",
                                     "mutable",      "namespace",
                                     "new",          "noexcept",
                                     "not",          "not_eq",
                                     "nullptr",      "operator",
                                     "or",           "or_eq",
                                     "override",     "private",
                                     "protected",    "public",
                                     "register",     "reinterpret_cast",
                                     "return",       "short",
                                     "signed",       "sizeof",
                                     "static",       "static_assert",
                                     "static_cast",  "struct",
                                     "switch",       "template",
                                     "this",         "thread_local",
                                     "throw",        "true",
                                     "try",          "typedef",
                                     "typeid",       "typename",
                                     "union",        "unsigned",
                                     "using",        "virtual",
                                     "void",         "volatile",
                                     "wchar_t",      "while",
                                     "xor",          "xor_eq" });
}

TagEntryPtrVector_t TagsManager::ParseBuffer(const wxString& content, const wxString& filename, const wxString& kinds)
{
    TagEntryPtrVector_t tagsVec;
    CTags::ParseBuffer(filename, content, clStandardPaths::Get().GetBinaryFullPath("codelite-ctags"),
                       GetCtagsOptions().GetTokensWxMap(), tagsVec);
    return tagsVec;
}

void TagsManager::GetTagsByPartialNames(const wxArrayString& partialNames, std::vector<TagEntryPtr>& tags)
{
    GetDatabase()->GetTagsByPartName(partialNames, tags);
}

void TagsManager::ParseWorkspaceIncremental()
{
    // restart ctagsd (this way we ensure that new settings are loaded)
    clLanguageServerEvent stop_event{ wxEVT_LSP_RESTART };
    stop_event.SetLspName("ctagsd");
    EventNotifier::Get()->AddPendingEvent(stop_event);
}

void TagsManager::ParseWorkspaceFull(const wxString& workspace_dir)
{
    // stop ctagsd
    clLanguageServerEvent stop_event{ wxEVT_LSP_STOP };
    stop_event.SetLspName("ctagsd");
    EventNotifier::Get()->ProcessEvent(stop_event);

    // delete the tags.db file
    wxFileName tags_db{ workspace_dir, "tags.db" };
    tags_db.AppendDir(".ctagsd");

    if(tags_db.FileExists()) {
        FileUtils::RemoveFile(tags_db, wxEmptyString);
    }

    // start ctagsd again
    clLanguageServerEvent start_event{ wxEVT_LSP_START };
    start_event.SetLspName("ctagsd");
    EventNotifier::Get()->ProcessEvent(start_event);
}
