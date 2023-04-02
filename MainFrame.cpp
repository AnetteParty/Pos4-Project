#include "MainFrame.h"
#include <sqlite3.h>
#include <wx/wx.h>
#include <wx/grid.h>



MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {

    wxPanel *panel = new wxPanel(this);
    
    dbButton = new wxButton(panel, wxID_ANY, "Generate Data", wxPoint(100+100, 50+50), wxSize(150,50));
    dbButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

    dbSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(dbSizer);

    dbGrid = new wxGrid(panel, wxID_ANY);
    dbSizer->Add(dbGrid, 1, wxEXPAND);
}
    
void MainFrame::OnButtonClicked(wxCommandEvent& event) {

    sqlite3_open("datenbank.db", &db);
    sqlite3_exec(db,"CREATE TABLE Cocktails (id INTEGER PRIMARY KEY,name TEXT,price REAL)", NULL, NULL, NULL);
    sqlite3_close(db);
      
}

