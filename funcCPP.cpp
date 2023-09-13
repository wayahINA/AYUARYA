//---------------------------------------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <vcl.h>
#include <FileCtrl.hpp>
#pragma hdrstop

#include "funcCPP.h"
//#include "ividp.h"
#include "BARCPP.h"
//#include "ividp.h"
#include "mnuCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfunc *func;
//---------------------------------------------------------------------------
__fastcall Tfunc::Tfunc(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void Tfunc::DirPack(AnsiString whose_)
{
 AnsiString fromFile;
 bool pack_;
 TSearchRec hasil_;
 //AnsiString filenya="\\DB"+FormatDateTime("mmyyyy",mnufrm->DateTimePicker1->Date);
 AnsiString bypath=path_+"\\"+whose_;
 if (FindFirst(bypath+"\\*.*", faAnyFile, hasil_) == 0)
  {
   TbarFRM *FRM=new TbarFRM(this);
   FRM->gmb->Visible=true;
   FRM->gmb->Active=true;

   FRM->bar->Step=1;
   FRM->bar->Left=0;

   FRM->Caption="Wait... compacting database";
   FRM->Show();

   fromFile=hasil_.Name;
   FRM->txt->Caption="Compacting File ... "+
   fromFile;
   FRM->Update();

   pack_=true;
   if(atSTR(fromFile,AnsiString("."),pos))pack_=false;
   if(pack_ && atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

   if(pack_) packDBF(fromFile);

   while (FindNext(hasil_) == 0)
   {
    fromFile=hasil_.Name;
    FRM->txt->Caption="Compacting File ... "+
    fromFile;
    FRM->Update();

    //pack dbf first
   pack_=true;
   if(atSTR(fromFile,AnsiString("."),pos) && fromFile.Length()<2 ) pack_=false;

   if(pack_ && atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

    if(pack_)packDBF(fromFile);

   }
   FRM->Hide();
   delete FRM;
  }
 FindClose(hasil_);
}
//---------------------------------------------------------
void  Tfunc::DirCopy(AnsiString where_,AnsiString to_)
{
 bool pack_;
 TSearchRec hasil_;
 //AnsiString filenya="\\DB"+FormatDateTime("mmyyyy",mnufrm->DateTimePicker1->Date);
 AnsiString topath=to_;
 AnsiString bypath=where_;
 AnsiString fromFile,_frm,_to;
 _frm=bypath;
 _to=topath;
 ForceDirectories(topath);
 if (!DirectoryExists(topath)) ShowMessage("failed while creating directory structure "+topath);
 if (FindFirst(_frm+"\\*.*", faAnyFile, hasil_) == 0)
  {
   loged=true;
   TbarFRM *FRM=new TbarFRM(this);
   FRM->gmb->Visible=true;
   FRM->gmb->Active=true;

   FRM->bar->Step=1;
   FRM->bar->Left=0;

   FRM->Caption="Wait... backup database";
   FRM->Show();

   fromFile=hasil_.Name;
   FRM->txt->Caption="Copying File ... "+
   fromFile+" from "+_frm+" to "+_to;
   FRM->Update();

   FileCopy(_frm+"\\"+fromFile,_to+"\\"+fromFile);
   while (FindNext(hasil_) == 0)
   {
    fromFile=hasil_.Name;
    FRM->txt->Caption="Copying File ... "+
    fromFile+" from "+_frm+" to "+_to;;
    FRM->Update();

    FileCopy(_frm+"\\"+fromFile,_to+"\\"+fromFile);
   }
   FRM->Hide();
   delete FRM;
  }
 FindClose(hasil_);
}
//---------------------------------------------------------------------------
bool Tfunc::packDBF(AnsiString tbNM)
{
 bool active = dummyTB->Active;
 bool exclusive = dummyTB->Exclusive;
 bool retval = true;

 dummyTB->Close();
 dummyTB->TableName=tbNM;

 try
 {
  if (!exclusive)
  {
   dummyTB->Active = false;
   dummyTB->Exclusive = true;
  }
  dummyTB->Active = true;

  CURProps props;
  Check(DbiGetCursorProps(dummyTB->Handle, props));

  String tableType = props.szTableType;
  if (tableType == szPARADOX)
  {
   CRTblDesc tableDesc;
   memset(&tableDesc, 0, sizeof(tableDesc));
   lstrcpy(tableDesc.szTblName,
           dummyTB->TableName.c_str());
   lstrcpy(tableDesc.szTblType, szPARADOX);
   tableDesc.bPack = true;

   hDBIDb hDb = dummyTB->DBHandle;
   dummyTB->Close();
   Check(DbiDoRestructure(hDb, 1, &tableDesc,
         0, 0, 0, false));
   dummyTB->Open();
  }
  else
  {
   if (tableType == szDBASE)
     Check(DbiPackTable(dummyTB->DBHandle,
           dummyTB->Handle, 0, szDBASE, true));
   else retval = false;
  }
 }
 catch(...)
 {
  retval = false;
 }

 dummyTB->Active = false;
 dummyTB->Exclusive = exclusive;
 dummyTB->Active = active;
 return(retval);
}
//---------------------------------------------------------------------------
void Tfunc::fixDate(TDateTimePicker *lstmth,TDateTimePicker *&newmth)
{
 bool udah;
 AnsiString lmth,nmth;
 while(!udah)
 {
  lmth=FormatDateTime("mm",lstmth->Date);
  nmth=FormatDateTime("mm",newmth->Date);
  if(StrToInt(nmth)<=StrToInt(lmth))udah=true;
  else newmth->Date=newmth->Date-1;
 }
 return;
}
//---------------------------------------------------------
void Tfunc::sortLST(TStringList *lst,AnsiString subSTR)
{
 int thereIS=-1;
 if(lst->Count!=0) thereIS=lst->IndexOf(subSTR);
 if(thereIS<0)lst->Add(subSTR);
 else lst->Delete(thereIS);
}
//---------------------------------------------------------
void Tfunc::uploadTB(AnsiString myFile)
{
 AnsiString oldF=myFile+".DBE";
 AnsiString newF=myFile+".BAC";
 try
 {
  dummy->DatabaseName=path_+"\\DATA";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(" insert into "+oldF+" select * from "+newF);
  dummy->Prepare();
  dummy->ExecSQL();
  dummy->Close();
 }
 catch(...)
 {
  ShowMessage("Failed on uploading File "+myFile);
 }
}
//---------------------------------------------------------
void  Tfunc::PathCopy(bool to_,AnsiString whose_)
{
 bool pack_;
 TSearchRec hasil_;
 //AnsiString filenya="\\DB"+FormatDateTime("mmyyyy",mnufrm->DateTimePicker1->Date);
 AnsiString topath=path_+"\\BACKUP\\"+whose_;
 AnsiString bypath=path_+"\\"+whose_;
 AnsiString fromFile,_frm,_to;
 _frm=to_==true?bypath:topath;
 _to=to_==true?topath:bypath;
 ForceDirectories(topath);
 if (!DirectoryExists(topath)) ShowMessage("failed while creating directory structure "+topath);
 if (FindFirst(_frm+"\\*.*", faAnyFile, hasil_) == 0)
  {
   TbarFRM *FRM=new TbarFRM(this);
   FRM->gmb->Visible=true;
   FRM->gmb->Active=true;

   FRM->bar->Step=1;
   FRM->bar->Left=0;

   FRM->Caption="Wait... backup database";
   FRM->Show();

   fromFile=hasil_.Name;
   FRM->txt->Caption="Copying File ... "+
   fromFile;
   FRM->Update();

   pack_=true;
   if(atSTR(fromFile,AnsiString("."),pos))pack_=false;
   if(pack_ && atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

   if(pack_) packDBF(fromFile);

   FileCopy(_frm+"\\"+fromFile,_to+"\\"+fromFile);
   while (FindNext(hasil_) == 0)
   {
    fromFile=hasil_.Name;
    FRM->txt->Caption="Copying File ... "+
    fromFile;
    FRM->Update();

    //pack dbf first
   pack_=true;
   if(atSTR(fromFile,AnsiString("."),pos))pack_=false;
   if(pack_ && atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

    if(pack_)packDBF(fromFile);

    FileCopy(_frm+"\\"+fromFile,_to+"\\"+fromFile);
   }
   FRM->Hide();
   delete FRM;
  }
 FindClose(hasil_);
}
//-------------------------------------------------------------------------------
bool Tfunc::isBDE()
{
 try
 {
 func->hisDB->Connected=true;
 return(true);
 }
 catch(...)
 {
  func->bdeINST();
  Application->Terminate();
  return(false);
 }
}
//---------------------------------------------------------
void Tfunc::bdeINST()
{
 //AnsiString fiPATH=mnuFRM->pathAPP_+"\\MASTER\\BDE\\144MB\\DISK1";
 AnsiString fiPATH=mnuFRM->pathAPP_+"\\BDE";
 AnsiString fiHELP=fiPATH+"\\setup.EXE";
 //print using shellobj
 if(FileExists(fiHELP))
 {
 ShellExecute(
    Handle,	// handle to parent window
    "open",	// pointer to string that specifies operation to perform
    fiHELP.c_str(),	// pointer to filename or folder name string
    NULL,	// pointer to string that specifies executable-file parameters
    fiPATH.c_str(),	// pointer to string that specifies default directory
    SW_SHOW 	// whether file is shown when opened
   );
 }
 else
  {
   AnsiString msg_="";
   msg_+="Database Engine Not Found\n";
   msg_+="Please make sure you have SETUP.EXE in\n";
   msg_+="Directory \:";
   msg_+=fiPATH.c_str();
   msg_+="\n\n";
   msg_+="Application now will terminated\n";
   ShowMessage(msg_);
   Application->Terminate();
  }
}
//---------------------------------------------------------
void Tfunc::session_START()
{
 Session->PrivateDir="c:";
 hisDB->Directory=Session->PrivateDir;
}
//---------------------------------------------------------
void Tfunc::listIT(TStringList *&myLIST,AnsiString srcSTR)
{
 AnsiString mySTR=srcSTR;
 AnsiString yaSTR="";
 int pos_;
 myLIST->Clear();
 bool udah;
 while(!udah)
  {
   mySTR=Trim(mySTR);
   pos_=mySTR.AnsiPos(AnsiString(" "));
   pos_=pos_==0?mySTR.AnsiPos(AnsiString(",")):pos_;
   if(pos_>0)
   {
    yaSTR=mySTR.SubString(1,pos_);
    mySTR=mySTR.SubString(pos_,mySTR.Length()-yaSTR.Length()+1);
    myLIST->Add(Trim(yaSTR));
   }
   else udah=true;
  }
  myLIST->Add(mySTR);
}
//---------------------------------------------------------
void Tfunc::seekIT(TQuery *&mySQL,TStringList *myFND,AnsiString sqlSTR,AnsiString fldNM)
{
 AnsiString posSTR;
 mySQL->Close();
 mySQL->SQL->Clear();
 mySQL->SQL->Add(sqlSTR);
 //"SELECT * FROM "+QuotedStr(tbname)+" book");
 AnsiString seekSTR=" WHERE ";
 int pos;
 if(atSTR(sqlSTR,AnsiString("where"),pos))seekSTR=" AND ";
 for(int i=1;i<=myFND->Count;i++)
  {
   posSTR=myFND->Strings[i-1];
   seekSTR+="UPPER(book."+
             QuotedStr(fldNM)+") LIKE "+
             QuotedStr("%"+Trim(posSTR.UpperCase())+"%");

   if(myFND->Count>1 && i<myFND->Count) seekSTR+=" AND ";

  }
  mySQL->SQL->Add(seekSTR);
  AnsiString ordSTR="   ORDER BY book."+QuotedStr(fldNM)+"  ";
  mySQL->SQL->Add(ordSTR);
  mySQL->Open();
}
//---------------------------------------------------------
void Tfunc::sortIT(TColumn *kolom,AnsiString &sortSTR)
{
 int thereIS=-1;
 if(sortNM->Count!=0) thereIS=sortNM->IndexOf(kolom->Title->Caption.Trim());
 if(thereIS<0)
  {
   sortNM->Add(kolom->Title->Caption.Trim());
   sortVAL->Add(kolom->FieldName);
  }
 else
  {
   sortNM->Delete(thereIS);
   sortVAL->Delete(thereIS);
  }

 sortSTR="";
 for(int i=0;i<=sortVAL->Count-1;i++)
   {
    AnsiString space_=" ";
    if(i<sortVAL->Count-1) space_=",";
    sortSTR+=sortVAL->Strings[i];
    sortSTR+=space_;
   }

}
//----------------------------------------------------------
bool Tfunc::strTRUE(AnsiString strDEST,AnsiString strSRC)
{
 return(Trim(strDEST)==Trim(strSRC));
}
//----------------------------------------------------------
bool Tfunc::scanSTR(TStringList *srcSTR,AnsiString subSTR,int &pos)
{
 pos=srcSTR->IndexOf(subSTR);
 if( pos>=0)return(true);
 else return(false);
}
//----------------------------------------------------------
bool Tfunc::atSTR(AnsiString srcSTR,AnsiString subSTR,int &pos)
{
 srcSTR=Trim(srcSTR);
 subSTR=Trim(subSTR);
 pos=srcSTR.AnsiPos(subSTR);
 if(pos>0)return(true);
 else return(false);
}
//----------------------------------------------------------
bool Tfunc::VarLOCATE(AnsiString mixFIELD,Variant *mixVAL,int varLENGTH,TDataSet *mySQL)
{
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive <<loPartialKey;
 mySQL->First();
 return(mySQL->Locate(mixFIELD,VarArrayOf(mixVAL,varLENGTH), Opts));
}
//----------------------------------------------------------
void Tfunc::setupFILL(TDataSet *&mySQL,AnsiString keySTR,TComboBox *myCMB)
{
  if(!mySQL->Active)mySQL->Open();
  mySQL->Append();
  mySQL->Edit();
  mySQL->FieldByName("IDKEY")->AsString=keySTR;
  mySQL->FieldByName("ID1")->AsString=myCMB->Text.Trim();
  mySQL->Post();
  mySQL->Edit();
  mySQL->FieldByName("RECID")->AsFloat=mySQL->RecNo;
  mySQL->Post();
}
//----------------------------------------------------------
void Tfunc::comboSET(TQuery *mySQL,AnsiString keySTR,TComboBox *&myCMB,TStringList *&myVAL)
{
 mySQL->Close();
 mySQL->SQL->Clear();
 mySQL->SQL->Add("select * from setup where upper(idkey) like '"+UpperCase(Trim(keySTR))+"' ");
 mySQL->Open();
 while(!mySQL->Eof)
 {
  myCMB->Items->Add(Trim(mySQL->FieldByName("id1")->AsString));
  myVAL->Add(mySQL->FieldByName("RECID")->AsString);
  mySQL->Next();
 }
}
//----------------------------------------------------------
bool Tfunc::isFILL(TComboBox *&myCMB)
{
 bool result=true;
 int ada=myCMB->Items->IndexOf(myCMB->Text.Trim());
 if (ada<0)
  {
   if(MessageDlg("Add New Choice?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
    {
     myCMB->Items->Add(myCMB->Text.Trim());
     result=false;
    }
   else
    {
     myCMB->Text="";
     result=true;
    }
  }
 return(result);
}
//----------------------------------------------------------
void Tfunc::openSQL(TQuery *&mySQL,AnsiString sqlSTR,AnsiString filterSTR,AnsiString sortSTR)
{
AnsiString sqlTXT;
sqlTXT+=sqlSTR;
if (filterSTR.Trim()!="") sqlTXT+= " WHERE ( "+filterSTR+" )";
if (sortSTR.Trim()!="") sqlTXT+= " ORDER BY "+sortSTR;
mySQL->SQL->Clear();
mySQL->SQL->Add(sqlTXT);
}
//----------------------------------------------------------
AnsiString Tfunc::fndLST(TStringList *lstCMB,TStringList *lstVAL,bool mode,AnsiString seekSTR)
{
 //mode 0 , seek to cmb
 //mode 1 seek to val
 AnsiString fndSTR="";
 int found;
 if(!mode)found=lstCMB->IndexOf(seekSTR);
 else found=lstVAL->IndexOf(seekSTR);
 if(found>=0)
  if(!mode)fndSTR=lstVAL->Strings[found];
  else fndSTR=lstCMB->Strings[found];
 return(fndSTR);
}
//---------------------------------------------------------------------------
void Tfunc::recPOP(AnsiString recSTR,TBookmark rec_,TDataSet *mySQL)
{
 mySQL->Bookmark=recSTR;
 mySQL->GotoBookmark(rec_);
 mySQL->FreeBookmark(rec_);
}
//---------------------------------------------------------------------------
void Tfunc::recPUSH(TBookmark &rec_,TDataSet *mySQL)
{
 rec_=mySQL->GetBookmark();
}
//---------------------------------------------------------------------------
void Tfunc::createTable(AnsiString path_,AnsiString tblName,AnsiString sourceName)
{
 if(!FileExists(path_+tblName))
   {
    dummy->Close();
    dummy->DatabaseName=path_;
    dummy->SQL->Clear();
    dummy->SQL->LoadFromFile(sourceName);
    dummy->Prepare();
    dummy->ExecSQL();
    dummy->Close();
   }
 dummy->Close();
}
//---------------------------------------------------------------------------
bool  Tfunc::cekPAS(AnsiString procname_)
{
 bool retu_=false;
 passKEY=mnuFRM->varPAS->Strings[mnuFRM->varMNU->IndexOf(procname_)];
 AnsiString s_=AnsiString::StringOfChar('0',passKEY.Trim().Length());
 if(passKEY.Trim()!=s_.Trim()) retu_=true;
 if(mnuFRM->pass_) retu_=true;
 if(retu_==false) ShowMessage("Sorry ,Authorization cancelled...");
 return(retu_) ;
}
//===========================================================================
bool Tfunc::valPAS(int idx_)
{
 bool retu_=false;
 if(passKEY.Trim().SubString(idx_,1)=="1") retu_=true;
 if(retu_==false) ShowMessage("Sorry ,Authorization cancelled...");
 return(retu_) ;
}
//===========================================================================
void Tfunc::ceksetup()
{
//AnsiString setupDir=mnuFRM->path_+"\\SETUPDIR";
AnsiString setupDir=mnuFRM->path_+"\\DATA";
ForceDirectories(setupDir);
setupDir+="\\setup.DBF";
if (!FileExists(setupDir))
  {
   AnsiString s_=setupQRY->SQL->Text;
   setupQRY->SQL->Clear();
   setupQRY->SQL->Add("create table '"+setupDir+"'");
   setupQRY->SQL->Add("( recid numeric(10,3),");
   setupQRY->SQL->Add(" idkey char(20),");
   setupQRY->SQL->Add(" id1 char(20),");
   setupQRY->SQL->Add(" id2 char(20),");
   setupQRY->SQL->Add(" id3 char(20),");
   setupQRY->SQL->Add(" id4 char(20) )");

   setupQRY->ExecSQL();
   setupQRY->Close();

   setupQRY->SQL->Clear();
   setupQRY->SQL->Add(s_);
  }
}
//========================================================
void Tfunc::seekSTR(TStringList *strTEXT,AnsiString subTEXT,int &strPOS)
{
 int num;
 for(int i=1;i<strTEXT->Count+1;i++)
    {
     if((num=AnsiPos(subTEXT,strTEXT->Strings[i-1]))>0)
       {
        strPOS=i-1;
        break;
       }
     else strPOS=-1;
    }
}
//========================================================
void Tfunc::replaceSTR(TStringList *&strTEXT,AnsiString orgTEXT,AnsiString newTEXT)
{
 bool masih=true;
 flgHTML<<rfIgnoreCase<<rfReplaceAll;
 int strPOS;
 while(masih)
 {
 seekSTR(strTEXT,orgTEXT,strPOS);
 if(strPOS>=0)
  {
   AnsiString ctmp=strTEXT->Strings[strPOS];
   AnsiString cmod=StringReplace(ctmp,orgTEXT,newTEXT,flgHTML);
   strTEXT->Strings[strPOS]=cmod;
  }
 else masih=false;

 }
}
//========================================================
int Tfunc::spin(int num)
{
 randomize();
 return(random(num));
}
//========================================================
AnsiString __fastcall Tfunc::cutLTR(AnsiString LTR,int l)
{
 return LTR.SubString(1,l);
}
//========================================================
AnsiString __fastcall Tfunc::fillLTR(AnsiString LTR,int l)
{
 if(LTR.Length()<l)
   {
    AnsiString addSTR=AnsiString::StringOfChar('0',15-LTR.Length());
    LTR+=addSTR;
   }
 return LTR;
}
//---------------------------------------------------------------------------
AnsiString __fastcall Tfunc::squeezeLTR(AnsiString LTR)
{
 LTR=UpperCase(LTR.Trim());
 AnsiString myLTR="";
 for(int i=1;i<=LTR.Length();i++)
   {
    if(LTR.SubString(i,1)!=" " &&
      (StrPos(myLTR.c_str(),LTR.SubString(i,1).c_str())==NULL) ) myLTR+=LTR.SubString(i,1);
   }
 return myLTR;
}
//==================================================================
void Tfunc::locateIT(TComboBox *fndCMB,TEdit *fndEDT,TQuery *dafQRY,TStringList *valFND,AnsiString fldNM)
{
 TLocateOptions juk;
 juk<<loCaseInsensitive<<loPartialKey;
 if(fndCMB->Text.Trim()!="")
 {
  dafQRY->First();
  //mencari berdasar fieldname dan posisi valFND atas whenFND
  //dafQRY->Locate(mnuFRM->valFND->Strings[cmbFND->ItemIndex],fndEDT->Text.Trim(),juk);
  if (valFND->Count>0)
    {
     dafQRY->Locate(valFND->Strings[fndCMB->ItemIndex],fndEDT->Text.Trim(),juk);
    }
  else dafQRY->Locate(fldNM,fndEDT->Text.Trim(),juk);
 }
}
//==============================================================================
void  Tfunc::savepath(AnsiString ttl)
{
TStringList *param_=new TStringList();
if(Trim(ttl)=="")ttl=Application->Title;
try
   {
    Session->GetAliasParams(ttl,param_);
   }
catch(...)
   {
    if(dataku->Execute())
     {
      hisDB->Connected=false;
      AnsiString cari=dataku->FileName.Trim();
      cari=cari.SubString(1,cari.LastDelimiter("\\")-1);
      //Session->NetFileDir=cari+"\\PDOXNET";
      hisDB->Params->Strings[0]=
      hisDB->Params->Strings[0].SubString(1,5)+cari;
      Session->AddAlias(ttl,"DBASE",hisDB->Params);
      hisDB->Connected=true;
      Session->SaveConfigFile();
      Session->GetAliasParams(ttl,param_);
      ShowMessage("Configuration changed,\n Application need to restarted");
      Application->Terminate();
     }
    else Application->Terminate();
   }
path_=Trim(param_->Strings[0]);
path_=path_.SubString(6,path_.Length()-5);
hisDB->Connected=false;
hisDB->Params=param_;

ForceDirectories(path_+"\\DATA");
ForceDirectories(path_+"\\PDOXNET");

hisDB->Params->Strings[0]=param_->Strings[0].SubString(1,5)+path_+"\\DATA";

//set network dir here
AnsiString netDir="";
netDir+=path_+"\\PDOXNET";
Session->NetFileDir=netDir;
Session->SaveConfigFile();

AnsiString s_="DB PARAMS\n";
for(int i=1;i<hisDB->Params->Count;i++)
{
 s_+="\n"+hisDB->Params->Strings[i-1];
}
ShowMessage(s_);


if(DirectoryExists(path_)) hisDB->Connected=true;
else del_alias(ttl);

}
//-------------------------------------------------------------------------------
void Tfunc::restorepath(AnsiString ttl)
{
 TStringList *paramlist=new TStringList();
 paramlist->Clear();
 paramlist->Add("PATH="+path_);
 Session->ModifyAlias(ttl,paramlist);
}
//------------------------------------------------------------------------
void  Tfunc::del_alias(AnsiString ttl)
{
 Session->DeleteAlias(ttl);
 hisDB->Connected=true;
 Session->SaveConfigFile();
 savepath(ttl);
}
//-------------------------------------------------------------------------
void  Tfunc::FileCopy(AnsiString f_dari,AnsiString f_ke)
{
  int s_handle,d_handle;
  int ilength;
  int baca_;
  char *s_buffer;   //so why it should be pointer to type char?? ..ya find out soon

  f_dari=f_dari.Trim();
  f_ke=f_ke.Trim();

  try
  {
  s_handle=FileOpen(f_dari ,fmOpenRead);
  ilength =FileSeek(s_handle,0,2);
  ilength=ilength<0?0:ilength;
  FileSeek(s_handle,0,0);

  s_buffer=new char[ilength+1];
  baca_=FileRead(s_handle,s_buffer,ilength);

  d_handle=FileCreate(f_ke);
  FileWrite(d_handle,s_buffer,ilength);
  }
  catch(...)
  {
   ShowMessage("Some error occured for file being proccess") ;
  }
  FileClose(d_handle);
  FileClose(s_handle);
}
//---------------------------------------------------------------------------

void __fastcall Tfunc::FormCreate(TObject *Sender)
{
 sortNM=new TStringList();sortNM->Clear();
 sortVAL=new TStringList();sortVAL->Clear();
}
//---------------------------------------------------------------------------

