//---------------------------------------------------------------------------
#ifndef passwCPPH
#define passwCPPH
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
//---------------------------------------------------------------------------
class TpasswFRM : public TForm
{
__published:	// IDE-managed Components
    TComboBox *passwCMB;
    TEdit *oldpassw;
    TEdit *newpassw;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TCheckListBox *cekVER;
    TQuery *setup;
    TQuery *users;
    TDataSource *usersDS;
    TDBGrid *DBGrid1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn6;
    TDataSource *setupDS;
        TFloatField *setuprecid;
        TStringField *setupidkey;
        TStringField *setupid1;
        TStringField *setupid2;
        TStringField *usersidkey;
        TStringField *usersid1;
        TStringField *usersid2;
        TStringField *usersid3;
        TStringField *usersid4;
        TStringField *usersMNURMRK;
        TFloatField *usersrecid;
    void __fastcall oldpasswKeyPress(TObject *Sender, char &Key);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall newpasswKeyPress(TObject *Sender, char &Key);
    void __fastcall setupAfterPost(TDataSet *DataSet);
    void __fastcall setupNewRecord(TDataSet *DataSet);
    void __fastcall usersNewRecord(TDataSet *DataSet);
    void __fastcall usersCalcFields(TDataSet *DataSet);
    void __fastcall passwCMBEnter(TObject *Sender);
    void __fastcall cekVEREnter(TObject *Sender);
    void __fastcall usersBeforeScroll(TDataSet *DataSet);
    void __fastcall usersAfterScroll(TDataSet *DataSet);
    void __fastcall BitBtn4Click(TObject *Sender);
    
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
        
private:	// User declarations
    TLocateOptions seek_;
    AnsiString oldVER;
public:		// User declarations
    __fastcall TpasswFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TpasswFRM *passwFRM;
//---------------------------------------------------------------------------
#endif
