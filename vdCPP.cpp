//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "vdCPP.h"
#include "mnuCPP.h"
#include "funcCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TvdFRM *vdFRM;
//---------------------------------------------------------------------------
__fastcall TvdFRM::TvdFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//-----------------------------------------------------
void TvdFRM::idCHECK()
{
 /*
  prosedur idCHECK
  cek if not empty item id
  cek on item for gdrec=item->gd
 */
 if(itemSQL->Eof || itemSQL->FieldByName("gd")->IsNull)return;
 //check id supaya benar-benar hanya category itu saja
 gdSQL->First();
 if(!gdSQL->Locate("gdrec",IntToStr(itemSQL->FieldByName("gd")->AsFloat),seek))
  {
   ShowMessage(" Not Valid Category ");
   if(itemDS->State!=dsEdit)itemSQL->Edit();
   itemSQL->FieldByName("gd")->AsFloat=0;
  }
}
//-----------------------------------------------------
void TvdFRM::gdOPEN()
{
 /*
 prosedur gdOPEN
 for opening item regarding category if choosen
 not complished yet
 */
 gdSQL->Close();
 //open gdSQL by param from ctgryCMB
 //so first seek the valid ctgryVAL to put in
 gdSQL->SQL->Clear();
 gdSQL->SQL->Add(gdORG);
 if(ctgryCHG && Trim(ctgryCMB->Text)!="")
  {
   gdSQL->SQL->Add(" where (ctgry in (select catrec from catitem.dbe where catparent='"+
                   Trim(ctgryVAL->Strings[ctgryCMB->ItemIndex])+"')) ");
  }
  gdSQL->Open();
}
//-----------------------------------------------------
void TvdFRM::ctgrySET()
{
 /*
   prosedur ctgrySET
   to fill in ctgryCMB
 */
 tmpSQL->Close();
 tmpSQL->SQL->Clear();
 tmpSQL->SQL->Add(" select * from catitem.dbe "); //where catcld=false ");
 tmpSQL->Open();
 while(!tmpSQL->Eof)
  {
   ctgryCMB->Items->Add(tmpSQL->FieldByName("catid")->AsString);
   ctgryVAL->Add(tmpSQL->FieldByName("catcomb")->AsString);
   tmpSQL->Next();
  }
 tmpSQL->Close();
}
//-----------------------------------------------------
void TvdFRM::idSAY()
{
 /*
  prosedur idSAY
  say it ctyCMB
  cntryCMB
  provCMB
 */
 TStringList *myLST=new TStringList();
 //ctyCMB
 myLST->Clear();
 myLST->CommaText=ctyCMB->Items->CommaText;
 ctyCMB->Text=
 func->fndLST(myLST,ctyVAL,true,vdSQL->FieldByName("cty")->AsString);
 //provCMB
 myLST->Clear();
 myLST->CommaText=provCMB->Items->CommaText;
 provCMB->Text=
 func->fndLST(myLST,provVAL,true,vdSQL->FieldByName("prov")->AsString);
 //cntryCMB
 myLST->Clear();
 myLST->CommaText=cntryCMB->Items->CommaText;
 cntryCMB->Text=
 func->fndLST(myLST,cntryVAL,true,vdSQL->FieldByName("cntry")->AsString);
}
//-----------------------------------------------------
void TvdFRM::idFILL(TComboBox *myCMB,TStringList *myVAL,AnsiString strSEEK,AnsiString fieldNM)
{
 /*
  prosedur idFILL
  to fill in fieldNM by myVAL regarding myCMB->Text
 */
 TStringList *myLST=new TStringList();
 myLST->Clear();
 myLST->CommaText=myCMB->Items->CommaText;
 AnsiString strNUM=
 func->fndLST(myLST,myVAL,false,myCMB->Text);
 if(vdSQL->FieldByName(fieldNM)->AsString !=strNUM)
  {
   if(vdDS->State!=dsEdit)vdSQL->Edit();
   vdSQL->FieldByName(fieldNM)->AsFloat=
   StrToInt(strNUM);
   vdSQL->Post();
  }
}
//----------------------------------------------------
void TvdFRM::recSAY(TDataSet *mySQL)
{
 /*
 say record id
 */
 if(!mySQL->Eof)recEDT->Text=IntToStr(vdSQL->RecNo)+"/"+IntToStr(vdSQL->RecordCount);
 else recEDT->Text="";
}
//----------------------------------------------------
void TvdFRM::setCMB()
{
 //use to fill combo choices
 ctgryCMB->Items->Clear();ctgryVAL->Clear();
 ctgrySET();
 ctyCMB->Items->Clear();ctyVAL->Clear();
 func->comboSET(tmpSQL,"CTY",ctyCMB,ctyVAL);
 provCMB->Items->Clear();provVAL->Clear();
 func->comboSET(tmpSQL,"PROV",provCMB,provVAL);
 cntryCMB->Items->Clear();cntryVAL->Clear();
 func->comboSET(tmpSQL,"CNTRY",cntryCMB,cntryVAL);
}
//----------------------------------------------------
void TvdFRM::vdPOST()
{
 if(vdDS->State==dsEdit || vdSQL->State==dsInsert)vdSQL->Post();
}
//----------------------------------------------------
void TvdFRM::itemPOST()
{
 if(itemDS->State==dsEdit || itemDS->State==dsInsert)itemSQL->Post();
}
//----------------------------------------------------
void TvdFRM::key13(TObject *Sender, char &Key)
{
 if(int(Key)==13)
   {
   if(idEDT->Focused()){nmEDT->SetFocus();return;}
   if(nmEDT->Focused()){contEDT->SetFocus();return;}
   if(contEDT->Focused()){ttlEDT->SetFocus();return;}
   if(ttlEDT->Focused()){gmEDT->SetFocus();return;}
   if(gmEDT->Focused()){st1EDT->SetFocus();return;}
   if(st1EDT->Focused()){st2EDT->SetFocus();return;}
   if(st2EDT->Focused()){phEDT->SetFocus();return;}
   if(phEDT->Focused()){fxEDT->SetFocus();return;}
   if(fxEDT->Focused()){mailEDT->SetFocus();return;}
   if(mailEDT->Focused()){ctyCMB->SetFocus();return;}
  }
}
//-----------------------------------------------------
void TvdFRM::vdDEL()
{
 bool goes_=true;
 //where vendor goes?
 //1.vditem
 //2.req
 TStringList *tbDESC=new TStringList();tbDESC->Clear();
 TStringList *tbLIST=new TStringList();tbLIST->Clear();
 tbLIST->Add("VDITEM.dbe");tbDESC->Add(" Item By Vendor ");
 tbLIST->Add("REQ.dbe");tbDESC->Add(" Transaction ");
 for(int i=1;i<=tbLIST->Count;i++)
  {
   tmpSQL->Close();
   tmpSQL->SQL->Clear();
   tmpSQL->SQL->Add(" select vd from VDITEM.dbe where vd=:VDID ");
   tmpSQL->ParamByName("VDID")->AsFloat=
   vdSQL->FieldByName("vdrec")->AsFloat;
   tmpSQL->Prepare();
   tmpSQL->ExecSQL();
   tmpSQL->Close();
   if(!tmpSQL->Eof)
    {
     goes_=false;
     ShowMessage(" vendor already used in transaction,process cancel ");
     return;
    }
  }
 if(goes_)vdSQL->Delete();

}
//-----------------------------------------------------
void TvdFRM::itemDEL()
{
if (MessageDlg("Delete this Item?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
 {
  if(srPG->ActivePage==detSHEET)vdDEL();
  else
   if(srPG->ActivePage==itemSHEET) itemSQL->Delete();
 }
}
//-----------------------------------------------------
void TvdFRM::itemNEW()
{
if (MessageDlg("Add new Item?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
 {
  if(srPG->ActivePage==detSHEET) vdSQL->Append();
  else
   if(srPG->ActivePage==itemSHEET) itemSQL->Append();
 }
}
//-----------------------------------------------------
void TvdFRM::itemOPEN()
{
 /*
   prosedur itemOPEN
   re-open if vd id!=item->vd or "ctgryCHG"
   so open by param vdrec + ctgry
 */

 bool go;
 if(vdSQL->FieldByName("vdrec")->AsFloat!=
    itemSQL->FieldByName("vd")->AsFloat) go=true;
 else ctgryCMB->Text="";
 if(!go && ctgryCHG) go=true;
 if(go)
  {
   itemSQL->Close();
   itemSQL->SQL->Clear();
   itemSQL->SQL->Add(itemORG);
   itemSQL->ParamByName("VDID")->AsFloat=
   vdSQL->FieldByName("vdrec")->AsFloat;
   if(ctgryCHG && Trim(ctgryCMB->Text)!="")
    {
     AnsiString gdSEL="";
     AnsiString catSEL="";
     catSEL+="(select c.catrec from catitem.dbe c where c.catparent='"+
             Trim(ctgryVAL->Strings[ctgryCMB->ItemIndex])+"' )";
     gdSEL+=" AND (v.gd in ";
     gdSEL+="(select i.gdrec from item.dbe i where i.ctgry in"+
            catSEL+"))";
     itemSQL->SQL->Add(gdSEL);
     //ShowMessage(itemSQL->SQL->Text);
    }
   itemSQL->Open();
  }
}
//-----------------------------------------------------
void TvdFRM::tbOPEN()
{
 tbCLOSE();
 AnsiString path_=mnuFRM->path_;
 func->setupQRY->DatabaseName=path_+"\\DATA";
 gdSQL->DatabaseName=path_+"\\DATA";
 vdSQL->DatabaseName=path_+"\\DATA";
 itemSQL->DatabaseName=path_+"\\DATA";
 tmpSQL->DatabaseName=path_+"\\DATA";
}
//-----------------------------------------------------
void TvdFRM::tbCLOSE()
{
 func->setupQRY->Close();
 tmpSQL->Close();
 gdSQL->Close();
 vdSQL->Close();
 itemSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::FormClose(TObject *Sender, TCloseAction &Action)
{
 if(vdDS->State==dsEdit)vdSQL->Post();
 if(itemDS->State==dsEdit)itemSQL->Post();
 tbCLOSE();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::FormActivate(TObject *Sender)
{
 srPG->ActivePage=detSHEET;
 srNAV->DataSource=vdDS;
 tbCLOSE();
 tbOPEN();
 setCMB();
 vdSQL->Open();
 idEDT->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Shift.Contains(ssAlt) && Key==int('1') ) srPG->ActivePage=detSHEET;
 if (Shift.Contains(ssAlt) && Key==int('2') ) srPG->ActivePage=lstSHEET;
 if (Shift.Contains(ssAlt) && Key==int('3') ) srPG->ActivePage=itemSHEET;
 if(Key==107 && Shift.Contains(ssCtrl))
   itemNEW();

 if(Key==109 && Shift.Contains(ssCtrl))
  itemDEL();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::FormCreate(TObject *Sender)
{
 tbCLOSE();
 ctgryCHG=false;
 itemORG=itemSQL->SQL->Text;
 seek<<loPartialKey<<loCaseInsensitive;
 gdORG=gdSQL->SQL->Text;
 ctgryVAL=new TStringList();
 ctgryVAL->Clear();ctgryCMB->Items->Clear();
 fndCMB->Items->Clear();
 fndVAL=new TStringList();
 fndVAL->Clear();
 ctyCMB->Items->Clear();
 ctyVAL=new TStringList();
 ctyVAL->Clear();
 provCMB->Items->Clear();
 provVAL=new TStringList();
 provVAL->Clear();
 cntryCMB->Items->Clear();
 cntryVAL=new TStringList();
 cntryVAL->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::vdSQLAfterPost(TDataSet *DataSet)
{
 if(vdSQL->FieldByName("vdrec")->IsNull)
  {
   if(vdDS->State!=dsEdit)vdSQL->Edit();
   vdSQL->FieldByName("vdrec")->AsFloat=
   vdSQL->RecNo;
   vdSQL->Post();
  }
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::srPGEnter(TObject *Sender)
{
 if(srPG->ActivePage==detSHEET || srPG->ActivePage==lstSHEET) srNAV->DataSource=vdDS;
 else srNAV->DataSource=itemDS;
 if(srPG->ActivePage==itemSHEET)
  {
   ctgryCHG=false;
   vdTEXT->Caption="Item Product By : "+
                   vdSQL->FieldByName("nm")->AsString;
   itemOPEN();
  }
 else itemSQL->Close();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::itemSQLNewRecord(TDataSet *DataSet)
{
 /*
  set item->vd by vd->record id
 */
 itemSQL->FieldByName("vd")->AsFloat=
 vdSQL->FieldByName("vdrec")->AsFloat;
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::idEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::nmEDTKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::contEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::ttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::gmEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::st1EDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::st2EDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::phEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::fxEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::mailEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::ctyCMBKeyPress(TObject *Sender, char &Key)
{
 /*
  in case key13
  fill in if doesn't exist, n put also to table setup
  then set cty=ctyVAL by setup->record id
 */
if(int(Key)==13)
 {
  if(!func->isFILL(ctyCMB))
   {
    func->setupFILL(func->setupQRY,"CTY",ctyCMB);
    ctyVAL->Add(func->setupQRY->FieldByName("recid")->AsString);
   }
   ctyCMBExit(Sender);
   provCMB->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::provCMBKeyPress(TObject *Sender, char &Key)
{
 /*
  in case key13
  fill in if doesn't exist, n put also to table setup
  then set prov=provVAL by setup->record id
 */
if(int(Key)==13)
 {
  if(!func->isFILL(provCMB))
   {
    func->setupFILL(func->setupQRY,"PROV",provCMB);
    provVAL->Add(func->setupQRY->FieldByName("recid")->AsString);
   }
  provCMBExit(Sender);
  cntryCMB->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::cntryCMBKeyPress(TObject *Sender, char &Key)
{
 /*
  in case key13
  fill in if doesn't exist, n put also to table setup
  then set cntry=cntryVAL by setup->record id
 */

if(int(Key)==13)
 {
   if(!func->isFILL(cntryCMB))
   {
    func->setupFILL(func->setupQRY,"CNTRY",cntryCMB);
    cntryVAL->Add(func->setupQRY->FieldByName("recid")->AsString);
   }
  cntryCMBExit(Sender);
  idEDT->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::itemGRDColEnter(TObject *Sender)
{
 //check if lastdate enter , try to give default date if null
 if(UpperCase(itemGRD->SelectedField->FieldName)=="LAST" &&
    itemSQL->FieldByName("LAST")->IsNull)
   {
    if(itemDS->State!=dsEdit)itemSQL->Edit();
    itemSQL->FieldByName("last")->Value=Date();
   }
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::vdSQLAfterScroll(TDataSet *DataSet)
{
//dont forget to say cmb
ctyCMB->Text="";provCMB->Text="";cntryCMB->Text="";
idSAY();
recSAY(vdSQL);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::itemSQLAfterScroll(TDataSet *DataSet)
{
recSAY(itemSQL);
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::itemSQLBeforeClose(TDataSet *DataSet)
{
itemPOST();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::vdSQLBeforeClose(TDataSet *DataSet)
{
vdPOST();
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::ctyCMBExit(TObject *Sender)
{
 /*
  set vd->cty=ctyVAL by seeking ctyCMB->Text
 */
idFILL(ctyCMB,ctyVAL,ctyCMB->Text,"cty");
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::provCMBExit(TObject *Sender)
{
 /*
  set vd->prov=provVAL by seeking provCMB->Text
 */
idFILL(provCMB,provVAL,provCMB->Text,"prov");
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::cntryCMBExit(TObject *Sender)
{
 /*
  set vd->cntry=cntryVAL by seeking cntryCMB->Text
 */
idFILL(cntryCMB,cntryVAL,cntryCMB->Text,"cntry");
}
//---------------------------------------------------------------------------
void __fastcall TvdFRM::addBTNClick(TObject *Sender)
{
 itemNEW();
}
//---------------------------------------------------------------------------

void __fastcall TvdFRM::ctgryCMBChange(TObject *Sender)
{
 ctgryCHG=true;
 gdOPEN();
 itemOPEN();
}
//---------------------------------------------------------------------------

void __fastcall TvdFRM::itemGRDColExit(TObject *Sender)
{
 //please do remember do filter id n nm
 //for parameterized by ctgry
 if(UpperCase(itemGRD->SelectedField->FieldName)=="ID")idCHECK();
 if(UpperCase(itemGRD->SelectedField->FieldName)=="NM")idCHECK();
}
//---------------------------------------------------------------------------


