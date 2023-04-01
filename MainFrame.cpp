#include "MainFrame.h"
#include <wx/wx.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {

    //wxColour *myColor = new wxColour(wxBLUE);

    wxPanel *panel = new wxPanel(this);

    wxButton *myButton = new wxButton(panel, wxID_ANY, "DisplayDb", wxPoint(100+100, 50+50), wxSize(150,50));
    
    wxCheckBox *myCheckBox = new wxCheckBox(panel, wxID_ANY, "checkMe", wxPoint(450,10));

    wxStaticText *myStaticText = new wxStaticText(panel, wxID_ANY, "cant touch this!", wxPoint(200,200));

    wxTextCtrl *myTextCtrl = new wxTextCtrl(panel, wxID_ANY, "change me!", wxPoint(200,250), wxSize(200,-1));
}