#include "eoswiki.h"

#include "EOSProjectWizard.h"
#include "Zip/clZipReader.h"
#include "build_settings_config.h"
#include "cl_standard_paths.h"
#include "compiler.h"
#include "event_notifier.h"
#include "file_logger.h"
#include "fileutils.h"
#include "workspace.h"

#include <algorithm>
#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>

// Define the plugin entry point
CL_PLUGIN_API IPlugin* CreatePlugin(IManager* manager)
{
    return new EOSWiki(manager);
}

CL_PLUGIN_API PluginInfo* GetPluginInfo()
{
    static PluginInfo info;
    info.SetAuthor(wxT("eran"));
    info.SetName(wxT("EOSWiki"));
    info.SetDescription(_("CodeLite for EOS"));
    info.SetVersion(wxT("v1.0"));
    return &info;
}

CL_PLUGIN_API int GetPluginInterfaceVersion() { return PLUGIN_INTERFACE_VERSION; }

EOSWiki::EOSWiki(IManager* manager)
    : IPlugin(manager)
{
    m_longName = _("CodeLite for EOS");
    m_shortName = wxT("EOSWiki");

    // Extract eoswiki.zip file (the "resources")
    ExtractResources();
    wxTheApp->Bind(wxEVT_MENU, &EOSWiki::OnNewProject, this, XRCID("eosio_new_project"));
}

EOSWiki::~EOSWiki() {}

void EOSWiki::CreateToolBar(clToolBarGeneric* toolbar)
{
    // You can add items to the main toolbar here
    wxUnusedVar(toolbar);
}

void EOSWiki::CreatePluginMenu(wxMenu* pluginsMenu)
{
    wxMenu* menu = new wxMenu();
    menu->Append(new wxMenuItem(menu, XRCID("eosio_new_project"), _("New Project...")));
    pluginsMenu->Append(wxID_ANY, _("EOS Wiki"), menu);
}

void EOSWiki::UnPlug() { wxTheApp->Unbind(wxEVT_MENU, &EOSWiki::OnNewProject, this, XRCID("eosio_new_project")); }

void EOSWiki::OnNewProject(wxCommandEvent& event)
{
    wxUnusedVar(event);
    // Check if we have the toolchain installed
    if(EOSProjectWizard::GetToolchainPath().IsEmpty()) {
        ::wxMessageBox(_("Could not find EOS CDT installation. Please install it and try again"), "CodeLite",
                       wxICON_WARNING | wxOK | wxOK_DEFAULT);
        return;
    }
    EOSProjectWizard dlg(EventNotifier::Get()->TopFrame());
    if(dlg.ShowModal() == wxID_OK) { CreateProject(dlg.GetData()); }
}

enum eAction { kLoadWorkspace, kReloadWorkspace };

void EOSWiki::CreateProject(const EOSProjectData& data)
{
    eAction action = kReloadWorkspace;

    // Make sure no project with this name already exists
    if(clCxxWorkspaceST::Get()->IsOpen()) {
        ProjectPtr proj = clCxxWorkspaceST::Get()->GetProject(data.GetName());
        if(proj) {
            ::wxMessageBox(wxString() << _("A project with the same name: '") << data.GetName() << "'\nalready exists",
                           "CodeLite", wxICON_ERROR | wxOK | wxOK_DEFAULT);
            return;
        }
    }

    // Create the folder first
    wxFileName::Mkdir(data.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    // If no workspace is opened, create and open it
    wxString errMsg;
    if(!clCxxWorkspaceST::Get()->IsOpen()) {
        // Create a workspace first
        if(!clCxxWorkspaceST::Get()->CreateWorkspace(data.GetName(), data.GetPath(), errMsg)) {
            ::wxMessageBox(wxString() << _("Failed to create workspace:\n") << errMsg, "CodeLite",
                           wxICON_ERROR | wxOK | wxOK_DEFAULT);
            return;
        }
        action = kLoadWorkspace;
    }

    // Create new project
    if(!clCxxWorkspaceST::Get()->CreateProject(data.GetName(), data.GetPath(), PROJECT_TYPE_EXECUTABLE, "", true,
                                               errMsg)) {
        ::wxMessageBox(wxString() << _("Failed to create project:\n") << errMsg, "CodeLite",
                       wxICON_ERROR | wxOK | wxOK_DEFAULT);
        return;
    }

    // Get the project
    ProjectPtr proj = clCxxWorkspaceST::Get()->GetProject(data.GetName());
    ProjectSettingsPtr settings = proj->GetSettings();

    // Create template file
    CreateSampleFile(proj, data);

    // Create the CMakeLists.txt file
    CreateCMakeListsFile(proj, data);

    ProjectSettingsCookie cookie;
    BuildConfigPtr bldConf = settings->GetFirstBuildConfiguration(cookie);
    while(bldConf) {
        bldConf->SetBuildSystem("CMake");
        bldConf->SetCompilerType("eosio");

        // update code completion search paths
        wxFileName ccpath(data.GetToolchainPath(), "");
        ccpath.AppendDir("include");
        bldConf->SetCcSearchPaths(ccpath.GetPath());

        // Update the next build configuration
        bldConf = settings->GetNextBuildConfiguration(cookie);
    }
    proj->SetSettings(settings);

    if(action == kLoadWorkspace) {
        wxCommandEvent loadEvent(wxEVT_MENU, XRCID("switch_to_workspace"));
        loadEvent.SetEventObject(EventNotifier::Get()->TopFrame());
        loadEvent.SetString(clCxxWorkspaceST::Get()->GetWorkspaceFileName().GetFullPath());
        EventNotifier::Get()->TopFrame()->GetEventHandler()->AddPendingEvent(loadEvent);
    } else {
        // reload
        wxCommandEvent reloadEvent(wxEVT_MENU, XRCID("reload_workspace"));
        reloadEvent.SetEventObject(EventNotifier::Get()->TopFrame());
        EventNotifier::Get()->TopFrame()->GetEventHandler()->AddPendingEvent(reloadEvent);
    }
}

void EOSWiki::CreateSampleFile(ProjectPtr proj, const EOSProjectData& d)
{
    // Load the main file sample file
    wxString fileContent = ReadResource("SmartContractSample.txt");
    ReplacePlaceHolders(fileContent, { { "%CLASS_NAME%", d.GetName() } });

    wxFileName sourceFile(d.GetPath(), d.GetName());
    sourceFile.SetExt("cpp");
    sourceFile.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    if(FileUtils::WriteFileContent(sourceFile, fileContent)) {
        proj->AddFile(sourceFile.GetFullPath(), "src");
    } else {
        clWARNING() << "Failed to write file content:" << sourceFile;
        return;
    }
}

void EOSWiki::ExtractResources()
{
    wxFileName fnZip(clStandardPaths::Get().GetDataDir(), "eoswiki.zip");
    if(!fnZip.FileExists()) {
        clWARNING() << "EOS Wiki resource file can't be found:" << fnZip;
        return;
    }

    clZipReader zip(fnZip);
    wxFileName tmpFolder(clStandardPaths::Get().GetTempDir(), "");
    tmpFolder.AppendDir("eoswiki");

    // Clear any previous instalation
    if(tmpFolder.DirExists()) { tmpFolder.Rmdir(wxPATH_RMDIR_RECURSIVE); }

    // Create the tmp folder
    tmpFolder.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    // Extract the resources
    zip.Extract("*", tmpFolder.GetPath());
}

wxString EOSWiki::ReadResource(const wxString& filename) const
{
    wxFileName resourceFile(clStandardPaths::Get().GetTempDir(), "");
    resourceFile.AppendDir("eoswiki");
    resourceFile.SetFullName(filename);
    if(!resourceFile.FileExists()) { return ""; }

    // Load the file
    wxString content;
    if(!FileUtils::ReadFileContent(resourceFile, content)) { return ""; }
    return content;
}

void EOSWiki::ReplacePlaceHolders(wxString& text, const wxStringMap_t& m)
{
    for (const auto& [placeholder, value] : m) {
        text.Replace(placeholder, value);
    }
}

void EOSWiki::CreateCMakeListsFile(ProjectPtr proj, const EOSProjectData& d)
{
    // Load the main file sample file
    wxString fileContent = ReadResource("CMakeLists.txt");
    wxFileName cmakeModulePath(d.GetToolchainPath(), "");
    cmakeModulePath.AppendDir("lib");
    cmakeModulePath.AppendDir("cmake");
    cmakeModulePath.AppendDir("eosio.cdt");

    ReplacePlaceHolders(fileContent,
                        { { "%CMAKE_MODULE_PATH%", cmakeModulePath.GetPath() }, { "%CONTRACT_NAME%", d.GetName() } });

    wxFileName cmakeListTxt(d.GetPath(), "CMakeLists.txt");
    cmakeListTxt.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    if(FileUtils::WriteFileContent(cmakeListTxt, fileContent)) {
        proj->CreateVirtualDir("resources");
        proj->AddFile(cmakeListTxt.GetFullPath(), "resources");
    } else {
        clWARNING() << "Failed to write file content:" << cmakeListTxt;
    }
}
