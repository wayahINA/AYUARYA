//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "opCPP.h"
#include "funcCPP.h"
#include "ividp.h"
#include "filterCPP.h"
#include "explorCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TopFRM *opFRM;
//---------------------------------------------------------------------------
__fastcall TopFRM::TopFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void TopFRM::sayADJ()
{
 unposSQL->Close();
 unposSQL->ParamByName("REFNO")->AsString=Trim(opEDT->Text);
 unposSQL->ParamByName("GD")->AsFloat=
 tmpSQL->FieldByName("gd")->AsFloat;
 unposSQL->Open();

 if(!unposSQL->Eof && !begCHECK->Checked)
 {
  if(!unposSQL->FieldByName("adjDB")->IsNull)
  {
   tmpSQL->Edit();
   tmpSQL->FieldByName("adjDB")->AsFloat+=
   unposSQL->FieldByName("adjDB")->AsFloat*vektor;
   tmpSQL->Post();
  }
  if(!unposSQL->FieldByName("adjCR")->IsNull)
  {
   tmpSQL->Edit();
   tmpSQL->FieldByName("adjCR")->AsFloat+=
   unposSQL->FieldByName("adjCR")->AsFloat*vektor;
   tmpSQL->Post();
  }
 }
} 
//---------------------------------------------------------
void TopFRM::sayBAL()
{
 float bal=0;
 bal+=tmpSQL->FieldByName("beg")->AsFloat;
 if(!tmpSQL->FieldByName("db")->IsNull) bal+=tmpSQL->FieldByName("db")->AsFloat;
 if(!tmpSQL->FieldByName("cr")->IsNull) bal-=tmpSQL->FieldByName("cr")->AsFloat;
 if(!tmpSQL->FieldByName("adjdb")->IsNull) bal+=tmpSQL->FieldByName("adjdb")->AsFloat;
 if(!tmpSQL->FieldByName("adjcr")->IsNull) bal-=tmpSQL->FieldByName("adjcr")->AsFloat;

 tmpSQL->Edit();
 tmpSQL->FieldByName("last")->AsFloat=bal;
 tmpSQL->Post();
}
//---------------------------------------------------------
void TopFRM::reqDEL()
{
 if(!allowPOST())
  {
   opSQL->Cancel();
   return;
  }
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" delete from bincrd.dbe where refno=:TRFNO ");
 tmpSQL->SQL->Add(" and mode=5 ");
 tmpSQL->ParamByName("TRFNO")->AsString=
 opEDT->Text;
 tmpSQL->Prepare();
 tmpSQL->ExecSQL();
 tmpSQL->Close();

 opSQL->Delete();
 tmpSQL->Close();
}
//---------------------------------------------------------
void TopFRM::reqOPEN()
{
 opSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 opSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 opSQL->Open();
}
//---------------------------------------------------------
void TopFRM::sortGO()
{
 opSQL->Close();
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
 opSQL->SQL->Clear();
 opSQL->SQL->Add(orgSQL);
 opSQL->SQL->Add(sortSTR);
 //ShowMessage(reqSQL->SQL->Text);
 reqOPEN();
}
//---------------------------------------------------------
void TopFRM::sortME(TColumn *Column)
{
 func->sortIT(Column,sortSTR);
 fndEDT->Text=func->sortNM->CommaText;
}
//---------------------------------------------------------
void TopFRM::reqGO(AnsiString seekSTR)
{
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 opSQL->First();
 opSQL->Locate("trfno",seekSTR,Opts);
}
//---------------------------------------------------------
bool TopFRM::isEXIST(AnsiString seekSTR)
{
 //find reqno at numSQL
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive;
 numSQL->First();
 return(numSQL->Locate("trfno",seekSTR,Opts));
}
//---------------------------------------------------------
void TopFRM::dtlUPDATE()
{
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" update bincrd.dbe set refno=:REFNO where refno='"+refOLD+"'");
 tmpSQL->ParamByName("REFNO")->AsString=
 refNEW;
}
//---------------------------------------------------------
void TopFRM::numNEW()
{
 opSQL->Append();
 numSQL->Refresh();
 numSQL->Last();
 //oke tampilin nomer terakhir brow
 refNEW =
 InputBox("Input OPNAME# No.", " Last No. ="+
          Trim(numSQL->FieldByName("trfno")->AsString)+
          ", Enter New No. :",
          numSQL->FieldByName("trfno")->AsString);
 opSQL->FieldByName("trfno")->AsString=refNEW;
 opEDT->Text=refNEW;
 if(isEXIST(refNEW))
   {
    opSQL->Cancel();
    reqGO(refNEW);
   } 
 opEDT->SetFocus();
}
//---------------------------------------------------------
void TopFRM::numOPEN()
{
 numSQL->Close();
 numSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 numSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 numSQL->Open();
}
//---------------------------------------------------------
bool TopFRM::onPOST()
{
 int pos;
 return(!func->atSTR(Trim(UpperCase(posBTN->Caption)),"UN",pos));
}
//---------------------------------------------------------
void TopFRM::byPOST()
{
if(!allowPOST())opEDT->SetFocus();
}
//---------------------------------------------------------
void TopFRM::gdSET()
{
 if(dtlDS->State!=dsEdit)dtlSQL->Edit();
 dtlSQL->FieldByName("gd")->AsFloat=
 dtlSQL->FieldByName("gdid")->AsFloat;
 gdFILL();
 dtlSQL->Post();
}
//---------------------------------------------------------
void TopFRM::rowPRT()
{
 //cetak row disini
 dtlSQL->First();
 while(!dtlSQL->Eof)
  {
   teksEDIT->LoadFromFile(pathHTML+"datarow.txt");
   //replacing formula
   func->replaceSTR(teksEDIT,"@BATCH@",dtlSQL->FieldByName("batch")->AsString);
   func->replaceSTR(teksEDIT,"@ID@",dtlSQL->FieldByName("id")->AsString);
   func->replaceSTR(teksEDIT,"@NM@",dtlSQL->FieldByName("gdnm")->AsString);
   func->replaceSTR(teksEDIT,"@UOM@",dtlSQL->FieldByName("unit")->AsString);
   func->replaceSTR(teksEDIT,"@BAL@",dtlSQL->FieldByName("bal")->AsString);
   func->replaceSTR(teksEDIT,"@STATE@",Trim(condCMB->Text));
   teksHTML->Add(teksEDIT->Text);

   if(FileExists(pathHTML+"txtROW.txt"))
    {
     teksEDIT->LoadFromFile(pathHTML+"txtROW.txt");
     func->replaceSTR(teksEDIT,"@BATCH@",dtlSQL->FieldByName("batch")->AsString);
     func->replaceSTR(teksEDIT,"@ID@",dtlSQL->FieldByName("id")->AsString);
     func->replaceSTR(teksEDIT,"@NM@",dtlSQL->FieldByName("gdnm")->AsString);
     func->replaceSTR(teksEDIT,"@UOM@",dtlSQL->FieldByName("unit")->AsString);
     func->replaceSTR(teksEDIT,"@BAL@",dtlSQL->FieldByName("bal")->AsString);
     func->replaceSTR(teksEDIT,"@STATE@",Trim(condCMB->Text));
     teksTXT->Add(teksEDIT->Text);
    }
    AnsiString raw="";
    raw+=dtlSQL->FieldByName("id")->AsString+"\t";
    raw+=dtlSQL->FieldByName("gdnm")->AsString;
    raw+="  "+dtlSQL->FieldByName("batch")->AsString+"\t";
    //raw+=dtlSQL->FieldByName("gdnm")->AsString+"\t";
    raw+=dtlSQL->FieldByName("unit")->AsString+"\t";
    raw+=dtlSQL->FieldByName("bal")->AsString+"\t";
    raw+=Trim(condCMB->Text)+"\t";
    teksRPT->Add(raw);

   dtlSQL->Next();
  }
}
//---------------------------------------------------------
void TopFRM::opRPT()
{
 //buat mencetak formulir ini
 pathAPP_=mnuFRM->pathAPP_;
 prtFRM->pathAPP_=pathAPP_;
 pathPRT=pathAPP_+"\\REPORT\\";
 prtFRM->pathPRT=pathPRT;
 pathHTML=pathAPP_+"\\HTML\\OPRPT\\";
 prtFRM->pathHTML=pathHTML;
 //pathRPT=pathAPP_+"\\TXT\\";
 pathRPT="c:\\temp\\";
 //prtFRM->pathRPT=pathRPT;
 prtFRM->pathRPT="c:\\temp\\";
 //ShowMessage(pathAPP_+"\n"+pathPRT+"\n"+pathHTML);
 AnsiString s_=InputBox("Masukkan Judul Report","" ,"Form Transfer No "+opEDT->Text);
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
 func->replaceSTR(teksHTML,"@REFDT@",DateToStr(opDT->Date));
 func->replaceSTR(teksHTML,"@REFNO@",Trim(opEDT->Text));
 func->replaceSTR(teksHTML,"@WH@",Trim(whCMB->Text)+" - "+Trim(locCMB->Text));
 func->replaceSTR(teksHTML,"@ITEMCOND@",Trim(condCMB->Text));

 func->replaceSTR(teksTXT,"@REFDT@",DateToStr(opDT->Date));
 func->replaceSTR(teksTXT,"@REFNO@",Trim(opEDT->Text));
 func->replaceSTR(teksTXT,"@WH@",Trim(whCMB->Text)+" - "+Trim(locCMB->Text));
 func->replaceSTR(teksTXT,"@ITEMCOND@",Trim(condCMB->Text));

 teksRPT->Add(DateToStr(opDT->Date));
 teksRPT->Add(Trim(opEDT->Text));
 teksRPT->Add(Trim(whCMB->Text)+" - "+Trim(locCMB->Text));
 teksRPT->Add(Trim(condCMB->Text));
 teksRPT->Add("\r\n\r\n");

 //prtFRM->botPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);

 //print specific detail
 rowPRT();

 //footer
 //prtFRM->botPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
 //prtFRM->validPRT(teksHTML,teksEDIT,teksTXT,teksRPT);
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
            //pil = InputBox("Open File With",
            //      " [1] XL-2000 , [2] NOTEPAD , [3] XL-98, Print By. [?]:",
            //      pil);
 AnsiString app_="EXPLORER.EXE";
 if(StrToInt(pil)==1)app_="EXCEL.EXE";
 if(StrToInt(pil)==3)app_="EXCEL.EXE";
 AnsiString txt_=txtME;
 if(StrToInt(pil)==1)txt_=prtME;
 if(StrToInt(pil)==3)
  {
   pathPRT=pathAPP_+"\\FORM"  ;
   txt_=pathPRT+"\\GETOP.XLS";
  }
 AnsiString mod_="open";
 if(StrToInt(pil)==1)mod_="edit";

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
//---------------------------------------------------------
void TopFRM::condFILL()
{
 if(opDS->State!=dsEdit)opSQL->Edit();
 AnsiString strNUM=cmbFND(condCMB,mnuFRM->gdSTATUS);
 opSQL->FieldByName("state")->AsString=strNUM;
}
//---------------------------------------------------------
void TopFRM::gdFILL()
{
 /*
  prosedur gdFILL
  to fill in dtl->unit by gd->uom
 */
 if(dtlDS->State!=dsEdit)dtlSQL->Edit();
 dtlSQL->FieldByName("unit")->AsString=
 gdSQL->FieldByName("UOM")->AsString;
}
//---------------------------------------------------------
void TopFRM::balFIX(TDataSet *mySQL)
{
 /*
  prosedur balFIX
  to fix handstck by qty*vektor
 */
 if(!tmpSQL->Eof)
  {
   AnsiString remark=Trim(tmpSQL->FieldByName("remark")->AsString);
   remark+="\r\n"+DateToStr(opDT->Date)+";OPNAME by: "+Trim(mnuFRM->user_)+" ";
   if(vektor<0)remark+=";Mode :UN-POST";
   else         remark+=";Mode :POST";
   remark+="\r\nRef# "+Trim(opEDT->Text);

   //if beginning balance
   AnsiString fldNM;
   if(!begCHECK->Checked )fldNM="bal";
   else fldNM="beg";

   tmpSQL->Edit();

   if(vektor<0)tmpSQL->FieldByName(fldNM)->AsFloat=0;
   else
     tmpSQL->FieldByName(fldNM)->AsFloat+=
     mySQL->FieldByName("qty")->AsFloat;

   tmpSQL->Post();

   //how to know adjustment
   bool alow=true;
   if(!begCHECK->Checked)
    {
     if(vektor>=0)
     {
      if(mySQL->Name!="btchSQL" &&
         mySQL->FieldByName("batch")->AsString!="")alow=false;
      if(alow)
      {
       AnsiString fldNM="";
       float adj=tmpSQL->FieldByName("bal")->AsFloat-
                tmpSQL->FieldByName("last")->AsFloat;
       if(adj>0) fldNM="adjDB";
       else fldNM="adjCR";

       if(mySQL->Name=="btchSQL")
       {
        mySQL->Edit();
        mySQL->FieldByName(fldNM)->AsFloat=abs(adj);
        mySQL->Post();
       }
      }
     }

     if(!begCHECK->Checked && mySQL->Name=="btchSQL")
     {
      if(!mySQL->FieldByName("adjDB")->IsNull)
      {
       tmpSQL->Edit();
       tmpSQL->FieldByName("adjDB")->AsFloat+=
       mySQL->FieldByName("adjDB")->AsFloat*vektor;
       tmpSQL->Post();
      }
      if(!mySQL->FieldByName("adjCR")->IsNull)
      {
       tmpSQL->Edit();
       tmpSQL->FieldByName("adjCR")->AsFloat+=
       mySQL->FieldByName("adjCR")->AsFloat*vektor;
       tmpSQL->Post();
      }
     }
    }
    if(mySQL->Name=="btchSQL")
    {
    sayBAL();
    remark+=";QTY ADJ ";
    remark+="by ";
    remark+=UpperCase(fldNM)=="ADJDB"?"DEBET ":"CREDIT ";
    remark+=Trim(mySQL->FieldByName("qty")->AsString);
    remark+=";BALANCE :"+Trim(tmpSQL->FieldByName("last")->AsString);
    tmpSQL->Edit();
    tmpSQL->FieldByName("remark")->AsString=
    tmpSQL->FieldByName("remark")->AsString+remark;
    tmpSQL->Post();
    }
  }
}
//---------------------------------------------------------
void TopFRM::isBLC()
{
 begCHECK->Checked=false;
 if(Trim(opSQL->FieldByName("job")->AsString)!="")begCHECK->Checked=true;
}
//---------------------------------------------------------
void TopFRM::bincrdFIX()
{
 //fix bincrd whenever loc+cond+opdate changed
 if(opCHG)
  {
   opCHG=false;
   tmpSQL->Close();
   tmpSQL->SQL->Clear();
   tmpSQL->SQL->Add(" UPDATE bincrd.dbe set wh=:LOC,cond=:COND,refdt=:OPDATE ");
   tmpSQL->SQL->Add(" where ref=:REF ");
   tmpSQL->ParamByName("LOC")->AsFloat=StrToInt(loc);
   tmpSQL->ParamByName("COND")->AsString=cond;
   tmpSQL->ParamByName("OPDATE")->AsDate=opdate;
   tmpSQL->ParamByName("REF")->AsString=opEDT->Text;
   tmpSQL->Prepare();
   tmpSQL->ExecSQL();
   tmpSQL->Close();
  }
}
//---------------------------------------------------------
void TopFRM::batchFIX()
{
 /*
  post n fix to stckbatch file
 */
 while(!btchSQL->Eof)
  {
   if(btchSQL->FieldByName("batch")->AsString!="")
   {
   //wa pake locate deh lho
   Variant locVAL[4];
   locVAL[0] = btchSQL->FieldByName("wh")->AsFloat;
   locVAL[1] = btchSQL->FieldByName("gd")->AsFloat;
   locVAL[2] = btchSQL->FieldByName("batch")->AsString;
   locVAL[3] = btchSQL->FieldByName("unit")->AsString;
   if(tmpSQL->Eof || !func->VarLOCATE("whrec;gd;batch;unit",locVAL,3,tmpSQL))
    {
     tmpSQL->Append();
     tmpSQL->FieldByName("whrec")->AsFloat=locVAL[0];
     tmpSQL->FieldByName("gd")->AsFloat=locVAL[1];
     tmpSQL->FieldByName("batch")->AsString=locVAL[2];
     tmpSQL->FieldByName("unit")->AsString=locVAL[3];
     tmpSQL->FieldByName("refdt")->Value=mnuFRM->stDT;
     tmpSQL->FieldByName("lstdt")->Value=mnuFRM->lstDT;
     tmpSQL->Post();
    }
   //edit batchstock
   balFIX(btchSQL);
   }
   btchSQL->Next();
  }
  btchSQL->Close();
}
//---------------------------------------------------------
void TopFRM::stkFIX()
{
 /*
  post n fix to stckwarehouse file
 */
 //dtlSQL->First;
 TQuery *me;
 //if(!stkSQL->Eof &&
 //    stkSQL->FieldByName("gd")->AsString!="" &&
 //    stkSQL->FieldByName("qty")->AsString>"0")me=stkSQL;
 //else me=binSQL;
 me=binSQL;
 if(me->Name=="binSQL" && !me->Active)
 {
  me->Close();
  me->ParamByName("REFNO")->AsString=Trim(opEDT->Text);
  me->Open();
 }
 //explor->gridDS->DataSet=me;
 //explor->ShowModal();
 AnsiString gd="";
 while(!me->Eof)
  {
   //wa pake locate deh lho
   Variant locVAL[3];
   locVAL[0] = me->FieldByName("wh")->AsFloat;
   locVAL[1] = me->FieldByName("gd")->AsFloat;
   locVAL[2] = me->FieldByName("unit")->AsString;
   if(tmpSQL->Eof || !func->VarLOCATE("whrec;gd;unit",locVAL,2,tmpSQL))
    {
     tmpSQL->Append();
     tmpSQL->FieldByName("whrec")->AsFloat=locVAL[0];
     tmpSQL->FieldByName("gd")->AsFloat=locVAL[1];
     tmpSQL->FieldByName("unit")->AsString=locVAL[2];
     tmpSQL->FieldByName("refdt")->Value=mnuFRM->stDT;
     tmpSQL->FieldByName("lstdt")->Value=mnuFRM->lstDT;
     tmpSQL->Post();
    }
   //edit opnamestock

   balFIX(me);

//=========================================================
   if(vektor>0 && !begCHECK->Checked && me->FieldByName("batch")->AsString=="")
   {
    AnsiString fldNM="";
    float adj=tmpSQL->FieldByName("bal")->AsFloat-
              tmpSQL->FieldByName("last")->AsFloat;
    if(adj>0) fldNM="adjDB";
    else fldNM="adjCR";

    me->Edit();
    me->FieldByName(fldNM)->AsFloat=abs(adj);
    me->Post();
   }
   if(gd!=Trim(me->FieldByName("gd")->AsString))
   {
    gd=Trim(me->FieldByName("gd")->AsString);
    sayADJ();
   }
   sayBAL();
   explor->gridDS->DataSet=tmpSQL;
   explor->ShowModal();

//=========================================================

   me->Next();
   }
   me->Close();
}
//---------------------------------------------------------
void TopFRM::itemPOST()
{
 //this proccess intend to post stock opname
 //to 1.wh stock
 //to 2.batch stock
 //will be grouping bincard by wh+gd+batch

 //warn if detail location not set

 opPOST();

 //AnsiString balSTR=" opstck ";
 //if(begCHECK->Checked)balSTR=" begstck ";
 //proccess here ...

 vektor=1;
 if(!onPOST())
  {
   vektor*=-1;
   posBTN->Caption="&POST";
  }

 tmpSQL->Close();
 btchSQL->ParamByName("REFNO")->AsString=opEDT->Text;
 btchSQL->Open();

 if(!btchSQL->Eof && !btchSQL->FieldByName("batch")->IsNull)
 {
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select whrec,gd,unit,uom,batch,refdt,lstdt,");
 tmpSQL->SQL->Add(" opstck as bal,handstck as last,adjstckDB as adjDB,");
 tmpSQL->SQL->Add(" adjstckCR as adjCR,adjremark as remark,");
 tmpSQL->SQL->Add(" begstck as beg,instck as db,outstck as cr from whbatch.dbe ");
 tmpSQL->SQL->Add(" where refdt>=:STDT and lstdt<=:LSTDT ");
 tmpSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 tmpSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 tmpSQL->Open();
 //proccess here
 if(!btchSQL->Eof)batchFIX();
 }
 tmpSQL->Close();
 btchSQL->Close();
/*
 stkSQL->ParamByName("REFNO")->AsString=opEDT->Text;
 stkSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 stkSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 stkSQL->Open();
*/
 //
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select whrec,gd,unit,uom,refdt,lstdt,");
 tmpSQL->SQL->Add(" opstck as bal,handstck as last,adjstckDB as adjDB,");
 tmpSQL->SQL->Add(" adjstckCR as adjCR,adjremark as remark,");
 tmpSQL->SQL->Add(" begstck as beg,instck as db,outstck as cr from whstck.dbe ");
 tmpSQL->SQL->Add(" where refdt>=:STDT and lstdt<=:LSTDT ");
 tmpSQL->ParamByName("STDT")->AsDate=mnuFRM->stDT;
 tmpSQL->ParamByName("LSTDT")->AsDate=mnuFRM->lstDT;
 tmpSQL->Open();

 //if(!stkSQL->Eof)
 stkFIX();


 //all proccess done so close
 tmpSQL->Close();
 stkSQL->Close();
 btchSQL->Close();

 if(opDS->State!=dsEdit)opSQL->Edit();
 if(vektor>0) opSQL->FieldByName("needed")->Value=opDT->Date;
 else opSQL->FieldByName("needed")->AsString="";
 opSQL->Post();
 isPOST();
}
//---------------------------------------------------------
bool TopFRM::allowPOST()
{
 bool retu;
 if(!onPOST()) ShowMessage("Already POSTED Edit not Allowed");
 else retu=true;
 return(retu);
}
//---------------------------------------------------------
void TopFRM::isPOST()
{
 //if posted
 //1. caption should be UN-POST
 //2.begCHECK=false;
 if(opSQL->FieldByName("needed")->IsNull ||
    opSQL->FieldByName("needed")->AsString=="")
  {
   posBTN->Caption="&POST";
   begCHECK->Enabled=true;
  }
 else
  {
   posBTN->Caption="&UN-POST";
   begCHECK->Enabled=false;
  }
}
//---------------------------------------------------------
void TopFRM::cmbSAY()
{
 //how to display
 //0.ware house
 //1.location
 //2.goods state
 //
 whCMB->Text=opSQL->FieldByName("whtonm")->AsString;
 locCMB->Text=opSQL->FieldByName("locnm")->AsString;
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=condCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,mnuFRM->gdSTATUS,true,opSQL->FieldByName("state")->AsString);
 condCMB->Text=strNUM;
}
//---------------------------------------------------------
void TopFRM::dateSAY()
{
 /*
 prosedur dateSAY
 set opDT->Combo by fieldNM->Value
 */
 if(!opSQL->FieldByName("trfdt")->IsNull)
 opDT->Date=
 opSQL->FieldByName("trfdt")->Value;

}
//---------------------------------------------------------
void TopFRM::itemDEL()
{
 //what the reason to delete master record?
 //detail?... hmm can do , what should be concerned about
 //hmm tanyain dulu kah mo bener hapus nih critanya
 if(!allowPOST())
  {
   if(master)opSQL->Cancel();
   dtlSQL->Cancel();
   return;
  }

 AnsiString cat_;
 if(!master)cat_=" item ";
 else cat_=" Stock Taking Form ";
 if(allowPOST() && MessageDlg("Delete this "+cat_+" ?", mtConfirmation,
                   TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
   if(!master) dtlSQL->Delete();
   else reqDEL();
  }
}
//---------------------------------------------------------
AnsiString TopFRM::cmbFND(TComboBox *myCMB,TStringList *myVAL)
{
 /*
  funct cmbFND
  return strNUM as AnsiString containing myCMB->Text
 */
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=myCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,myVAL,false,myCMB->Text);
 return(strNUM);
}
//---------------------------------------------------------
void TopFRM::varFILL()
{
 //to fill loc,cond,opdate all will be string
 wh=opSQL->FieldByName("whto")->AsString;
 loc=opSQL->FieldByName("trto")->AsString;
 cond=cmbFND(condCMB,mnuFRM->gdSTATUS);
 opdate=DateToStr(opDT->Date);
}
//---------------------------------------------------------
void TopFRM::whFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM)
{
 //to fill trfto by loc id warehouse
 //to fill state by good status
 //to fill whto by wh id
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=myCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,myVAL,false,myCMB->Text);
 if(opSQL->FieldByName(fieldNM)->AsString !=strNUM)
  {
   if(opDS->State!=dsEdit)opSQL->Edit();
   opSQL->FieldByName(fieldNM)->AsFloat=
   StrToInt(strNUM);
  }
}
//---------------------------------------------------------
void TopFRM::isDATE()
{
 /*
  prosedur isDATE
  to set fieldNM->trfDT by opDT->Value
 */
 AnsiString dt="";
 if(!opSQL->FieldByName("trfDT")->IsNull)
    dt=DateToStr(opSQL->FieldByName("trfDT")->Value);
 if(dt!=DateToStr(opDT->Date))
  {
   if(opDS->State!=dsEdit)opSQL->Edit();
   opSQL->FieldByName("trfdt")->Value=opDT->Date;
  }
}
//---------------------------------------------------------
void TopFRM::setCOND()
{
 /*
  to fill in good status combo
 */
 condCMB->Items->Clear();
 condCMB->Items->CommaText=mnuFRM->gdCMB->CommaText;
}
//---------------------------------------------------------
void TopFRM::refCHG()
{
 //   if(!opSQL->FieldByName("trfno")->IsNull)
 //   if(Trim(opEDT->Text)!=Trim(oldREF))refCHG();
 refNEW=opEDT->Text;
 if(Trim(refOLD)!=Trim(refNEW) && Trim(refOLD)!="")
  {
   //update refno if accepted
   if(MessageDlg("Change Number to "+opEDT->Text+" ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)
   {
   opEDT->Text=refOLD;
   return;
   }
   else dtlUPDATE();
  }
}
//---------------------------------------------------------
void TopFRM::key13(TObject *Sender,char Key)
{
 if(int(Key)==13)
  {
   if(opEDT->Focused()){opDT->SetFocus();return;}
   if(opDT->Focused()){relEDT->SetFocus();return;}
   if(relEDT->Focused()){relttlEDT->SetFocus();return;}
   if(relttlEDT->Focused()){aprEDT->SetFocus();return;}
   if(aprEDT->Focused()){aprttlEDT->SetFocus();return;}
   if(aprttlEDT->Focused()){whCMB->SetFocus();return;}
   if(whCMB->Focused()){locCMB->SetFocus();return;}
   if(locCMB->Focused()){dtlGRD->SetFocus();return;}
  }
}
//---------------------------------------------------------
void TopFRM::locOPEN()
{
 /*
   prosedur locOPEN
   to open location regarding wh
 */
 if(Trim(whCMB->Text)=="")return;
 bool isCHILD;
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=whCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,whVAL,false,whCMB->Text);
 //test if only one count then use one
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select count(tperec) as numb from outtype.dbe ");
 tmpSQL->SQL->Add(" where par=:PAR ");
 tmpSQL->ParamByName("PAR")->AsString=
 strNUM;
 tmpSQL->Open();
 if(tmpSQL->FieldByName("numb")->AsFloat>=1)isCHILD=true;
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select * from outlet.dbe ");
 if(!isCHILD)
  tmpSQL->SQL->Add(" where  whtpe in (select  tperec from outtype.dbe where comb=:PAR) ");
 else
  tmpSQL->SQL->Add(" where  whtpe in (select  tperec from outtype.dbe where par=:PAR) ");

 locCMB->Items->Clear();locVAL->Clear();
 tmpSQL->Close();
 tmpSQL->ParamByName("PAR")->AsString=
 strNUM;
 tmpSQL->Open();
 while(!tmpSQL->Eof)
  {
   locCMB->Items->Add(tmpSQL->FieldByName("whid")->AsString);
   locVAL->Add(tmpSQL->FieldByName("whrec")->AsString);
   tmpSQL->Next();
  }
  tmpSQL->Close();
}
//---------------------------------------------------------
void TopFRM::setWH()
{
 /*
  prosedur setWH
  fill whCMB by wh->comb
 */
 whCMB->Items->Clear();whVAL->Clear();
 whSQL->Close();
 whSQL->Open();
 while(!whSQL->Eof)
  {
   whCMB->Items->Add(whSQL->FieldByName("id")->AsString);
   whVAL->Add(whSQL->FieldByName("comb")->AsString);
   whSQL->Next();
  }
}
//---------------------------------------------------------
void TopFRM::itemADD()
{
 AnsiString cat_="";
 if(!master)cat_=" Item";
 else cat_=" Stock Taking";
 if(MessageDlg("Add New "+cat_+"?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0)==mrNo)return;
 if(master)
  {
   if(sortIT->Checked)
     {
      ShowMessage("Release sort order first");
      return;
     }
   if(!numSQL->Active)numOPEN();
   numNEW();
   opEDT->SetFocus();
  }
 else
  {
  dtlSQL->Append();
  dtlSQL->Edit();
  }
}
//---------------------------------------------------------
void TopFRM::opPOST()
{
 if(opDS->State==dsEdit || opDS->State==dsInsert)opSQL->Post();
}
//---------------------------------------------------------
void TopFRM::subPOST()
{
 if(dtlDS->State==dsEdit || dtlDS->State==dsInsert)dtlSQL->Post();
}
//---------------------------------------------------------
void TopFRM::recSAY(TDataSet *mySQL)
{
 if(!mySQL->Eof)
 recEDT->Text=IntToStr(mySQL->RecNo)+"/"+IntToStr(mySQL->RecordCount);
 else recEDT->Text="";
}
//---------------------------------------------------------
void TopFRM::subOPEN()
{
 if(Trim(opEDT->Text)!=
    Trim(dtlSQL->FieldByName("ref")->AsString))
  {
   dtlSQL->Close();
   dtlSQL->ParamByName("REFNO")->AsString=
   Trim(opSQL->FieldByName("trfno")->AsString);
   dtlSQL->Open();
  }
}
//---------------------------------------------------------
void TopFRM::tbOPEN()
{
 path_=mnuFRM->path_;
 binSQL->DatabaseName=path_+"\\DATA";
 unposSQL->DatabaseName=path_+"\\DATA";
 numSQL->DatabaseName=path_+"\\DATA";
 btchSQL->DatabaseName=path_+"\\DATA";
 stkSQL->DatabaseName=path_+"\\DATA";
 gdSQL->DatabaseName=path_+"\\DATA";
 locSQL->DatabaseName=path_+"\\DATA";
 whSQL->DatabaseName=path_+"\\DATA";
 func->setupQRY->DatabaseName=path_+"\\DATA";
 tmpSQL->DatabaseName=path_+"\\DATA";
 dtlSQL->DatabaseName=path_+"\\DATA";
 opSQL->DatabaseName=path_+"\\DATA";
}
//---------------------------------------------------------
void TopFRM::tbCLOSE()
{
 binSQL->Close();
 unposSQL->Close();
 numSQL->Close();
 btchSQL->Close();
 stkSQL->Close();
 gdSQL->Close();
 locSQL->Close();
 whSQL->Close();
 func->setupQRY->Close();
 tmpSQL->Close();
 dtlSQL->Close();
 opSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::FormClose(TObject *Sender, TCloseAction &Action)
{
tbCLOSE();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::FormCreate(TObject *Sender)
{
 teksHTML=new TStringList();teksHTML->Clear();
 teksEDIT=new TStringList();teksEDIT->Clear();
 teksTXT=new TStringList();teksTXT->Clear();
 teksRPT=new TStringList();teksRPT->Clear();

 whVAL=new TStringList();
 whVAL->Clear();whCMB->Items->Clear();
 locVAL=new TStringList();
 locVAL->Clear();locCMB->Items->Clear();
 condCMB->Items->Clear();

 fndCMB->Items->Clear(); fndCMB->Items->Add("Project/Take No.");
 fndVAL=new TStringList();fndVAL->Clear();
 fndVAL->Add("trfno");


}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opPGEnter(TObject *Sender)
{
 if(opPG->ActivePage==listSHEET)opNAV->DataSource=opDS;
 else
     if(opPG->ActivePage==opSHEET)
       if(dtlGRD->Focused())opNAV->DataSource=dtlDS;
       else opNAV->DataSource=opDS;

}
//---------------------------------------------------------------------------
void __fastcall TopFRM::addBTNClick(TObject *Sender)
{
 itemADD();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlSQLNewRecord(TDataSet *DataSet)
{
 dtlSQL->FieldByName("mode")->AsString="5";
 dtlSQL->FieldByName("ref")->AsString=
 opSQL->FieldByName("trfno")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opSQLAfterScroll(TDataSet *DataSet)
{
 refOLD="";refNEW="";
 subOPEN();
 recSAY(opSQL);
 dateSAY();
 cmbSAY();
 isPOST();
 isBLC();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlSQLAfterScroll(TDataSet *DataSet)
{
 recSAY(dtlSQL);
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::FormActivate(TObject *Sender)
{
 tbCLOSE();
 tbOPEN();
 setWH();
 setCOND();
 orgSQL=opSQL->SQL->Text;
 reqOPEN();
 //refOLD=opEDT->Text;
 opPG->ActivePage=opSHEET;
 opNAV->DataSource=opDS;
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlGRDEnter(TObject *Sender)
{
 master=false;
 opPOST();
 varFILL();
 subOPEN();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlSQLBeforeClose(TDataSet *DataSet)
{
 subPOST();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::whCMBChange(TObject *Sender)
{
 whCMBExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opEDTKeyPress(TObject *Sender, char &Key)
{
 //jaga juga bila no ini diedit
 if(int(Key)==13)refCHG();
 key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::opEDTEnter(TObject *Sender)
{
 //save no
 master=true;
 refOLD=opEDT->Text;
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::relEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::aprEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::relttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::aprttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::whCMBKeyPress(TObject *Sender, char &Key)
{
 if((int)Key==13)
  {
   whCMBExit(Sender);
   locCMB->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::locCMBKeyPress(TObject *Sender, char &Key)
{
 if((int)Key==13)
  {
  locCMBExit(Sender);
  condCMB->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::condCMBKeyPress(TObject *Sender, char &Key)
{
 if((int)Key==13)
  {
   condCMBExit(Sender);
   dtlGRD->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::whCMBExit(TObject *Sender)
{
 opCHG=true;
 if(Trim(whCMB->Text)=="")return;
 //whFILL(whCMB,whVAL,whCMB->Text,"WHTO");
 if(!whSQL->Active)whSQL->Open();
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=whCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,whVAL,false,whCMB->Text);
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive <<loPartialKey;
 whSQL->First();
 if(whSQL->Locate("COMB",strNUM,Opts))
  {
   if(opDS->State!=dsEdit)opSQL->Edit();
   opSQL->FieldByName("whto")->AsFloat=
   whSQL->FieldByName("tperec")->AsFloat;
  }
 whSQL->Close();
 locOPEN();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::locCMBExit(TObject *Sender)
{
 opCHG=true;
 if(Trim(locCMB->Text)=="")return;
 whFILL(locCMB,locVAL,locCMB->Text,"TRTO");
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opSQLBeforePost(TDataSet *DataSet)
{
 if(!allowPOST()) opSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::dtlSQLBeforeEdit(TDataSet *DataSet)
{
 //varFILL();
 if(!allowPOST())dtlSQL->Cancel();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Shift.Contains(ssAlt) && Key==int('1') ) opPG->ActivePage=listSHEET;
 if (Shift.Contains(ssAlt) && Key==int('2') ) opPG->ActivePage=opSHEET;
 if(Key==107 && Shift.Contains(ssCtrl))itemADD();
 if(Key==109 && Shift.Contains(ssCtrl))itemDEL();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opDTChange(TObject *Sender)
{
 opCHG=true;
 isDATE();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opSQLNewRecord(TDataSet *DataSet)
{
 if(opDS->State!=dsEdit)opSQL->Edit();
 opSQL->FieldByName("mode")->AsFloat=5;
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlSQLidChange(TField *Sender)
{
 gdSET();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::posBTNClick(TObject *Sender)
{
 itemPOST();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::dtlSQLBeforePost(TDataSet *DataSet)
{
 //ambil varFILL tadi buat ngepost ke bincrd
 if(allowPOST())
  {
   if(dtlDS->State!=dsEdit)dtlSQL->Edit();
   dtlSQL->FieldByName("wh")->AsString=loc;
   dtlSQL->FieldByName("refdt")->Value=StrToDate(opdate);
   dtlSQL->FieldByName("cond")->AsString=cond;
  }
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::begCHECKExit(TObject *Sender)
{
 /*
  update bincrd for beginning balance
  set jobid n ref
 */
 //exit will be set var?
 if(!allowPOST())
  {
   opSQL->Cancel();
   return;
  }
 AnsiString jobid=begCHECK->Checked?"Beginning Blc":" ";
 if(opDS->State!=dsEdit)opSQL->Edit();
 opSQL->FieldByName("job")->AsString=jobid;
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" update bincrd.dbe ");
 tmpSQL->SQL->Add(" set jobid=:JOBID ");
 tmpSQL->SQL->Add(" where ref=:REF ");
 tmpSQL->ParamByName("JOBID")->AsString=jobid;
 tmpSQL->ParamByName("REF")->AsString=
 Trim(opEDT->Text);
 tmpSQL->ExecSQL();
 tmpSQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::opSQLBeforeEdit(TDataSet *DataSet)
{
 if(!allowPOST())opSQL->Cancel();
 refOLD=Trim(opEDT->Text);
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::opSQLAfterEdit(TDataSet *DataSet)
{
 if(!allowPOST())opSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::opSQLBeforeDelete(TDataSet *DataSet)
{
 if(!allowPOST())opSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::dtlSQLBeforeDelete(TDataSet *DataSet)
{
 if(!allowPOST())dtlSQL->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::dtlSQLAfterEdit(TDataSet *DataSet)
{
 if(!allowPOST())dtlSQL->Cancel();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::opSQLAfterPost(TDataSet *DataSet)
{
 varFILL();
 bincrdFIX();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::condCMBExit(TObject *Sender)
{
 opCHG=true;
 if(Trim(condCMB->Text)=="")return;
 condFILL();
}
//---------------------------------------------------------------------------
void __fastcall TopFRM::prtBTNClick(TObject *Sender)
{
 opRPT();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::dtlSQLgdnmChange(TField *Sender)
{
 //gdSET();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::whCMBEnter(TObject *Sender)
{
 byPOST();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::opDTEnter(TObject *Sender)
{
 byPOST();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::relEDTEnter(TObject *Sender)
{
 byPOST();    
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::aprEDTEnter(TObject *Sender)
{
 byPOST();    
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::aprttlEDTEnter(TObject *Sender)
{
 byPOST();
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::locCMBEnter(TObject *Sender)
{
 byPOST();    
}
//---------------------------------------------------------------------------

void __fastcall TopFRM::condCMBEnter(TObject *Sender)
{
 byPOST();
}
//---------------------------------------------------------------------------


void __fastcall TopFRM::fndEDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)
 func->locateIT(fndCMB,fndEDT,opSQL,fndVAL,AnsiString("NONEED"));
}
//---------------------------------------------------------------------------

