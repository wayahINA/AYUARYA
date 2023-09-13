//---------------------------------------------------------------------------
#include <vcl.h>
#include <FileCtrl.hpp>
#pragma hdrstop

#include "reqCPP.h"
#include "mnuCPP.h"
#include "funcCPP.h"
#include "PICKCPP.h"
#include "filterCPP.h"
#include "BARCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TreqFRM *reqFRM;
//---------------------------------------------------------------------------
__fastcall TreqFRM::TreqFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TreqFRM::reqRPT()
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
 AnsiString s_=InputBox("Masukkan Judul Report","" ,"PR No. "+Trim(reqnoEDT->Text));
 TStringList *ss_=new TStringList();
 ss_->Clear();
 ss_->Add(s_);

 func->replaceSTR(ss_,AnsiString("/"),AnsiString("-") );
 s_=Trim(ss_->Text);

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
 teksRPT->Add(s_);
 //cetak periode
 //teksRPT->Add(PERIOD);
 teksRPT->Add("\n\r");
 //cetak reqno
 teksRPT->Add(reqnoEDT->Text);
 //cetak reqdt
 teksRPT->Add("'"+DateToStr(reqDT->Date));
 //cetak reqby
 teksRPT->Add(byEDT->Text);
 //cetak reqdept
 teksRPT->Add(deptCMB->Text);
 //cetak subdept
 teksRPT->Add(divCMB->Text);
 //
 teksRPT->Add("\n\r");

 /*
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select distinct ");
 tmpSQL->SQL->Add(" gdnm,uom,sum(req) as qty,cur,price, ");
 tmpSQL->SQL->Add(" sum(req*price) as ttl ");
 tmpSQL->SQL->Add(" from dtlreq.dbe  ");
 tmpSQL->SQL->Add(" where reqno=:PO ");
 tmpSQL->SQL->Add(" group by gdnm,uom,cur,price ");
 tmpSQL->ParamByName("PO")->AsString=Trim(reqnoEDT->Text);
 tmpSQL->Open();
 */
 dtlSQL->First();
 int cntr=0;
 while(!dtlSQL->Eof)
  {
   //cetak detail po
   AnsiString raw="";
   raw+=IntToStr(++cntr)+" \t";
   raw+=dtlSQL->FieldByName("GDNM")->AsString+" \t";
   raw+=dtlSQL->FieldByName("UOM")->AsString+" \t";
   raw+=dtlSQL->FieldByName("REQ")->AsString+" \t";
   raw+=dtlSQL->FieldByName("CUR")->AsString+" \t";
   raw+=dtlSQL->FieldByName("PRICE")->AsString+" \t";
   raw+=" \t";
   raw+=dtlSQL->FieldByName("TOTAL")->AsString;
   teksRPT->Add(raw);
   dtlSQL->Next();
  }

  tmpSQL->Close();
/*  if(noteMEMO->Lines->Count>0)
   teksRPT->Add("item validation--");
  //cetak validasi tabel
  for(int i=1;i<=noteMEMO->Lines->Count;i++)
  {
   teksRPT->Add("'"+noteMEMO->Lines->Strings[i-1]);
  }
  */
  teksRPT->Add("");
  if(othMEMO->Lines->Count>0)teksRPT->Add("other/note--");
  //cetak notes
  for(int i=1;i<=othMEMO->Lines->Count;i++)
  {
   teksRPT->Add("'"+othMEMO->Lines->Strings[i-1]);
  }
 dtlSQL->First();
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
 AnsiString txt_=rptME;
 AnsiString mod_="open";
 pathPRT=pathAPP_+"\\FORM"  ;
 txt_=pathPRT+"\\GETPR.XLS";


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
//---------------------------------------------------------------------------
void TreqFRM::setDT()
{
 /*count for empty refdt
   n thn set if necessary
   using tmpSQL*/
 countSQL->Close();
 countSQL->ParamByName("PARENT")->AsString=
 Trim(reqnoEDT->Text);
 //countSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 //countSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 countSQL->Open();
 if(!countSQL->Eof)
 {
  while(!dtlSQL->Eof)
  {
   dtlSQL->Edit();
   dtlSQL->FieldByName("refdt")->AsString=
   reqSQL->FieldByName("reqdt")->AsString;
   dtlSQL->Post();
   dtlSQL->Next();
  }
 }
 countSQL->Close();
}
//---------------------------------------------------
void TreqFRM::dtlDAF()
{
   teksEDIT->LoadFromFile(pathHTML+"datarow.txt");
   func->replaceSTR(teksEDIT,"@CNTR@",IntToStr(++cntr));
   func->replaceSTR(teksEDIT,"@REFDT@",rptSQL->FieldByName("REFDT")->AsString);
   func->replaceSTR(teksEDIT,"@REFNO@",rptSQL->FieldByName("REFNO")->AsString);
   func->replaceSTR(teksEDIT,"@NEED@",rptSQL->FieldByName("NEEDED")->AsString);
   func->replaceSTR(teksEDIT,"@JOBID@",rptSQL->FieldByName("JOB")->AsString);
   func->replaceSTR(teksEDIT,"@WO@",rptSQL->FieldByName("WONO")->AsString);
   func->replaceSTR(teksEDIT,"@STATE@","");
   func->replaceSTR(teksEDIT,"@REQBY@",rptSQL->FieldByName("REQBY")->AsString);
   func->replaceSTR(teksEDIT,"@APPBY@",rptSQL->FieldByName("APBY")->AsString);
   teksHTML->Add(teksEDIT->Text);

   if(FileExists(pathHTML+"txtROW.txt"))
    {
     teksEDIT->LoadFromFile(pathHTML+"txtROW.txt");
     func->replaceSTR(teksEDIT,"@CNTR@",IntToStr(cntr));
     func->replaceSTR(teksEDIT,"@REFDT@",rptSQL->FieldByName("REFDT")->AsString);
     func->replaceSTR(teksEDIT,"@REFNO@",rptSQL->FieldByName("REFNO")->AsString);
     func->replaceSTR(teksEDIT,"@NEED@",rptSQL->FieldByName("NEEDED")->AsString);
     func->replaceSTR(teksEDIT,"@JOBID@",rptSQL->FieldByName("JOB")->AsString);
     func->replaceSTR(teksEDIT,"@WO@",rptSQL->FieldByName("WONO")->AsString);
     func->replaceSTR(teksEDIT,"@STATE@","");
     func->replaceSTR(teksEDIT,"@REQBY@",rptSQL->FieldByName("REQBY")->AsString);
     func->replaceSTR(teksEDIT,"@APPBY@",rptSQL->FieldByName("APBY")->AsString);
     teksTXT->Add(teksEDIT->Text);
    }
     AnsiString raw="";
     raw+="'"+IntToStr(cntr)+"\t";
     raw+="'"+rptSQL->FieldByName("REFDT")->AsString+"\t";
     raw+=rptSQL->FieldByName("REFNO")->AsString+"\t";
     raw+="'"+rptSQL->FieldByName("NEEDED")->AsString+"\t";
     raw+=rptSQL->FieldByName("JOB")->AsString+"\t";
     raw+=rptSQL->FieldByName("WONO")->AsString+"\t";
     raw+="\t";
     raw+=rptSQL->FieldByName("REQBY")->AsString+"\t";
     raw+=rptSQL->FieldByName("APBY")->AsString+"\t";
     teksRPT->Add(teksEDIT->Text);
}
//---------------------------------------------------
bool TreqFRM::sameCAT()
{
 return(func->strTRUE(CAT,Trim(rptSQL->FieldByName("DEPT")->AsString)+
                          Trim(rptSQL->FieldByName("DIV")->AsString)));
}
//---------------------------------------------------
void TreqFRM::depPRT()
{
 CAT=Trim(rptSQL->FieldByName("DEPT")->AsString)+
                Trim(rptSQL->FieldByName("DIV")->AsString);
 teksEDIT->LoadFromFile(pathHTML+"ctgry.txt");
 AnsiString sayCAT=Trim(rptSQL->FieldByName("DIVISI")->AsString);
 func->replaceSTR(teksEDIT,"@DEPT@",sayCAT);
 teksHTML->Add(teksEDIT->Text);
 if(FileExists(pathHTML+"txtCAT.txt"))
  {
   teksEDIT->LoadFromFile(pathHTML+"txtCAT.txt");
   func->replaceSTR(teksEDIT,"@DEPT@",sayCAT);
   teksTXT->Add(teksEDIT->Text);
  }
 teksRPT->Add("Location :"+sayCAT);

 cntr=0;
 while(!rptSQL->Eof && sameCAT())  //and masih sama ctgry
  {
   dtlDAF();
   rptSQL->Next();
  }
 //cetak spare baris kosong deh
 teksEDIT->LoadFromFile(pathHTML+"empty.txt");
 teksRPT->Add("\r\n");

}
//---------------------------------------------------
void TreqFRM::rowDAF()
{
 AnsiString old_=rptSQL->SQL->Text;
 AnsiString ord_=" order by dept,div,refdt";
 rptSQL->Close();
 rptSQL->SQL->Clear();
 rptSQL->SQL->Add(old_);
 rptSQL->SQL->Add(ord_);
 rptSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 rptSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 rptSQL->Open();
 while(!rptSQL->Eof)
  {
   //catat dept
   depPRT();
  }
 rptSQL->Close();
 rptSQL->SQL->Text=old_;
}
//---------------------------------------------------
void TreqFRM::dafRPT()
{
 teksHTML=new TStringList();teksHTML->Clear();
 teksEDIT=new TStringList();teksEDIT->Clear();
 teksTXT=new TStringList();teksTXT->Clear();
 teksRPT=new TStringList();teksRPT->Clear();

 PERIOD=FormatDateTime(" dd mmmm ",mnuFRM->stDT)+" - "+FormatDateTime("dd mmmm yyyy ",mnuFRM->lstDT);
 //buat mencetak formulir ini
 pathAPP_=mnuFRM->pathAPP_;
 prtFRM->pathAPP_=pathAPP_;
 pathPRT=pathAPP_+"\\REPORT\\";
 prtFRM->pathPRT=pathPRT;
 pathHTML=pathAPP_+"\\HTML\\REQDAF\\";
 prtFRM->pathHTML=pathHTML;
 //pathRPT=pathAPP_+"\\TXT\\";
 pathRPT="c:\\temp\\";
 prtFRM->pathRPT="c:\\temp\\";
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
 //func->replaceSTR(teksRPT,"@PERIODE@",PERIOD);
 //print specific detail
 cntr=0;
 prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 rowDAF();
 //footer
 prtFRM->botPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //disini untuk yang buat laporan dong
 //create hard print
 ForceDirectories(pathPRT);
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
   txt_=pathPRT+"\\GETREQ.XLS";
  }

 //ShowMessage("File "+txt_+"\nPrint By "+app_);

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
//---------------------------------------------------
void TreqFRM::noteFIX()
{
 /*
 mengganti note apabila ada perubahan, mencatat
 apa yang dirubah, bisa req atau price,
 tidak lupa usernya juga
 */
 AnsiString note=AnsiString("\r\n")+mnuFRM->user_+";"+
                 FormatDateTime("dd-mm-yyyy",Now());
 note+=" modi "+Trim(oldFLD)+", fr. "+oldVAL+" to "+newVAL;
 if(StrToInt(newVAL)-StrToInt(oldVAL)!=0)
  {
   if(reqDS->State!=dsEdit)reqSQL->Edit();
   reqSQL->FieldByName("note")->AsString=
   reqSQL->FieldByName("note")->AsString+
   note;
   reqSQL->Post();
  }

  reqSQL->Refresh();

}
//---------------------------------------------------
bool TreqFRM::findIT()
{
 bool retu_;
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select gdnm from 'dtlreq.dbe' where upper(gdnm) like '%"+Trim(UpperCase(dtlSQL->FieldByName("gdnm")->AsString))+"%' order by gdnm ");
 tmpSQL->Open();
 if(!tmpSQL->Eof)
  {
   if(tmpSQL->RecordCount==1 &&
      Trim(tmpSQL->FieldByName("gdnm")->AsString)==Trim(spaceSTR))retu_=false;
   else retu_=true;
  }
 tmpSQL->Close();
 return(retu_);
}
//---------------------------------------------------
void TreqFRM::pickmeUP()
{
    pickform->Query1->Close();
    pickform->Query1->DatabaseName=mnuFRM->path_+"\\DATA";
    pickform->Query1->SQL->Clear();
    pickform->Query1->SQL->Add(" select distinct trim(book.'gdnm') as gdnm from 'dtlreq.dbe' book ");
    pickform->DBGrid1->Columns->Items[0]->Title->Caption="Description";
    pickform->DBGrid1->Columns->Items[0]->FieldName="gdnm";
    pickform->DBGrid1->SelectedIndex=0;

    pickform->findEDT->Text=dtlSQL->FieldByName("gdnm")->AsString;

    dtlSQL->Edit();
    pickform->ShowModal();
    if(pickform->findEDT->Text!="")
     {
      dtlSQL->FieldByName("gdnm")->AsString=
      pickform->findEDT->Text;
     }
    else
     {
      dtlSQL->FieldByName("gdnm")->AsString=
      pickform->Query1->FieldByName("gdnm")->AsString;
     }
}
//---------------------------------------------------
void TreqFRM::balCHG()
{
 if(dtlDS->State!=dsEdit)dtlSQL->Edit();
 dtlSQL->FieldByName("bal")->AsFloat=
 dtlSQL->FieldByName("req")->AsFloat-
 dtlSQL->FieldByName("rel")->AsFloat;
}
//---------------------------------------------------
void TreqFRM::reqDEL()
{
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" delete from 'dtlreq.dbe' where refno=:TRFNO ");
 tmpSQL->ParamByName("TRFNO")->AsString=
 reqnoEDT->Text;
 tmpSQL->Prepare();
 tmpSQL->ExecSQL();
 tmpSQL->Close();
 reqSQL->Delete();
 tmpSQL->Close();
}
//---------------------------------------------------
void TreqFRM::itemDEL()
{
 AnsiString cat_;
 cat_=master!=true?" Item":" Request";
  if(MessageDlg("Delete this "+cat_+"?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
   {
    if(!master && dtlSQL->FieldByName("rel")->AsFloat==0 )dtlSQL->Delete();
    else reqDEL();
   }
}
//---------------------------------------------------
bool TreqFRM::gdFIND(AnsiString fldNM,AnsiString fndSTR)
{
 TLocateOptions fnd;
 fnd<<loCaseInsensitive;
 gdSQL->First();
 return (gdSQL->Locate(fldNM,fndSTR,fnd));
}
//---------------------------------------------------
void TreqFRM::pickME(AnsiString qrySTR,AnsiString fldNM,AnsiString fldGET)
{
 pickform->Query1->Close();
 pickform->Query1->DatabaseName=mnuFRM->path_+"\\data";
 pickform->Query1->SQL->Clear();
 pickform->Query1->SQL->Add(qrySTR);
 if(pickform->ShowModal()==mrOk)
  {
   if(reqDS->State!=dsEdit)reqSQL->Edit();
   reqSQL->FieldByName(fldNM)->AsString=
   pickform->Query1->FieldByName(fldGET)->AsString;
   reqSQL->Post();
  }
}
//---------------------------------------------------
void TreqFRM::cmbME(TComboBox *myCMB,AnsiString fldNM,TStringList *myVAL)
{
 int ada=myCMB->Items->IndexOf(myCMB->Text.Trim());
 if(ada<0)
 {
  myCMB->Items->Add(myCMB->Text.Trim());
  setupQRY->Append();
  setupQRY->Edit();
  setupQRY->FieldByName("IDKEY")->AsString=fldNM;
  setupQRY->FieldByName("ID1")->AsString=myCMB->Text.Trim();
  setupQRY->Post();
  setupQRY->Edit();
  setupQRY->FieldByName("RECID")->AsFloat=setupQRY->RecNo;
  setupQRY->Post();
  myVAL->Add(IntToStr(setupQRY->RecNo));
  //dafQRY->Edit();
  //dafQRY->FieldByName("EMPBRANCH")->AsFloat=setupQRY->RecNo;
 }
}
//---------------------------------------------------
void TreqFRM::deptFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM)
{
 if(Trim(myCMB->Text)=="")return;
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=myCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,myVAL,false,myCMB->Text);
 if(reqSQL->FieldByName(fieldNM)->AsString !=strNUM)
  {
   if(reqDS->State!=dsEdit)reqSQL->Edit();
   reqSQL->FieldByName(fieldNM)->AsFloat=
   StrToInt(strNUM);
  }
}
//---------------------------------------------------
void TreqFRM::isDATE()
{
 AnsiString dt="";
 if(!reqSQL->FieldByName("reqDT")->IsNull)
    dt=DateToStr(reqSQL->FieldByName("reqDT")->Value);
 if(dt!=DateToStr(reqDT->Date))
  {
   if(reqDS->State!=dsEdit)reqSQL->Edit();
   reqSQL->FieldByName("reqdt")->Value=reqDT->Date;
  }
 dt="";
 if(!reqSQL->FieldByName("needed")->IsNull)
   dt=DateToStr(reqSQL->FieldByName("needed")->Value);
 if(dt!=DateToStr(needDT->Date))
  {
   if(reqDS->State!=dsEdit)reqSQL->Edit();
   reqSQL->FieldByName("needed")->Value=needDT->Date;
  }
}
//---------------------------------------------------
void TreqFRM::cmbSET(TComboBox *myCMB,AnsiString fldNM,TStringList *myVAL)
{
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add("select * from setup where idkey like '"+fldNM+"%' ");
 tmpSQL->Open();
 while(!tmpSQL->Eof)
 {
  myCMB->Items->Add(tmpSQL->FieldByName("id1")->AsString.Trim());
  myVAL->Add(tmpSQL->FieldByName("RECID")->AsString.Trim());
  tmpSQL->Next();
 }
 tmpSQL->Close();
}
//---------------------------------------------------
void TreqFRM::setCMB()
{
 //use to fill combo choices
 deptCMB->Items->Clear();deptVAL->Clear();
 cmbSET(deptCMB,"DEPT",deptVAL);
 divCMB->Items->Clear();divVAL->Clear();
 cmbSET(divCMB,"DIVISI",divVAL);
}
//---------------------------------------------------
void TreqFRM::key13(TObject *Sender,char Key)
{
 if(int(Key)==13)
  {
   if(reqnoEDT->Focused()){reqDT->SetFocus();return;}
   if(reqDT->Focused()){needDT->SetFocus();return;}
   if(needDT->Focused()){deptCMB->SetFocus();return;}
   if(byEDT->Focused()){byttlEDT->SetFocus();return;}
   if(byttlEDT->Focused()){apEDT->SetFocus();return;}
   if(apEDT->Focused()){apttlEDT->SetFocus();return;}
   if(apttlEDT->Focused()){jobEDT->SetFocus();return;}
   if(jobEDT->Focused()){woEDT->SetFocus();return;}
   if(woEDT->Focused()){reqGRD->SetFocus();return;}
  }
}
//---------------------------------------------------
void TreqFRM::dtlUPDATE()
{
 //update anakan
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" update 'dtlreq.dbe' set reqno='"+Trim(refNEW)+"' ");
 //tmpSQL->SQL->Add(" ,refno='"+refNEW+"'");
 tmpSQL->SQL->Add(" where Trim(reqno)='"+Trim(refOLD)+"' ");
 tmpSQL->SQL->Add(" and refdt=:REFDT ");
 tmpSQL->ParamByName("REFDT")->AsDate=
 reqSQL->FieldValues["reqdt"];
 tmpSQL->Prepare();
 tmpSQL->ExecSQL();
 tmpSQL->Close();

 //check updated
 reqSQLAfterScroll(reqSQL);
}
//---------------------------------------------------
bool TreqFRM::isEXIST(AnsiString seekSTR)
{
 //find reqno at numSQL
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 numSQL->First();
 return(numSQL->Locate("reqno",seekSTR,Opts));
}
//---------------------------------------------------
void TreqFRM::reqGO(AnsiString seekSTR)
{
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 reqSQL->First();
 reqSQL->Locate("reqno",seekSTR,Opts);
}
//---------------------------------------------------
void TreqFRM::numNEW()
{
 reqSQL->Append();
 numSQL->Close();
 numSQL->Open();
 numSQL->Last();
 //oke tampilin nomer terakhir brow
 refNEW = numSQL->FieldByName("reqno")->AsString;
 refNEW = InputBox("Input Req# No.", " Last No. ="+
          Trim(numSQL->FieldByName("reqno")->AsString)+
          ", Enter New No. :",
          refNEW);
 reqSQL->FieldByName("reqno")->AsString=refNEW;
 reqnoEDT->Text=refNEW;
 if(isEXIST(refNEW))
   {
    reqSQL->Cancel();
    reqGO(refNEW);
   }
 reqnoEDT->SetFocus();
}
//---------------------------------------------------
void TreqFRM::numOPEN()
{
 numSQL->Close();
 numSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 numSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 numSQL->Open();
}
//---------------------------------------------------
void TreqFRM::itemADD()
{
 AnsiString cat_="";
 if(master)cat_=" Form Requisition ";
 else cat_=" Item ";

 if(MessageDlg("Add New "+cat_+" ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
   if(master)
    {
     if(sortIT->Checked)
     {
      ShowMessage("Release sort order first");
      return;
     }
     if(!numSQL->Active)numOPEN();
     numNEW();
    }
   else
    {
     dtlSQL->Append();
     dtlSQL->Edit();
    }
  }
}
//---------------------------------------------------
void TreqFRM::isCHG()
{
 refNEW=reqnoEDT->Text;
 if(Trim(refOLD)!=Trim(refNEW) && Trim(refOLD)!="")
  {
   if(MessageDlg("Change Number to :"+refNEW+"?", mtConfirmation,
      TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)
    {
     reqnoEDT->Text=refOLD;
     return;
    }
   else dtlUPDATE();
  }
}
//---------------------------------------------------
void TreqFRM::reqOPEN()
{
 reqSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 reqSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 reqSQL->Open();
}
//---------------------------------------------------
void TreqFRM::sortGO()
{
 reqSQL->Close();
 //disaringClick(Sender);
 if(!sortIT->Checked )
  {
   sortSTR="";
   func->sortNM->Clear();
   func->sortVAL->Clear();
   fndEDT->Text="";
  }
 else
   if(sortSTR!="")sortSTR.Insert(" order BY ",0);
 //func->openSQL(reqSQL,orgSQL,"",sortSTR);
 reqSQL->SQL->Clear();
 reqSQL->SQL->Add(orgSQL);
 reqSQL->SQL->Add(sortSTR);
 //ShowMessage(reqSQL->SQL->Text);
 reqOPEN();
}
//---------------------------------------------------
void TreqFRM::sortME(TColumn *Column)
{
 func->sortIT(Column,sortSTR);
 fndEDT->Text=func->sortNM->CommaText;
}
//---------------------------------------------------------
void TreqFRM::varFILL()
{
 trdate=DateToStr(needDT->Date);
}
//---------------------------------------------------------
void TreqFRM::dtlPOST()
{
 if(dtlDS->State==dsInsert || dtlDS->State==dsEdit)dtlSQL->Post();
}
//---------------------------------------------------------
void TreqFRM::reqPOST()
{
 if(reqDS->State==dsInsert || reqDS->State==dsEdit)reqSQL->Post();
}
//---------------------------------------------------------
void TreqFRM::reqSET()
{
 divSQL->Close();
 deptSQL->Close();
 if(reqDS->State!=dsEdit)reqSQL->Edit();
 reqSQL->FieldByName("mode")->AsFloat=0;
 reqSQL->FieldByName("reqdept")->AsString="0";
 reqSQL->FieldByName("reqdiv")->AsString="0";
 reqSQL->FieldByName("state")->AsString="0";
}
//---------------------------------------------------------
void TreqFRM::idSAY()
{
 if(reqPG->ActivePage==reqSHEET)
 {
  TStringList *myLST=new TStringList();
  myLST->Clear();
  myLST->CommaText=deptCMB->Items->CommaText;

  AnsiString strNUM=
  func->fndLST(myLST,deptVAL,true,reqSQL->FieldByName("reqdept")->AsString);
  deptCMB->Text=strNUM;

  myLST->Clear();
  myLST->CommaText=divCMB->Items->CommaText;

  strNUM=func->fndLST(myLST,divVAL,true,reqSQL->FieldByName("reqdiv")->AsString);
  divCMB->Text=strNUM;

  //deptCMB->Text=reqSQL->FieldByName("deptnm")->AsString;
  //divCMB->Text=reqSQL->FieldByName("divnm")->AsString;
 }
}
//---------------------------------------------------------
void TreqFRM::dateSAY()
{
 if(reqPG->ActivePage==reqSHEET)
 {
  if(!reqSQL->FieldByName("reqdt")->IsNull)
  reqDT->Date=
  reqSQL->FieldByName("reqdt")->Value;
  if(!reqSQL->FieldByName("needed")->IsNull)
  needDT->Date=
  reqSQL->FieldByName("needed")->Value;
 }
}
//---------------------------------------------------------
void TreqFRM::sumOPEN()
{
 dtlPOST();
 //if(sumSQL->Active) sumSQL->Refresh();
 //else
 // {
   if(!dtlSQL->FieldByName("cur")->IsNull)
   {
    sumSQL->Close();
    sumSQL->ParamByName("PARENT")->AsString=
    reqSQL->FieldByName("reqno")->AsString;
    sumSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
    sumSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
    sumSQL->Open();
   }
 //}
}
//---------------------------------------------------------
void TreqFRM::subOPEN()
{
 AnsiString s_;
 s_="";
 s_=s_+ " select * from DTLREQ.dbe ";
 s_=s_+ " where reqno like  '%"+Trim(reqnoEDT->Text)+"%'  ";
 s_=s_+ " and (refdt is null or (refdt>=:STDT and refdt<=:LSTDT) ) ";

 bool go;
 go=true;
 //if(dtlSQL->Active && Trim(reqnoEDT->Text)!=Trim(dtlSQL->FieldByName("reqno")->AsString)) go=true;
 //if(!dtlSQL->Active) go=true;
 if(go)
 {
  sumSQL->Close();
  dtlSQL->Close();
  dtlSQL->SQL->Clear();
  dtlSQL->SQL->Text=s_;

  dtlSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
  dtlSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;

  loged=true;
  TbarFRM *bar=new TbarFRM(Application);
  bar->gmb->Visible=true;
  bar->gmb->Active=true;
  bar->Show();

  bar->Caption=" Contacting Server ... Collecting Invoice ";
  bar->txt->Caption=" searching invoice details No.# "+Trim(reqnoEDT->Text);
  bar->Update();

  dtlSQL->Open();

  bar->Hide();
  delete bar;
 }
}
//---------------------------------------------------------
void TreqFRM::tbOPEN()
{
 path_=mnuFRM->path_;
 countSQL->DatabaseName=path_+"\\DATA";
 sumSQL->DatabaseName=path_+"\\DATA";
 setupQRY->DatabaseName=path_+"\\DATA";
 numSQL->DatabaseName=path_+"\\DATA";
 rptSQL->DatabaseName=path_+"\\DATA";
 divSQL->DatabaseName=path_+"\\DATA";
 deptSQL->DatabaseName=path_+"\\DATA";
 gdSQL->DatabaseName=path_+"\\DATA";
 tmpSQL->DatabaseName=path_+"\\DATA";
 dtlSQL->DatabaseName=path_+"\\DATA";
 reqSQL->DatabaseName=path_+"\\DATA";
}
//---------------------------------------------------------
void TreqFRM::tbCLOSE()
{
 countSQL->Close();
 sumSQL->Close();
 setupQRY->Close();
 numSQL->Close();
 rptSQL->Close();
 divSQL->Close();
 deptSQL->Close();
 gdSQL->Close();
 tmpSQL->Close();
 dtlSQL->Close();
 reqSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::FormActivate(TObject *Sender)
{
if(!loged)
{
 stDT->Date=mnuFRM->stDT;
 chgDT->Date=mnuFRM->lstDT;
 tbCLOSE();
 tbOPEN();
 setupQRY->Open();

 reqPG->ActivePage=listSHEET;//reqSHEET;
 reqNAV->DataSource=reqDS;

 setCMB();
 orgSQL=reqSQL->SQL->Text;
 reqOPEN();

 //reqnoEDT->SetFocus();
}
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqSQLAfterScroll(TDataSet *DataSet)
{
 refOLD=reqSQL->FieldByName("reqno")->AsString;
 refNEW="";
 if(reqPG->ActivePage!=reqSHEET)dtlSQL->Close();
 //else subOPEN();
 if(reqPG->ActivePage==reqSHEET)subOPEN();
 if(reqPG->ActivePage==reqSHEET)
 {
  dateSAY();
  idSAY();
 } 
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqSQLNewRecord(TDataSet *DataSet)
{
reqSET();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqGRDEnter(TObject *Sender)
{

 reqPOST();
 deptSQL->Close();
 divSQL->Close();

 reqSQLAfterScroll(reqSQL);
 if(master)
 {
  master=false;
  reqGRD->SelectedIndex=0;
 }
 varFILL();
 idSAY();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::dtlSQLNewRecord(TDataSet *DataSet)
{
 if(dtlDS->State!=dsEdit)dtlDS->Edit();
 dtlSQL->FieldByName("reqno")->AsString=
 reqSQL->FieldByName("reqno")->AsString;
 dtlSQL->FieldByName("refdt")->AsString=
 reqSQL->FieldByName("reqdt")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::fndEDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 func->locateIT(fndCMB,fndEDT,reqSQL,fndVAL,AnsiString("NONEED"));

}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::listGRDTitleClick(TColumn *Column)
{
sortME(Column);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::sortITClick(TObject *Sender)
{
sortGO();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::dtlSQLCalcFields(TDataSet *DataSet)
{
 dtlSQL->FieldByName("total")->AsFloat=
 dtlSQL->FieldByName("req")->AsFloat*
 dtlSQL->FieldByName("price")->AsFloat;
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::FormCreate(TObject *Sender)
{
 fndCMB->Items->Clear();
 fndVAL=new TStringList();fndVAL->Clear();

 fndCMB->Items->Add("PR# No.");fndVAL->Add("reqno");
 teksHTML=new TStringList();teksHTML->Clear();
 teksEDIT=new TStringList();teksEDIT->Clear();

 deptVAL=new TStringList();
 divVAL=new TStringList();

 reqDT->Date=Date();
 needDT->Date=Date();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqnoEDTEnter(TObject *Sender)
{
master=true;
refOLD=reqSQL->FieldByName("reqno")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqnoEDTKeyPress(TObject *Sender, char &Key)
{
 if(int(Key)==13)isCHG();
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::addBTNClick(TObject *Sender)
{
itemADD();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqDTChange(TObject *Sender)
{
isDATE();
needDT->Date=reqDT->Date;
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::needDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::needDTChange(TObject *Sender)
{
isDATE();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::deptCMBExit(TObject *Sender)
{
deptFILL(deptCMB,deptVAL,deptCMB->Text,"reqdept");
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::deptCMBKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 {
   cmbME(deptCMB,"DEPT",deptVAL);
   deptCMBExit(Sender);
   divCMB->SetFocus();
 }
else
 {
  if(Key=='\\')
   {
    pickME(" select id1 as Departement,recid as ID from setup where idkey='DEPT' ","REQDEPT","ID");
    Key=0;
   }
 }
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::divCMBExit(TObject *Sender)
{
deptFILL(divCMB,divVAL,divCMB->Text,"reqdiv");
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::divCMBKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 {
  if(Trim(divCMB->Text)!="")
  {
   cmbME(divCMB,"DIVISI",divVAL);
   TStringList *myLST=new TStringList();
   myLST->Clear();
   myLST->CommaText=deptCMB->Items->CommaText;
   AnsiString strNUM=
   func->fndLST(myLST,deptVAL,false,deptCMB->Text);
   setupQRY->Edit();
   setupQRY->FieldByName("id2")->AsString=
   strNUM;
   setupQRY->Post();
   }
   divCMBExit(Sender);
   byEDT->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::byEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::apEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::byttlEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::apttlEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::jobEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);

}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::woEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqSQLBeforePost(TDataSet *DataSet)
{
 reqSQL->FieldByName("reqdt")->Value=reqDT->Date;
 reqSQL->FieldByName("needed")->Value=needDT->Date;
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Shift.Contains(ssAlt) && Key==int('1') ) reqPG->ActivePage=listSHEET;
 if (Shift.Contains(ssAlt) && Key==int('2') ) reqPG->ActivePage=reqSHEET;
 if (Shift.Contains(ssAlt) && Key==int('3') ) reqPG->ActivePage=noteSHEET;

 if(Key==107 && Shift.Contains(ssCtrl))itemADD();
 if(Key==109 && Shift.Contains(ssCtrl))itemDEL();

}
//---------------------------------------------------------------------------


void __fastcall TreqFRM::dtlSQLAfterOpen(TDataSet *DataSet)
{
 //--------------------debug on 14-10-2010
 //setDT();

 //--------------------for debugging on 13-10-2010
 //sumOPEN();
 sumSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::dtlSQLpriceChange(TField *Sender)
{
 if(dtlDS->State!=dsInsert)
 {
  oldFLD=" price "+Trim(dtlSQL->FieldByName("gdnm")->AsString)+" "+oldCUR+" to "+Trim(dtlSQL->FieldByName("cur")->AsString)+" ";
  oldVAL=IntToStr(oldPRC);
  newVAL=dtlSQL->FieldByName("price")->AsString;
  noteFIX();
 }
 //sumOPEN();
 sumSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::dtlSQLreqChange(TField *Sender)
{
 if(dtlDS->State!=dsInsert)
 {
 oldFLD=" qty "+Trim(dtlSQL->FieldByName("gdnm")->AsString)+" ";
 oldVAL=IntToStr(oldREQ);
 newVAL=dtlSQL->FieldByName("req")->AsString;
 noteFIX();
 }
 balCHG();
 //sumOPEN();
 sumSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqGRDKeyPress(TObject *Sender, char &Key)
{
int col_;
if(Key==' ')
 {
  if(UpperCase(reqGRD->SelectedField->FieldName)=="GDNM")
   {
    if(dtlDS->State==dsInsert || dtlDS->State==dsEdit)dtlSQL->Post();
    spaceSTR=dtlSQL->FieldByName("gdnm")->AsString;
    //if(findIT())
    pickmeUP();
    dtlSQL->Edit();
    //Key=0;
    return;
   }
 }
if(int(Key)==13)
 {
  //SHIFT+ENTER?
  //if(dtlSQL->State==dsEdit) ShowMessage(" editing mode ");
  //if(HiWord(GetKeyState(VK_SHIFT))!=0)
  if(reqGRD->EditorMode==true)
  {
   col_=reqGRD->SelectedIndex;
   switch(col_)
   {
    case 0:    {reqGRD->SelectedIndex=1; break;}
    case 1:    {reqGRD->SelectedIndex=2; break;}
    case 2:    {reqGRD->SelectedIndex=3; break;}
    case 3:    {reqGRD->SelectedIndex=4; break;}
    case 4:
    {
     dtlSQL->Next();
     if(dtlSQL->Eof) dtlSQL->Append();
     reqGRD->SelectedIndex=0;
    }
    default: break;
   }
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::dtlSQLBeforeEdit(TDataSet *DataSet)
{
 oldCUR=Trim(dtlSQL->FieldByName("cur")->AsString);
 oldREQ=dtlSQL->FieldByName("req")->AsFloat;
 oldREL=dtlSQL->FieldByName("rel")->AsFloat;
 oldPRC=dtlSQL->FieldByName("price")->AsFloat;
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::reqSQLBeforeClose(TDataSet *DataSet)
{
if(reqDS->State==dsEdit || reqDS->State==dsInsert)reqSQL->Post();
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::reqSQLBeforeScroll(TDataSet *DataSet)
{
 if(reqDS->State==dsEdit)reqSQL->Post();
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::prtBTNClick(TObject *Sender)
{
 if(reqPG->ActivePage==listSHEET)dafRPT();
 else reqRPT();
}
//---------------------------------------------------------------------------



void __fastcall TreqFRM::reqGRDColEnter(TObject *Sender)
{
 int idx=reqGRD->SelectedIndex;
 if(idx==(4-1) && Trim(dtlSQL->FieldByName("cur")->AsString)=="")
 {
  if(reqDS->State==dsInsert || reqDS->State==dsEdit) reqSQL->Edit();
  dtlSQL->FieldByName("cur")->AsString=reqGRD->Columns->Items[idx]->PickList->Strings[0];
  dtlSQL->Post();
 }
}
//---------------------------------------------------------------------------

void __fastcall TreqFRM::byEDTEnter(TObject *Sender)
{
 if(reqDS->State!=dsInsert || reqDS->State!=dsEdit) reqSQL->Edit();
 if(reqSQL->FieldByName("reqby")->IsNull)
 {
 reqSQL->FieldByName("reqby")->AsString="EDI";
 reqSQL->FieldByName("reqttl")->AsString="SPV";
 }

}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::apEDTEnter(TObject *Sender)
{
 if(reqDS->State!=dsInsert || reqDS->State!=dsEdit) reqSQL->Edit();
 if(reqSQL->FieldByName("aprby")->IsNull)
 {
 reqSQL->FieldByName("aprby")->AsString="FRANSISCA";
 reqSQL->FieldByName("aprttl")->AsString="MANAGER";
 }
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::reqPGChange(TObject *Sender)
{
 if(reqPG->ActivePage==reqSHEET)subOPEN();
 else dtlSQL->Close();

 if(reqPG->ActivePage==noteSHEET)reqSQL->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::shwBTNClick(TObject *Sender)
{
 sumOPEN();
 sumGRD->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::dtlSQLrelChange(TField *Sender)
{
 if(dtlDS->State!=dsInsert)
 {
 oldFLD=" released/receive "+Trim(dtlSQL->FieldByName("gdnm")->AsString)+" ";
 oldVAL=IntToStr(oldREL);
 newVAL=dtlSQL->FieldByName("rel")->AsString;
 noteFIX();
 }
 balCHG();
 //sumOPEN();
 sumSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::chgBTNClick(TObject *Sender)
{
 /*
 refresh list difilter atas tanggal yang diinput
 */
 reqSQL->Close();
 reqSQL->ParamByName("STDT")->AsDate=stDT->Date;
 reqSQL->ParamByName("LSTDT")->AsDate=chgDT->Date;
 reqSQL->Open();
}
//---------------------------------------------------------------------------
void __fastcall TreqFRM::chgDTKeyPress(TObject *Sender, char &Key)
{
 if(int(Key)==13)chgBTN->SetFocus();
}
//---------------------------------------------------------------------------

