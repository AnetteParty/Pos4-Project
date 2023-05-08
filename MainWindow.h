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

//class SecondWindow;

class MainWindow : public wxFrame {
	private:	
		void OnButtonClicked(wxCommandEvent& event);
		void OnCheckBox(wxCommandEvent& event);
		void UpdateListBox(const std::string &sql, wxListBox* listBox);
		
	public:
		// muss genau so Konstruiert werden! Jede kleine Abweichung führt sofort zu GTK crit errors!!!
		MainWindow(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = wxT(""), 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_FRAME_STYLE|wxSYSTEM_MENU|wxALWAYS_SHOW_SB|wxSIMPLE_BORDER|wxTAB_TRAVERSAL);

		wxButton *m_button4;

		wxCheckBox *checkBox;

		wxTextCtrl *textCtrl1;
    	wxTextCtrl *textCtrl2;
    	wxTextCtrl *textCtrl3;

		wxString name;
    	wxString description;
    	wxString ingredients;

		wxListBox *listBox;
		wxListBox *listBoxTwo;

		sqlite3 *db;

		//sqlite3_stmt *stmt;
    	sqlite3_stmt *stmt2;

		SecondWindow *secondWindow;
		
		~MainWindow();	
};


