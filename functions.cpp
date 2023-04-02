#include "MainFrame.h"
#include <wx/wx.h>
#include <sqlite3.h>
#include <wx/grid.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {


    
    wxPanel *panel = new wxPanel(this);

    wxButton *myButton = new wxButton(panel, wxID_ANY, "click me", wxPoint(100+100, 50+50), wxSize(150,50));
    
    wxCheckBox *myCheckBox = new wxCheckBox(panel, wxID_ANY, "check me", wxPoint(450,10));

    wxStaticText *myStaticText = new wxStaticText(panel, wxID_ANY, "cant touch this!", wxPoint(200,200));

    wxTextCtrl *myTextCtrl = new wxTextCtrl(panel, wxID_ANY, "change me!", wxPoint(200,250), wxSize(200,-1));


    wxGrid *grid = new wxGrid(panel, wxID_ANY, wxPoint(200,500), wxSize(200, -1));


        grid->CreateGrid(5, 3);                 // Erstellt ein Grid mit 5 Zeilen und 3 Spalten
        grid->SetColLabelValue(0, "Name"); 
        grid->SetColLabelValue(1, "Alter");
        grid->SetColLabelValue(2, "Hobby"); 
        grid->SetCellValue(0, 0, "Max"); 
        grid->SetCellValue(0, 1, "25"); 
        grid->SetCellValue(0, 2, "Schwimmen"); 
        grid->AutoSizeColumns(true);            // Passt die Spaltenbreite an den Inhalt an
        grid->AutoSizeRows(true);               // Passt die Zeilenhöhe an den Inhalt an






    
    // wxSqlite3 functions
    // --------------------
    sqlite3 *db;
    sqlite3_stmt *stmt;

    SQLITE_API int sqlite3_prepare_v2 (
        sqlite3 *db,                  /* Ein Zeiger auf die geöffnete SQLite-Datenbank, auf die die SQL-Anweisung angewendet werden soll. */
        const char *zSql,             /* Die SQL-Anweisung als eine Zeichenfolge. Diese Anweisung kann eine einzelne Anweisung oder eine Gruppe von Anweisungen sein, die durch Semikolons getrennt sind. */
        int nBytes,                   /* Die Länge der SQL-Anweisung als eine Ganzzahl. Wenn die Länge nicht bekannt ist oder die Anweisung nullterminiert ist, kann dieser Wert auf -1 gesetzt werden. */
        sqlite3_stmt **ppStmt,        /* Ein Zeiger auf eine Variable, in der SQLite einen Zeiger auf den vorbereiteten Anweisung speichert. Diese Variable wird verwendet, um später auf den vorbereiteten Anweisung zuzugreifen. */
        const char **pzTail           /* Ein Zeiger auf eine Variable, in der SQLite den nicht ausgeführten Rest der SQL-Anweisung speichert. Diese Variable kann verwendet werden, um zu überprüfen, ob die gesamte Anweisung erfolgreich ausgeführt wurde. */
    );
        
    sqlite3_prepare_v2(db,"CREATE TABLE Cocktails (id INTEGER PRIMARY KEY,name TEXT,description TEXT,price REAL)", -1, &stmt, NULL);
    

    SQLITE_API int sqlite3_exec (
            sqlite3* db,              /* Ein Zeiger auf die geöffnete SQLite-Datenbank, auf die die SQL-Anweisungen angewendet werden sollen. */
            const char* sql,          /* Die SQL-Anweisung als eine Zeichenfolge. Diese Anweisung kann eine einzelne Anweisung oder eine Gruppe von Anweisungen sein, die durch Semikolons getrennt sind. */
            int (*callback)(          /* Der Callback-Funktion, die aufgerufen wird */
                void*,                /* Parameter, die an die Callback-Funktion übergeben werden */
                int,                  /* Anzahl der Spalten im Ergebnis */
                char**,               /* Array von Zeigern auf Ergebniszeilen */
                char**                /* Array von Zeigern auf Spaltennamen */
            ),
            void* data,               /* Ein optionaler Parameter, der an die Callback-Funktion übergeben wird. Dieser Parameter kann verwendet werden, um zusätzliche Daten an die Callback-Funktion zu übergeben. */
            char** errmsg             /* Ein Zeiger auf eine Zeichenfolge, in der SQLite eine Fehlermeldung speichern kann, falls ein Fehler auftritt. Wenn die Anweisung erfolgreich ausgeführt wird, bleibt diese Variable unverändert. */
    );
    
    sqlite3_exec(db,"CREATE TABLE Cocktails (id INTEGER PRIMARY KEY,name TEXT,price REAL)", NULL, NULL, NULL);



}