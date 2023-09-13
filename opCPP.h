//---------------------------------------------------------------------------
#ifndef opCPPH
#define opCPPH
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
class TopFRM : public TForm
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
    TDBNavigator *opNAV;
    TEdit *recEDT;
    TBitBtn *addBTN;
    TPageControl *opPG;
    TTabSheet *listSHEET;
    TDBGrid *listGRD;
    TTabSheet *opSHEET;
    TLabel *Label3;
    TLabel *Label6;
    TLabel *Label2;
    TLabel *Label5;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TDBGrid *dtlGRD;
    TComboBox *whCMB;
    TComboBox *locCMB;
    TDBEdit *opEDT;
    TDateTimePicker *opDT;
    TDBEdit *relEDT;
    TDBEdit *aprEDT;
    TDBEdit *relttlEDT;
    TDBEdit *aprttlEDT;
    TComboBox *condCMB;
    TQuery *opSQL;
    TDataSource *opDS;
    TQuery *tmpSQL;
    TQuery *dtlSQL;
    TDataSource *dtlDS;
    TFloatField *dtlSQLwh;
    TFloatField *dtlSQLgd;
    TStringField *dtlSQLcond;
    TFloatField *dtlSQLbal;
    TFloatField *dtlSQLprice;
    TStringField *dtlSQLref;
    TFloatField *dtlSQLmode;
    TQuery *whSQL;
    TQuery *locSQL;
    TStringField *opSQLtrfno;
    TDateField *opSQLtrfdt;
    TFloatField *opSQLtrto;
    TStringField *opSQLrelby;
    TStringField *opSQLrelttl;
    TStringField *opSQLaprby;
    TStringField *opSQLaprttl;
    TStringField *opSQLstate;
    TFloatField *opSQLmode;
    TStringField *opSQLlocnm;
    TQuery *gdSQL;
    TStringField *dtlSQLid;
    TStringField *dtlSQLgdnm;
    TFloatField *dtlSQLgdid;
    TDateField *dtlSQLrefdt;
    TStringField *dtlSQLjobid;
    TStringField *dtlSQLbatch;
    TCheckBox *begCHECK;
    TStringField *opSQLjob;
    TDateField *opSQLneeded;
    TQuery *stkSQL;
    TQuery *btchSQL;
    TBitBtn *posBTN;
    TStringField *dtlSQLuom;
    TFloatField *dtlSQLuom2;
    TBitBtn *prtBTN;
    TFloatField *opSQLwhto;
    TStringField *opSQLwhtonm;
    TQuery *numSQL;
    TCheckBox *sortIT;
    TQuery *unposSQL;
    TQuery *binSQL;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall opPGEnter(TObject *Sender);
    void __fastcall addBTNClick(TObject *Sender);
    void __fastcall dtlSQLNewRecord(TDataSet *DataSet);
    void __fastcall opSQLAfterScroll(TDataSet *DataSet);
    void __fastcall dtlSQLAfterScroll(TDataSet *DataSet);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall dtlGRDEnter(TObject *Sender);
    void __fastcall dtlSQLBeforeClose(TDataSet *DataSet);
    void __fastcall whCMBChange(TObject *Sender);
    void __fastcall opEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall opEDTEnter(TObject *Sender);
    void __fastcall relEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall aprEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall opDTKeyPress(TObject *Sender, char &Key);
    void __fastcall relttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall aprttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall whCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall locCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall condCMBKeyPress(TObject *Sender, char &Key);
    void __fastcall whCMBExit(TObject *Sender);
    void __fastcall locCMBExit(TObject *Sender);
    void __fastcall opSQLBeforePost(TDataSet *DataSet);
    void __fastcall dtlSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall opDTChange(TObject *Sender);
    
    void __fastcall opSQLNewRecord(TDataSet *DataSet);
    void __fastcall dtlSQLidChange(TField *Sender);

    void __fastcall posBTNClick(TObject *Sender);
    void __fastcall dtlSQLBeforePost(TDataSet *DataSet);
    void __fastcall begCHECKExit(TObject *Sender);
    void __fastcall opSQLBeforeEdit(TDataSet *DataSet);
    void __fastcall opSQLAfterEdit(TDataSet *DataSet);
    void __fastcall opSQLBeforeDelete(TDataSet *DataSet);
    void __fastcall dtlSQLBeforeDelete(TDataSet *DataSet);
    void __fastcall dtlSQLAfterEdit(TDataSet *DataSet);
    void __fastcall opSQLAfterPost(TDataSet *DataSet);

    void __fastcall condCMBExit(TObject *Sender);
    void __fastcall prtBTNClick(TObject *Sender);
    void __fastcall dtlSQLgdnmChange(TField *Sender);
    void __fastcall whCMBEnter(TObject *Sender);
    void __fastcall opDTEnter(TObject *Sender);
    void __fastcall relEDTEnter(TObject *Sender);
    void __fastcall aprEDTEnter(TObject *Sender);
    void __fastcall aprttlEDTEnter(TObject *Sender);
    void __fastcall locCMBEnter(TObject *Sender);
    void __fastcall condCMBEnter(TObject *Sender);
    
    void __fastcall fndEDTKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    AnsiString orgSQL;
    AnsiString sortSTR;
    TStringList *fndVAL;
    AnsiString refOLD,refNEW;
    bool master;
    AnsiString oldREF;
    AnsiString path_;
    TStringList *locVAL,*whVAL;
    float vektor;
    bool opCHG;
    //all prosedur
    void TopFRM::sayADJ();
    void TopFRM::sayBAL();
    void TopFRM::reqDEL();
    void TopFRM::reqOPEN();
    void TopFRM::sortGO();
    void TopFRM::sortME(TColumn *Column);
    void TopFRM::reqGO(AnsiString seekSTR);
    bool TopFRM::isEXIST(AnsiString seekSTR);
    void TopFRM::dtlUPDATE();
    void TopFRM::numNEW();
    void TopFRM::numOPEN();
    bool TopFRM::onPOST();
    void TopFRM::byPOST();
    void TopFRM::gdSET();
    void TopFRM::rowPRT();
    void TopFRM::opRPT();
    void TopFRM::condFILL();
    void TopFRM::gdFILL();
    void TopFRM::balFIX(TDataSet *mySQL);
    void TopFRM::isBLC();
    void TopFRM::bincrdFIX();
    void TopFRM::batchFIX();
    void TopFRM::stkFIX();
    void TopFRM::itemPOST();
    bool TopFRM::allowPOST();
    void TopFRM::isPOST();
    void TopFRM::cmbSAY();
    void TopFRM::dateSAY();
    void TopFRM::itemDEL();
    AnsiString TopFRM::cmbFND(TComboBox *myCMB,TStringList *myVAL);
    void TopFRM::varFILL();
    void TopFRM::whFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM);
    AnsiString wh,loc,cond,opdate;
    void TopFRM::opcmbFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM);
    void TopFRM::isDATE();
    void TopFRM::setCOND();
    void TopFRM::refCHG();
    void TopFRM::key13(TObject *Sender,char Key);
    void TopFRM::locOPEN();
    void TopFRM::setWH();
    void TopFRM::itemADD();
    void TopFRM::opPOST();
    void TopFRM::subPOST();
    void TopFRM::recSAY(TDataSet *mySQL);
    void TopFRM::subOPEN();
    void TopFRM::tbOPEN();
    void TopFRM::tbCLOSE();
public:		// User declarations
    AnsiString pathPRT,pathAPP_,pathHTML,pathRPT;
    TStringList *teksHTML,*teksEDIT,*teksTXT,*teksRPT;
    __fastcall TopFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TopFRM *opFRM;
//---------------------------------------------------------------------------
#endif
