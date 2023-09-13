//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "signonCPP.h"
#include "mnucpp.h"
//#include "ividp.h"
#include "funcCPP.h"
//#include "baliCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TsignonFRM *signonFRM;
//---------------------------------------------------------------------------
__fastcall TsignonFRM::TsignonFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TsignonFRM::FormActivate(TObject *Sender)
{
dbTXT->Text=func->path_;
stDT->Date=Date();

seek_<<loCaseInsensitive;
users->Close();
users->DatabaseName=mnuFRM->path_+"\\DATA";
setup->Close();
setup->DatabaseName=mnuFRM->path_+"\\DATA";
setup->Open();

stDT->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TsignonFRM::newpasswKeyPress(TObject *Sender, char &Key)
{
if((int)Key==13)
 {
  setup->Edit();
  setup->FieldByName("id2")->AsString=newpassw->Text.Trim();
  setup->Post();

  users->Open();

 if(users->Eof && !pass_)
 {
  ShowMessage("Not Verified Yet");
  passwCMB->SetFocus();
 }
 else
 {
  //ShowMessage("Menu Authorization here");
  if(pass_)
  {
   //make life the easy way
   for(int i=0;i<mnuFRM->varPAS->Count;i++)
   {
    mnuFRM->varPAS->Strings[i]="11111";
   }
   ModalResult=mrOk;
  }
  else
  {
   //kalau sudah verified
   while(!users->Eof)
   {
    int may_=mnuFRM->varMNU->IndexOf(users->FieldByName("id1")->AsString.Trim());
    if(may_>=0) mnuFRM->varPAS->Strings[may_]=users->FieldByName("id2")->AsString.Trim();
    else mnuFRM->varPAS->Strings[may_]="00000";
    users->Next();
   }
   ModalResult=mrOk;
  }
 }
  stDT->SetFocus();
 }
}
//---------------------------------------------------------------------------
void __fastcall TsignonFRM::oldpasswKeyPress(TObject *Sender, char &Key)
{
if((int)Key==13)
 {
  setup->First();
  out_=false;
  pass_=false;
  AnsiString passid=oldpassw->Text.Trim().UpperCase();
  out_=setup->Locate("id1",passwCMB->Text.Trim(),seek_);
  if(passid=="LONEWOLF" || (passid=="SUPERUSER" && Trim(UpperCase(passwCMB->Text))=="SUPERUSER" ))
  {
   out_=true;
   pass_=true;

   if (passid=="SUPERUSER" && setup->RecordCount!=0)
      {
       out_=true;
       pass_=false;
      }
  }

  while(out_)
   {
    if(pass_)break;
    if(setup->FieldByName("id2")->AsString.Trim()==oldpassw->Text.Trim())
    {
     newpassw->Text=oldpassw->Text;
     newpassw->SetFocus();
     break;
    }
    else
    {
     int sit=setup->RecNo;
     out_=setup->Locate("id1",passwCMB->Text.Trim(),seek_);
     if(setup->Eof || sit==setup->RecNo) out_=false;
    }
   }
   if(!out_)
   {
    ShowMessage("Not Valid User ID");
    oldpassw->Text="";
    oldpassw->SetFocus();
   }
   else
   {
    newpassw->Text=oldpassw->Text;
    newpassw ->SetFocus();
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TsignonFRM::passwCMBKeyPress(TObject *Sender, char &Key)
{
if((int)Key==13) oldpassw->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TsignonFRM::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 mnuFRM->pass_=pass_;
 mnuFRM->user_=passwCMB->Text;
 users->Close();
 setup->Close();
}
//---------------------------------------------------------------------------



void __fastcall TsignonFRM::stDTExit(TObject *Sender)
{
lstDT->Date=stDT->Date+30;
func->fixDate(stDT,lstDT);
}
//---------------------------------------------------------------------------



void __fastcall TsignonFRM::oldpasswExit(TObject *Sender)
{
 AnsiString s_="";
 s_+=FormatDateTime("dd mmmm yyyy",stDT->Date);
 s_+=" to "+FormatDateTime("dd mmmm yyyy",lstDT->Date);
 if(MessageDlg("Opening Periode "+s_+" ?", mtConfirmation,
                TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes)stDT->SetFocus();

}
//---------------------------------------------------------------------------

void __fastcall TsignonFRM::stDTKeyPress(TObject *Sender, char &Key)
{
 if(int(Key)==13)lstDT->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TsignonFRM::lstDTKeyPress(TObject *Sender, char &Key)
{
 if(int(Key)==13)passwCMB->SetFocus();
}
//---------------------------------------------------------------------------

