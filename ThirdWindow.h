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
    
public:
    ThirdWindow(wxWindow *parent);
    sqlite3 *db;
    wxChoice *choice;
    wxComboBox *comboBox;
    
    ~ThirdWindow();
};
