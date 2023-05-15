#include "MainWindow.h"
#include "SecondWindow.h"

SecondWindow::SecondWindow(wxWindow *parent) : wxFrame(parent, wxID_ANY, "Add Zutat", wxDefaultPosition, wxSize(900, 700)) {

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerTitle = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *BoxSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *checkSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *buttonDelete = new wxButton(this, wxID_ANY, "Delete");
    wxButton *buttonClose = new wxButton(this, wxID_ANY, "Close", wxPoint(800, 660), wxDefaultSize);
    wxButton *buttonChange = new wxButton(this, wxID_ANY, "Change");
    wxButton *buttonAdd = new wxButton(this, wxID_ANY, "Add");

    checkBox = new wxCheckBox(this, wxID_ANY, "Alcoholic");
    checkBoxAll = new wxCheckBox(this, wxID_ANY, "show all");
    listBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(330, 300));

    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, "Your Incredients:");
    wxFont font = staticText->GetFont();
    font.SetPointSize(17);
    staticText->SetFont(font);

    UpdateListBox("SELECT name FROM zutaten;", listBox);

    textCtrlName = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxSize(200, -1));	
    textCtrlName->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(SecondWindow::OnTextCtrlFocus), NULL, this);

    buttonDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonDeleteClicked), NULL, this);
    buttonAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonAddZutatClicked), NULL, this);
    buttonClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonCloseClicked), NULL, this);

    checkBox->Bind(wxEVT_CHECKBOX, &SecondWindow::OnCheckBox, this);
    checkBoxAll->Bind(wxEVT_CHECKBOX, &SecondWindow::OnCheckBox, this);
    
    sizerTitle->Add(staticText, 0, wxALL, 10);
    checkSizer->Add(textCtrlName, 0, wxALL, 5);
    checkSizer->Add(checkBox, 0, wxALL, 5);
    checkSizer->Add(checkBoxAll, 0, wxALL, 5);

    BoxSizer->Add(sizerTitle, 0, wxLEFT, 10);
    BoxSizer->Add(listBox, 0, wxALL, 20);
    BoxSizer->Add(checkSizer, 0, wxLEFT, 15);

    buttonSizer->Add(buttonAdd, 0, wxALL, 5);
    buttonSizer->Add(buttonChange, 0, wxALL, 5);
    buttonSizer->Add(buttonDelete, 0, wxALL, 5);

    mainSizer->Add(BoxSizer, 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxLEFT, 20);

    SetSizer(mainSizer);

}

// kleiner helper für textcontrol felder
void SecondWindow::OnTextCtrlFocus(wxFocusEvent& event) {

    wxTextCtrl* textCtrlName = (wxTextCtrl*)event.GetEventObject();
    textCtrlName->Clear();
    event.Skip();
}

void SecondWindow::OnCheckBox(wxCommandEvent &event) {
    if (checkBoxAll->IsChecked()) {
        UpdateListBox("SELECT name FROM zutaten;", listBox);
    } else {
        if (checkBox->IsChecked()) {
            UpdateListBox("SELECT name FROM zutaten WHERE alkoholhaltig = 1;", listBox);
        } else {
            UpdateListBox("SELECT name FROM zutaten WHERE alkoholhaltig = 0;", listBox);
        }
    }
}

void SecondWindow::UpdateListBox(const std::string &sql,wxListBox *listBox) {
        listBox->Clear();

        sqlite3_stmt *stmt = nullptr;
        
        int rc = sqlite3_open("/mnt/c/Users/foxfo/Desktop/Pos4-Project/cocktail_db.db", &db);

        if (rc != SQLITE_OK) {
            wxMessageBox("Fehler beim Öffnen der Datenbank!");
            return;
        }

        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

        if (rc != SQLITE_OK) {
            std::cerr << "Fehler bei sqlite3_prepare_v2: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            const unsigned char *name = sqlite3_column_text(stmt, 0);
            listBox->Append(wxString::FromUTF8((const char*)name));
        }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SecondWindow::OnButtonAddZutatClicked(wxCommandEvent& event) {

    wxString name = textCtrlName->GetValue();
    bool alkoholhaltig = checkBox->IsChecked();
    //bool alkoholhaltig = true;
    bool deko = false; // brauch ich vllt später noch
    
    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/foxfo/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("cant open db");
        return;
    }

    const char *query = "INSERT INTO zutaten (name, alkoholhaltig, deko) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxString::Format("cant prepare: %s", sqlite3_errmsg(db)));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_int(stmt, 2, alkoholhaltig);
    rc = sqlite3_bind_int(stmt, 3, deko);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        wxMessageBox("check stmt");
        return;
    }

    wxMessageBox("incredient %s inserted", name);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SecondWindow::OnButtonDeleteClicked(wxCommandEvent& event) {
    int selection = listBox->GetSelection();

    if (selection == wxNOT_FOUND) {
        wxMessageBox("No incredient selected");
        return;
    }

    wxString name = listBox->GetString(selection);

    int answer = wxMessageBox(wxString::Format("sure delete '%s'?", name), "Confirm", wxYES_NO);
        if (answer != wxYES) {
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/foxfo/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no open");
        return;
    }

    const char *query = "DELETE FROM zutaten WHERE name = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxString::Format("db says no prepare: %s", sqlite3_errmsg(db)));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    wxMessageBox("incredient deleted");
    listBox->Delete(selection);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void SecondWindow::OnButtonCloseClicked(wxCommandEvent& event) {
    this->Show(false);
    this->Close();
}
 
SecondWindow::~SecondWindow() {}
