#include "clRemoteBuilder.hpp"

#include "AsyncProcess/processreaderthread.h"
#include "cl_command_event.h"
#include "codelite_events.h"
#include "event_notifier.h"
#include "file_logger.h"
#include "globals.h"
#include "imanager.h"

#include <wx/msgdlg.h>

#if USE_SFTP
#include "sftp_settings.h"
#endif

clRemoteBuilder::clRemoteBuilder()
{
    Bind(wxEVT_ASYNC_PROCESS_OUTPUT, &clRemoteBuilder::OnProcessOutput, this);
    Bind(wxEVT_ASYNC_PROCESS_TERMINATED, &clRemoteBuilder::OnProcessTerminated, this);
}

clRemoteBuilder::~clRemoteBuilder()
{
    Unbind(wxEVT_ASYNC_PROCESS_OUTPUT, &clRemoteBuilder::OnProcessOutput, this);
    Unbind(wxEVT_ASYNC_PROCESS_TERMINATED, &clRemoteBuilder::OnProcessTerminated, this);
}

bool clRemoteBuilder::IsRunning() const { return m_remoteProcess != nullptr; }

void clRemoteBuilder::Build(const wxString& sshAccount, const wxString& command, const wxString& workingDirectory)
{
#if USE_SFTP
    if(m_remoteProcess) {
        return;
    }
    SFTPSettings settings;
    settings.Load();
    SSHAccountInfo account;
    if(!settings.GetAccount(sshAccount, account)) {
        ::wxMessageBox(wxString() << _("Could not find SSH account: ") << sshAccount, "CodeLite",
                       wxICON_WARNING | wxCENTER);
        return;
    }

    wxFileName ssh;
    if (!FileUtils::FindExe("ssh", ssh)) {
        wxMessageBox(_("Could not locate ssh executable"), "CodeLite", wxICON_WARNING | wxOK);
        return;
    }

    // Prepare the command
    wxString cmd;
    cmd << ssh.GetFullPath() << " " << account.GetUsername() << "@" << account.GetHost() << " -p " << account.GetPort()
        << " 'cd " << workingDirectory << " && " << command << "'";
    clGetManager()->ClearOutputTab(kOutputTab_Build);
    clGetManager()->AppendOutputTabText(
        kOutputTab_Build, wxString() << "Remote build started using ssh account: " << account.GetAccountName() << "\n");
    clGetManager()->AppendOutputTabText(kOutputTab_Build, cmd + "\n");
    m_remoteProcess.reset(::CreateAsyncProcess(this, cmd));

    clBuildEvent eventStart(wxEVT_BUILD_STARTED);
    EventNotifier::Get()->AddPendingEvent(eventStart);

#else
    wxUnusedVar(sshAccount);
    wxUnusedVar(command);
    wxUnusedVar(workingDirectory);
#endif
}

void clRemoteBuilder::OnProcessOutput(clProcessEvent& event)
{
    clGetManager()->AppendOutputTabText(kOutputTab_Build, event.GetOutput());
}

void clRemoteBuilder::OnProcessTerminated(clProcessEvent& event)
{
    clGetManager()->AppendOutputTabText(kOutputTab_Build, "==== Done ====\n");
    m_remoteProcess.reset();

    clBuildEvent eventStopped(wxEVT_BUILD_ENDED);
    EventNotifier::Get()->AddPendingEvent(eventStopped);
}

void clRemoteBuilder::Stop() { m_remoteProcess->Terminate(); }
