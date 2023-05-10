#include "MainWindow.h"
#include "SecondWindow.h"

class MyApp : public wxApp {

public:   
    bool OnInit();      
};

bool MyApp::OnInit() {
    
    MainWindow *mainWindow = new MainWindow(NULL);
    mainWindow->SetSize(wxSize(900, 900));
    mainWindow->Center(true);
    mainWindow->Show(true);
    return true;

}

wxIMPLEMENT_APP(MyApp);


MainWindow::MainWindow(wxWindow *parent) : wxFrame(parent, wxID_ANY, "Cocktail App", wxDefaultPosition, wxDefaultSize) {

    secondWindow = new SecondWindow(this);
    thirdWindow = new ThirdWindow(this);

    wxBoxSizer *BoxSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerTitle = new wxBoxSizer(wxHORIZONTAL);

    wxButton *myButtonDelete = new wxButton(this, wxID_ANY, "Delete");
    wxButton *myButtonClose = new wxButton(this, wxID_ANY, "Close", wxPoint(800, 860), wxDefaultSize);
    wxButton *myButtonShow = new wxButton(this, wxID_ANY, "Show Cocktail");
    wxButton *myButtonNew = new wxButton(this, wxID_ANY, "Add Cocktail");
    wxButton *myButtonRefresh = new wxButton(this, wxID_ANY, "Refresh");

    myButtonShow->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnButtonClicked, this);
    myButtonNew->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnButtonAddClicked, this);
    myButtonDelete->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnButtonDeleteClicked, this);
    myButtonClose->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnButtonCloseClicked, this);
    myButtonRefresh->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnButtonRefreshClicked, this);

    
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

    listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(600, 400), wxLC_REPORT);
    listCtrl->InsertColumn(0, "ID", 0, 60);
    listCtrl->InsertColumn(1, "Name", 0, 200);
    listCtrl->InsertColumn(2, "Beschreibung", 0 , 340);
    

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *beschreibung = sqlite3_column_text(stmt, 2);
        long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format("%d", id));
        listCtrl->SetItem(index, 1, wxString::FromUTF8((const char*)name));
        listCtrl->SetItem(index, 2, wxString::FromUTF8((const char*)beschreibung));
    }

    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, "Your Cocktail Database:");
    wxFont font = staticText->GetFont();
    font.SetPointSize(17);
    staticText->SetFont(font);

    sizerTitle->Add(staticText);
    BoxSizer->Add(sizerTitle);
    BoxSizer->Add(listCtrl,0, wxTOP, 20);

    buttonSizer->Add(myButtonShow, 0, wxALL, 5);
    buttonSizer->Add(myButtonNew, 0, wxALL, 5);
    buttonSizer->Add(myButtonRefresh, 0, wxALL, 5);
    buttonSizer->Add(myButtonDelete, 0, wxALL, 5);

    mainSizer->Add(BoxSizer, 0, wxALL, 20);
    mainSizer->Add(buttonSizer, 0, wxALL, 20);

    SetSizer(mainSizer);

}



void MainWindow::OnButtonClicked(wxCommandEvent &event) {

    secondWindow->Show(true);
}

void MainWindow::OnButtonAddClicked(wxCommandEvent &event) {

    thirdWindow->Show(true);
}

void MainWindow::OnButtonDeleteClicked(wxCommandEvent& event) {

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

void MainWindow::OnButtonRefreshClicked(wxCommandEvent& event) {
    UpdateList();
}

void MainWindow::UpdateList() {

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
        long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format("%d", id));
        listCtrl->SetItem(index, 1, wxString::FromUTF8((const char*)name));
        listCtrl->SetItem(index, 2, wxString::FromUTF8((const char*)beschreibung));     
    }
    sqlite3_finalize(stmt);
}


void MainWindow::OnButtonCloseClicked(wxCommandEvent& event) {
    
    //mainWin->UpdateListBox(mainWin->listBox);
    sqlite3_close(db);
    this->Close();
}

// destructor
MainWindow::~MainWindow() {
    sqlite3_close(db);
}



/* 
void MainWindow::UpdateListCtrl(wxListCtrl *listCtrl) {
    listCtrl->ClearAll();

    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

    if (db == nullptr) {
        std::cerr << "Error: Database connection is null" << std::endl;
        return;
    }
    // Retrieve data from database
    wxString sql = "SELECT cocktail.name, zutaten.name, cocktail_list.menge,cocktail_list.einheit FROM cocktail JOIN cocktail_list ON cocktail.id = cocktail_list.cocktail_id JOIN zutaten ON zutaten.id = cocktail_list.zutat_id";

    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error: Failed to prepare SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 0);
        const unsigned char *zutat = sqlite3_column_text(stmt, 1);
        int menge = sqlite3_column_int(stmt, 2);
        const unsigned char *einheit = sqlite3_column_text(stmt, 3);
        long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString(name));
        listCtrl->SetItem(index, 1, wxString(zutat));
        listCtrl->SetItem(index, 2, wxString::Format("%d", menge));
        listCtrl->SetItem(index, 3, wxString(einheit));
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Error: Failed to retrieve data from database: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}
*/


