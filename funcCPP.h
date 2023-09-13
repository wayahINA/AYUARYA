//---------------------------------------------------------------------------
#ifndef funcCPPH
#define funcCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <Dialogs.hpp>
#include <dbgrids.hpp>
#include "BARCPP.h"
//---------------------------------------------------------------------------
class Tfunc : public TForm
{
__published:	// IDE-managed Components
    TQuery *Query1;
    TDatabase *hisDB;
    TTable *dummyTB;
    TQuery *dummy;
    TOpenDialog *dataku;
    TQuery *setupQRY;
    TBatchMove *copyin;
    TFloatField *setupQRYrecid;
    TStringField *setupQRYid1;
    TStringField *setupQRYid2;
    TStringField *setupQRYid3;
    TStringField *setupQRYid4;
    TStringField *setupQRYidkey;
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    AnsiString passKEY;
public:		// User declarations
    //global variables here
    bool loged;
    int pos;
    TStringList *sortNM,*sortVAL;
    TReplaceFlags flgHTML;
    AnsiString path_;
    //here comes global functions n procedures
    void Tfunc::DirPack(AnsiString whose_);
    void Tfunc::DirCopy(AnsiString where_,AnsiString to_);
    bool Tfunc::packDBF(AnsiString tbNM);
    void Tfunc::fixDate(TDateTimePicker *lstmth,TDateTimePicker *&newmth);
    void Tfunc::sortLST(TStringList *lst,AnsiString subSTR);
    void Tfunc::uploadTB(AnsiString myFile);
    void Tfunc::PathCopy(bool to_,AnsiString whose_);
    bool Tfunc::isBDE();
    void Tfunc::bdeINST();
    void Tfunc::session_START();
    void Tfunc::listIT(TStringList *&myLIST,AnsiString srcSTR);
    void Tfunc::seekIT(TQuery *&mySQL,TStringList *myFND,AnsiString sqlSTR,AnsiString fldNM);
    void Tfunc::sortIT(TColumn *kolom,AnsiString &sortSTR);
    bool Tfunc::strTRUE(AnsiString strDEST,AnsiString strSRC);
    bool Tfunc::scanSTR(TStringList *srcSTR,AnsiString subSTR,int &pos);
    bool Tfunc::atSTR(AnsiString srcSTR,AnsiString subSTR,int &pos);
    bool Tfunc::VarLOCATE(AnsiString mixFIELD,Variant *mixVAL,int varLENGTH,TDataSet *mySQL);
    void Tfunc::setupFILL(TDataSet *&mySQL,AnsiString keySTR,TComboBox *myCMB);
    void Tfunc::comboSET(TQuery *mySQL,AnsiString keySTR,TComboBox *&myCMB,TStringList *&myVAL);
    bool Tfunc::isFILL(TComboBox *&myCMB);
    void Tfunc::openSQL(TQuery *&mySQL,AnsiString sqlSTR,AnsiString filterSTR,AnsiString sortSTR);
    void  Tfunc::FileCopy(AnsiString f_dari,AnsiString f_ke);
    AnsiString Tfunc::fndLST(TStringList *lstCMB,TStringList *lstVAL,bool mode,AnsiString seekSTR);
    void Tfunc::recPOP(AnsiString recSTR,TBookmark rec_,TDataSet *mySQL);
    void Tfunc::recPUSH(TBookmark &rec_,TDataSet *mySQL);
    void Tfunc::createTable(AnsiString path_,AnsiString tblName,AnsiString sourceName);
    AnsiString __fastcall Tfunc::cutLTR(AnsiString LTR,int l);
    AnsiString __fastcall Tfunc::fillLTR(AnsiString LTR,int l);
    AnsiString __fastcall Tfunc::squeezeLTR(AnsiString LTR);
    void Tfunc::seekSTR(TStringList *strTEXT,AnsiString subTEXT,int &strPOS);
    void Tfunc::replaceSTR(TStringList *&strTEXT,AnsiString orgTEXT,AnsiString newTEXT);
    void Tfunc::locateIT(TComboBox *fndCMB,TEdit *fndEDT,TQuery *dafQRY,TStringList *valFND,AnsiString fldNM);
    void Tfunc::ceksetup();
    int Tfunc::spin(int num) ;
    void  Tfunc::del_alias(AnsiString ttl);
    void Tfunc::restorepath(AnsiString ttl);
    void  Tfunc::savepath(AnsiString ttl);
    bool Tfunc::valPAS(int idx_);
    bool  Tfunc::cekPAS(AnsiString procname_);
    __fastcall Tfunc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfunc *func;
//---------------------------------------------------------------------------
#endif
