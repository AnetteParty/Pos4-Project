#include <wx/grid.h>
#include <sqlite3.h>
#include <wx/tokenzr.h>
#include <vector>
 
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
class App : public wxApp {

public:
        
    bool OnInit();      
};
 


class MainFrame : public wxFrame {

public:

    MainFrame(const wxString &title);
    void OnButtonClicked(wxCommandEvent& event);
    void OnButton2Clicked(wxCommandEvent& event);
    void OnDeleteButtonClicked(wxCommandEvent& event);
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    bool checkDatabaseAccess();
    void printDatabase();
    void printOnPanel();

    enum class InputType {
        SingleLine,
        MultiLine
    };

    
private:

    wxPanel *panel;
    
    wxButton *dbButton;
    wxButton *dbButton2;
    wxButton *dbButtonDel;

    wxGrid *dbGrid;
    wxBoxSizer *dbSizer;
    sqlite3 *db;

};

wxIMPLEMENT_APP(App);

bool App::OnInit() {

    //wxSystemOptions::SetOption("gtk.no-gtk-cursor", "1");
    MainFrame *mainFrame = new MainFrame("Cocktail Menu");
    mainFrame->Center(true);
    mainFrame->Show(true);
    mainFrame->SetClientSize(800, 800);
    return true;

}

bool MainFrame::checkDatabaseAccess() {
    
    int rc = sqlite3_open("database.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "keine connection zur db weil: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    else {
        std::cout << "yay db connected!!" << std::endl;
        sqlite3_close(db);
        return true;
    }
}

int MainFrame::callback(void *NotUsed, int argc, char **argv, char **azColName) {
    
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

void MainFrame::printDatabase() {
    
    char *zErrMsg = 0;
    int rc;
    const char* data = "Callback function called";

    rc = sqlite3_open("database.db", &db);

    if( rc ) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::string sql = "SELECT * FROM cocktails";

    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Operation done successfully" << std::endl;
    }

    sqlite3_close(db);
}

void MainFrame::printOnPanel() {

    wxTextCtrl* resultText = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);

    int rc = sqlite3_open("database.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("Failed to open database");
        return;
    }

    const char* sql = "SELECT * FROM users";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("Failed to prepare statement");
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = std::to_string(sqlite3_column_int(stmt, 0));
        std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string desc = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

        resultText->AppendText(id + " " + name + " " + desc + "\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}





MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {

    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
    panel->SetBackgroundColour(wxColor(100, 100, 200));
    wxPanel *btnPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300,100));
    btnPanel->SetBackgroundColour(wxColor(160, 220, 120));
    
    //dbSizer = new wxBoxSizer(wxVERTICAL);
    //dbSizer->Add(panel, 1, wxEXPAND | wxALL, 5);   // proportion = 0 -> bleibt gleich wenn resizing
    //dbSizer->Add(btnPanel, 1, wxEXPAND | wxALL, 5);
    //this->SetSizerAndFit(dbSizer);
       
    dbButton = new wxButton(panel, wxID_ANY, "Show", wxPoint(100, 10), wxSize(150,50));
    dbButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    dbButton2 = new wxButton(panel, wxID_ANY, "Add", wxPoint(300, 10), wxSize(150,50));
    dbButton2->Bind(wxEVT_BUTTON, &MainFrame::OnButton2Clicked, this);
    dbButtonDel = new wxButton(panel, wxID_ANY, "Delete", wxPoint(500, 10), wxSize(150,50));
    dbButtonDel->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteButtonClicked, this);
    
    std::vector<std::pair<wxString, InputType>> form = {
        {"Name", InputType::SingleLine},
        {"Description", InputType::MultiLine},
        {"Incredients", InputType::SingleLine},
        {"Price", InputType::SingleLine}
    };
   
    auto boxSizer = new wxBoxSizer(wxVERTICAL);
    //wxPanel *panel = new wxPanel(this, wxID_ANY);
    auto margin = 20;

    auto sizer = new wxFlexGridSizer(form.size(), 2, margin,margin);
    
    sizer->AddGrowableCol(1);

    for (const auto &[label, type] : form) {
        auto labelCtrl = new wxStaticText(btnPanel, wxID_ANY, label);
        sizer->Add(labelCtrl, 0, wxALIGN_CENTER_VERTICAL, 10);

        auto style = type == InputType::SingleLine ? 0 : wxTE_MULTILINE;
        auto inputCtrl = new wxTextCtrl(btnPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, style);

        sizer->Add(inputCtrl, 1, wxEXPAND | wxTOP, 5);
    }

    panel->SetSizer(sizer);

    boxSizer->Add(panel, 1, wxEXPAND | wxALL, 20);
    boxSizer->Add(btnPanel, 1, wxEXPAND | wxALL, 20);
    this->SetSizerAndFit(boxSizer);

}





////////////////////////////////////////////////////////////////
void MainFrame::OnButtonClicked(wxCommandEvent& event) {

    checkDatabaseAccess();   
}

void MainFrame::OnButton2Clicked(wxCommandEvent& event) {

    printDatabase();
}

void MainFrame::OnDeleteButtonClicked(wxCommandEvent& event) {

    printOnPanel();
}





/*

void MainWindow::OnCheckBoxOne(wxCommandEvent& event) {

    UpdateListBoxTwo("SELECT name FROM zutaten WHERE alkoholhaltig = 0;");
}
void MainWindow::OnCheckBoxTwo(wxCommandEvent& event) {

    UpdateListBoxTwo("SELECT name FROM zutaten WHERE alkoholhaltig = 1;");
}




void MainWindow::UpdateListBox(wxListBox* listBox, sqlite3_stmt *stmt) {

    listBox->Clear();

    int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/neu/cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox("Fehler beim Öffnen der Datenbank!");
        return;
    }

    rc = sqlite3_prepare_v2(db, "SELECT name FROM zutaten WHERE alkoholhaltig = 0;", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox("Fehler bei der Abfrage!");
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        const unsigned char *name = sqlite3_column_text(stmt, 0);
        listBox->Append(wxString::FromUTF8((const char*)name));

    }

    rc = sqlite3_prepare_v2(db, "SELECT * FROM zutaten WHERE alkoholhaltig = 1;", -1, &stmt2, NULL);
    while (sqlite3_step(stmt2) == SQLITE_ROW) {

        const unsigned char *nametemp = sqlite3_column_text(stmt2, 0);
        listBox->Append(wxString::FromUTF8((const char*)nametemp));

    }

    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt2);
    
}


















void MainFrame::OnButtonClicked(wxCommandEvent& event) {

    checkDatabaseAccess();
     sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cocktails (id INTEGER PRIMARY KEY,name TEXT,description TEXT,ingredients TEXT,price TEXT)", NULL, NULL, NULL);
    sqlite3_close(db); 

    sqlite3_open("database.db", &db);

    // SQL-Abfrage zum Abrufen der Daten aus der Tabelle
    wxArrayString data;
    sqlite3_exec(db, "SELECT * FROM cocktails", callback, &data, NULL);

    // Schließen der Verbindung zur Datenbank
    sqlite3_close(db);

    // Anzeigen der Daten in einem Grid
    dbGrid = new wxGrid(this, wxID_ANY, wxPoint(10, 10), wxSize(400,400));
    dbSizer->Add(dbGrid, 1, wxALL|wxEXPAND, 10);  
    dbSizer->AddStretchSpacer();  

    dbGrid->CreateGrid(data.GetCount(), 5);
    for (int i = 0; i < data.GetCount(); i++) {
        wxStringTokenizer tkz(data[i], ",");
        for (int j = 0; j < 5 && tkz.HasMoreTokens(); j++) {
            wxString token = tkz.GetNextToken();
            dbGrid->SetCellValue(i, j, token);
        }
    }

    dbGrid->ForceRefresh(); 
}  
    wxArrayString data; // Array zum Speichern des Abfrageergebnisses
    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, "SELECT FROM Cocktails", callback, &data, NULL);
    sqlite3_close(db);

    // Tabellenansicht löschen und mit dem Abfrageergebnis füllen
    dbGrid = new wxGrid(this, wxID_ANY, wxPoint(10, 10), wxSize(400,400));
    dbSizer->Add(dbGrid, 1, wxALL|wxEXPAND, 10);  // Setzt die Größe des Grids auf alle Seiten des Panels
    dbSizer->AddStretchSpacer();  // Fügt ein Stretch-Element hinzu, um Platz zu schaffen

    dbGrid->CreateGrid(data.GetCount(), 3);
        for (int i = 0; i < data.GetCount(); i++) {
            wxStringTokenizer tkz(data[i], ",");
            for (int j = 0; j < 3 && tkz.HasMoreTokens(); j++) {
                wxString token = tkz.GetNextToken();
                dbGrid->SetCellValue(i, j, token);
            }
        }

    dbGrid->ForceRefresh();


void MainFrame::OnButton2Clicked(wxCommandEvent& event) {

    printDatabase();

    sqlite3_open("datenbank.db", &db);

    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Mojito', 8.50)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Caipirinha', 7.00)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Pina Colada', 9.00)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Margarita', 8.00)", NULL, NULL, NULL);
    sqlite3_close(db);

    dbGrid->ForceRefresh();
    
   
    wxString name = dynamic_cast<wxTextCtrl*>(FindWindowByName("Name"))->GetValue();
    wxString desc = dynamic_cast<wxTextCtrl*>(FindWindowByName("Description"))->GetValue();
    wxString ing = dynamic_cast<wxTextCtrl*>(FindWindowByName("Ingredients"))->GetValue();
    //wxString price = dynamic_cast<wxTextCtrl*>(FindWindowByName("Price"))->GetValue();

    wxString sql = wxString::Format("INSERT INTO Cocktails (name, description, ingredients) VALUES ('%s', '%s', '%s')",
                                    name, desc, ing);

    sqlite3_open("database.db", &db);
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_close(db);   
}
*/




/* 
int MainFrame::callback(void *data, int argc, char **argv, char **azColName) {
    wxArrayString *arr = static_cast<wxArrayString*>(data);
    wxString row;
    for (int i = 0; i < argc; i++) {
        row += wxString::Format("%s%s", (i > 0 ? "," : ""), argv[i]);
    }
    arr->Add(row);
    return 0;
}
*/



















///////////////////////////////////////////////////////////////////////////////

class AnotherFrame : public wxFrame {
public:
    AnotherFrame();
 
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

};

AnotherFrame::AnotherFrame(): wxFrame(nullptr, wxID_ANY, "Hello World") {

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(1, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, &AnotherFrame::OnHello, this, 1);
    Bind(wxEVT_MENU, &AnotherFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &AnotherFrame::OnExit, this, wxID_EXIT);
}
 
void AnotherFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
 
void AnotherFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void AnotherFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}
