//---------------------------------------------------------------------------
#ifndef seekCPPH
#define seekCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TseekprFRM : public TForm
{
__published:	// IDE-managed Components
    TPanel *topPNL;
    TLabel *Label1;
    TComboBox *fndCMB;
    TEdit *fndEDT;
    TPanel *bottPNL;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TPanel *midPNL;
    TLabel *Label4;
    TDBNavigator *catNAV;
    TEdit *recEDT;
    TPageControl *reqPG;
    TTabSheet *reqSHEET;
    TTabSheet *poSHEET;
    TDBGrid *prGRD;
    TDBGrid *DBGrid1;
    TCheckBox *seekCHK;
    TEdit *freeEDT;
    TBitBtn *prtBTN;
    TBitBtn *BitBtn1;
    TTabSheet *doSHEET;
    TDBGrid *DBGrid2;
    TQuery *prSQL;
    TDataSource *prDS;
    TQuery *poSQL;
    TDataSource *poDS;
    TQuery *doSQL;
    TDataSource *doDS;
    TQuery *deptSQL;
    TQuery *divSQL;
    TStringField *prSQLreqno;
    TDateField *prSQLreqdt;
    TFloatField *prSQLreq;
    TStringField *prSQLuom;
    TStringField *prSQLcur;
    TFloatField *prSQLprice;
    TStringField *prSQLgdnm;
    TStringField *prSQLdeptnm;
    TStringField *prSQLdivnm;
    TFloatField *prSQLreqdept;
    TFloatField *prSQLreqdiv;
    TCheckBox *outCHK;
    TLabel *Label2;
    TLabel *Label3;
    TDateTimePicker *stDT;
    TDateTimePicker *lstDT;
    TStringField *poSQLrefno;
    TStringField *poSQLreqno;
    TDateField *poSQLtrfdt;
    TFloatField *poSQLreq;
    TStringField *poSQLcur;
    TFloatField *poSQLprice;
    TStringField *poSQLgdnm;
    TStringField *poSQLuom;
    TStringField *doSQLref;
    TStringField *doSQLreq;
    TDateField *doSQLrefdt;
    TFloatField *doSQLdb;
    TStringField *doSQLgdnm;
    TStringField *doSQLunit;
    TCheckBox *exact;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall freeEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall reqPGChange(TObject *Sender);
    void __fastcall prtBTNClick(TObject *Sender);
private:	// User declarations
    //var here
    AnsiString raw;
    AnsiString pathAPP_,pathPRT,pathHTML,pathRPT;
    AnsiString PERIOD;
    TStringList *teksHTML,*teksEDIT,*teksTXT,*teksRPT;
    //prosedur here
    void TseekprFRM::prtPR(TDataSet *mySQL);
    bool TseekprFRM::samedept(AnsiString mydept,TDataSet *mySQL );
    void TseekprFRM::prtRAW(TDataSet *mySQL);
    void TseekprFRM::prRPT();
    void TseekprFRM::doOPEN();
    void TseekprFRM::poOPEN();
    void TseekprFRM::reqOPEN();
    void TseekprFRM::tbOPEN();
    void TseekprFRM::tbCLOSE();
    void TseekprFRM::showEM(TQuery *mySQL);
public:		// User declarations
    __fastcall TseekprFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TseekprFRM *seekprFRM;
//---------------------------------------------------------------------------
#endif
