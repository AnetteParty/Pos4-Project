// SecondWindow.h
#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <wx/wx.h>
#include <sqlite3.h>
#include <wx/grid.h>
#include <wx/listctrl.h>
#include <wx/frame.h>

#include "MainWindow.h"

//class MainWindow;
class SecondWindow : public wxFrame {

private:
    
    
public:
    SecondWindow(wxWindow *parent);

    //MainWindow(wxWindow *mainWin);
    //std::shared_ptr<MainWindow> mainWin;

    wxButton *buttonAdd;
    wxButton *buttonDelete;
    wxButton *buttonClose;

    wxListCtrl *listCtrl;
    wxTextCtrl *textCtrl1;
    wxTextCtrl *textCtrl2;
    wxTextCtrl *textCtrl3;

    void UpdateList();
    void OnButtonAddClicked(wxCommandEvent& event);
    void OnButtonDeleteClicked(wxCommandEvent& event);
    void OnButtonCloseClicked(wxCommandEvent& event);
    void OnTextCtrlFocus(wxFocusEvent& event);
    
    ~SecondWindow();
};
