#include "MainWindow.h"
#include "SecondWindow.h"

SecondWindow::SecondWindow(wxWindow *parent) : wxFrame(parent, wxID_ANY, "Add Cocktail", wxDefaultPosition, wxSize(800, 800)) {

    //mainWin = std::make_shared<MainWindow>(this);
    wxBoxSizer *sizerTwo = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hboxSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer *staticBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this);
    wxStaticText *label = new wxStaticText(this, wxID_ANY, "Liste der Cocktails:");
    staticBoxSizer->Add(label, 0, wxALL, 5);

    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no open");
        return;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT id, name, beschreibung FROM cocktail", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no prepare");
        return;
    }

    listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(500, 300), wxLC_REPORT);
    listCtrl->InsertColumn(0, "ID");
    listCtrl->InsertColumn(1, "Name");
    listCtrl->InsertColumn(2, "Beschreibung");
    //listCtrl->InsertColumn(3, "Zutaten");
    staticBoxSizer->Add(listCtrl, 1, wxEXPAND);

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *beschreibung = sqlite3_column_text(stmt, 2);
        //const unsigned char *ingredients = sqlite3_column_text(stmt, 3);
        long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format("%d", id));
        listCtrl->SetItem(index, 1, wxString::FromUTF8((const char*)name));
        listCtrl->SetItem(index, 2, wxString::FromUTF8((const char*)beschreibung));
        //listCtrl->SetItem(index, 3, wxString::FromUTF8((const char*)ingredients));
    }

    
    hboxSizer->Add(staticBoxSizer, 0, wxALL, 5);
    hboxSizer->AddStretchSpacer();   
    sizerTwo->Add(hboxSizer, 0, wxALL, 5);   

    textCtrl1 = new wxTextCtrl(this, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize);	
	textCtrl2 = new wxTextCtrl(this, wxID_ANY, "Beschreibung", wxDefaultPosition, wxSize(200, -1));
	//textCtrl3 = new wxTextCtrl(this, wxID_ANY, "Zutaten", wxDefaultPosition, wxDefaultSize);
    buttonAdd = new wxButton(this, wxID_ANY, "Add");
    buttonDelete = new wxButton(this, wxID_ANY, "Delete");
    buttonClose = new wxButton(this, wxID_ANY, "Close",wxPoint(500, 600),wxDefaultSize);

    textCtrl1->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(SecondWindow::OnTextCtrlFocus), NULL, this);
    textCtrl2->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(SecondWindow::OnTextCtrlFocus), NULL, this);
    //textCtrl3->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(SecondWindow::OnTextCtrlFocus), NULL, this);
    buttonDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonDeleteClicked), NULL, this);
    buttonAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonAddClicked), NULL, this);
    buttonClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SecondWindow::OnButtonCloseClicked), NULL, this);
    //buttonClose->Bind(wxEVT_BUTTON, &SecondWindow::OnButtonCloseClicked, this);

    hbox->Add(textCtrl1, 0, wxALL, 5);
    hbox->Add(textCtrl2, 0, wxALL, 5);
	//hbox->Add(textCtrl3, 0, wxALL, 5);

	sizerTwo->Add(hbox, 0, wxALL, 5);
    hbox->Add(buttonAdd, 0, wxALL, 5);
    hbox->Add(buttonDelete, 0, wxALL, 5);
    SetSizer(sizerTwo);

    //wxMessageBox("noch kein fehler");
    sqlite3_finalize(stmt);
    sqlite3_close(db);
  
}

// kleiner helper für textcontrol felder
void SecondWindow::OnTextCtrlFocus(wxFocusEvent& event) {

    wxTextCtrl* textCtrl = (wxTextCtrl*)event.GetEventObject();
    textCtrl->Clear();
    event.Skip();
}

void SecondWindow::UpdateList() {

    listCtrl->DeleteAllItems();

    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no open");
        return;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT id, name, beschreibung FROM cocktail", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no prepare");
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *beschreibung = sqlite3_column_text(stmt, 2);
        //const unsigned char *ingredients = sqlite3_column_text(stmt, 3);
        long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format("%d", id));
        listCtrl->SetItem(index, 1, wxString::FromUTF8((const char*)name));
        listCtrl->SetItem(index, 2, wxString::FromUTF8((const char*)beschreibung));
        //listCtrl->SetItem(index, 3, wxString::FromUTF8((const char*)ingredients));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SecondWindow::OnButtonAddClicked(wxCommandEvent& event) {

	std::cout << "Inserting Data yay" << std::endl;

    wxString name = textCtrl1->GetValue();
    wxString beschreibung = textCtrl2->GetValue();
    //wxString ingredients = textCtrl3->GetValue();
    

    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no open");
        return;
    }

    wxString query = wxString::Format("INSERT INTO cocktail (name, beschreibung) VALUES ('%s', '%s')", name.c_str(), beschreibung.c_str());
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no insert");
        return;
    }

    UpdateList();
}

void SecondWindow::OnButtonDeleteClicked(wxCommandEvent& event) {

    long itemIndex = -1;
    itemIndex = listCtrl->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (itemIndex == -1) {
        wxMessageBox("select an item");
        return;
    }

    wxString id = listCtrl->GetItemText(itemIndex);

    sqlite3 *db;
    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no open");
        return;
    }

    wxString query = wxString::Format("DELETE FROM cocktail WHERE ID=%s", id.c_str());
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("db says no delete");
        return;
    }

    UpdateList();
}


void SecondWindow::OnButtonCloseClicked(wxCommandEvent& event) {
    
    //mainWin->UpdateListBox(mainWin->listBox);
    this->Show(false);
}
 
SecondWindow::~SecondWindow() {
    //wxMessageBox("Destuctor called");
    this->Show(false);
}
