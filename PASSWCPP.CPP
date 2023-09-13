//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "passwCPP.h"
//#include "ividp.h"
#include "funcCPP.h"
#include "mnucpp.h"
//#include "baliCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TpasswFRM *passwFRM;
//---------------------------------------------------------------------------
__fastcall TpasswFRM::TpasswFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TpasswFRM::oldpasswKeyPress(TObject *Sender, char &Key)
{
if((int)Key==13)
 {
  setup->First();
  if(!setup->Locate("id1",passwCMB->Text.Trim(),seek_)) //artinya record baru
  {
   //nanti langsung ambil fungsi di newwpassw;
   ShowMessage("User ID "+passwCMB->Text.Trim()+" not found ");
   newpassw->Text=oldpassw->Text;
   newpassw->SetFocus();
  }
  else
  {
   bool out_=false;
   while(!out_)
   {
    if(setup->FieldByName("id2")->AsString.Trim()==oldpassw->Text.Trim())
    {
     //ShowMessage("Found valid user ID and Password");
     newpassw->Text=oldpassw->Text;
     newpassw->SetFocus();
     out_=true;
     break;
    }
    else
    {
     out_=setup->Locate("id1",passwCMB->Text.Trim(),seek_);
    }
   }
   if(!out_)
   {
    ShowMessage("Not Valid User ID");
    oldpassw->Text="";
    oldpassw->SetFocus();
   }

  }

 }
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::BitBtn6Click(TObject *Sender)
{
oldpassw->Text="";
newpassw->Text="";
passwCMB->Text="";
users->Close();
setup->Close();
Close();
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::FormActivate(TObject *Sender)
{
seek_<<loCaseInsensitive;
users->Close() ;
setup->Close();
users->DatabaseName=mnuFRM->path_+"\\DATA"; //SETUPDIR";
setup->DatabaseName=mnuFRM->path_+"\\DATA"; //SETUPDIR";
setup->Open();
users->Open();
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::newpasswKeyPress(TObject *Sender, char &Key)
{
if((int)Key==13)
 {
  if(func->valPAS(1))
  {
  if(setup->Eof || passwCMB->Text.Trim()!=setup->FieldByName("id1")->AsString)
   {
    setup->Insert();
    setup->FieldByName("id2")->AsString=newpassw->Text.Trim();
    setup->FieldByName("id1")->AsString=passwCMB->Text.Trim();
    setup->Post();
   }
  else
   {
    setup->Edit();
    setup->FieldByName("id2")->AsString=newpassw->Text.Trim();
    setup->FieldByName("id1")->AsString=passwCMB->Text.Trim();
    setup->Post();
   }
  users->Open();
  if(users->Eof)
   {
    ShowMessage("not yet verified");
    //artinya record baru
    //oke kita copiin varMNU dan varPAS dari baliFRM
    for(int i=1;i<=mnuFRM->varMNU->Count;i++)
     {
      users->Insert();
      users->Edit();
      users->FieldByName("id1")->AsString=mnuFRM->varMNU->Strings[i-1];
      users->FieldByName("id2")->AsString="00000";
      users->Post();
     }
   }
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::setupAfterPost(TDataSet *DataSet)
{
if(setup->FieldByName("recid")->IsNull)
 {
  setup->Edit();
  setup->FieldByName("recid")->Value=setup->RecNo;
  setup->Post();
 }
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::setupNewRecord(TDataSet *DataSet)
{
 setup->Edit();
 setup->FieldByName("idkey")->AsString="PASSWORD";
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::usersNewRecord(TDataSet *DataSet)
{
 users->Edit();
 users->FieldByName("recid")->Value=setup->FieldByName("recid")->Value;
 users->FieldByName("idkey")->AsString="USERS";
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::usersCalcFields(TDataSet *DataSet)
{
 //if (users->State!=dsEdit) users->Edit();
 int may_=mnuFRM->varMNU->IndexOf(users->FieldByName("id1")->AsString.Trim());
 if(may_>=0) users->FieldByName("mnuRMRK")->AsString=mnuFRM->varDES->Strings[may_];
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::passwCMBEnter(TObject *Sender)
{
users->Close();
setup->Close();
setup->Open();
passwCMB->Clear();
while(!setup->Eof)
{
 passwCMB->Items->Add(setup->FieldByName("id1")->AsString);
 setup->Next();
}

}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::cekVEREnter(TObject *Sender)
{
oldVER=users->FieldByName("id2")->AsString.Trim();
if(!func->valPAS(2)) passwCMB->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::usersBeforeScroll(TDataSet *DataSet)
{
 AnsiString newVER;
 if(oldVER!=newVER)
 {
  for(int i=1;i<=cekVER->Items->Count;i++)
   {
    newVER+=cekVER->Checked[i-1]==true?"1":"0";
   }
  if(oldVER!=newVER)
   {
    users->Edit();
    users->FieldByName("id2")->AsString=newVER;
    users->Post();
    oldVER=newVER;
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::usersAfterScroll(TDataSet *DataSet)
{
 for(int i=1;i<=cekVER->Items->Count;i++)
 {
  cekVER->Checked[i-1]=users->FieldByName("id2")->AsString.Trim().SubString(i,1)=="1"?true:false;
 }
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::BitBtn4Click(TObject *Sender)
{
//cek item menu yang belum masuk
if(!func->valPAS(1) && !func->valPAS(2) ) return ;
AnsiString s_=AnsiString::StringOfChar('0',cekVER->Items->Count);
for(int i=1 ;i<mnuFRM->varPAS->Count;i++)
 {
  AnsiString mnuNM=mnuFRM->varMNU->Strings[i-1];
  users->First();
  seek_<<loCaseInsensitive;
  if(mnuFRM->varPAS->Strings[i-1]==s_ && !users->Locate("id1",mnuNM,seek_))
  {
   users->Insert();
   users->Edit();
   users->FieldByName("id1")->AsString=mnuNM;
   users->Post();
  }
 }
}
//---------------------------------------------------------------------------



void __fastcall TpasswFRM::BitBtn3Click(TObject *Sender)
{
if(!func->valPAS(3)) return;
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::BitBtn5Click(TObject *Sender)
{
bool boleh_=func->valPAS(2);
bool boleh1_=func->valPAS(3);
if(boleh_ && boleh1_)users->Delete();
else return;
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::BitBtn2Click(TObject *Sender)
{
usersBeforeScroll(users);
}
//---------------------------------------------------------------------------

void __fastcall TpasswFRM::BitBtn1Click(TObject *Sender)
{
if(func->valPAS(2)) newpasswKeyPress(Sender, (char)13);

}
//---------------------------------------------------------------------------

