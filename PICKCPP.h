//---------------------------------------------------------------------------
#ifndef pickcppH
#define pickcppH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class Tpickform : public TForm
{
__published:	// IDE-managed Components
    TQuery *Query1;
    TDataSource *DataSource1;
    TDBGrid *DBGrid1;
    TButton *Button3;
    TButton *Button1;
    TButton *Button2;
    TEdit *findEDT;
    TLabel *Label1;
    TScrollBox *ScrollBox1;
    TStaticText *StaticText1;
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall findEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    
    void __fastcall Button1Click(TObject *Sender);
    
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    void Tpickform::seekGO();
public:		// User declarations
    __fastcall Tpickform(TComponent* Owner);
    void Tpickform::siapkan(AnsiString dbname,AnsiString tbname);
    AnsiString sqlasli;
    bool adalah;
};
//---------------------------------------------------------------------------
extern PACKAGE Tpickform *pickform;
//---------------------------------------------------------------------------
#endif
