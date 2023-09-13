//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "validCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TvalFRM *valFRM;
//---------------------------------------------------------------------------
__fastcall TvalFRM::TvalFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void TvalFRM::key13(TObject *Sender,char &Key)
{
 if(int(Key)==13)
  {
   if(byEDT->Focused()){byttlEDT->SetFocus();return;}
   if(byttlEDT->Focused()){ackEDT->SetFocus();return;}
   if(ackEDT->Focused()){ackttlEDT->SetFocus();return;}
   if(ackttlEDT->Focused()){appEDT->SetFocus();return;}
   if(appEDT->Focused()){appttlEDT->SetFocus();return;}
   if(appttlEDT->Focused()){Button1->SetFocus();return;}
   Key=0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TvalFRM::FormKeyPress(TObject *Sender, char &Key)
{
 key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::byEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::byttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::ackEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::ackttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::appEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

void __fastcall TvalFRM::appttlEDTKeyPress(TObject *Sender, char &Key)
{
key13(Sender,Key);    
}
//---------------------------------------------------------------------------

