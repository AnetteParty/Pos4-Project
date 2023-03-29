#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
        wxPanel* panel = new wxPanel(this);
        
        // Textfeld erstellen, in dem der Button-Text angezeigt wird
        m_textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(20, 300), wxSize(200, 30));
        
        // Ersten 9 Buttons hinzufügen
        for (int i = 0; i < 9; i++) {
            wxButton* button = new wxButton(panel, wxID_ANY, wxString::Format("%d", i+1), 
                wxPoint((i%3)*50 + 50, (i/3)*50 + 50), wxSize(50, 50));
                
            // Event-Handler für jeden Button hinzufügen
            button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        }
        
        // Weitere Buttons hinzufügen
        wxButton* addButton = new wxButton(panel, wxID_ANY, "+", wxPoint(20, 200), wxSize(50, 50));
        addButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        
        wxButton* subtractButton = new wxButton(panel, wxID_ANY, "-", wxPoint(80, 200), wxSize(50, 50));
        subtractButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        
        wxButton* multiplyButton = new wxButton(panel, wxID_ANY, "*", wxPoint(140, 200), wxSize(50, 50));
        multiplyButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        
        wxButton* divideButton = new wxButton(panel, wxID_ANY, "/", wxPoint(200, 200), wxSize(50, 50));
        divideButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        
        wxButton* equalsButton = new wxButton(panel, wxID_ANY, "=", wxPoint(260, 200), wxSize(50, 50));
        equalsButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        
        wxButton* clearButton = new wxButton(panel, wxID_ANY, "C", wxPoint(320, 200), wxSize(50, 50));
        clearButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
    }
    
    void OnButtonClicked(wxCommandEvent& event) {
        // Button-Text im Textfeld anzeigen
        wxButton* button = (wxButton*)event.GetEventObject();
        wxString buttonText = button->GetLabel();
        
        // Nur Zahlen im Textfeld anzeigen
        if (buttonText.IsNumber()) {
            m_textCtrl->SetValue(buttonText);
        }
    }
    
private:
    wxTextCtrl* m_textCtrl;
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("Buttons Example");
        frame->Show(true);
        frame->SetClientSize(400, 400);
        frame->Center();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
