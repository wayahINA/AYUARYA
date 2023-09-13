//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "pickcpp.h"
#include "funcCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpickform *pickform;
//---------------------------------------------------------------------------
__fastcall Tpickform::Tpickform(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
void Tpickform::seekGO()
  {
   AnsiString addkey="";

   if (Trim(findEDT->Text)=="*" || Trim(findEDT->Text).Length()<=0)
   {
    Query1->Close();
    Query1->SQL->Clear();
    Query1->SQL->Add(sqlasli);
   }
   else
   {
    //disini kita pisahin kalu dia ketiknya lebih dari satu kata GT
    TStringList *seek_=new TStringList();
    if(findEDT->Text!="")
    {
     //should be abble to split char entried
     AnsiString myFLD=DBGrid1->Columns->Items[DBGrid1->SelectedIndex]->FieldName;
     Query1->Close();
     func->listIT(seek_,findEDT->Text);
     func->seekIT(Query1,seek_,sqlasli,myFLD);
    }
    //ShowMessage(Query1->SQL->Text);
    //oke finding it by query
    if (Query1->RecordCount<=0 && Query1->Eof)
    {
     //ShowMessage("Sorry Can't Find Searched Entry ");
     //findEDT->Text="";
     Query1->Close();
     Query1->SQL->Clear();
     Query1->SQL->Add(sqlasli);
    }
   }
   DBGrid1->DataSource->DataSet->Open();
   DBGrid1->DataSource->DataSet->Active=true;
   if(findEDT->Text=="")DBGrid1->SetFocus();
   else findEDT->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall Tpickform::Button3Click(TObject *Sender)
{
Label1->Caption=pickform->DBGrid1->Columns->Items[0]->Title->Caption+" =>";
findEDT->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall Tpickform::findEDTKeyPress(TObject *Sender, char &Key)
{
if (Key==13) seekGO();
}
//---------------------------------------------------------------------------


void __fastcall Tpickform::Button2Click(TObject *Sender)
{
ModalResult=mrNo;
}
//---------------------------------------------------------------------------
void Tpickform::siapkan(AnsiString dbname,AnsiString tbname)
{
 Query1->Close();
 Query1->DatabaseName=dbname;
 Query1->SQL->Clear();
 Query1->SQL->Add("SELECT * FROM "+QuotedStr(tbname)+" book");
 sqlasli=Query1->SQL->Text;
 //Activate();
}
void __fastcall Tpickform::FormActivate(TObject *Sender)
{
 sqlasli=Query1->SQL->Text;
 DBGrid1->DataSource->DataSet->Open();
 if(Trim(findEDT->Text)!="")seekGO();
}
//---------------------------------------------------------------------------


void __fastcall Tpickform::Button1Click(TObject *Sender)
{
ModalResult=mrOk;

}
//---------------------------------------------------------------------------


void __fastcall Tpickform::FormClose(TObject *Sender, TCloseAction &Action)
{
 if(ModalResult==mrOk)findEDT->Text="";
}
//---------------------------------------------------------------------------

