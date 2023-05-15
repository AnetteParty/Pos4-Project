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
#include <wx/combobox.h>
#include <wx/spinctrl.h>
#include <wx/frame.h>

#include "MainWindow.h"

//class MainWindow;
class ThirdWindow : public wxFrame {

private:
    void UpdateChoice();
    void UpdateComboBox();
    void OnButtonCloseClicked(wxCommandEvent &event);

public:
    ThirdWindow(wxWindow *parent);

    wxChoice *choice;
    sqlite3 *db;
    wxComboBox *comboBox;

    ~ThirdWindow();
};
