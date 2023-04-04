#include "MainFrame.h"
#include <sqlite3.h>
#include <vector>
#include <string.h>
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/tokenzr.h>


MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {

    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300,300));
    panel->SetBackgroundColour(wxColor(100, 100, 200));
    wxPanel *btnPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300,100));
    btnPanel->SetBackgroundColour(wxColor(200, 100, 100));
    
    //dbSizer = new wxBoxSizer(wxVERTICAL);
    //dbSizer->Add(panel, 1, wxEXPAND | wxALL, 5);   // proportion = 0 -> bleibt gleich wenn resizing
    //dbSizer->Add(btnPanel, 1, wxEXPAND | wxALL, 5);
    //this->SetSizerAndFit(dbSizer);
    
    dbButton = new wxButton(panel, wxID_ANY, "Generate Table", wxPoint(100, 10), wxSize(150,50));
    dbButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    dbButton2 = new wxButton(panel, wxID_ANY, "Generate Data", wxPoint(300, 10), wxSize(150,50));
    dbButton2->Bind(wxEVT_BUTTON, &MainFrame::OnButton2Clicked, this);
    dbButtonDel = new wxButton(panel, wxID_ANY, "Delete Data", wxPoint(500, 10), wxSize(150,50));
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

    boxSizer->Add(panel, 1, wxEXPAND | wxALL, 1);
    boxSizer->Add(btnPanel, 1, wxEXPAND | wxALL, 1);
    this->SetSizerAndFit(boxSizer);

}


void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Cocktails (id INTEGER PRIMARY KEY,name TEXT,description TEXT,ingredients TEXT,price TEXT)", NULL, NULL, NULL);
    sqlite3_close(db);

    /*     wxArrayString data; // Array zum Speichern des Abfrageergebnisses
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
     */
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
   
    wxString name = dynamic_cast<wxTextCtrl*>(FindWindowByName("Name"))->GetValue();
    wxString desc = dynamic_cast<wxTextCtrl*>(FindWindowByName("Description"))->GetValue();
    wxString ing = dynamic_cast<wxTextCtrl*>(FindWindowByName("Ingredients"))->GetValue();
    wxString price = dynamic_cast<wxTextCtrl*>(FindWindowByName("Price"))->GetValue();

    wxString sql = wxString::Format("INSERT INTO Cocktails (name, description, ingredients, price) VALUES ('%s', '%s', '%s', '%s')",
                                    name, desc, ing, price);
                                    
    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_close(db);
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
