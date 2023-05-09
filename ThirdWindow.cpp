#include <wx/spinctrl.h>
#include <wx/combobox.h>

#include "MainWindow.h"
#include "ThirdWindow.h"

ThirdWindow::ThirdWindow(wxWindow *parent) : wxFrame(parent, wxID_ANY, "Add Cocktail", wxDefaultPosition, wxSize(1000, 800)) {
    Center(true);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerTitle = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sizerChoice = new wxBoxSizer(wxHORIZONTAL);

    choice = new wxChoice(this, wxID_ANY);
    UpdateChoice();
    //comboBox = new wxComboBox(this, wxID_ANY);
    //UpdateComboBox();

    topSizer->Add(new wxStaticText(this, wxID_ANY, "Name: ", wxDefaultPosition, wxSize(250, -1)), 0, wxALL, 5);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Zubereitung: ", wxDefaultPosition, wxSize(120, -1)), 0, wxALL, 5);

    wxSpinCtrl *spinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS);

    wxComboBox *comboBox2 = new wxComboBox(this, wxID_ANY);
    comboBox2->Append(wxT("Liter"));
    comboBox2->Append(wxT("ml"));
    comboBox2->Append(wxT("cl"));
    comboBox2->Append(wxT("Löffel"));
    comboBox2->Append(wxT("Schuss"));
    comboBox2->Append(wxT("Prise"));
    comboBox2->Append(wxT("Stück"));

    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Zutat: ", wxDefaultPosition, wxSize(250, -1)), 0, wxALL, 5);
    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Menge: ", wxDefaultPosition, wxSize(120, -1)), 0, wxALL, 5);
    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Einheit: ", wxDefaultPosition, wxSize(80, -1)), 0, wxALL, 5);

    sizerChoice->Add(choice, 0, wxALL, 5);
    sizerChoice->Add(spinCtrl, 0, wxALL, 5);
    sizerChoice->Add(comboBox2, 0, wxALL, 5);
    
    mainSizer->Add(topSizer, 0, wxALL, 5);
    mainSizer->Add(sizerTitle, 0, wxALL, 5);
    mainSizer->Add(sizerChoice, 0, wxALL, 5);

    SetSizer(mainSizer);

}

void ThirdWindow::UpdateComboBox() {

    int rc = sqlite3_open("cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxT("Can't open database"));
        return;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT name FROM zutaten", -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxT("SQL error"));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 0);
        comboBox->Append(wxString::FromUTF8((const char*)name));
    }

    sqlite3_finalize(stmt);
}

void ThirdWindow::UpdateChoice() {

    int rc = sqlite3_open("cocktail_db.db", &db);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxT("Can't open database"));
        return;
    }

    sqlite3_stmt *stmt2;
    rc = sqlite3_prepare_v2(db, "SELECT name FROM zutaten", -1, &stmt2, NULL);

    if (rc != SQLITE_OK) {
        wxMessageBox(wxT("SQL error"));
        return;
    }

    while (sqlite3_step(stmt2) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt2, 0);
        choice->Append(wxString::FromUTF8((const char*)name));
    }

    sqlite3_finalize(stmt2);
}

ThirdWindow::~ThirdWindow() {
    //wxMessageBox("Destuctor called");
    sqlite3_close(db);
    this->Show(false);
}

