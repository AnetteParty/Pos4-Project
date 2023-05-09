#include "MainWindow.h"
#include "SecondWindow.h"


class MyApp : public wxApp {

public:
        
    bool OnInit();      
};

bool MyApp::OnInit() {
    // muss genau so definiert werden, alle Abweichungen sofort -> GTK crit error
    MainWindow *mainWindow = new MainWindow(NULL, wxID_ANY, "Cocktail Menu", wxDefaultPosition, wxSize(800,800), wxDEFAULT_FRAME_STYLE);
    mainWindow->Center(true);
    mainWindow->Show(true);
    return true;

}

wxIMPLEMENT_APP(MyApp);

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
: wxFrame(parent, id, title, pos, size, style) {

    secondWindow = new SecondWindow(this);
    thirdWindow = new ThirdWindow(this);

    wxButton *myButton = new wxButton(this, wxID_ANY, "Open DB", wxPoint(350, 350));
    wxButton *myButton2 = new wxButton(this, wxID_ANY, "Zutaten", wxPoint(400, 350));

    wxBoxSizer *sizerOne = new wxBoxSizer(wxVERTICAL);

    myButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButtonClicked), NULL, this);
    myButton2->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButton2Clicked), NULL, this);

    listBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(300, 300), 0, NULL, wxLB_SINGLE);
    listBoxTwo = new wxListBox(this, wxID_ANY, wxPoint(310, 0), wxSize(300, 300), 0, NULL, wxLB_SINGLE);
    listBox->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    checkBox = new wxCheckBox(this, wxID_ANY, "mit Alkohol");
    checkBox->Bind(wxEVT_CHECKBOX, &MainWindow::OnCheckBox, this);

    sizerOne->Add(listBox, 0, wxALL, 5);
    sizerOne->Add(checkBox, 0, wxALL, 5);
    sizerOne->Add(myButton, 0, wxALL, 5);
    sizerOne->Add(myButton2, 0, wxALL, 5);
   
    UpdateListBox("SELECT name FROM zutaten;", listBox);
    UpdateListBox("SELECT name FROM cocktail;", listBoxTwo);
    SetSizer(sizerOne);

    // zum inhalt an sizer anpassen
    //Fit();
}

void MainWindow::OnCheckBox(wxCommandEvent &event) {

    if (checkBox->IsChecked()) {
        UpdateListBox("SELECT name FROM zutaten WHERE alkoholhaltig = 1;", listBox);
    } else {
        UpdateListBox("SELECT name FROM zutaten WHERE alkoholhaltig = 0;", listBox);
    }
}


void MainWindow::OnButtonClicked(wxCommandEvent &event) {

    secondWindow->Show(true);
}

void MainWindow::OnButton2Clicked(wxCommandEvent &event) {

    thirdWindow->Show(true);
}

void MainWindow::UpdateListBox(const std::string &sql,wxListBox *listBox) {
        listBox->Clear();

        sqlite3_stmt *stmt = nullptr;
        
        int rc = sqlite3_open("/mnt/c/Users/Robin/Desktop/Pos4-Project/cocktail_db.db", &db);

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
}


// destructor
MainWindow::~MainWindow() {
    sqlite3_close(db);
}

