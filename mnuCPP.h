//---------------------------------------------------------------------------
#ifndef mnuCPPH
#define mnuCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TmnuFRM : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TImage *lgoIMG;
    TStaticText *StaticText1;
    TRadioGroup *mnuGRP;
    TButton *goBTN;
    TButton *Button4;
    TMainMenu *MainMenu1;
    TMenuItem *Keluar1;
    TMenuItem *Kami1;
    TMenuItem *Panduan1;
    TQuery *dummy;
    TTable *dummyTB;
    TBatchMove *copyin;
    TDataSource *dummyDS;
    TQuery *setupQRY;
    TOpenDialog *dataku;
    TQuery *deptSQL;
    TQuery *divSQL;
    TQuery *Query1;
    TTable *dtlSQL;
    TTable *reqSQL;
    TMenuItem *Maintenance1;
    TMenuItem *ArsipFile1;
    TMenuItem *FolderLaporan1;
    TMenuItem *PerbaikiDatabase1;
    TMenuItem *PotongData1;
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Panduan1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall Keluar1Click(TObject *Sender);
    void __fastcall goBTNClick(TObject *Sender);
    
    void __fastcall deptSQLAfterPost(TDataSet *DataSet);
    void __fastcall divSQLAfterPost(TDataSet *DataSet);
    void __fastcall ArsipFile1Click(TObject *Sender);
    void __fastcall FolderLaporan1Click(TObject *Sender);
    void __fastcall PerbaikiDatabase1Click(TObject *Sender);
    void __fastcall PotongData1Click(TObject *Sender);
private:	// User declarations
    AnsiString ttl;
    bool TmnuFRM::packDBF(AnsiString tbNM);
    void TmnuFRM::tbCUT2();
    void TmnuFRM::chgREQ();
    void TmnuFRM::chgQRY();
    void TmnuFRM::goCHG(TQuery *tempQRY,TSearchRec hasil_,AnsiString SQLDir);
    void TmnuFRM::maintain();
    AnsiString TmnuFRM::seekTB(TDataSet *mySQL,AnsiString fldNM,AnsiString seekSTR,AnsiString retNM);
    void TmnuFRM::openTB();
    void TmnuFRM::closeTB();
    void TmnuFRM::uploadPR();
    void TmnuFRM::bdeINST();
    void TmnuFRM::other();
    void TmnuFRM::login();
    void TmnuFRM::menuCREATE();
    void TmnuFRM::tbCREATE();
    void TmnuFRM::createSTRUCT();
    void TmnuFRM::setVAR();
public:		// User declarations
    TDate stDT,lstDT;
    AnsiString user_;
    TStringList *whCMB,*whTYPE;
    TStringList *varPAS,*varMNU,*varDES;
    bool loged,pass_;
    AnsiString path_,pathAPP_;
    TStringList *sqlSTRUCT,*tbSTRUCT;
    TStringList *ivTYPE,*cmbTYPE,*ivSTATUS,*cmbSTATUS,*gdSTATUS,*gdCMB;
    __fastcall TmnuFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TmnuFRM *mnuFRM;
//---------------------------------------------------------------------------
#endif
