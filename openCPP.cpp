//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "openCPP.h"
#include "mnuCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TopenFRM *openFRM;
//---------------------------------------------------------------------------
__fastcall TopenFRM::TopenFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TopenFRM::BitBtn1Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TopenFRM::BitBtn2Click(TObject *Sender)
{
 mnuFRM->stDT=stDT->Date;
 mnuFRM->lstDT=lstDT->Date;    
}
//---------------------------------------------------------------------------
void __fastcall TopenFRM::stDTKeyPress(TObject *Sender, char &Key)
{
if(int(Key)==13)lstDT->SetFocus();    
}
//---------------------------------------------------------------------------

void __fastcall TopenFRM::stDTExit(TObject *Sender)
{
 lstDT->Date=stDT->Date+30;      
}
//---------------------------------------------------------------------------

void __fastcall TopenFRM::FormActivate(TObject *Sender)
{
 stDT->Date=mnuFRM->stDT;
 lstDT->Date=mnuFRM->lstDT;
}
//---------------------------------------------------------------------------

