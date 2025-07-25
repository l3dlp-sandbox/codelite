#include "LSPJdtlsDetector.hpp"

#include "Platform/Platform.hpp"
#include "globals.h"

#include <wx/filename.h>
#include <wx/regex.h>

LSPJdtlsDetector::LSPJdtlsDetector()
    : LSPDetector("java-jdtls")
{
}

LSPJdtlsDetector::~LSPJdtlsDetector() {}

bool LSPJdtlsDetector::DoLocate()
{
    const auto path = ThePlatform->Which("jdtls");
    if (!path) {
        return false;
    }

    LSP_DEBUG() << "Found jdtls ==>" << *path << endl;
    ConfigureFile(*path);
    return true;
}

void LSPJdtlsDetector::ConfigureFile(const wxFileName& jdtls)
{
    LSP_DEBUG() << "==> Found" << jdtls;
    wxString command;
    command << jdtls.GetFullPath();
    ::WrapWithQuotes(command);

    SetCommand(command);
    GetLanguages().Add("java");
    SetConnectionString("stdio");
}
