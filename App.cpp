#include "App.h"
#include "MainFrame.h"
#include <sqlite3.h>
#include <wx/wx.h>

bool App::OnInit() {

    //wxSystemOptions::SetOption("gtk.no-gtk-cursor", "1");
    MainFrame *mainFrame = new MainFrame("Cocktail Menu");
    mainFrame->Center(true);
    mainFrame->Show(true);
    mainFrame->SetClientSize(800, 800);
    return true;

}
