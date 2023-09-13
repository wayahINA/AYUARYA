//---------------------------------------------------------------------------
#include <vcl.h>
#include <FileCtrl.hpp>
#pragma hdrstop

#include "poCPP.h"
#include "funcCPP.h"
#include "mnuCPP.h"
#include "filterCPP.h"
#include "PICKCPP.h"
#include "explorCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TpoFRM *poFRM;
//---------------------------------------------------------------------------
__fastcall TpoFRM::TpoFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void TpoFRM::dounPOSTIT()
{
   podtlSQL->Last();
   bool udah;
   udah=podtlSQL->Bof;
   float sisa=doOLD;
   if(!udah)udah=sisa<=0;
   float rel;
   float req;
   float bal;
   float akum;
   while(!udah)
   {
    if(Trim(podtlSQL->FieldByName("gdnm")->AsString)==
       Trim(doSQL->FieldByName("gdnm")->AsString) )
    {
     bal=podtlSQL->FieldByName("bal")->AsFloat;
     req=podtlSQL->FieldByName("req")->AsFloat;
     rel=podtlSQL->FieldByName("rel")->AsFloat;
     if(req!=bal)
     {
      relQTY=podtlSQL->FieldByName("rel")->AsFloat;
      akum=(req-bal);
      if(sisa>0)
       if(sisa>=req)relQTY-=akum;
       else relQTY-=(req-sisa);

      //===============MASIH KASUS DISINI=================//
      sisa=sisa-akum;
      //ngelepas rel dan update bal
      podtlSQL->Edit();
      podtlSQL->FieldByName("rel")->AsFloat=
      relQTY;
      podtlSQL->Post();

      podtlSQL->Edit();
      podtlSQL->FieldByName("bal")->AsFloat=
      podtlSQL->FieldByName("req")->AsFloat-
      podtlSQL->FieldByName("rel")->AsFloat;
      podtlSQL->Post();
      }
    }
    podtlSQL->Prior();
    udah=podtlSQL->Bof;
    if(!udah)udah=sisa<=0;
  }
}
//---------------------------------------------------------
void TpoFRM::doPOSTIT()
{
   if(baru<=0)podtlSQL->Last();
   else podtlSQL->First();
   bool udah;
   udah=baru>0?podtlSQL->Eof:podtlSQL->Bof;
   float sisa=baru;
   if(!udah)udah=sisa<=0;
   float bal;
   while(!udah)
   {
    if(Trim(podtlSQL->FieldByName("gdnm")->AsString)==
       Trim(doSQL->FieldByName("gdnm")->AsString) )
    {

     bal=podtlSQL->FieldByName("bal")->AsFloat;
     if(baru>0 && bal>0)
     {
      relQTY=podtlSQL->FieldByName("rel")->AsFloat;
      if(sisa>bal)relQTY+=bal;
      else relQTY+=sisa;
      //relQTY-=doOLD;

      //===============MASIH KASUS DISINI=================//
      if(sisa>bal)sisa=sisa-bal;
      else sisa=0;
      //ngelepas rel dan update bal

      podtlSQL->Edit();
      podtlSQL->FieldByName("rel")->AsFloat=relQTY;
      podtlSQL->Post();

      podtlSQL->Edit();
      podtlSQL->FieldByName("bal")->AsFloat=
      podtlSQL->FieldByName("req")->AsFloat-
      podtlSQL->FieldByName("rel")->AsFloat;
      podtlSQL->Post();
      }
    }
    if(baru>0) podtlSQL->Next();
    else podtlSQL->Prior();
    udah=baru>0?podtlSQL->Eof:podtlSQL->Bof;
    if(!udah)udah=sisa<=0;
  }
}
//---------------------------------------------------------
bool TpoFRM::sameWH()
{
 return(func->strTRUE(WH,Trim(rptSQL->FieldByName("VDNM")->AsString)));
}
//---------------------------------------------------------
void TpoFRM::dtlPRT()
{
 ++cntr;
 teksEDIT->LoadFromFile(pathHTML+"datarow.txt");
 func->replaceSTR(teksEDIT,"@CNTR@",IntToStr(cntr));
 func->replaceSTR(teksEDIT,"@REFDT@","");
 func->replaceSTR(teksEDIT,"@REFNO@","");
 func->replaceSTR(teksEDIT,"@VD@","");
 func->replaceSTR(teksEDIT,"@VDREF@","");
 teksHTML->Add(teksEDIT->Text);

 if(FileExists(pathHTML+"txtROW.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtROW.txt");
   func->replaceSTR(teksEDIT,"@CNTR@",IntToStr(cntr));
   func->replaceSTR(teksEDIT,"@REFDT@","");
   func->replaceSTR(teksEDIT,"@REFNO@","");
   func->replaceSTR(teksEDIT,"@VD@","");
   func->replaceSTR(teksEDIT,"@VDREF@","");
   teksTXT->Add(teksEDIT->Text);
  }
  AnsiString raw="";
  raw+=IntToStr(cntr)+"\t";
  raw+="'"+rptSQL->FieldByName("trfdt")->AsString+"\t";
  raw+="'"+rptSQL->FieldByName("needed")->AsString+"\t";
  raw+=rptSQL->FieldByName("trfno")->AsString+"\t";
  raw+=rptSQL->FieldByName("poby")->AsString+"\t";
  raw+=rptSQL->FieldByName("aprby")->AsString+"\t";
  teksRPT->Add(raw);
}
//---------------------------------------------------------
void TpoFRM::whPRT()
{
 WH=Trim(rptSQL->FieldByName("VDNM")->AsString);
 teksEDIT->LoadFromFile(pathHTML+"whctgry.txt");
 AnsiString sayWH=Trim(rptSQL->FieldByName("VDNM")->AsString);
 func->replaceSTR(teksEDIT,"@WH@",sayWH);
 teksHTML->Add(teksEDIT->Text);
 if(FileExists(pathHTML+"txtwhCAT.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtwhCAT.txt");
   func->replaceSTR(teksEDIT,"@WH@",sayWH);
   teksTXT->Add(teksEDIT->Text);
  }
 teksRPT->Add("Vendor :"+sayWH);

 cntr=0;
 while(!rptSQL->Eof && sameWH())  //and masih sama lokasi
  {
   dtlPRT();
   rptSQL->Next();
  }
 //cetak spare baris kosong deh
 teksEDIT->LoadFromFile(pathHTML+"empty.txt");
 teksRPT->Add("\r\n");
}
//---------------------------------------------------------
void TpoFRM::rowDAF()
{
 AnsiString old_=rptSQL->SQL->Text;
 AnsiString ord_=" order by v.VDNM,r.trfdt";
 rptSQL->Close();
 rptSQL->SQL->Clear();
 rptSQL->SQL->Add(old_);
 rptSQL->SQL->Add(ord_);
 rptSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 rptSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 rptSQL->Open();
 while(!rptSQL->Eof)
  {
   //catat gudang
   whPRT();
  }
 rptSQL->Close();
 rptSQL->SQL->Text=old_;
}
//---------------------------------------------------------
void TpoFRM::poDEL()
{
 //jangan kasi kalu req>0
 if(podtlSQL->FieldByName("req")->AsFloat<=0)podtlSQL->Delete();
}
//---------------------------------------------------------
void TpoFRM::relFIX()
{
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" update 'dtlreq.dbe' ");
 tmpSQL->SQL->Add(" set rel=:REL,bal=:BAL ");
 tmpSQL->SQL->Add(" where reqno=:REQNO and gdnm=:GDNM and uom=:UOM");
 tmpSQL->ParamByName("REL")->AsFloat=relSTR;
 tmpSQL->ParamByName("BAL")->AsFloat=balSTR;
 tmpSQL->ParamByName("REQNO")->AsString=
 podtlSQL->FieldByName("reqno")->AsString;
 tmpSQL->ParamByName("GDNM")->AsString=
 podtlSQL->FieldByName("gdnm")->AsString;
 tmpSQL->ParamByName("UOM")->AsString=
 podtlSQL->FieldByName("uom")->AsString;
 tmpSQL->Prepare();
 tmpSQL->ExecSQL();
 tmpSQL->Close();
}
//---------------------------------------------------------
void TpoFRM::tblFIX()
{
 TLocateOptions Opts;
 Opts.Clear();
 Opts << loPartialKey;

 //fixTBL->Active=false;
 //fixTBL->Filtered=true;
 //fixTBL->Filter=AnsiString(" reqno ='"+podtlSQL->FieldByName("reqno")->AsString+"' ");
 //fixTBL->Filter+=AnsiString(" AND refno=Null  ");
 //fixTBL->Filter+=AnsiString(" AND gdnm='"+podtlSQL->FieldByName("gdnm")->AsString+"'");
 //fixTBL->Filter+=AnsiString(" AND uom='"+podtlSQL->FieldByName("uom")->AsString+"'");
 //ShowMessage(fixTBL->Filter);
 //fixTBL->Active=true;
 //explor->gridDS->DataSet=fixTBL;
 //if(!fixTBL->Eof)explor->ShowModal();
 //fixTBL->First();
 if(!reqTBL->Eof)
  {
     if(Trim(reqTBL->FieldByName("gdnm")->AsString)+
      Trim(reqTBL->FieldByName("uom")->AsString)==
   Trim(podtlSQL->FieldByName("gdnm")->AsString)+
   Trim(podtlSQL->FieldByName("uom")->AsString))
   {
    reqTBL->Edit();
    reqTBL->FieldByName("rel")->AsFloat=relSTR;
    reqTBL->FieldByName("bal")->AsFloat=balSTR;
    reqTBL->Post();
    reqbalFIX();
   }
  }
 reqTBL->Active=false;
}
//---------------------------------------------------------
void TpoFRM::reqbalFIX()
{
 if(!reqTBL->Eof)
 {
 reqTBL->Edit();
 reqTBL->FieldByName("bal")->AsFloat=
 reqTBL->FieldByName("req")->AsFloat-
 reqTBL->FieldByName("rel")->AsFloat;
 reqTBL->Post();
 //explor->ShowModal();
 if(reqSQL->Active)refreshBTNClick(refreshBTN);//reqSQL->Refresh();
 }
}
//---------------------------------------------------------
void TpoFRM::reqPOST()
{
  relSTR=0;
  balSTR=0;
  //locate reqno,gdnm,uom
  Variant locVAL[3];
  locVAL[0] = podtlSQL->FieldByName("reqno")->AsString;
  locVAL[1] = podtlSQL->FieldByName("gdnm")->AsString;
  locVAL[2] = podtlSQL->FieldByName("uom")->AsString;

  //ganti reqTBL dengan fixTBL
  //ganti lagi fixTBL dengan regTBL
  reqTBL->Active=false;
  reqTBL->Filtered=true;
  reqTBL->Filter=AnsiString(" reqno ='"+podtlSQL->FieldByName("reqno")->AsString+"' ");
  reqTBL->Filter+=AnsiString(" AND refno=Null  ");
  reqTBL->Filter+=AnsiString(" AND gdnm='"+podtlSQL->FieldByName("gdnm")->AsString+"'");
  reqTBL->Filter+=AnsiString(" AND uom='"+podtlSQL->FieldByName("uom")->AsString+"'");
  //ShowMessage(reqTBL->Filter);
  reqTBL->Active=true;
  //explor->gridDS->DataSet=reqTBL;
  //if(!reqTBL->Eof)explor->ShowModal();

  //fixTBL->First();
  if(!reqTBL->Eof)
  {
   if(Trim(reqTBL->FieldByName("gdnm")->AsString)+
      Trim(reqTBL->FieldByName("uom")->AsString)==
   Trim(podtlSQL->FieldByName("gdnm")->AsString)+
   Trim(podtlSQL->FieldByName("uom")->AsString))
   {
    relSTR=reqTBL->FieldByName("rel")->IsNull?0:reqTBL->FieldByName("rel")->AsFloat;
    relSTR+=podtlSQL->FieldByName("req")->AsFloat;
    relSTR-=reqOLD;

    balSTR=
    reqTBL->FieldByName("req")->AsFloat-relSTR;

    tblFIX();
    //reqbalFIX();

   }
  }
}
//---------------------------------------------------------
void TpoFRM::doDEL()
{
 //condition when allowing deleting
 if(doSQL->FieldByName("db")->AsFloat<=0)doSQL->Delete();
}
//---------------------------------------------------------
void TpoFRM::poPOS()
{
  //locate reqno,gdnm,uom
  Variant locVAL[2];
  locVAL[0] = Trim(podtlSQL->FieldByName("reqno")->AsString);
  locVAL[1] = Trim(podtlSQL->FieldByName("gdnm")->AsString);
  locVAL[2] = Trim(podtlSQL->FieldByName("uom")->AsString);
  fixTBL->First();
  if(!fixTBL->Eof && func->VarLOCATE("reqno;gdnm;uom",locVAL,2,fixTBL))
  {
   fixTBL->Edit();
   fixTBL->FieldByName("rel")->AsFloat=
   podtlSQL->FieldByName("req")->AsFloat;
   fixTBL->Post();
   reqbalFIX();
  }
}
//---------------------------------------------------------
void TpoFRM::doPOS()
{
 //nilai asli tadi sudah dicatat pada doOLD
 //if(doDS->State==dsEdit || doDS->State==dsInsert)doSQL->Post();
 relQTY=0;
 baru=doSQL->FieldByName("db")->AsFloat;
 //update pada PO untuk gdnm,uom,reqno

 //locate ->ganti pake filter
 podtlSQL->First();
 Variant locVAL[3];
 locVAL[0]=Trim(doSQL->FieldByName("req")->AsString);
 locVAL[1]=Trim(doSQL->FieldByName("gdnm")->AsString);
 locVAL[2]=Trim(doSQL->FieldByName("unit")->AsString);

 podtlSQL->Filtered=true;
 podtlSQL->Filter=AnsiString(" refno='"+Trim(doSQL->FieldByName("req")->AsString)+"*'");
 podtlSQL->Filter+=AnsiString(" AND gdnm='"+Trim(doSQL->FieldByName("gdnm")->AsString)+"'");
 podtlSQL->Filter+=AnsiString(" AND ='"+Trim(doSQL->FieldByName("unit")->AsString)+"*'");
 podtlSQL->Refresh();
 //explor->gridDS->DataSet=podtlSQL;
 //if(!podtlSQL->Eof)explor->ShowModal();

 //ShowMessage(" Filter STR :"+podtlSQL->Filter+
 //           "\nFiltered   :"+IntToStr(podtlSQL->RecordCount));

 if(!podtlSQL->Eof && Trim(podtlSQL->FieldByName("refno")->AsString)==Trim(doSQL->FieldByName("req")->AsString))
  {
   if(baru<=0)dounPOSTIT();
   else doPOSTIT();
  }
 podtlSQL->Filter="";
 podtlSQL->Filtered=false;
 //doSQL->Post();
}
//---------------------------------------------------------
void TpoFRM::doOPEN()
{
 doSQL->Close();
 doSQL->ParamByName("PARENT")->AsString=
 poEDT->Text;
 doSQL->Open();
}
//---------------------------------------------------------
void TpoFRM::pickmeUP()
{
    pickform->Query1->Close();
    pickform->Query1->DatabaseName=mnuFRM->path_+"\\DATA";
    pickform->Query1->SQL->Clear();
    pickform->Query1->SQL->Add(" select distinct trim(book.'gdnm') as gdnm,uom as unit from 'dtlreq.dbe' book ");
    pickform->Query1->SQL->Add(" where book.'bal'>0 and book.'refno' is not null ");
    pickform->Query1->SQL->Add(" and refno='"+Trim(poEDT->Text)+"'");
    pickform->findEDT->Text="";
    pickform->DBGrid1->DataSource->DataSet->Open();
    pickform->DBGrid1->Columns->Items[0]->Title->Caption="Description";
    pickform->DBGrid1->Columns->Items[0]->FieldName="gdnm";
    pickform->DBGrid1->SelectedIndex=0;

    doSQL->Edit();
    if(pickform->ShowModal())
     {
      doSQL->FieldByName("gdnm")->AsString=
      pickform->Query1->FieldByName("gdnm")->AsString;
      doSQL->FieldByName("unit")->AsString=
      pickform->Query1->FieldByName("unit")->AsString;
     }
}
//---------------------------------------------------------
void TpoFRM::reqRPT()
{
 teksHTML=new TStringList();teksHTML->Clear();
 teksEDIT=new TStringList();teksEDIT->Clear();
 teksTXT=new TStringList();teksTXT->Clear();
 teksRPT=new TStringList();teksRPT->Clear();

 PERIOD=FormatDateTime(" dd mmmm yyyy",mnuFRM->stDT);
 //buat mencetak formulir ini
 pathAPP_=mnuFRM->pathAPP_;
 prtFRM->pathAPP_=pathAPP_;
 //pathPRT=pathAPP_+"\\REPORT\\";
 pathPRT="c:\\temp\\";
 prtFRM->pathPRT=pathPRT;
 pathHTML=pathAPP_+"\\HTML\\POFRM\\";
 prtFRM->pathHTML=pathHTML;
 //pathRPT=pathAPP_+"\\TXT\\";
 pathRPT="c:\\temp\\";
 //ShowMessage(pathAPP_+"\n"+pathPRT+"\n"+pathHTML);
 AnsiString s_=InputBox("Masukkan Judul Report","" ,"PO No. "+Trim(poEDT->Text));
 AnsiString prtME=pathPRT+s_+".HTML";
 AnsiString txtME=pathPRT+s_+".TXT";
 //AnsiString rptME=pathRPT+s_+".TXT";
 AnsiString rptME="c:\\temp\\report.txt";
 //will be using TStringList or TMemo
 teksHTML->Clear();
 teksEDIT->Clear();
 teksTXT->Clear();
 teksRPT->Clear();
 //disini untuk yang buat laporan dong

 //cetak nomerpo
 teksRPT->Add(s_);
 teksRPT->Add(poEDT->Text);
 //cetak periode
 teksRPT->Add(PERIOD);
 teksRPT->Add("\n\r");
 //cetak vd
 teksRPT->Add(nmCMB->Text);
 //cetak vd address, cty, cntry
 teksRPT->Add(Trim(addrEDT->Text)+", "+Trim(ctyEDT->Text));
 //cetak ph, fax
 teksRPT->Add(Trim(phEDT->Text)+", "+Trim(fxEDT->Text));
 teksRPT->Add("\n\r");

 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select distinct ");
 tmpSQL->SQL->Add(" gdnm,uom,sum(req) as qty,cur,price, ");
 tmpSQL->SQL->Add(" sum(req*price) as ttl ");
 tmpSQL->SQL->Add(" from dtlreq.dbe  ");
 tmpSQL->SQL->Add(" where refno=:PO ");
 tmpSQL->SQL->Add(" group by gdnm,uom,cur,price ");
 tmpSQL->ParamByName("PO")->AsString=Trim(poEDT->Text);
 tmpSQL->Open();
 int cntr=0;
 while(!tmpSQL->Eof)
  {
   //cetak detail po
   AnsiString raw="";
   raw+=IntToStr(++cntr)+" \t";
   raw+=tmpSQL->FieldByName("GDNM")->AsString+" \t";
   raw+=tmpSQL->FieldByName("UOM")->AsString+" \t";
   raw+=tmpSQL->FieldByName("QTY")->AsString+" \t";
   raw+=tmpSQL->FieldByName("CUR")->AsString+" \t";
   raw+=tmpSQL->FieldByName("PRICE")->AsString+" \t";
   raw+=" \t";
   raw+=tmpSQL->FieldByName("TTL")->AsString;
   teksRPT->Add(raw);
   tmpSQL->Next();
  }

  tmpSQL->Close();

 //create hard print
 ForceDirectories(pathPRT);
 teksHTML->SaveToFile(prtME);
 teksTXT->SaveToFile(txtME);
 teksRPT->SaveToFile(rptME);
 teksHTML->Clear();
 teksEDIT->Clear();
 teksTXT->Clear();
 teksRPT->Clear();

 AnsiString app_="EXCEL.EXE";
 AnsiString txt_=txtME;
 AnsiString mod_="open";
 pathPRT=pathAPP_+"\\FORM"  ;
 txt_=pathPRT+"\\GETPO.XLS";


 //print using shellobj
 ShellExecute(
    Handle,	// handle to parent window
    mod_.c_str(),	// pointer to string that specifies operation to perform
    txt_.c_str(),	// pointer to filename or folder name string
    app_.c_str(),	// pointer to string that specifies executable-file parameters
    pathPRT.c_str(),	// pointer to string that specifies default directory
    SW_SHOW 	// whether file is shown when opened
   );

}
//---------------------------------------------------------
void TpoFRM::dafRPT()
{
 teksHTML=new TStringList();teksHTML->Clear();
 teksEDIT=new TStringList();teksEDIT->Clear();
 teksTXT=new TStringList();teksTXT->Clear();
 teksRPT=new TStringList();teksRPT->Clear();

 PERIOD=FormatDateTime(" dd mmmm ",mnuFRM->stDT)+" - "+FormatDateTime("dd mmmm yyyy ",mnuFRM->lstDT);
 //buat mencetak formulir ini
 pathAPP_=mnuFRM->pathAPP_;
 prtFRM->pathAPP_=pathAPP_;
 //pathPRT=pathAPP_+"\\REPORT\\";
 pathPRT="c:\\temp\\";
 prtFRM->pathPRT=pathPRT;
 pathHTML=pathAPP_+"\\HTML\\PODAF\\";
 prtFRM->pathHTML=pathHTML;
 //pathRPT=pathAPP_+"\\TXT\\";
 pathRPT="c:\\temp\\";
 //ShowMessage(pathAPP_+"\n"+pathPRT+"\n"+pathHTML);
 AnsiString s_=InputBox("Masukkan Judul Report","" ,"Daftar Requisition Periode "+PERIOD);
 AnsiString prtME=pathPRT+s_+".HTML";
 AnsiString txtME=pathPRT+s_+".TXT";
 //AnsiString rptME=pathRPT+s_+".TXT";
 AnsiString rptME="c:\\temp\\report.txt";
 //will be using TStringList or TMemo
 teksHTML->Clear();
 teksEDIT->Clear();
 teksTXT->Clear();
 teksRPT->Clear();
 //heading up
 teksRPT->Add(s_);
 prtFRM->topPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 func->replaceSTR(teksHTML,"@PERIODE@",PERIOD);
 func->replaceSTR(teksTXT,"@PERIODE@",PERIOD);
 prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //func->replaceSTR(teksRPT,"@PERIODE@",PERIOD);
 //print specific detail
 cntr=0;
 rowDAF();
 //footer
 prtFRM->botPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //disini untuk yang buat laporan dong
 //create hard print
 ForceDirectories(pathPRT);
 ForceDirectories(pathRPT);
 teksHTML->SaveToFile(prtME);
 //teksTXT->SaveToFile(txtME);
 teksRPT->SaveToFile(rptME);
 teksHTML->Clear();
 teksEDIT->Clear();
 teksTXT->Clear();
 teksRPT->Clear();
 AnsiString pil = "3";
//            pil = InputBox("Open File With",
//                  " [1] XL-2000 , [2] NOTEPAD , [3] XL-98, Print By. [?]:",
//                  pil);
 AnsiString app_="EXPLORER.EXE";
 if(StrToInt(pil)==1)app_="EXCEL.EXE";
 if(StrToInt(pil)==3)app_="EXCEL.EXE";
 AnsiString txt_=txtME;
 if(StrToInt(pil)==1)txt_=prtME;
 if(StrToInt(pil)==3)txt_=txtME;
 AnsiString mod_="open";
 if(StrToInt(pil)==1)mod_="edit";
 if(StrToInt(pil)==3)
  {
   pathPRT=pathAPP_+"\\FORM"  ;
   txt_=pathPRT+"\\GETPOL.XLS";
  }

 //print using shellobj
 ShellExecute(
    Handle,	// handle to parent window
    mod_.c_str(),	// pointer to string that specifies operation to perform
    txt_.c_str(),	// pointer to filename or folder name string
    app_.c_str(),	// pointer to string that specifies executable-file parameters
    pathPRT.c_str(),	// pointer to string that specifies default directory
    SW_SHOW 	// whether file is shown when opened
   );

}
//---------------------------------------------------------
void TpoFRM::getIT()
{
 multi=true;
 TBookmarkList *raw=reqGRD->SelectedRows;
 for(int i=1;i<=raw->Count;i++)
  {
   reqSQL->Bookmark=raw->Items[i-1];
   podtlSQL->Append();
   podtlSQL->Edit();
   podtlSQL->FieldByName("gdnm")->AsString=
   reqSQL->FieldByName("gdnm")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("uom")->AsString=
   reqSQL->FieldByName("uom")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("refno")->AsString=
   poSQL->FieldByName("trfno")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("cur")->AsString=
   reqSQL->FieldByName("cur")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("price")->AsString=
   reqSQL->FieldByName("price")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("reqno")->AsString=
   reqSQL->FieldByName("reqno")->AsString;
   podtlSQL->Edit();
   podtlSQL->FieldByName("req")->AsString=
   reqSQL->FieldByName("bal")->AsString;
   podtlSQL->Post();
   //poPOS();
  }
}
//---------------------------------------------------------
bool TpoFRM::dtlUPDATE()
{
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" update dtlreq.dbe set refno=:NEWREF where refno='"+refOLD+"' ");
 tmpSQL->ParamByName("NEWREF")->AsString=
 poEDT->Text;
 tmpSQL->ExecSQL();
}
//---------------------------------------------------------
void TpoFRM::refCHG()
{
 refNEW=poEDT->Text;
 if(Trim(refOLD)!=Trim(refNEW) && Trim(refOLD)!="")
  {
   if(MessageDlg("Change Number to "+poEDT->Text+" ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)
    {
     poEDT->Text=refOLD;
     return;
    }
   else dtlUPDATE();
  }
}
//---------------------------------------------------------
bool TpoFRM::onPOST()
{
 bool retu_;
 if(Trim(poSQL->FieldByName("reqdt")->AsString)=="")retu_=true;
 return(retu_);
}
//---------------------------------------------------------
bool TpoFRM::allowPOST()
{
 bool retu;
 if(!onPOST())ShowMessage("Already PROCESS Edit not Allowed");
 else retu=true;
 return(retu);
}
//---------------------------------------------------------
void TpoFRM::reqDEL()
{
 if(!allowPOST())
  {
   poSQL->Cancel();
   return;
  }
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" delete from dtlreq.dbe where refno=:TRFNO ");
 tmpSQL->ParamByName("TRFNO")->AsString=
 poEDT->Text;
 tmpSQL->Prepare();
 tmpSQL->ExecSQL();
 tmpSQL->Close();

 poSQL->Delete();
 tmpSQL->Close();
}
//---------------------------------------------------------
void TpoFRM::itemDEL()
{
 //request header can be deleted if not posting yet
 //
 /*
 if(!allowPOST())
  {
   if(master)poSQL->Cancel();
   else podtlSQL->Cancel();
   return;
  }
 */
 AnsiString cat_;
 cat_=!master?" Item":" Purchase Form ";
 if(MessageDlg("Delete this "+cat_+"?", mtConfirmation,
    TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
   {
    if(doGRD->Focused())doDEL();
    else
    {
     if(!master)poDEL();
     else reqDEL();
    }
   }
}
//---------------------------------------------------------
void TpoFRM::setVD()
{
 //tujuannya biar bisa isi vendor on the fly
 //in case user males bolak-balik ke table vendor
 //gak ding, incase barang datang langsung ke reciving tanpa PO
 //emang bisa kah?
 vdSQL->Open();
 while(!vdSQL->Eof)
 {
  nmCMB->Items->Add(vdSQL->FieldByName("nm")->AsString.Trim());
  vdVAL->Add(vdSQL->FieldByName("vdrec")->AsString);
  vdSQL->Next();
 }
vdSQL->Close();
}
//---------------------------------------------------------
void TpoFRM::statusSAY()
{
 AnsiString s="";
 stsTEXT->Text=s;
 s=func->fndLST(mnuFRM->cmbSTATUS,mnuFRM->ivSTATUS,true,reqSQL->FieldByName("state")->AsString);
 stsTEXT->Text=s;
}
//---------------------------------------------------------
void TpoFRM::key13(TObject *Sender,char Key)
{
 if(int(Key)==13)
  {
   if(poEDT->Focused()){ poDT->SetFocus();return;}
   if(poDT->Focused()){needDT->SetFocus();return;}
   if(needDT->Focused()){nmCMB->SetFocus();return;}
   if(nmCMB->Focused()){addrEDT->SetFocus();return;}
   if(addrEDT->Focused()){ctyEDT->SetFocus();return;}
   if(ctyEDT->Focused()){phEDT->SetFocus();return;}
   if(phEDT->Focused()){fxEDT->SetFocus();return;}
   if(fxEDT->Focused()){byEDT->SetFocus();return;}
   if(byEDT->Focused()){byttlEDT->SetFocus();return;}
   if(byttlEDT->Focused()){apEDT->SetFocus();return;}
   if(apEDT->Focused()){apttlEDT->SetFocus();return;}
   if(apttlEDT->Focused()){poGRD->SetFocus();return;}
  }
}
//---------------------------------------------------------
void TpoFRM::gdFILL()
{
 if(!podtlSQL->FieldByName("gduom")->IsNull)
  {
   if(!podtlDS->State==dsEdit)podtlSQL->Edit();
   podtlSQL->FieldByName("UOM")->AsString=
   podtlSQL->FieldByName("gduom")->AsString;
  }
}
//---------------------------------------------------------
void TpoFRM::vdSAY()
{
 nmCMB->Text=poSQL->FieldByName("vdnm")->AsString;
 addrEDT->Text=poSQL->FieldByName("add1")->AsString;
 phEDT->Text=poSQL->FieldByName("ph")->AsString;
 fxEDT->Text=poSQL->FieldByName("fx")->AsString;
}
//---------------------------------------------------------
void TpoFRM::idFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM)
{
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=myCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,myVAL,false,myCMB->Text);
 if(poSQL->FieldByName(fieldNM)->AsString !=strNUM)
  {
   if(poDS->State!=dsEdit)poSQL->Edit();
   poSQL->FieldByName(fieldNM)->AsFloat=
   StrToInt(strNUM);
   //rcvSQL->Post();
  }
}
//---------------------------------------------------------
void TpoFRM::vdADD()
{
 vdNEW=true;
 if(!vdSQL->Active)vdSQL->Open();
 vdSQL->Append();
 vdSQL->Edit();
 vdSQL->FieldByName("nm")->AsString=
 nmCMB->Text;
 vdSQL->Post();
}
//---------------------------------------------------------
void TpoFRM::varFILL()
{
 strDATE=DateToStr(poDT->Date);
}
//---------------------------------------------------------
void TpoFRM::dtlPOST()
{
 if(podtlDS->State==dsEdit || podtlDS->State==dsInsert)podtlSQL->Post();
}
//---------------------------------------------------------
void TpoFRM::poPOST()
{
 if(poDS->State==dsEdit || poDS->State==dsInsert)poSQL->Post();
  if(vdNEW)
  {
   //assume that vendor on record position
   vdSQL->Edit();
   vdSQL->FieldByName("cty")->AsString=ctyEDT->Text;
   vdSQL->FieldByName("add1")->AsString=addrEDT->Text;
   vdSQL->FieldByName("ph")->AsString=phEDT->Text;
   vdSQL->FieldByName("fx")->AsString=fxEDT->Text;
   vdSQL->Post();
   vdSQL->Close();
   vdNEW=false;
  }
  poSQLAfterScroll(poSQL);
}
//---------------------------------------------------------
void TpoFRM::isDATE()
{
 AnsiString dt="";
 if(!poSQL->FieldByName("trfDT")->IsNull)
    dt=DateToStr(poSQL->FieldByName("trfDT")->Value);
 if(dt!=DateToStr(poDT->Date))
  {
   if(poDS->State!=dsEdit)poSQL->Edit();
   poSQL->FieldByName("trfdt")->Value=poDT->Date;
  }

 if(!poSQL->FieldByName("needed")->IsNull)
    dt=DateToStr(poSQL->FieldByName("needed")->Value);
 if(dt!=DateToStr(needDT->Date))
  {
   if(poDS->State!=dsEdit)poSQL->Edit();
   poSQL->FieldByName("needed")->Value=needDT->Date;
  }

}
//---------------------------------------------------------
void TpoFRM::showPO()
{
 if(!poSQL->FieldByName("trfdt")->IsNull)
   poDT->Date=poSQL->FieldByName("trfdt")->Value;
 if(!poSQL->FieldByName("needed")->IsNull)
   needDT->Date=poSQL->FieldByName("needed")->Value;

 AnsiString s="";
 stsTEXT->Text=s;
 s=func->fndLST(mnuFRM->cmbSTATUS,mnuFRM->ivSTATUS,true,poSQL->FieldByName("state")->AsString);
 stsTEXT->Text=s;

 vdSAY();

}
//---------------------------------------------------------
void TpoFRM::reqGO(AnsiString seekSTR)
{
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 reqSQL->First();
 reqSQL->Locate("trfno",seekSTR,Opts);
}
//---------------------------------------------------------
bool TpoFRM::isEXIST(AnsiString seekSTR)
{
 //find reqno at numSQL
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 numSQL->First();
 return(numSQL->Locate("trfno",seekSTR,Opts));
}
//---------------------------------------------------------
void TpoFRM::numNEW()
{
 poSQL->Append();
 numSQL->Refresh();
 numSQL->Last();
 //oke tampilin nomer terakhir brow
 refNEW = numSQL->FieldByName("trfno")->AsString;
 refNEW = InputBox("Input Req# No.", " Last No. ="+
          Trim(numSQL->FieldByName("trfno")->AsString)+
          ", Enter New No. :",
          refNEW);
 poSQL->FieldByName("trfno")->AsString=refNEW;
 poEDT->Text=refNEW;
 if(isEXIST(refNEW))
   {
    poSQL->Cancel();
    reqGO(refNEW);
   }
 poEDT->SetFocus();
}
//---------------------------------------------------------
void TpoFRM::numOPEN()
{
 numSQL->Close();
 numSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 numSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 numSQL->Open();
}
//---------------------------------------------------------
void TpoFRM::itemADD()
{
 if(reqPG->ActivePage==reqSHEET)
  {
   AnsiString cat_=" Form Purchase";
   if(!master) cat_=" Item ";
   if(MessageDlg("Add New "+cat_+"?", mtConfirmation,
      TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {
     if(!master)
      {
      //podtlSQL->Append();
      }
     else
      {
       //poSQL->Append();
       if(!numSQL->Active)numOPEN();
       numNEW();
      }
    }
  }
}
//---------------------------------------------------------
void TpoFRM::dtlOPEN()
{
 podtlSQL->Close();
 podtlSQL->ParamByName("PO")->AsString=
 poSQL->FieldByName("trfno")->AsString;
 podtlSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 podtlSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 podtlSQL->Open();
}
//---------------------------------------------------------
void TpoFRM::reqOPEN()
{
 AnsiString ord_=" order by gdnm,reqno ";
 //use to open req
 //param to dept
 //div
 //reqno if specific
 reqSQL->Close();
 reqSQL->SQL->Clear();
 reqSQL->SQL->Add(reqORG);
 if(reqCMB->Text!="")
  {
   reqSQL->SQL->Add(" and refno=:REQ ");
   reqSQL->ParamByName("REQ")->AsString=
   Trim(reqCMB->Text);
  }
 reqSQL->SQL->Add(ord_);
 //ShowMessage(reqSQL->SQL->Text);
 reqSQL->Open();
}
//---------------------------------------------------------
void TpoFRM::tbOPEN()
{
 path_=mnuFRM->path_;
 reqTBL->DatabaseName=path_+"\\DATA";
 rptSQL->DatabaseName=path_+"\\DATA";
 fixTBL->DatabaseName=path_+"\\DATA";
 fixSQL->DatabaseName=path_+"\\DATA";
 doSQL->DatabaseName=path_+"\\DATA";
 gdSQL->DatabaseName=path_+"\\DATA";
 numSQL->DatabaseName=path_+"\\DATA";
 tmpSQL->DatabaseName=path_+"\\DATA";
 podtlSQL->DatabaseName=path_+"\\DATA";
 poSQL->DatabaseName=path_+"\\DATA";
 reqSQL->DatabaseName=path_+"\\DATA";
 vdSQL->DatabaseName=path_+"\\DATA";
}
//---------------------------------------------------------
void TpoFRM::tbCLOSE()
{
reqTBL->Close();
rptSQL->Close();
fixTBL->Close();
fixSQL->Close();
doSQL->Close();
gdSQL->Close();
numSQL->Close();
tmpSQL->Close();
vdSQL->Close();
podtlSQL->Close();
poSQL->Close();
reqSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::FormClose(TObject *Sender, TCloseAction &Action)
{
tbCLOSE();
reqSQL->SQL->Text=reqORG;
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::FormActivate(TObject *Sender)
{
 tbCLOSE();
 tbOPEN();
 //fixSQL->Open();
 setVD();
 reqORG=reqSQL->SQL->Text;
 poSQL->Close();
 poSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 poSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 poSQL->Open();
 dtlOPEN();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::refreshBTNClick(TObject *Sender)
{
 reqOPEN();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::frmPGEnter(TObject *Sender)
{
 if(frmPG->ActivePage==pickSHEET )
  {
   if(!reqSQL->Active)reqOPEN();
  }
 else reqSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::addBTNClick(TObject *Sender)
{
itemADD();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poEDTEnter(TObject *Sender)
{
catNAV->DataSource=poDS;
master=true;
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poGRDEnter(TObject *Sender)
{
master=false;
poPOST();
poSQLAfterScroll(poSQL);
varFILL();

//dtlOPEN();
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::podtlSQLNewRecord(TDataSet *DataSet)
{
// if(master)
//  {
   if(poGRD->Focused())
   {
    podtlSQL->Cancel();
    return;
   }
   //if(podtlDS->State!=dsEdit)podtlSQL->Edit();
   podtlSQL->FieldByName("refno")->AsString=
   poEDT->Text;
   podtlSQL->FieldByName("refdt")->AsString=
   poSQL->FieldByName("trfdt")->AsString;
   podtlSQL->FieldByName("rel")->AsFloat=0;
   podtlSQL->FieldByName("bal")->AsFloat=0;
   podtlSQL->FieldByName("req")->AsFloat=0;
//  }
// else podtlSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poSQLAfterScroll(TDataSet *DataSet)
{
 dtlOPEN();
 showPO();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 {
  poDTExit(Sender);
  needDT->SetFocus();
 }
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::nmCMBKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
  {
  if(!func->isFILL(nmCMB))
   {
    //oke tar lu save dah gue ke vendor oyi
    vdADD();
    vdVAL->Add(vdSQL->FieldByName("vdrec")->AsString);
    //trus isi dah cmb gua dengan lu punya vendor
    nmCMB->Text=vdSQL->FieldByName("nm")->AsString;
   }
   nmCMBExit(Sender);
   addrEDT->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::nmCMBExit(TObject *Sender)
{
 //disini nanti dicek apabila
 //ga ketemu vendor berarti dia mo isi baru kali
 //so should be available to append vendor
 poCHG=true;
 idFILL(nmCMB,vdVAL,nmCMB->Text,"VD");
 //dah itu tampilkan dong id ku
 vdSAY();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLgdChange(TField *Sender)
{
 //gdFILL();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLreqChange(TField *Sender)
{
//assume for new request
// if(podtlDS->State!=dsEdit)podtlSQL->Edit();
if(podtlSQL->FieldByName("rel")->AsFloat>0)
{
 podtlSQL->Cancel();
 return;
}

if(podtlDS->State==dsEdit ||
   podtlDS->State==dsInsert &&
   podtlSQL->FieldByName("rel")->AsFloat<=0)
 {
  //dtlPOST();
  //ShowMessage("isi req :"+podtlSQL->FieldByName("req")->AsFloat);
  reqPOST();
  podtlSQL->Edit();
  podtlSQL->FieldByName("bal")->AsFloat=
  podtlSQL->FieldByName("req")->AsFloat-
  podtlSQL->FieldByName("rel")->AsFloat;
  podtlSQL->Post();
  podtlSQL->Edit();
 }
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poGRDColEnter(TObject *Sender)
{
 bool go;
 if(podtlSQL->FieldByName("bal")->AsFloat!=0)go=true;
 if(UpperCase(poGRD->SelectedField->FieldName)=="BAL")go=false;
 if(UpperCase(poGRD->SelectedField->FieldName)=="REL")go=false;
 //
 if(!go)return;
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poEDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)refCHG();
key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poDTExit(TObject *Sender)
{
 poCHG=true;
 isDATE();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::needDTExit(TObject *Sender)
{
 poCHG=true;
 isDATE();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::needDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 {
  needDTExit(Sender);
  nmCMB->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::FormCreate(TObject *Sender)
{
 vdVAL=new TStringList();vdVAL->Clear();
 fndCMB->Items->Clear(); fndCMB->Items->Add("Form Purchase No.");
 fndVAL=new TStringList();fndVAL->Clear();
 fndVAL->Add("trfno");
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::addrEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::ctyEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::phEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::fxEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::byEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::byttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::apEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::apttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poSQLNewRecord(TDataSet *DataSet)
{
 if(poDS->State!=dsEdit)poSQL->Edit();
 poSQL->FieldByName("mode")->AsFloat=1;
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Shift.Contains(ssAlt) && Key==int('1') ) reqPG->ActivePage=listSHEET;
 if (Shift.Contains(ssAlt) && Key==int('2') ) reqPG->ActivePage=reqSHEET;
 if (Shift.Contains(ssAlt) && Key==int('3') ) frmPG->ActivePage=frmSHEET;
 if (Shift.Contains(ssAlt) && Key==int('4') ) frmPG->ActivePage=pickSHEET;

 if(Key==107 && Shift.Contains(ssCtrl))itemADD();
 if(Key==109 && Shift.Contains(ssCtrl))itemDEL();

}
//---------------------------------------------------------------------------


void __fastcall TpoFRM::poSQLBeforePost(TDataSet *DataSet)
{
if(!allowPOST())poSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poSQLBeforeEdit(TDataSet *DataSet)
{
if(!allowPOST())poSQL->Cancel();
refOLD=Trim(poEDT->Text);
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poSQLAfterEdit(TDataSet *DataSet)
{
if(!allowPOST())poSQL->Cancel();    
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::poSQLBeforeDelete(TDataSet *DataSet)
{
if(!allowPOST())poSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLBeforeEdit(TDataSet *DataSet)
{
 //if(!allowPOST())podtlSQL->Cancel();
 reqOLD=podtlSQL->FieldByName("req")->AsFloat;
 if(podtlSQL->FieldByName("rel")->AsFloat>0)
 {
 podtlSQL->Cancel();
 return;
 }
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLAfterEdit(TDataSet *DataSet)
{
//if(!allowPOST())podtlSQL->Cancel();    
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLBeforePost(TDataSet *DataSet)
{
 //if(!allowPOST())podtlSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::podtlSQLBeforeDelete(TDataSet *DataSet)
{
 //if(!allowPOST())podtlSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::fndEDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 func->locateIT(fndCMB,fndEDT,poSQL,fndVAL,AnsiString("NONEED"));
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::getBTNClick(TObject *Sender)
{
getIT();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::prtBTNClick(TObject *Sender)
{
if(reqPG->ActivePage==reqSHEET)reqRPT();
else
   if(reqPG->ActivePage==listSHEET)dafRPT();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::reqPGChange(TObject *Sender)
{
 if(reqPG->ActivePage==reqSHEET)catNAV->DataSource=poDS;
 if(reqPG->ActivePage==listSHEET)catNAV->DataSource=poDS;
 if(poGRD->Focused())catNAV->DataSource=podtlDS;
 if(reqPG->ActivePage==doSHEET)catNAV->DataSource=doDS;
 if(reqPG->ActivePage==doSHEET)doOPEN();
 else doSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::doSQLBeforeEdit(TDataSet *DataSet)
{
 doOLD=doSQL->FieldByName("db")->AsFloat;
 gdnmOLD=doSQL->FieldByName("gdnm")->AsString;
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::doGRDKeyPress(TObject *Sender, char &Key)
{
 if(Key=='\\')
  if(UpperCase(doGRD->SelectedField->FieldName)=="GDNM")
   {
    pickmeUP();
    Key=0;
   }

}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::doSQLNewRecord(TDataSet *DataSet)
{
 doSQL->FieldByName("req")->AsString=
 poEDT->Text;
 doSQL->FieldByName("refdt")->AsString=DateToStr(Now());
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::doGRDExit(TObject *Sender)
{
 if(doDS->State==dsInsert || doDS->State==dsEdit)doSQL->Post();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::BitBtn1Click(TObject *Sender)
{
doPOS();
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::posBTNClick(TObject *Sender)
{
poPOS();
}
//---------------------------------------------------------------------------


void __fastcall TpoFRM::doSQLdbChange(TField *Sender)
{
//wa kalu gitu jangan kasih ubah kalu db>0
if(doOLD>0 && doSQL->FieldByName("db")->AsFloat>0)
 {
  doSQL->Cancel();
  return;
 }
if(doDS->State==dsEdit || doDS->State==dsInsert)doPOS();
}
//---------------------------------------------------------------------------
void __fastcall TpoFRM::filterEDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 {
  if(Trim(filterEDT->Text)!="")
   {
    reqSQL->Filtered=true;
    reqSQL->Filter=" gdnm='"+Trim(filterEDT->Text)+"*'";
   }
  else
   {
    reqSQL->Filtered=false;
    reqSQL->Filter="";
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TpoFRM::vdSQLAfterPost(TDataSet *DataSet)
{
if(vdSQL->FieldByName("vdrec")->IsNull)
  {
   vdSQL->Edit();
   vdSQL->FieldByName("vdrec")->AsFloat=
   vdSQL->RecNo;
   vdSQL->Post();
  }    
}
//---------------------------------------------------------------------------

