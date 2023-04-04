#include "MainFrame.h"
#include <sqlite3.h>
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/tokenzr.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel *panel = new wxPanel(this);

    dbButton = new wxButton(panel, wxID_ANY, "Generate Table", wxPoint(100, 500), wxSize(150,50));
    dbButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    dbButton2 = new wxButton(panel, wxID_ANY, "Generate Data", wxPoint(300, 500), wxSize(150,50));
    dbButton2->Bind(wxEVT_BUTTON, &MainFrame::OnButton2Clicked, this);
    dbButtonDel = new wxButton(panel, wxID_ANY, "Delete Data", wxPoint(500, 500), wxSize(150,50));
    dbButtonDel->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteButtonClicked, this);


    dbSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(dbSizer);
}


void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cocktails (id INTEGER PRIMARY KEY,name TEXT,price REAL)", NULL, NULL, NULL);
    //sqlite3_close(db);
    wxArrayString data; // Array zum Speichern des Abfrageergebnisses
    //sqlite3_open("datenbank.db", &db);
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
}

void MainFrame::OnButton2Clicked(wxCommandEvent& event) {

    /*     sqlite3_open("datenbank.db", &db);

    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Mojito', 8.50)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Caipirinha', 7.00)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Pina Colada', 9.00)", NULL, NULL, NULL);
    sqlite3_exec(db,"INSERT INTO Cocktails (name, price) VALUES ('Margarita', 8.00)", NULL, NULL, NULL);
    sqlite3_close(db);

    dbGrid->ForceRefresh();
    */

    int rc = sqlite3_open("datenbank.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }

    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS Cocktails (id INTEGER PRIMARY KEY, name TEXT, price REAL)";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }

    const char *insert_data_sql = "INSERT INTO Cocktails (name, price) VALUES ('Mojito', 8.50), ('Caipirinha', 7.00), ('Pina Colada', 9.00), ('Margarita', 8.00)";
    rc = sqlite3_exec(db, insert_data_sql, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }

    std::cout << "Database created successfully!" << std::endl;
    sqlite3_close(db);
    dbGrid->ForceRefresh();
}


void MainFrame::OnDeleteButtonClicked(wxCommandEvent& event) {
    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, "DELETE * FROM Cocktails", NULL, NULL, NULL);
    sqlite3_close(db);

    // Nach dem Löschen alle Datensätze entfernen und Tabellenansicht aktualisieren
    dbGrid->DeleteRows(0, dbGrid->GetNumberRows());
    dbGrid->ForceRefresh();
}


int MainFrame::callback(void *data, int argc, char **argv, char **azColName) {
    wxArrayString *arr = static_cast<wxArrayString*>(data);
    wxString row;
    for (int i = 0; i < argc; i++) {
        row += wxString::Format("%s%s", (i > 0 ? "," : ""), argv[i]);
    }
    arr->Add(row);
    return 0;
}
