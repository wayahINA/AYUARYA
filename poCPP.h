//---------------------------------------------------------------------------
#ifndef poCPPH
#define poCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TpoFRM : public TForm
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
    TBitBtn *addBTN;
    TPageControl *reqPG;
    TTabSheet *listSHEET;
    TDBGrid *listGRD;
    TTabSheet *reqSHEET;
    TDBGrid *poGRD;
    TPageControl *frmPG;
    TTabSheet *frmSHEET;
    TLabel *Label2;
    TDBEdit *poEDT;
    TLabel *Label3;
    TDateTimePicker *poDT;
    TLabel *Label7;
    TDateTimePicker *needDT;
    TLabel *Label5;
    TEdit *addrEDT;
    TEdit *ctyEDT;
    TEdit *phEDT;
    TEdit *fxEDT;
    TShape *Shape1;
    TLabel *Label8;
    TDBEdit *byEDT;
    TLabel *Label10;
    TDBEdit *apEDT;
    TLabel *Label9;
    TDBEdit *byttlEDT;
    TLabel *Label6;
    TDBEdit *apttlEDT;
    TLabel *Label11;
    TTabSheet *pickSHEET;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TComboBox *deptCMB;
    TComboBox *divCMB;
    TComboBox *reqCMB;
    TLabel *Label16;
    TDBGrid *reqGRD;
    TQuery *poSQL;
    TDataSource *poDS;
    TQuery *podtlSQL;
    TDataSource *podtlDS;
    TQuery *reqSQL;
    TDataSource *reqDS;
    TQuery *vdSQL;
    TDataSource *vdDS;
    TComboBox *nmCMB;
    TQuery *tmpSQL;
    TBitBtn *refreshBTN;
    TQuery *numSQL;
    TEdit *stsTEXT;
    TStringField *poSQLtrfno;
    TDateField *poSQLtrfdt;
    TStringField *poSQLrcvby;
    TStringField *poSQLrcvttl;
    TDateField *poSQLneeded;
    TFloatField *poSQLmode;
    TStringField *poSQLvdref;
    TFloatField *poSQLvd;
    TStringField *poSQLadd1;
    TStringField *poSQLph;
    TStringField *poSQLfx;
    TStringField *poSQLvdnm;
    TFloatField *podtlSQLgd;
    TStringField *podtlSQLuom;
    TFloatField *podtlSQLreq;
    TFloatField *podtlSQLrel;
    TFloatField *podtlSQLbal;
    TQuery *gdSQL;
    TDataSource *gdDS;
    TStringField *podtlSQLgdid;
    TStringField *poSQLstate;
    TStringField *poSQLaprby;
    TStringField *poSQLaprttl;
    TStringField *podtlSQLrefno;
    TDateField *poSQLreqdt;
    TBitBtn *getBTN;
    TStringField *podtlSQLgdnm;
    TTabSheet *doSHEET;
    TQuery *doSQL;
    TDataSource *doDS;
    TDBGrid *doGRD;
    TStringField *doSQLunit;
    TStringField *doSQLgdnm;
    TStringField *doSQLref;
    TFloatField *doSQLdb;
    TStringField *podtlSQLreqno;
    TStringField *doSQLreq;
    TDateField *doSQLrefdt;
    TBitBtn *posBTN;
    TBitBtn *BitBtn1;
    TQuery *fixSQL;
    TTable *fixTBL;
    TQuery *rptSQL;
    TStringField *podtlSQLcur;
    TFloatField *podtlSQLprice;
    TEdit *filterEDT;
    TTable *reqTBL;
    TBitBtn *prtBTN;
    TDateField *podtlSQLrefdt;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall refreshBTNClick(TObject *Sender);
    void __fastcall frmPGEnter(TObject *Sender);
    
    void __fastcall addBTNClick(TObject *Sender);
    void __fastcall poEDTEnter(TObject *Sender);
    void __fastcall poGRDEnter(TObject *Sender);
    void __fastcall podtlSQLNewRecord(TDataSet *DataSet);
    void __fastcall poSQLAfterScroll(TDataSet *DataSet);
    void __fastcall poDTKeyPress(TObject *Sender, char &Key);
    void __fastcall nmCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall nmCMBExit(TObject *Sender);
    void __fastcall podtlSQLgdChange(TField *Sender);
    void __fastcall podtlSQLreqChange(TField *Sender);
    void __fastcall poGRDColEnter(TObject *Sender);
    void __fastcall poEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall poDTExit(TObject *Sender);
    void __fastcall needDTExit(TObject *Sender);
    void __fastcall needDTKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall addrEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall ctyEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall phEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall fxEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall byEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall byttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall apEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall apttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall poSQLNewRecord(TDataSet *DataSet);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    
    void __fastcall poSQLBeforePost(TDataSet *DataSet);
    void __fastcall poSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall poSQLAfterEdit(TDataSet *DataSet);
    void __fastcall poSQLBeforeDelete(TDataSet *DataSet);
    void __fastcall podtlSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall podtlSQLAfterEdit(TDataSet *DataSet);
    void __fastcall podtlSQLBeforePost(TDataSet *DataSet);
    void __fastcall podtlSQLBeforeDelete(TDataSet *DataSet);
    void __fastcall fndEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall getBTNClick(TObject *Sender);
    void __fastcall prtBTNClick(TObject *Sender);
    void __fastcall reqPGChange(TObject *Sender);
    void __fastcall doSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall doGRDKeyPress(TObject *Sender, char &Key);
    void __fastcall doSQLNewRecord(TDataSet *DataSet);
    void __fastcall doGRDExit(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall posBTNClick(TObject *Sender);
    
    void __fastcall doSQLdbChange(TField *Sender);
    void __fastcall filterEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall vdSQLAfterPost(TDataSet *DataSet);
private:	// User declarations
    //var declaration here
    float relQTY,baru;
    AnsiString WH;
    float relSTR,balSTR;
    float reqOLD,doOLD;
    AnsiString gdnmOLD;
    AnsiString strDATE,reqORG;
    int cntr;
    AnsiString pathAPP_,pathPRT,pathHTML,pathRPT;
    AnsiString PERIOD;
    TStringList *teksHTML,*teksEDIT,*teksTXT,*teksRPT;
    bool multi;
    AnsiString refNEW,refOLD;
    bool master,poCHG;
    bool vdNEW;
    TStringList *vdVAL,*fndVAL;
    //all prosedur here
    void TpoFRM::dounPOSTIT();
    void TpoFRM::doPOSTIT();
    bool TpoFRM::sameWH();
    void TpoFRM::dtlPRT();
    void TpoFRM::whPRT();
    void TpoFRM::rowDAF();
    void TpoFRM::poDEL();
    void TpoFRM::relFIX();
    void TpoFRM::tblFIX();
    void TpoFRM::reqbalFIX();
    void TpoFRM::reqPOST();
    void TpoFRM::doDEL();
    void TpoFRM::poPOS();
    void TpoFRM::doPOS();
    void TpoFRM::doOPEN();
    void TpoFRM::pickmeUP();
    void TpoFRM::reqRPT();
    void TpoFRM::dafRPT();
    void TpoFRM::getIT();
    bool TpoFRM::dtlUPDATE();
    void TpoFRM::refCHG();
    bool TpoFRM::onPOST();
    bool TpoFRM::allowPOST();
    void TpoFRM::reqDEL();
    void TpoFRM::itemDEL();
    void TpoFRM::setVD();
    void TpoFRM::statusSAY();
    void TpoFRM::key13(TObject *Sender,char Key);
    void TpoFRM::gdFILL();
    void TpoFRM::vdSAY();
    void TpoFRM::idFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM);
    void TpoFRM::varFILL();
    void TpoFRM::vdADD();
    void TpoFRM::dtlPOST();
    void TpoFRM::poPOST();
    void TpoFRM::isDATE();
    void TpoFRM::showPO();
    void TpoFRM::reqGO(AnsiString seekSTR);
    bool TpoFRM::isEXIST(AnsiString seekSTR);
    void TpoFRM::numNEW();
    void TpoFRM::numOPEN();
    void TpoFRM::itemADD();
    void TpoFRM::dtlOPEN();
    void TpoFRM::reqOPEN();
    AnsiString path_;
    void TpoFRM::tbOPEN();
    void TpoFRM::tbCLOSE();
public:		// User declarations
    __fastcall TpoFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TpoFRM *poFRM;
//---------------------------------------------------------------------------
#endif
