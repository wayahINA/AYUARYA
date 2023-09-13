//---------------------------------------------------------------------------
#ifndef vdCPPH
#define vdCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TvdFRM : public TForm
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
    TPageControl *srPG;
    TTabSheet *detSHEET;
    TScrollBox *idSCROLL;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TSpeedButton *mailBTN;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TDBEdit *idEDT;
    TDBEdit *nmEDT;
    TDBEdit *contEDT;
    TDBEdit *st1EDT;
    TDBEdit *st2EDT;
    TDBEdit *phEDT;
    TDBEdit *fxEDT;
    TDBEdit *mailEDT;
    TTabSheet *lstSHEET;
    TDBGrid *paGRD;
    TDBNavigator *srNAV;
    TEdit *recEDT;
    TQuery *tmpSQL;
    TQuery *vdSQL;
    TDataSource *vdDS;
    TComboBox *ctyCMB;
    TComboBox *provCMB;
    TComboBox *cntryCMB;
    TLabel *Label2;
    TDBEdit *gmEDT;
    TLabel *Label3;
    TDBEdit *ttlEDT;
    TTabSheet *itemSHEET;
    TLabel *Label5;
    TComboBox *ctgryCMB;
    TShape *Shape1;
    TDBGrid *itemGRD;
    TQuery *itemSQL;
    TDataSource *itemDS;
    TQuery *gdSQL;
    TStringField *itemSQLnm;
    TLabel *vdTEXT;
    TFloatField *itemSQLgd;
    TFloatField *itemSQLvd;
    TDateField *itemSQLlast;
    TFloatField *itemSQLprice;
    TStringField *itemSQLid;
    TBitBtn *addBTN;
    TLabel *Label17;
    TDBEdit *workEDT;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall vdSQLAfterPost(TDataSet *DataSet);
    void __fastcall srPGEnter(TObject *Sender);
    void __fastcall itemSQLNewRecord(TDataSet *DataSet);

    void __fastcall idEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall nmEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall contEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall ttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall gmEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall st1EDTKeyPress(TObject *Sender, char &Key);
    void __fastcall st2EDTKeyPress(TObject *Sender, char &Key);
    void __fastcall phEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall fxEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall mailEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall ctyCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall provCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall cntryCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall itemGRDColEnter(TObject *Sender);
    void __fastcall vdSQLAfterScroll(TDataSet *DataSet);
    void __fastcall itemSQLAfterScroll(TDataSet *DataSet);
    void __fastcall itemSQLBeforeClose(TDataSet *DataSet);
    void __fastcall vdSQLBeforeClose(TDataSet *DataSet);
    void __fastcall ctyCMBExit(TObject *Sender);
    void __fastcall provCMBExit(TObject *Sender);
    void __fastcall cntryCMBExit(TObject *Sender);
    void __fastcall addBTNClick(TObject *Sender);
    void __fastcall ctgryCMBChange(TObject *Sender);
    void __fastcall itemGRDColExit(TObject *Sender);
    
private:	// User declarations
    AnsiString itemORG;
    bool ctgryCHG;
    TLocateOptions seek;
    AnsiString gdORG;
    void TvdFRM::idCHECK();
    void TvdFRM::gdOPEN();
    void TvdFRM::ctgrySET();
    void TvdFRM::idSAY();
    void TvdFRM::idFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM);
    void TvdFRM::recSAY(TDataSet *mySQL);
    void TvdFRM::setCMB();
    TStringList *ctyVAL,*provVAL,*cntryVAL,*ctgryVAL;
    void TvdFRM::vdPOST();
    void TvdFRM::itemPOST();
    void TvdFRM::key13(TObject *Sender, char &Key);
    TStringList *fndVAL;
    void TvdFRM::vdDEL();
    void TvdFRM::itemDEL();
    void TvdFRM::itemNEW();
    void TvdFRM::itemOPEN();
    void TvdFRM::tbOPEN();
    void TvdFRM::tbCLOSE();
public:		// User declarations
    __fastcall TvdFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TvdFRM *vdFRM;
//---------------------------------------------------------------------------
#endif
