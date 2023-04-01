#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {

    
    MainFrame *mainFrame = new MainFrame("Cocktail Menu");
    mainFrame->Center();
    mainFrame->Show();
    mainFrame->SetClientSize(500, 500);
    //mainFrame->SetOwnBackgroundColour(wxBLUE);
    return true;

}