// MainWindow.h
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

#include "SecondWindow.h"
#include "ThirdWindow.h"

//class SecondWindow;
class MainWindow : public wxFrame {
	private:	
		void OnButtonClicked(wxCommandEvent &event);
		void OnButtonAddClicked(wxCommandEvent &event);
		void OnButtonCloseClicked(wxCommandEvent &event);
		void OnButtonDeleteClicked(wxCommandEvent &event);
		void OnButtonRefreshClicked(wxCommandEvent& event);
		void UpdateList();
		
	public:
		MainWindow(wxWindow *parent);

		wxListCtrl *listCtrl;
		wxCheckBox *checkBox;

		wxListBox *listBox;
		wxListBox *listBoxTwo;

		sqlite3 *db;

		sqlite3_stmt *stmt;

		SecondWindow *secondWindow;
		ThirdWindow *thirdWindow;
		
		~MainWindow();	
};


