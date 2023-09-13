//---------------------------------------------------------------------------
#include <shlobj.hpp>
#include <vcl.h>
#include <FileCtrl.hpp>
#pragma hdrstop

#include "seekCPP.h"
#include "funcCPP.h"
#include "PICKCPP.h"
#include "mnuCPP.h"
#include "validCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TseekprFRM *seekprFRM;
//---------------------------------------------------------------------------
__fastcall TseekprFRM::TseekprFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void TseekprFRM::prtPR(TDataSet *mySQL)
{
 //cetak PR menurut format EDY (14-05-2008)
 AnsiString gdnm="";
 AnsiString deptnm="";
 AnsiString dept="";
 AnsiString div="";
 AnsiString mydept="";

  while(!mySQL->Eof)
  {
   if(samedept(mydept,mySQL))
   {
    AnsiString nama=
    Trim(mySQL->FieldByName("gdnm")->AsString);
    AnsiString unit=
    reqPG->ActivePage==doSHEET?Trim(mySQL->FieldByName("unit")->AsString):Trim(mySQL->FieldByName("uom")->AsString);
    //cetak detail po
    if(gdnm!=nama+unit)
    {
     //teksRPT->Add("");
     deptnm="";
     gdnm=nama;
     gdnm+=unit;
     //raw="";
     //raw+=nama+" - "+unit;
     //teksRPT->Add(raw);
     //if(mydept!="" && showdept )showdept=false;
     //if(mydept=="")showdept=true;
    }
    if(reqPG->ActivePage==reqSHEET)
    {
    dept=Trim(mySQL->FieldByName("deptnm")->AsString);
    div=Trim(mySQL->FieldByName("divnm")->AsString);
    }
    if(deptnm!=dept+div)
    {
    deptnm=dept;
    deptnm+=div;
    //raw="";
    //raw+="Location By : "+dept+" - "+div;
    //if(showdept)teksRPT->Add(raw);
    //if(mydept!="")showdept=false;
    }
    float req_,prc_;
    raw="";
    raw+="'"+mySQL->FieldByName("reqdt")->AsString+" \t";
    raw+="'"+mySQL->FieldByName("reqno")->AsString+" \t";
    req_=mySQL->FieldByName("req")->AsFloat;
    raw+=mySQL->FieldByName("req")->AsString+" \t";
    raw+=nama+" - "+unit+" \t";
    raw+=mySQL->FieldByName("CUR")->AsString+" \t";
    prc_=mySQL->FieldByName("price")->AsFloat;
    raw+=mySQL->FieldByName("PRICE")->AsString+" \t";
    raw+=FloatToStr(req_*prc_)+"\t";
    raw+=dept+" - "+div;
    teksRPT->Add(raw);
   }
   mySQL->Next();
  }
 mySQL->Close();


 return;
}
//---------------------------------------------------------
bool TseekprFRM::samedept(AnsiString mydept,TDataSet *mySQL)
{
 bool retu_=true;
 if(reqPG->ActivePage==reqSHEET && mydept!="")
  if(Trim(mydept)!=Trim(mySQL->FieldByName("deptnm")->AsString))retu_=false;
 return(retu_);
}
//---------------------------------------------------------
void TseekprFRM::prtRAW(TDataSet *mySQL)
{
 AnsiString gdnm="";
 AnsiString deptnm="";
 AnsiString dept="";
 AnsiString div="";
 AnsiString mydept="";

 bool showdept=true;
 if(reqPG->ActivePage==reqSHEET)
  if(MessageDlg("Print All Location ?", mtConfirmation,
                TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes)
    mydept=Trim(mySQL->FieldByName("deptnm")->AsString);

 if(outCHK->Checked && reqPG->ActivePage!=doSHEET)PERIOD+="(OUTSTANDING ONLY)";

 //cetak periode
 teksRPT->Add(PERIOD);
 //teksRPT->Add("\n");

 TvalFRM *FRM=new TvalFRM(Application);
 FRM->ShowModal();

 teksRPT->Add(FRM->byEDT->Text);
 teksRPT->Add(FRM->byttlEDT->Text);
 teksRPT->Add(FRM->ackEDT->Text);
 teksRPT->Add(FRM->ackttlEDT->Text);
 teksRPT->Add(FRM->appEDT->Text);
 teksRPT->Add(FRM->appttlEDT->Text);

 delete FRM;

 mySQL->First();
 if(reqPG->ActivePage==reqSHEET)
  {
   prtPR(mySQL);
   return;
  }
 while(!mySQL->Eof)
  {
   if(samedept(mydept,mySQL))
   {
    AnsiString nama=
    Trim(mySQL->FieldByName("gdnm")->AsString);
    AnsiString unit=
    reqPG->ActivePage==doSHEET?Trim(mySQL->FieldByName("unit")->AsString):Trim(mySQL->FieldByName("uom")->AsString);
    //cetak detail po
    if(gdnm!=nama+unit)
    {
     teksRPT->Add("");
     deptnm="";
     gdnm=nama;
     gdnm+=unit;
     raw="";
     raw+=nama+" - "+unit;
     teksRPT->Add(raw);
     if(mydept!="" && showdept )showdept=false;
     if(mydept=="")showdept=true;
    }
    if(reqPG->ActivePage==reqSHEET)
    {
    dept=Trim(mySQL->FieldByName("deptnm")->AsString);
    div=Trim(mySQL->FieldByName("divnm")->AsString);
    }
    if(deptnm!=dept+div)
    {
    deptnm=dept;
    deptnm+=div;
    raw="";
    raw+="Location By : "+dept+" - "+div;
    if(showdept)teksRPT->Add(raw);
    //if(mydept!="")showdept=false;
    }
    raw="";
    if(reqPG->ActivePage==doSHEET)
       raw+="'"+mySQL->FieldByName("ref")->AsString+" \t";
    if(reqPG->ActivePage==poSHEET ||
       reqPG->ActivePage==doSHEET)
       if(mySQL->Name=="doSQL")raw+="'"+mySQL->FieldByName("req")->AsString+" \t";
       else raw+="'"+mySQL->FieldByName("refno")->AsString+" \t";
    if(reqPG->ActivePage==reqSHEET ||
       reqPG->ActivePage==poSHEET)
       raw+="'"+mySQL->FieldByName("reqno")->AsString+" \t";
    if(reqPG->ActivePage==poSHEET ||
       reqPG->ActivePage==doSHEET)
       if(reqPG->ActivePage==doSHEET )raw+="'"+mySQL->FieldByName("refdt")->AsString+" \t";
       else raw+="'"+mySQL->FieldByName("trfdt")->AsString+" \t";
    else raw+="'"+mySQL->FieldByName("reqdt")->AsString+" \t";

    if(mySQL->Name=="doSQL")raw+=mySQL->FieldByName("db")->AsString+" \t";
    else raw+=mySQL->FieldByName("req")->AsString+" \t";

    if(reqPG->ActivePage==poSHEET ||
       reqPG->ActivePage==reqSHEET)
    {
     raw+=mySQL->FieldByName("CUR")->AsString+" \t";
     raw+=mySQL->FieldByName("PRICE")->AsString+" \t";
    }
    teksRPT->Add(raw);
   }
   mySQL->Next();
  }
 mySQL->Close();
}
//---------------------------------------------------------
void TseekprFRM::prRPT()
{
 teksRPT=new TStringList();teksRPT->Clear();

 PERIOD=FormatDateTime(" dd mmmm yyyy",mnuFRM->stDT)+
        " - "+
        FormatDateTime(" dd mmmm yyyy",mnuFRM->lstDT);

 //buat mencetak formulir ini
 pathAPP_=mnuFRM->pathAPP_;
 pathPRT=pathAPP_+"\\REPORT\\";
 //pathRPT=pathAPP_+"\\TXT\\";
 pathRPT="c:\\temp\\";
 //ShowMessage(pathAPP_+"\n"+pathPRT+"\n"+pathHTML);
 AnsiString jd;
 if(reqPG->ActivePage==reqSHEET)jd="Daftar PR Atas Barang Periode ";
 if(reqPG->ActivePage==poSHEET)jd="Daftar PO Atas Barang Periode ";
 if(reqPG->ActivePage==doSHEET)jd="Daftar DO Atas Barang Periode ";
 AnsiString s_=InputBox("Masukkan Judul Report","" ,jd+PERIOD);
 //AnsiString rptME=pathRPT+s_+".TXT";
 AnsiString rptME="c:\\temp\\report.txt";
 //will be using TStringList or TMemo
 teksRPT->Clear();
 teksRPT->Add(s_);

 //teksRPT->Add(jd+PERIOD);
 //disini untuk yang buat laporan dong


 if(reqPG->ActivePage==reqSHEET)prtRAW(prSQL);
 if(reqPG->ActivePage==poSHEET)prtRAW(poSQL);
 if(reqPG->ActivePage==doSHEET)prtRAW(doSQL);

 //create hard print
 ForceDirectories(pathPRT);
 teksRPT->SaveToFile(rptME);
 teksRPT->Clear();

 AnsiString app_="EXCEL.EXE";
 AnsiString txt_=rptME;
 AnsiString mod_="open";
 pathPRT=pathAPP_+"\\FORM"  ;

 AnsiString fName;
 if(reqPG->ActivePage==reqSHEET)fName="GETPRD.XLS";
 if(reqPG->ActivePage==poSHEET)fName="GETPOD.XLS";
 if(reqPG->ActivePage==doSHEET)fName="GETDOD.XLS";
 txt_=pathPRT+"\\"+fName;

 //print using shellobj
 ShellExecute(
    Handle,	// handle to parent window
    mod_.c_str(),	// pointer to string that specifies operation to perform
    txt_.c_str(),	// pointer to filename or folder name string
    app_.c_str(),	// pointer to string that specifies executable-file parameters
    pathPRT.c_str(),	// pointer to string that specifies default directory
    SW_SHOW 	// whether file is shown when opened
   );
 //DeleteFile(rptME);
}
//---------------------------------------------------------
void TseekprFRM::doOPEN()
{
doSQL->Close();
doSQL->Open();
}
//---------------------------------------------------------
void TseekprFRM::poOPEN()
{
 poSQL->Close();
 poSQL->Open();
}
//---------------------------------------------------------
void TseekprFRM::reqOPEN()
{
 prSQL->Close();
 prSQL->Open();
}
//---------------------------------------------------------
void TseekprFRM::showEM(TQuery *mySQL)
{
 AnsiString comb=" OR ";
 if(exact->Checked)comb=" AND ";
 AnsiString orgREQ="";
 orgREQ+=" select r.reqdept,r.reqdiv,r.reqno,r.reqdt,rd.req,rd.bal,rd.uom,rd.cur,rd.price,rd.gdnm "+
 orgREQ+=" from req.dbe r inner join dtlreq.dbe  rd on (r.reqno=rd.reqno) "+
 orgREQ+=" WHERE (rd.refno is null and r.mode=0) ";
 AnsiString orgPO="";
 orgPO+=" select rd.refno,rd.reqno,r.trfdt,rd.req,rd.bal,rd.cur,rd.price,rd.gdnm,rd.uom "+
 orgPO+=" from dtlreq.dbe rd inner join req.dbe r on (r.trfno=rd.refno) "+
 orgPO+=" WHERE (rd.refno is not null and rd.reqno is not null) ";
 AnsiString orgDO="";
 orgDO+=" select rd.ref,rd.req,rd.refdt,rd.db,rd.gdnm,rd.unit "+
 orgDO+=" from bincrd.dbe rd ";

 AnsiString ord_="";
 if(reqPG->ActivePage==reqSHEET)ord_=" order by r.reqdept,r.reqdt,r.reqdiv,r.reqno,rd.gdnm,rd.uom ";
 if(reqPG->ActivePage==poSHEET)ord_=" order by rd.refno,r.trfdt,rd.gdnm,rd.uom ";
 if(reqPG->ActivePage==doSHEET)ord_=" order by rd.ref,rd.refdt,rd.gdnm,rd.unit ";

 TStringList *seek_=new TStringList();
 if(seekCHK->Checked && freeEDT->Text!="")func->listIT(seek_,freeEDT->Text);

 AnsiString org_="";
 if(reqPG->ActivePage==reqSHEET)org_=orgREQ;
 if(reqPG->ActivePage==poSHEET)org_=orgPO;
 if(reqPG->ActivePage==doSHEET)org_=orgDO;

 mySQL->Close();
 mySQL->SQL->Clear();
 mySQL->SQL->Add(org_);
 int pos_;

 if(!func->atSTR(mySQL->SQL->Text,"WHERE",pos_) )
 mySQL->SQL->Add(" WHERE ");
 else mySQL->SQL->Add(" AND ");
 if(reqPG->ActivePage==reqSHEET)
  mySQL->SQL->Add(" (r.reqdt>=:STDT and r.reqdt<=:LSTDT ) ");
 if(reqPG->ActivePage==poSHEET)
  mySQL->SQL->Add(" (r.trfdt>=:STDT and r.trfdt<=:LSTDT ) ");
 if(reqPG->ActivePage==doSHEET)
  mySQL->SQL->Add(" (rd.refdt>=:STDT and rd.refdt<=:LSTDT ) ");

 mySQL->ParamByName("STDT")->AsDate=stDT->Date;
 mySQL->ParamByName("LSTDT")->AsDate=lstDT->Date;

 if(reqPG->ActivePage!=doSHEET)
 {
 if(outCHK->Checked)
  {
  if(!func->atSTR(mySQL->SQL->Text,"WHERE",pos_) )
   mySQL->SQL->Add(" WHERE ");
  else mySQL->SQL->Add(" AND ");
  mySQL->SQL->Add(" rd.bal>0 ");
  }
 }
 AnsiString seekSTR="";
 if(seek_->Count>0)
  if(!func->atSTR(mySQL->SQL->Text,"WHERE",pos_) )
   seekSTR+=" WHERE ";
  else seekSTR+=" AND ";


 if(seek_->Count>0) seekSTR+= " ( ";
 AnsiString posSTR;
 for(int i=1;i<=seek_->Count;i++)
  {
   posSTR=seek_->Strings[i-1];
   seekSTR+="UPPER(rd."+
             QuotedStr(AnsiString("gdnm"))+") LIKE "+
             QuotedStr("%"+Trim(posSTR.UpperCase())+"%");

   if(seek_->Count>1 && i<seek_->Count) seekSTR+=comb;

  }
  if(seek_->Count>0) seekSTR+=" ) ";
 mySQL->SQL->Add(seekSTR);

 mySQL->SQL->Add(ord_);
 //ShowMessage(mySQL->SQL->Text);
 mySQL->Open();

}
//---------------------------------------------------------------------------
void __fastcall TseekprFRM::BitBtn1Click(TObject *Sender)
{
 if(reqPG->ActivePage==reqSHEET)showEM(prSQL);
 if(reqPG->ActivePage==poSHEET )showEM(poSQL);
 if(reqPG->ActivePage==doSHEET )showEM(doSQL);

}
//---------------------------------------------------------------------------
void __fastcall TseekprFRM::freeEDTKeyPress(TObject *Sender, char &Key)
{
 if(Key=='\\')
  {
   //pickform->siapkan(mnuFRM->path_+"\\DATA","DTLREQ.DBE");
   pickform->Query1->Close();
   pickform->Query1->DatabaseName=mnuFRM->path_+"\\DATA";
   pickform->Query1->SQL->Clear();
   pickform->Query1->SQL->Add(" select distinct book.'gdnm' from 'dtlreq.dbe' book ");
   pickform->DBGrid1->Columns->Items[0]->Title->Caption="Description";
   pickform->DBGrid1->Columns->Items[0]->FieldName="gdnm";
   if(pickform->ShowModal()==mrOk)
    freeEDT->Text=pickform->Query1->FieldByName("gdnm")->AsString;
   Key=0;
  }
}
//---------------------------------------------------------
void TseekprFRM::tbOPEN()
{
 AnsiString pathDB=mnuFRM->path_+"\\DATA";
 deptSQL->DatabaseName=pathDB;
 divSQL->DatabaseName=pathDB;
 prSQL->DatabaseName=pathDB;
 poSQL->DatabaseName=pathDB;
 doSQL->DatabaseName=pathDB;
}
//---------------------------------------------------------
void TseekprFRM::tbCLOSE()
{
 deptSQL->Close();
 divSQL->Close();
 prSQL->Close();
 poSQL->Close();
 doSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TseekprFRM::FormActivate(TObject *Sender)
{
tbCLOSE();
tbOPEN();
reqPG->ActivePage=reqSHEET;
stDT->Date=mnuFRM->stDT;
lstDT->Date=mnuFRM->lstDT;
}
//---------------------------------------------------------------------------
void __fastcall TseekprFRM::FormClose(TObject *Sender,
      TCloseAction &Action)
{
tbCLOSE();
}
//---------------------------------------------------------------------------
void __fastcall TseekprFRM::reqPGChange(TObject *Sender)
{
 BitBtn1Click(BitBtn1);
}
//---------------------------------------------------------------------------

void __fastcall TseekprFRM::prtBTNClick(TObject *Sender)
{
 prRPT();
}
//---------------------------------------------------------------------------

