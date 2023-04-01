#include <wx/wx.h>
#include <wx/listctrl.h>
#include <sqlite3.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxDEFINE_EVENT(LIST_UPDATED_EVENT, wxCommandEvent);

wxDECLARE_EVENT(LIST_UPDATED_EVENT, wxCommandEvent);

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnListUpdated(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    wxListCtrl *m_list;
};

enum {
    ID_Quit = 1
};

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("SQLite3 List Example", wxPoint(50, 50), wxSize(450, 350));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    m_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    m_list->InsertColumn(0, "ID");
    m_list->InsertColumn(1, "Name");
    m_list->InsertColumn(2, "Ingredients");

    // Connect the List Updated event to OnListUpdated method
    Connect(wxID_ANY, LIST_UPDATED_EVENT, wxCommandEventHandler(MyFrame::OnListUpdated));

    // Connect the Quit event to OnExit method
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(ID_Quit, "&Quit");
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);
    Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnExit));

    // Open the SQLite database and execute the query
    sqlite3 *db;
    sqlite3_open("database.db", &db);

    // Add 5 cocktails to the database
    sqlite3_exec(db, "INSERT INTO cocktails (name) VALUES ('Mojito');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO cocktails (name) VALUES ('Caipirinha');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO cocktails (name) VALUES ('Sex on the Beach');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO cocktails (name) VALUES ('Cosmopolitan');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO cocktails (name) VALUES ('Margarita');", NULL, NULL, NULL);

    wxCommandEvent event(LIST_UPDATED_EVENT);

    wxListItem item;
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM cocktails", -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        item.SetId(m_list->GetItemCount());
        item.SetText(wxString::Format("%d", sqlite3_column_int(stmt, 0)));
        m_list->InsertItem(item);

        m_list->SetItem(item.GetId(), 1, wxString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));

        // Get the ingredients for the cocktail
        wxString ingredients;
        sqlite3_stmt *stmt2;
        wxString query = "SELECT name FROM ingredients WHERE id IN (SELECT ingredient_id FROM cocktail_ingredients WHERE cocktail_id = " + wxString::Format("%d", sqlite3_column_int(stmt, 0)) + ")";
        sqlite3_prepare_v2(db, query, -1, &stmt2, NULL);
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
        if (!ingredients.empty()) {
        ingredients += ", ";
        }
        ingredients += wxString(reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 0)));
        }
            m_list->SetItem(item.GetId(), 2, ingredients);
        }

        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
        }

        void MyFrame::OnListUpdated(wxCommandEvent& event) {
        // Redraw the list
        m_list->Refresh();
        }

        void MyFrame::OnExit(wxCommandEvent& event) {
        Close(true);
        }

        wxIMPLEMENT_APP(MyApp);

