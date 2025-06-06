//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2014 Eran Ifrah
// file name            : clcommandlineparser.h
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

#ifndef CLCOMMANDLINEPARSER_H
#define CLCOMMANDLINEPARSER_H

#include <wx/string.h>
#include <wx/arrstr.h>

class clCommandLineParser
{
    wxString m_commandline;
    wxArrayString m_tokens;

protected:
    void DoParse();

public:
    clCommandLineParser(const wxString& str);

    const wxArrayString& ToArray() const { return m_tokens; }
};

#endif // CLCOMMANDLINEPARSER_H
