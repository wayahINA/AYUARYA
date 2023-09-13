//---------------------------------------------------------------------------
#ifndef signonCPPH
#define signonCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <checklst.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TsignonFRM : public TForm
{
__published:	// IDE-managed Components
    TEdit *oldpassw;
    TEdit *newpassw;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TQuery *setup;
    TQuery *users;
    TDataSource *usersDS;
    TDataSource *setupDS;
    TEdit *passwCMB;
    TLabel *Label4;
    TDateTimePicker *stDT;
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TLabel *Label5;
    TEdit *dbTXT;
    TLabel *Label6;
    TDateTimePicker *lstDT;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall newpasswKeyPress(TObject *Sender, char &Key);
    void __fastcall oldpasswKeyPress(TObject *Sender, char &Key);
    void __fastcall passwCMBKeyPress(TObject *Sender, char &Key);
    
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        
    
    void __fastcall stDTExit(TObject *Sender);
    void __fastcall oldpasswExit(TObject *Sender);
    void __fastcall stDTKeyPress(TObject *Sender, char &Key);
    void __fastcall lstDTKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    TLocateOptions seek_;
    bool out_,pass_;
public:		// User declarations
    __fastcall TsignonFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TsignonFRM *signonFRM;
//---------------------------------------------------------------------------
#endif
