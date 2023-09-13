//---------------------------------------------------------------------------
#ifndef filterCPPH
#define filterCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <checklst.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TprtFRM : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TDBGrid *whGRD;
    TButton *Button1;
    TCheckBox *CheckBox1;
    TCheckListBox *CheckListBox1;
    TButton *Button2;
    TCheckBox *CheckBox2;
    TCheckListBox *CheckListBox2;
    TLabel *Label2;
    TDBGrid *deptGRD;
    TButton *Button3;
    TCheckBox *CheckBox3;
    TCheckListBox *CheckListBox3;
    TLabel *Label3;
    TDBGrid *DBGrid3;
    TButton *Button4;
    TCheckBox *CheckBox4;
    TCheckListBox *CheckListBox4;
    TPanel *bottPNL;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TQuery *tmpSQL;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
    int countr;
    TStringList *teksHTML,*teksEDIT,*teksTXT,*teksRPT;
    void TprtFRM::ctgryPRT(int &countr);
    void TprtFRM::dtlPRT(int &countr);
    void TprtFRM::tbOPEN();
    void TprtFRM::tbCLOSE();
    void TprtFRM::itemPRT();
public:		// User declarations
    AnsiString pathPRT,pathAPP_,pathHTML,pathRPT;
    void TprtFRM::validPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT);
    void TprtFRM::itemRPT();
    void TprtFRM::topPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT);
    void TprtFRM::botPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT);


    __fastcall TprtFRM(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TprtFRM *prtFRM;
//---------------------------------------------------------------------------
#endif
