#include <wx/wx.h>
#include <wx/grid.h>
#include <sqlite3.h>
#include <wx/sizer.h>


class MainFrame : public wxFrame {

public:

    MainFrame(const wxString &title);
    void OnButtonClicked(wxCommandEvent& event);


private:

    wxButton *dbButton;
    wxGrid *dbGrid;
    wxBoxSizer *dbSizer;
    sqlite3 *db;


};

