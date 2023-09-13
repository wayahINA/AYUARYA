//---------------------------------------------------------------------------
#ifndef reqCPPH
#define reqCPPH
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
class TreqFRM : public TForm
{
__published:	// IDE-managed Components
    TPanel *topPNL;
    TLabel *Label1;
    TComboBox *fndCMB;
    TEdit *fndEDT;
    TCheckBox *sortIT;
    TPanel *bottPNL;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TPanel *midPNL;
    TDBNavigator *reqNAV;
    TBitBtn *addBTN;
    TPageControl *reqPG;
    TTabSheet *listSHEET;
    TDBGrid *listGRD;
    TTabSheet *reqSHEET;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label7;
    TLabel *Label5;
    TLabel *Label8;
    TLabel *Label10;
    TLabel *Label6;
    TLabel *Label9;
    TLabel *Label11;
    TShape *Shape1;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TDateTimePicker *reqDT;
    TDateTimePicker *needDT;
    TDBEdit *byEDT;
    TDBEdit *apEDT;
    TDBEdit *byttlEDT;
    TDBEdit *apttlEDT;
    TDBGrid *reqGRD;
    TDBEdit *jobEDT;
    TDBEdit *woEDT;
    TDBEdit *reqnoEDT;
    TComboBox *deptCMB;
    TComboBox *divCMB;
    TEdit *deptTEXT;
    TEdit *divTEXT;
    TTabSheet *noteSHEET;
    TDBMemo *noteMEMO;
    TBitBtn *prtBTN;
    TQuery *tmpSQL;
    TQuery *reqSQL;
    TStringField *reqSQLreqno;
    TDateField *reqSQLreqdt;
    TDateField *reqSQLneeded;
    TFloatField *reqSQLreqdept;
    TFloatField *reqSQLreqdiv;
    TStringField *reqSQLwono;
    TStringField *reqSQLjob;
    TStringField *reqSQLdescr;
    TStringField *reqSQLreqby;
    TStringField *reqSQLreqttl;
    TStringField *reqSQLaprby;
    TStringField *reqSQLaprttl;
    TStringField *reqSQLnote;
    TStringField *reqSQLstate;
    TFloatField *reqSQLmode;
    TStringField *reqSQLdivnm;
    TFloatField *reqSQLtrfrom;
    TFloatField *reqSQLwhfrom;
    TDateField *reqSQLtrfdt;
    TStringField *reqSQLdeptnm;
    TDataSource *reqDS;
    TQuery *dtlSQL;
    TStringField *dtlSQLreqno;
    TStringField *dtlSQLrefno;
    TStringField *dtlSQLbatch;
    TFloatField *dtlSQLgd;
    TStringField *dtlSQLuom;
    TFloatField *dtlSQLreq;
    TFloatField *dtlSQLrel;
    TFloatField *dtlSQLbal;
    TFloatField *dtlSQLprice;
    TStringField *dtlSQLidgd;
    TStringField *dtlSQLnmgd;
    TStringField *dtlSQLgduom;
    TDataSource *dtlDS;
    TQuery *gdSQL;
    TDataSource *gdDS;
    TQuery *deptSQL;
    TQuery *divSQL;
    TQuery *rptSQL;
    TQuery *numSQL;
    TQuery *setupQRY;
    TDBGrid *sumGRD;
    TQuery *sumSQL;
    TDataSource *sumDS;
    TStringField *dtlSQLgdnm2;
    TStringField *dtlSQLcur;
    TFloatField *dtlSQLtotal;
    TStringField *sumSQLreqno;
    TStringField *sumSQLcur;
    TFloatField *sumSQLprice;
    TDBMemo *othMEMO;
    TLabel *Label4;
    TStringField *reqSQLothnote;
    TQuery *countSQL;
    TDateField *dtlSQLrefdt;
    TButton *shwBTN;
    TDateTimePicker *chgDT;
    TBitBtn *chgBTN;
    TLabel *Label15;
    TDateTimePicker *stDT;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall reqSQLAfterScroll(TDataSet *DataSet);
    void __fastcall reqSQLNewRecord(TDataSet *DataSet);
    void __fastcall reqGRDEnter(TObject *Sender);
    void __fastcall dtlSQLNewRecord(TDataSet *DataSet);
    void __fastcall fndEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall listGRDTitleClick(TColumn *Column);
    void __fastcall sortITClick(TObject *Sender);
    void __fastcall dtlSQLCalcFields(TDataSet *DataSet);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall reqnoEDTEnter(TObject *Sender);
    void __fastcall reqnoEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall addBTNClick(TObject *Sender);
    void __fastcall reqDTKeyPress(TObject *Sender, char &Key);
    void __fastcall reqDTChange(TObject *Sender);
    void __fastcall needDTKeyPress(TObject *Sender, char &Key);
    void __fastcall needDTChange(TObject *Sender);
    void __fastcall deptCMBExit(TObject *Sender);
    void __fastcall deptCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall divCMBExit(TObject *Sender);
    void __fastcall divCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall byEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall apEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall byttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall apttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall jobEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall woEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall reqSQLBeforePost(TDataSet *DataSet);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall dtlSQLAfterOpen(TDataSet *DataSet);
    void __fastcall dtlSQLpriceChange(TField *Sender);
    void __fastcall dtlSQLreqChange(TField *Sender);
    void __fastcall reqGRDKeyPress(TObject *Sender, char &Key);
    
    void __fastcall dtlSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall reqSQLBeforeClose(TDataSet *DataSet);
    void __fastcall reqSQLBeforeScroll(TDataSet *DataSet);
    void __fastcall prtBTNClick(TObject *Sender);
    

    void __fastcall reqGRDColEnter(TObject *Sender);
    void __fastcall byEDTEnter(TObject *Sender);
    void __fastcall apEDTEnter(TObject *Sender);

    void __fastcall reqPGChange(TObject *Sender);
    void __fastcall shwBTNClick(TObject *Sender);
    void __fastcall dtlSQLrelChange(TField *Sender);
    void __fastcall chgBTNClick(TObject *Sender);
    void __fastcall chgDTKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    bool loged;
    AnsiString CAT;
    AnsiString oldVAL,newVAL,oldCUR,newCUR,oldFLD;
    int cntr;
    float oldREQ,oldPRC,oldREL;
    AnsiString PERIOD;
    AnsiString spaceSTR,pathAPP_,pathPRT,pathHTML,pathRPT;
    AnsiString path_,trdate,sortSTR,orgSQL;
    AnsiString refNEW,refOLD;
    TStringList *fndVAL,*teksHTML,*teksEDIT,*deptVAL,*divVAL,*teksTXT,*teksRPT;
    bool master;
    //prosedur here
    void TreqFRM::reqRPT();
    void TreqFRM::setDT();
    void TreqFRM::dtlDAF();
    bool TreqFRM::sameCAT();
    void TreqFRM::depPRT();
    void TreqFRM::rowDAF();
    void TreqFRM::dafRPT();
    void TreqFRM::noteFIX();
    bool TreqFRM::findIT();
    void TreqFRM::pickmeUP();
    void TreqFRM::balCHG();
    void TreqFRM::reqDEL();
    void TreqFRM::itemDEL();
    bool TreqFRM::gdFIND(AnsiString fldNM,AnsiString fndSTR);
    void TreqFRM::pickME(AnsiString qrySTR,AnsiString fldNM,AnsiString fldGET);
    void TreqFRM::cmbME(TComboBox *myCMB,AnsiString fldNM,TStringList *myVAL);
    void TreqFRM::deptFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM);
    void TreqFRM::isDATE();
    void TreqFRM::cmbSET(TComboBox *myCMB,AnsiString fldNM,TStringList *myVAL);
    void TreqFRM::setCMB();
    void TreqFRM::key13(TObject *Sender,char Key);
    void TreqFRM::dtlUPDATE();
    bool TreqFRM::isEXIST(AnsiString seekSTR);
    void TreqFRM::reqGO(AnsiString seekSTR);
    void TreqFRM::numNEW();
    void TreqFRM::numOPEN();
    void TreqFRM::itemADD();
    void TreqFRM::isCHG();
    void TreqFRM::reqOPEN();
    void TreqFRM::sortGO();
    void TreqFRM::sortME(TColumn *Column);
    void TreqFRM::varFILL();
    void TreqFRM::reqSET();
    void TreqFRM::dtlPOST();
    void TreqFRM::reqPOST();
    void TreqFRM::idSAY();
    void TreqFRM::dateSAY();
    void TreqFRM::sumOPEN();
    void TreqFRM::subOPEN();
    void TreqFRM::tbOPEN();
    void TreqFRM::tbCLOSE();
public:		// User declarations
    AnsiString judul;
    __fastcall TreqFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TreqFRM *reqFRM;
//---------------------------------------------------------------------------
#endif
