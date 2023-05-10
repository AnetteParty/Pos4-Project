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
    void UpdateListBox(const std::string &sql,wxListBox *listBox);
    void UpdateList();
    void OnCheckBox(wxCommandEvent &event);
    void OnButtonAddZutatClicked(wxCommandEvent& event);
    void OnButtonDeleteClicked(wxCommandEvent& event);
    void OnButtonCloseClicked(wxCommandEvent& event);
    void OnTextCtrlFocus(wxFocusEvent& event);
    //void UpdateListBox(const std::string &sql, wxListBox *listBox);
    
public:
    SecondWindow(wxWindow *parent);

    sqlite3 *db;

    wxListCtrl *listCtrl;
	wxCheckBox *checkBox;

	wxListBox *listBox;
	wxListBox *listBoxTwo;

    wxButton *buttonAdd;
    wxButton *buttonDelete;
    wxButton *buttonClose;

    wxTextCtrl *textCtrl1;
    wxTextCtrl *textCtrl2;
    wxTextCtrl *textCtrl3;

    ~SecondWindow();
};
