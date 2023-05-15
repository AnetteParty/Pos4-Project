#include <wx/spinctrl.h>
#include <wx/combobox.h>

#include "MainWindow.h"
#include "ThirdWindow.h"

ThirdWindow::ThirdWindow(wxWindow *parent) : wxFrame(parent, wxID_ANY, "Add Cocktail", wxDefaultPosition, wxSize(1000, 800)) {
    choice = new wxChoice(this, wxID_ANY);
    UpdateChoice();

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerTitle = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *zubereitungSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *topAndListSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *listCtrlSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *displaySizer = new wxBoxSizer(wxVERTICAL);
    
    wxTextCtrl *nameTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
    wxTextCtrl *zubereitungTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 110), wxTE_MULTILINE|wxTE_BESTWRAP);

    wxButton *addButton = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxSize(70, -1));
    wxButton *addButton2 = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxSize(70, -1));
    wxButton *addButtonPerma = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxSize(70, -1));
    wxButton *addToDbButton = new wxButton(this, wxID_ANY, "Save", wxDefaultPosition, wxSize(70, -1));
    wxButton *myButtonClose = new wxButton(this, wxID_ANY, "Close", wxPoint(910, 760), wxDefaultSize);
    
    wxSpinCtrl *spinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS);
    wxComboBox *comboBox2 = new wxComboBox(this, wxID_ANY);

    wxStaticText *nameStaticText = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
    wxStaticText *zubereitungStaticText = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 110), wxTE_MULTILINE|wxTE_BESTWRAP);
    wxStaticText *displayStaticText = new wxStaticText(this, wxID_ANY, "");

    nameSizer->Add(nameTextCtrl, 1, wxALL|wxEXPAND, 5);
    nameSizer->Add(addButton, 0, wxALL , 5);
    nameSizer->Add(addToDbButton, 0, wxALL , 5);
    
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Name: "), 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL , 5);
    topSizer->Add(nameSizer, 0, wxALL, 5);

    zubereitungSizer->Add(zubereitungTextCtrl, 1, wxALL|wxEXPAND, 5);
    zubereitungSizer->Add(addButton2, 0, wxALL, 5);

    topSizer->Add(new wxStaticText(this, wxID_ANY, "Zubereitung: "), 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    topSizer->Add(zubereitungSizer, 0, wxALL, 5);  

    myButtonClose->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ThirdWindow::OnButtonCloseClicked, this);

    addButton->Bind(wxEVT_BUTTON, [nameTextCtrl, nameStaticText, addButton](wxCommandEvent&){ 
        nameTextCtrl->SetEditable(false); 
        nameTextCtrl->SetBackgroundColour(wxColour(232,232,232)); 
        nameStaticText->SetLabel(nameTextCtrl->GetValue());
        addButton->Hide();
    }); 
        
    addButton2->Bind(wxEVT_BUTTON, [zubereitungTextCtrl, zubereitungStaticText, addButton2](wxCommandEvent&){ 
        zubereitungTextCtrl->SetEditable(false); 
        zubereitungTextCtrl->SetBackgroundColour(wxColour(232,232,232)); 
        zubereitungStaticText->SetLabel(zubereitungTextCtrl->GetValue());
        addButton2->Hide();
    });

    addButtonPerma->Bind(wxEVT_BUTTON, [zubereitungTextCtrl, zubereitungStaticText, addButtonPerma](wxCommandEvent&){ 
        zubereitungTextCtrl->SetEditable(false); 
        zubereitungTextCtrl->SetBackgroundColour(wxColour(232,232,232)); 
        zubereitungStaticText->SetLabel(zubereitungTextCtrl->GetValue());
    });

    addButtonPerma->Bind(wxEVT_BUTTON, [this, comboBox2, spinCtrl, displayStaticText](wxCommandEvent&){
        wxString text = 
        wxString::Format("%d", spinCtrl->GetValue()) + " " + 
        comboBox2->GetValue() + " " + choice->GetStringSelection();
        wxString currentText = displayStaticText->GetLabel();
        if (!currentText.IsEmpty()) {
            currentText += "\n";
        }
        currentText += text;
        displayStaticText->SetLabel(currentText);
    });

    addToDbButton->Bind(wxEVT_BUTTON, [this,nameTextCtrl,zubereitungTextCtrl,displayStaticText,addButton2,addButton](wxCommandEvent&) {

        if (db == nullptr) {
            std::cerr << "Error: Database connection is null" << std::endl;
            return;
        }

        wxString name = nameTextCtrl->GetValue();
        std::cout << "schreibe fuer name: " << name << std::endl;
        wxString beschreibung = zubereitungTextCtrl->GetValue();
        std::cout << "schreibe fuer beschreibung: " << beschreibung << std::endl;

        // statt name.str() testen mit name.utf8_str()
        wxString sql = wxString::Format("INSERT INTO cocktail (name, beschreibung) VALUES ('%s', '%s')", name.utf8_str(), beschreibung.utf8_str());

        char *errMsg = nullptr;
        // int rc = sqlite3_exec(db, sql.str(), NULL, NULL, NULL);
        int rc = sqlite3_exec(db, sql.utf8_str(), nullptr, nullptr, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "Error: Failed to insert data into cocktail table: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return;
        }

        // gibt die zuletzt eingefügte row id, um in cocktail_list immer die selbe cocktail_id zu haben
        int cocktail_id = sqlite3_last_insert_rowid(db);

        // displayStaticText beinhaltet die concatenated strings im 
        // Format("%d", spinCtrl->GetValue()) + " " + comboBox2->GetValue() + " " + choice->GetStringSelection();
        wxString currentText = displayStaticText->GetLabel();
        // die abschnitte pro Eintrag sind durch newlines getrennt
        wxArrayString lines = wxSplit(currentText, '\n');

        for (size_t i = 0; i < lines.GetCount(); i++) {
            
            wxString line = lines[i];
            std::cout << "in der " << i << "ten Zeile steht: " << line << std::endl;
            wxArrayString parts = wxSplit(line, ' ');
            
            if (parts.GetCount() != 3) {
                continue;
            }
            int menge = wxAtoi(parts[0]);
            wxString einheit = parts[1];
            wxString zutat = parts[2];

            sql = wxString::Format("INSERT INTO cocktail_list (cocktail_id, zutat_id, menge, einheit) SELECT %d, id, %d, '%s' FROM zutaten WHERE name='%s'", cocktail_id, menge, einheit.utf8_str(), zutat.utf8_str());
            rc = sqlite3_exec(db, sql.utf8_str(), nullptr, nullptr, &errMsg);

            if (rc != SQLITE_OK) {
                std::cerr << "fehler in cocktail_list table= " << errMsg << std::endl;
                sqlite3_free(errMsg);
                return;
            }
        }
        
        sqlite3_close(db);
        wxMessageBox("Cocktail added to Db");
        this->Show(false); 
    });

    comboBox2->Append(wxT("Liter"));
    comboBox2->Append(wxT("ml"));
    comboBox2->Append(wxT("cl"));
    comboBox2->Append(wxT("Loeffel"));
    comboBox2->Append(wxT("Schuss"));
    comboBox2->Append(wxT("Prise"));
    comboBox2->Append(wxT("Stueck"));

    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Zutat: ", wxDefaultPosition, wxSize(250, -1)), 0, wxALL, 5); 
    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Menge: ", wxDefaultPosition, wxSize(120, -1)), 0, wxALL, 5); 
    sizerTitle->Add(new wxStaticText(this, wxID_ANY, "Einheit: ", wxDefaultPosition, wxSize(80, -1)), 0, wxALL, 5);

    bottomSizer->Add(choice, 0, wxALL, 5);
    bottomSizer->Add(spinCtrl, 0, wxALL, 5);
    bottomSizer->Add(comboBox2, 0, wxALL, 5);
    bottomSizer->Add(addButtonPerma, 0, wxALL, 5);

    displaySizer->Add(nameStaticText, 0, wxALL, 5);
    displaySizer->Add(zubereitungStaticText, 0, wxALL, 5);
    displaySizer->Add(displayStaticText, 0, wxALL, 5);
    
    // Sizer Nightmare //
    topAndListSizer->Add(topSizer); // zubereitungSizer + nameSizer -> topSizer(wxVERTICAL)
    //topAndListSizer->Add(listCtrlSizer, 0, wxLEFT, 80); // listCtrlSizer(wxVERTICAL) nur listCtrl

    mainSizer->Add(topAndListSizer, 0, wxLEFT, 15); // topAndListSizer(wxHORIZONTAL)
    mainSizer->Add(sizerTitle, 0, wxLEFT, 15);
    mainSizer->Add(bottomSizer, 0, wxLEFT, 15); // choice + spinCtrl + comboBox2  bottomSizer(wxHORIZONTAL)
    mainSizer->Add(displaySizer, 0, wxALL, 50);

    SetSizer(mainSizer);
}

void ThirdWindow::UpdateComboBox() {

    int rc = sqlite3_open("/mnt/c/Users/foxfo/Desktop/Pos4-Project/cocktail_db.db", &db);

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

    int rc = sqlite3_open("/mnt/c/Users/foxfo/Desktop/Pos4-Project/cocktail_db.db", &db);

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

void ThirdWindow::OnButtonCloseClicked(wxCommandEvent& event) {
    // leider unmöglich wäre aber die Lösung
    //MainWindow::UpdateList();

    this->Show(false);
    this->Close();
}

ThirdWindow::~ThirdWindow() {}

