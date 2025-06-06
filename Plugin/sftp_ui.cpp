//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: sftp_ui.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "sftp_ui.h"

#include "codelite_exports.h"

// Declare the bitmap loading function
extern void wxCE8CInitBitmapResources();

namespace
{
// return the wxBORDER_SIMPLE that matches the current application theme
wxBorder get_border_simple_theme_aware_bit()
{
#if wxVERSION_NUMBER >= 3300 && defined(__WXMSW__)
    return wxSystemSettings::GetAppearance().IsDark() ? wxBORDER_SIMPLE : wxBORDER_DEFAULT;
#else
    return wxBORDER_DEFAULT;
#endif
} // get_border_simple_theme_aware_bit
bool bBitmapLoaded = false;
} // namespace

SSHAccountManagerDlgBase::SSHAccountManagerDlgBase(
    wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if (!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer2);

    wxBoxSizer* boxSizer10 = new wxBoxSizer(wxHORIZONTAL);

    boxSizer2->Add(boxSizer10, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrl = new clThemedListCtrl(
        this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(200, -1)), wxDV_ROW_LINES | wxDV_SINGLE);

    boxSizer10->Add(m_dvListCtrl, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_dvListCtrl->AppendTextColumn(
        _("Account"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrl->AppendTextColumn(
        _("Host"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    m_dvListCtrl->AppendTextColumn(
        _("User"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    wxBoxSizer* boxSizer15 = new wxBoxSizer(wxVERTICAL);

    boxSizer10->Add(boxSizer15, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_buttonNew = new wxButton(this, wxID_ADD, _("&Add"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_buttonNew->SetToolTip(_("Add new account"));

    boxSizer15->Add(m_buttonNew, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_buttonDelete =
        new wxButton(this, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_buttonDelete->SetToolTip(_("Delete the selected accounts"));

    boxSizer15->Add(m_buttonDelete, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_buttonEdit = new wxButton(this, wxID_EDIT, _("&Edit"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_buttonEdit->SetToolTip(_("Edit the selected account"));

    boxSizer15->Add(m_buttonEdit, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer4 = new wxBoxSizer(wxHORIZONTAL);

    boxSizer2->Add(boxSizer4, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button6 = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button6->SetDefault();

    boxSizer4->Add(m_button6, 0, wxALL, WXC_FROM_DIP(5));

    m_button8 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    boxSizer4->Add(m_button8, 0, wxALL, WXC_FROM_DIP(5));

    SetName(wxT("SSHAccountManagerDlgBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(this);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
    // Connect events
    m_dvListCtrl->Bind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &SSHAccountManagerDlgBase::OnItemActivated, this);
    m_buttonNew->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnAddAccount, this);
    m_buttonDelete->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnDeleteAccount, this);
    m_buttonDelete->Bind(wxEVT_UPDATE_UI, &SSHAccountManagerDlgBase::OnDeleteAccountUI, this);
    m_buttonEdit->Bind(wxEVT_UPDATE_UI, &SSHAccountManagerDlgBase::OnEditAccountUI, this);
    m_buttonEdit->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnEditAccount, this);
    m_button6->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnOK, this);
}

SSHAccountManagerDlgBase::~SSHAccountManagerDlgBase()
{
    m_dvListCtrl->Unbind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &SSHAccountManagerDlgBase::OnItemActivated, this);
    m_buttonNew->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnAddAccount, this);
    m_buttonDelete->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnDeleteAccount, this);
    m_buttonDelete->Unbind(wxEVT_UPDATE_UI, &SSHAccountManagerDlgBase::OnDeleteAccountUI, this);
    m_buttonEdit->Unbind(wxEVT_UPDATE_UI, &SSHAccountManagerDlgBase::OnEditAccountUI, this);
    m_buttonEdit->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnEditAccount, this);
    m_button6->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &SSHAccountManagerDlgBase::OnOK, this);
}

AddSSHAcountDlgBase::AddSSHAcountDlgBase(
    wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if (!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer23 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer23);

    m_infobar = new wxInfoBar(this, wxID_ANY);
    m_infobar->SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));

    boxSizer23->Add(m_infobar, 0, wxEXPAND, WXC_FROM_DIP(5));

    wxFlexGridSizer* flexGridSizer31 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer31->SetFlexibleDirection(wxBOTH);
    flexGridSizer31->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer31->AddGrowableCol(1);

    boxSizer23->Add(flexGridSizer31, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText98 =
        new wxStaticText(this, wxID_ANY, _("Account Name:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_staticText98->SetToolTip(_("Give this account a unique name"));

    flexGridSizer31->Add(m_staticText98, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(250, -1)), 0);
    m_textCtrlName->SetToolTip(_("Give this account a unique name"));
    m_textCtrlName->SetFocus();
#if wxVERSION_NUMBER >= 3000
    m_textCtrlName->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlName, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticHostIp =
        new wxStaticText(this, wxID_ANY, _("Host / IP:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticHostIp, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlHost = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_textCtrlHost->SetToolTip(_("The remote host ip address or its known name"));
#if wxVERSION_NUMBER >= 3000
    m_textCtrlHost->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlHost, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText45 =
        new wxStaticText(this, wxID_ANY, _("Port:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticText45, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlPort = new wxTextCtrl(this, wxID_ANY, wxT("22"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_textCtrlPort->SetToolTip(_("The SSH port. If you don't know it, leave it as 22 (SSH default port)"));
#if wxVERSION_NUMBER >= 3000
    m_textCtrlPort->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlPort, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText37 =
        new wxStaticText(this, wxID_ANY, _("Username:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticText37, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlUsername =
        new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_textCtrlUsername->SetToolTip(_("Login user name"));
#if wxVERSION_NUMBER >= 3000
    m_textCtrlUsername->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlUsername, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText41 =
        new wxStaticText(this, wxID_ANY, _("Password:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticText41, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlPassword =
        new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTE_PASSWORD);
    m_textCtrlPassword->SetToolTip(_("This field is optional. By leaving this field empty, codelite will attempt to "
                                     "connect only using public key authentication"));
#if wxVERSION_NUMBER >= 3000
    m_textCtrlPassword->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlPassword, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText110 =
        new wxStaticText(this, wxID_ANY, _("Default folder:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticText110, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlHomeFolder =
        new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_textCtrlHomeFolder->SetToolTip(_("Set the home folder for this account"));
#if wxVERSION_NUMBER >= 3000
    m_textCtrlHomeFolder->SetHint(wxT(""));
#endif

    flexGridSizer31->Add(m_textCtrlHomeFolder, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText217 = new wxStaticText(
        this, wxID_ANY, _("Additional key files:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer31->Add(m_staticText217, 0, wxALL | wxALIGN_RIGHT, WXC_FROM_DIP(5));

    m_additionalFiles = new clThemedSTC(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    // Configure the fold margin
    m_additionalFiles->SetMarginType(4, wxSTC_MARGIN_SYMBOL);
    m_additionalFiles->SetMarginMask(4, wxSTC_MASK_FOLDERS);
    m_additionalFiles->SetMarginSensitive(4, true);
    m_additionalFiles->SetMarginWidth(4, 0);

    // Configure the tracker margin
    m_additionalFiles->SetMarginWidth(1, 0);

    // Configure the symbol margin
    m_additionalFiles->SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    m_additionalFiles->SetMarginMask(2, ~(wxSTC_MASK_FOLDERS));
    m_additionalFiles->SetMarginWidth(2, 0);
    m_additionalFiles->SetMarginSensitive(2, true);

    // Configure the line numbers margin
    m_additionalFiles->SetMarginType(0, wxSTC_MARGIN_NUMBER);
    m_additionalFiles->SetMarginWidth(0, 0);

    // Configure the line symbol margin
    m_additionalFiles->SetMarginType(3, wxSTC_MARGIN_FORE);
    m_additionalFiles->SetMarginMask(3, 0);
    m_additionalFiles->SetMarginWidth(3, 0);
    // Select the lexer
    m_additionalFiles->SetLexer(wxSTC_LEX_NULL);
    // Set default font / styles
    m_additionalFiles->StyleClearAll();
    m_additionalFiles->SetWrapMode(2);
    m_additionalFiles->SetIndentationGuides(0);
    m_additionalFiles->SetEOLMode(2);
    m_additionalFiles->SetKeyWords(0, wxT(""));
    m_additionalFiles->SetKeyWords(1, wxT(""));
    m_additionalFiles->SetKeyWords(2, wxT(""));
    m_additionalFiles->SetKeyWords(3, wxT(""));
    m_additionalFiles->SetKeyWords(4, wxT(""));

    flexGridSizer31->Add(m_additionalFiles, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_button51 =
        new wxButton(this, wxID_ANY, _("Test Connection"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    boxSizer23->Add(m_button51, 0, wxALL | wxEXPAND, WXC_FROM_DIP(10));

    boxSizer23->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer29 = new wxBoxSizer(wxHORIZONTAL);

    boxSizer23->Add(boxSizer29, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button27 = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button27->SetDefault();

    boxSizer29->Add(m_button27, 0, wxALL, WXC_FROM_DIP(5));

    m_button25 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    boxSizer29->Add(m_button25, 0, wxALL, WXC_FROM_DIP(5));

    SetName(wxT("AddSSHAcountDlgBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(this);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
    // Connect events
    m_textCtrlHomeFolder->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AddSSHAcountDlgBase::OnHomeFolderUpdated, this);
    m_button51->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AddSSHAcountDlgBase::OnTestConnection, this);
    m_button51->Bind(wxEVT_UPDATE_UI, &AddSSHAcountDlgBase::OnTestConnectionUI, this);
    m_button27->Bind(wxEVT_UPDATE_UI, &AddSSHAcountDlgBase::OnOKUI, this);
}

AddSSHAcountDlgBase::~AddSSHAcountDlgBase()
{
    m_textCtrlHomeFolder->Unbind(wxEVT_COMMAND_TEXT_UPDATED, &AddSSHAcountDlgBase::OnHomeFolderUpdated, this);
    m_button51->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &AddSSHAcountDlgBase::OnTestConnection, this);
    m_button51->Unbind(wxEVT_UPDATE_UI, &AddSSHAcountDlgBase::OnTestConnectionUI, this);
    m_button27->Unbind(wxEVT_UPDATE_UI, &AddSSHAcountDlgBase::OnOKUI, this);
}

SFTPBrowserBaseDlg::SFTPBrowserBaseDlg(
    wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if (!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer62 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer62);

    m_panel149 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    boxSizer62->Add(m_panel149, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer151 = new wxBoxSizer(wxVERTICAL);
    m_panel149->SetSizer(boxSizer151);

    m_toolbar = new clToolBar(
        m_panel149, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), wxTB_NODIVIDER | wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16, 16));

    boxSizer151->Add(m_toolbar, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxFlexGridSizer* flexGridSizer147 = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer147->SetFlexibleDirection(wxBOTH);
    flexGridSizer147->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer147->AddGrowableCol(1);

    boxSizer151->Add(flexGridSizer147, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText82 = new wxStaticText(
        m_panel149, wxID_ANY, _("Account:"), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), 0);

    flexGridSizer147->Add(m_staticText82, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceAccountArr;
    m_choiceAccount = new wxChoice(
        m_panel149, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), m_choiceAccountArr, 0);

    flexGridSizer147->Add(m_choiceAccount, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    flexGridSizer147->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));

    m_staticText66 = new wxStaticText(
        m_panel149, wxID_ANY, _("Path:"), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), 0);

    flexGridSizer147->Add(m_staticText66, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textCtrlRemoteFolder = new wxTextCtrl(
        m_panel149, wxID_ANY, wxT("/"), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), wxTE_PROCESS_ENTER);
    m_textCtrlRemoteFolder->SetToolTip(_("Set the remote folder to browse and click on the 'Refresh' button"));
    m_textCtrlRemoteFolder->SetFocus();
#if wxVERSION_NUMBER >= 3000
    m_textCtrlRemoteFolder->SetHint(wxT(""));
#endif

    flexGridSizer147->Add(m_textCtrlRemoteFolder, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_buttonRefresh = new wxButton(
        m_panel149, wxID_REFRESH, _("Browse..."), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), 0);

    flexGridSizer147->Add(m_buttonRefresh, 0, wxALL | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_dataview = new clThemedListCtrl(m_panel149,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDLG_UNIT(m_panel149, wxSize(500, 300)),
                                      wxDV_ROW_LINES | wxDV_SINGLE);

    boxSizer151->Add(m_dataview, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_dataview->AppendIconTextColumn(
        _("Name"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    m_dataview->AppendTextColumn(
        _("Type"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    m_dataview->AppendTextColumn(
        _("Size"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE);
    wxBoxSizer* boxSizer94 = new wxBoxSizer(wxVERTICAL);

    boxSizer151->Add(boxSizer94, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, WXC_FROM_DIP(5));

    m_textCtrlInlineSearch = new wxTextCtrl(
        m_panel149, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), wxTE_PROCESS_ENTER);
    m_textCtrlInlineSearch->Hide();
#if wxVERSION_NUMBER >= 3000
    m_textCtrlInlineSearch->SetHint(wxT(""));
#endif

    boxSizer94->Add(m_textCtrlInlineSearch, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer57 = new wxBoxSizer(wxHORIZONTAL);

    boxSizer151->Add(boxSizer57, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));

    m_button59 =
        new wxButton(m_panel149, wxID_OK, _("&OK"), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), 0);
    m_button59->SetDefault();

    boxSizer57->Add(m_button59, 0, wxALL, WXC_FROM_DIP(5));

    m_button61 = new wxButton(
        m_panel149, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDLG_UNIT(m_panel149, wxSize(-1, -1)), 0);

    boxSizer57->Add(m_button61, 0, wxALL, WXC_FROM_DIP(5));

    SetName(wxT("SFTPBrowserBaseDlg"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(this);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
    // Connect events
    m_textCtrlRemoteFolder->Bind(wxEVT_COMMAND_TEXT_ENTER, &SFTPBrowserBaseDlg::OnTextEnter, this);
    m_buttonRefresh->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SFTPBrowserBaseDlg::OnRefresh, this);
    m_buttonRefresh->Bind(wxEVT_UPDATE_UI, &SFTPBrowserBaseDlg::OnRefreshUI, this);
    m_dataview->Bind(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, &SFTPBrowserBaseDlg::OnItemSelected, this);
    m_dataview->Bind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &SFTPBrowserBaseDlg::OnItemActivated, this);
    m_textCtrlInlineSearch->Bind(wxEVT_COMMAND_TEXT_UPDATED, &SFTPBrowserBaseDlg::OnTextUpdated, this);
    m_textCtrlInlineSearch->Bind(wxEVT_COMMAND_TEXT_ENTER, &SFTPBrowserBaseDlg::OnEnter, this);
    m_textCtrlInlineSearch->Bind(wxEVT_KILL_FOCUS, &SFTPBrowserBaseDlg::OnFocusLost, this);
    m_button59->Bind(wxEVT_UPDATE_UI, &SFTPBrowserBaseDlg::OnOKUI, this);
}

SFTPBrowserBaseDlg::~SFTPBrowserBaseDlg()
{
    m_textCtrlRemoteFolder->Unbind(wxEVT_COMMAND_TEXT_ENTER, &SFTPBrowserBaseDlg::OnTextEnter, this);
    m_buttonRefresh->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &SFTPBrowserBaseDlg::OnRefresh, this);
    m_buttonRefresh->Unbind(wxEVT_UPDATE_UI, &SFTPBrowserBaseDlg::OnRefreshUI, this);
    m_dataview->Unbind(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, &SFTPBrowserBaseDlg::OnItemSelected, this);
    m_dataview->Unbind(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, &SFTPBrowserBaseDlg::OnItemActivated, this);
    m_textCtrlInlineSearch->Unbind(wxEVT_COMMAND_TEXT_UPDATED, &SFTPBrowserBaseDlg::OnTextUpdated, this);
    m_textCtrlInlineSearch->Unbind(wxEVT_COMMAND_TEXT_ENTER, &SFTPBrowserBaseDlg::OnEnter, this);
    m_textCtrlInlineSearch->Unbind(wxEVT_KILL_FOCUS, &SFTPBrowserBaseDlg::OnFocusLost, this);
    m_button59->Unbind(wxEVT_UPDATE_UI, &SFTPBrowserBaseDlg::OnOKUI, this);
}

clRemoteFindDialogBase::clRemoteFindDialogBase(
    wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if (!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer155 = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(boxSizer155);

    wxBoxSizer* boxSizer211 = new wxBoxSizer(wxVERTICAL);

    boxSizer155->Add(boxSizer211, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxFlexGridSizer* flexGridSizer163 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer163->SetFlexibleDirection(wxBOTH);
    flexGridSizer163->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    flexGridSizer163->AddGrowableCol(1);

    boxSizer211->Add(flexGridSizer163, 1, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText215 =
        new wxStaticText(this, wxID_ANY, _("Tool:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticText215, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceToolArr;
    m_choiceToolArr.Add(_("grep"));
    m_choiceToolArr.Add(_("sed"));
    m_choiceTool =
        new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceToolArr, 0);

    flexGridSizer163->Add(m_choiceTool, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticTextFind =
        new wxStaticText(this, wxID_ANY, _("Find:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticTextFind, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_comboBoxFindWhatArr;
    m_comboBoxFindWhat = new clThemedComboBox(this,
                                              wxID_ANY,
                                              wxT(""),
                                              wxDefaultPosition,
                                              wxDLG_UNIT(this, wxSize(250, -1)),
                                              m_comboBoxFindWhatArr,
                                              wxTE_PROCESS_ENTER);
    m_comboBoxFindWhat->SetFocus();
#if wxVERSION_NUMBER >= 3000
    m_comboBoxFindWhat->SetHint(wxT(""));
#endif

    flexGridSizer163->Add(m_comboBoxFindWhat, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticTextReplace =
        new wxStaticText(this, wxID_ANY, _("Replace with:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticTextReplace, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_comboBoxReplaceWithArr;
    m_comboBoxReplaceWith = new clThemedComboBox(this,
                                                 wxID_ANY,
                                                 wxT(""),
                                                 wxDefaultPosition,
                                                 wxDLG_UNIT(this, wxSize(-1, -1)),
                                                 m_comboBoxReplaceWithArr,
                                                 wxTE_PROCESS_ENTER);
#if wxVERSION_NUMBER >= 3000
    m_comboBoxReplaceWith->SetHint(wxT(""));
#endif

    flexGridSizer163->Add(m_comboBoxReplaceWith, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText169 =
        new wxStaticText(this, wxID_ANY, _("Where:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticText169, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_comboBoxWhereArr;
    m_comboBoxWhere = new clThemedComboBox(this,
                                           wxID_ANY,
                                           wxT(""),
                                           wxDefaultPosition,
                                           wxDLG_UNIT(this, wxSize(-1, -1)),
                                           m_comboBoxWhereArr,
                                           wxTE_PROCESS_ENTER);
#if wxVERSION_NUMBER >= 3000
    m_comboBoxWhere->SetHint(wxT(""));
#endif

    flexGridSizer163->Add(m_comboBoxWhere, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText220 = new wxStaticText(
        this, wxID_ANY, _("Exclude patterns:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticText220, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    m_textExcludePatterns = new clThemedTextCtrl(
        this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTE_PROCESS_ENTER);
    m_textExcludePatterns->SetToolTip(_("A semi colon list of exclude patterns"));
#if wxVERSION_NUMBER >= 3000
    m_textExcludePatterns->SetHint(wxT(""));
#endif

    flexGridSizer163->Add(m_textExcludePatterns, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText175 =
        new wxStaticText(this, wxID_ANY, _("File types:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticText175, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_comboBoxTypesArr;
    m_comboBoxTypes = new clThemedComboBox(this,
                                           wxID_ANY,
                                           wxT(""),
                                           wxDefaultPosition,
                                           wxDLG_UNIT(this, wxSize(-1, -1)),
                                           m_comboBoxTypesArr,
                                           wxTE_PROCESS_ENTER);
#if wxVERSION_NUMBER >= 3000
    m_comboBoxTypes->SetHint(wxT(""));
#endif

    flexGridSizer163->Add(m_comboBoxTypes, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_staticText177 =
        new wxStaticText(this, wxID_ANY, _("Account:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    flexGridSizer163->Add(m_staticText177, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));

    wxArrayString m_choiceAccountsArr;
    m_choiceAccounts =
        new clThemedChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), m_choiceAccountsArr, 0);

    flexGridSizer163->Add(m_choiceAccounts, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer204 = new wxBoxSizer(wxVERTICAL);

    boxSizer155->Add(
        boxSizer204, 0, wxRIGHT | wxTOP | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(10));

    m_buttonFind =
        new wxButton(this, ID_REMOTE_FIND, _("Find"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_buttonFind->SetDefault();

    boxSizer204->Add(m_buttonFind, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_buttonReplace =
        new wxButton(this, ID_REMOTE_REPLACE, _("Replace"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    boxSizer204->Add(m_buttonReplace, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_buttonCancel =
        new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);

    boxSizer204->Add(m_buttonCancel, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_checkBoxWholeWord =
        new wxCheckBox(this, wxID_ANY, _("Whole word"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_checkBoxWholeWord->SetValue(false);

    boxSizer204->Add(m_checkBoxWholeWord, 0, wxALL | wxALIGN_LEFT, WXC_FROM_DIP(5));

    m_checkBoxCase =
        new wxCheckBox(this, wxID_ANY, _("Case sensitive"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_checkBoxCase->SetValue(false);

    boxSizer204->Add(m_checkBoxCase, 0, wxALL | wxALIGN_LEFT, WXC_FROM_DIP(5));

    SetName(wxT("clRemoteFindDialogBase"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if (GetSizer()) {
        GetSizer()->Fit(this);
    }
    if (GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    if (!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
    // Connect events
    m_choiceTool->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &clRemoteFindDialogBase::OnTool, this);
    m_comboBoxFindWhat->Bind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_comboBoxReplaceWith->Bind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnReplace, this);
    m_comboBoxWhere->Bind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_textExcludePatterns->Bind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_comboBoxTypes->Bind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_buttonFind->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &clRemoteFindDialogBase::OnFind, this);
    m_buttonFind->Bind(wxEVT_UPDATE_UI, &clRemoteFindDialogBase::OnFindUI, this);
    m_buttonReplace->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &clRemoteFindDialogBase::OnReplace, this);
    m_buttonReplace->Bind(wxEVT_UPDATE_UI, &clRemoteFindDialogBase::OnReplaceUI, this);
}

clRemoteFindDialogBase::~clRemoteFindDialogBase()
{
    m_choiceTool->Unbind(wxEVT_COMMAND_CHOICE_SELECTED, &clRemoteFindDialogBase::OnTool, this);
    m_comboBoxFindWhat->Unbind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_comboBoxReplaceWith->Unbind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnReplace, this);
    m_comboBoxWhere->Unbind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_textExcludePatterns->Unbind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_comboBoxTypes->Unbind(wxEVT_COMMAND_TEXT_ENTER, &clRemoteFindDialogBase::OnFind, this);
    m_buttonFind->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &clRemoteFindDialogBase::OnFind, this);
    m_buttonFind->Unbind(wxEVT_UPDATE_UI, &clRemoteFindDialogBase::OnFindUI, this);
    m_buttonReplace->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &clRemoteFindDialogBase::OnReplace, this);
    m_buttonReplace->Unbind(wxEVT_UPDATE_UI, &clRemoteFindDialogBase::OnReplaceUI, this);
}
