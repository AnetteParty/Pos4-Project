#include <wx/wx.h>
#include <wx/grid.h>
#include <sqlite3.h>
#include <wx/sizer.h>


class MainFrame : public wxFrame {

public:

    MainFrame(const wxString &title);
    void OnButtonClicked(wxCommandEvent& event);
    void OnButton2Clicked(wxCommandEvent& event);
    void OnDeleteButtonClicked(wxCommandEvent& event);
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);


private:

    wxButton *dbButton;
    wxButton *dbButton2;
    wxButton *dbButtonDel;

    wxGrid *dbGrid;
    wxBoxSizer *dbSizer;
    sqlite3 *db;

};

