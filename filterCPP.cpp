//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <FileCtrl.hpp>
#include <shlobj.hpp>

#include "filterCPP.h"
#include "funcCPP.h"
#include "mnuCPP.h"
#include "validCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TprtFRM *prtFRM;
//---------------------------------------------------------------------------
__fastcall TprtFRM::TprtFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void TprtFRM::validPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT)
{
 botPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 TvalFRM *FRM=new TvalFRM(Application);
 FRM->ShowModal();
 func->replaceSTR(teksHTML,"@BY@",FRM->byEDT->Text);
 func->replaceSTR(teksHTML,"@ACK@",FRM->ackEDT->Text);
 func->replaceSTR(teksHTML,"@APR@",FRM->appEDT->Text);
 func->replaceSTR(teksHTML,"@BYTTL@",FRM->byttlEDT->Text);
 func->replaceSTR(teksHTML,"@ACKTTL@",FRM->ackttlEDT->Text);
 func->replaceSTR(teksHTML,"@APRTTL@",FRM->appttlEDT->Text);

 func->replaceSTR(teksTXT,"@BY@",FRM->byEDT->Text);
 func->replaceSTR(teksTXT,"@ACK@",FRM->ackEDT->Text);
 func->replaceSTR(teksTXT,"@APR@",FRM->appEDT->Text);
 func->replaceSTR(teksTXT,"@BYTTL@",FRM->byttlEDT->Text);
 func->replaceSTR(teksTXT,"@ACKTTL@",FRM->ackttlEDT->Text);
 func->replaceSTR(teksTXT,"@APRTTL@",FRM->appttlEDT->Text);

 teksRPT->Add(FRM->byEDT->Text);
 teksRPT->Add(FRM->byttlEDT->Text);
 teksRPT->Add(FRM->ackEDT->Text);
 teksRPT->Add(FRM->ackttlEDT->Text);
 teksRPT->Add(FRM->appEDT->Text);
 teksRPT->Add(FRM->appttlEDT->Text);
 teksRPT->Add("\r\n");
 teksRPT->Add("\r\n");

 delete FRM;
}
//---------------------------------------------------------
void TprtFRM::ctgryPRT(int &countr)
{
 AnsiString CAT=Trim(tmpSQL->FieldByName("PARENTAL")->AsString)+
                Trim(tmpSQL->FieldByName("CATEGORY")->AsString);
 teksEDIT->LoadFromFile(pathHTML+"ctgry.txt");
 AnsiString sayCAT=CAT;
 func->replaceSTR(teksEDIT,"@CTGRY@",sayCAT);
 teksHTML->Add(teksEDIT->Text);
 if(FileExists(pathHTML+"txtCAT.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtCAT.txt");
   func->replaceSTR(teksEDIT,"@CTGRY@",sayCAT);
   teksTXT->Add(teksEDIT->Text);
  }
 teksRPT->Add("Kategori :"+sayCAT);

 countr=0;
 while(!tmpSQL->Eof && CAT==Trim(tmpSQL->FieldByName("PARENTAL")->AsString)+
                            Trim(tmpSQL->FieldByName("CATEGORY")->AsString))  //and masih sama ctgry
  {
   dtlPRT(countr);
   tmpSQL->Next();
  }
  //cetak spare baris kosong deh
  teksEDIT->LoadFromFile(pathHTML+"empty.txt");
  teksRPT->Add("");
}
//---------------------------------------------------------
void TprtFRM::dtlPRT(int &countr)
{
 teksEDIT->LoadFromFile(pathHTML+"datarow.txt");
 //replacing formula
 func->replaceSTR(teksEDIT,"@NOMER@",IntToStr(++countr));
 func->replaceSTR(teksEDIT,"@ID@",tmpSQL->FieldByName("id")->AsString);
 func->replaceSTR(teksEDIT,"@DESCR@",tmpSQL->FieldByName("descr")->AsString);
 func->replaceSTR(teksEDIT,"@UNIT@",tmpSQL->FieldByName("unit")->AsString);
 func->replaceSTR(teksEDIT,"@BUY@",tmpSQL->FieldByName("buy")->AsString);
 teksHTML->Add(teksEDIT->Text);

 if(FileExists(pathHTML+"txtROW.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtROW.txt");
   //replacing formula
   func->replaceSTR(teksEDIT,"@NOMER@",IntToStr(countr));
   func->replaceSTR(teksEDIT,"@ID@",tmpSQL->FieldByName("id")->AsString);
   func->replaceSTR(teksEDIT,"@DESCR@",tmpSQL->FieldByName("descr")->AsString);
   func->replaceSTR(teksEDIT,"@UNIT@",tmpSQL->FieldByName("unit")->AsString);
   func->replaceSTR(teksEDIT,"@BUY@",tmpSQL->FieldByName("buy")->AsString);
   teksTXT->Add(teksEDIT->Text);
  }
  AnsiString raw="";
  raw+=IntToStr(countr)+"\t";
  raw+=tmpSQL->FieldByName("id")->AsString+"\t";
  raw+=tmpSQL->FieldByName("descr")->AsString+"\t";
  raw+=tmpSQL->FieldByName("unit")->AsString+"\t";
  raw+=tmpSQL->FieldByName("buy")->AsString+"\t";
  teksRPT->Add(raw);
}
//---------------------------------------------------------
void TprtFRM::tbOPEN()
{
 tmpSQL->DatabaseName=mnuFRM->path_+"\\DATA";
}
//---------------------------------------------------------
void TprtFRM::tbCLOSE()
{
 tmpSQL->Close();
}
//---------------------------------------------------------
void TprtFRM::itemPRT()
{
 //disini mencetak detail item beserta kategorinya
 tbOPEN();
 tmpSQL->Close();
 tmpSQL->SQL->Clear();

 tmpSQL->SQL->Add(" select ");
 tmpSQL->SQL->Add(" c.catparent as parental,");
 tmpSQL->SQL->Add(" c.catid as category ,");
 tmpSQL->SQL->Add(" i.id as id,");
 tmpSQL->SQL->Add(" i.nm as descr,");
 tmpSQL->SQL->Add(" i.uom as unit ,i.buy as buy ");
 tmpSQL->SQL->Add(" from catitem.dbe c ");
 tmpSQL->SQL->Add(" right outer join item.dbe i on c.catrec=i.ctgry ");
 tmpSQL->SQL->Add(" order by parental,category,id ");

 tmpSQL->Open();
 //AnsiString ctgry=tmpSQL->FieldByName("");
 int countr=0;
 while(!tmpSQL->Eof)
  {
   if(tmpSQL->FieldByName("descr")->AsString!="")
        ctgryPRT(countr);
   else tmpSQL->Next();
  }
 tmpSQL->Close();
}
//---------------------------------------------------------
void TprtFRM::botPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT)
{
 teksEDIT->LoadFromFile(pathHTML+"footer.txt");
 teksHTML->Add(teksEDIT->Text);
 teksEDIT->Clear();
 if(FileExists(pathHTML+"other.txt"))
    teksEDIT->LoadFromFile(pathHTML+"other.txt");
 teksHTML->Add(teksEDIT->Text);
 if(FileExists(pathHTML+"txtFOOT.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtFOOT.txt");
   teksTXT->Add(teksEDIT->Text);
  }
 //txtRPT->Add()
}
//---------------------------------------------------------
void TprtFRM::topPRT(TStringList *&teksHTML,TStringList *&teksEDIT,TStringList *&teksTXT,TStringList *&teksRPT)
{
 //globally printing out
 //1.header
 //meta
 //style
 //xmldef
 //tblset
 //colset
 //banner
 //head
 //colhead
 teksEDIT->LoadFromFile(pathHTML+"header.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"meta.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"style.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"xmldef.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"tblset.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"colset.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"banner.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"head.txt");teksHTML->Add(teksEDIT->Text);
 teksEDIT->LoadFromFile(pathHTML+"colhead.txt");teksHTML->Add(teksEDIT->Text);
 if(FileExists(pathHTML+"txtHEAD.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtHEAD.txt");
   teksTXT->Add(teksEDIT->Text);
  }
 AnsiString PERIOD=FormatDateTime("dd mmmm yyyy",mnuFRM->stDT)+" - "+
                   FormatDateTime("dd mmmm yyyy",mnuFRM->lstDT);
 teksRPT->Add(PERIOD+"\r\n");
 teksRPT->Add("\r\n");
}
//---------------------------------------------------------------------------
void TprtFRM::itemRPT()
{
 pathAPP_=mnuFRM->pathAPP_;
 pathPRT=pathAPP_+"\\REPORT\\";
 pathRPT=pathAPP_+"\\TXT\\";
 pathHTML=pathAPP_+"\\HTML\\STKLST\\";
 //ShowMessage(pathAPP_+"\n"+pathPRT+"\n"+pathHTML);
 AnsiString s_=InputBox("Masukkan Judul Report","" ,"Item List");
 AnsiString prtME=pathPRT+s_+".HTML";
 AnsiString txtME=pathPRT+s_+".TXT";
 AnsiString rptME=pathRPT+s_+".TXT";
 //ShowMessage(prtME);
 //will be using TStringList or TMemo
 teksTXT->Clear();
 teksHTML->Clear();
 teksEDIT->Clear();
 //heading up
 topPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //footer
 validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //print specific detail
 itemPRT();

 //create hard print
 ForceDirectories(pathPRT);

 teksHTML->SaveToFile(prtME);
 //teksTXT->SaveToFile(txtME);
 teksRPT->SaveToFile(rptME);
 teksRPT->Clear();
 teksTXT->Clear();
 teksHTML->Clear();
 teksEDIT->Clear();

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
   txt_=pathPRT+"\\GETITEM.XLS";
  }
 int wind_=SW_SHOW;
 if(StrToInt(pil)==3) wind_=SW_SHOWMINIMIZED;


 //ShowMessage("File "+txt_+"\nPrint By "+app_);

 //print using shellobj
 ShellExecute(
    Handle,	// handle to parent window
    mod_.c_str(),	// pointer to string that specifies operation to perform
    txt_.c_str(),	// pointer to filename or folder name string
    app_.c_str(),	// pointer to string that specifies executable-file parameters
    pathPRT.c_str(),	// pointer to string that specifies default directory
    wind_ 	// whether file is shown when opened
   );
}
//--------------------------------------------------------
void __fastcall TprtFRM::FormCreate(TObject *Sender)
{
 tbCLOSE();
 teksTXT=new TStringList();
 teksHTML=new TStringList();
 teksEDIT=new TStringList();
 teksRPT=new TStringList();
}
//---------------------------------------------------------------------------

void __fastcall TprtFRM::FormClose(TObject *Sender, TCloseAction &Action)
{
 tmpSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TprtFRM::FormActivate(TObject *Sender)
{
 tbOPEN();
}
//---------------------------------------------------------------------------

