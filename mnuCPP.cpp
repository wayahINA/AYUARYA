//---------------------------------------------------------------------------
#include <vcl.h>
#include <FileCtrl.hpp>
#include <shlobj.hpp>
#pragma hdrstop

#include "mnuCPP.h"
#include "SIGNONCPP.h"
#include "funcCPP.h"
#include "reqCPP.h"
#include "vdCPP.h"
#include "poCPP.h"
#include "PASSWCPP.h"
#include "openCPP.h"
#include "seekCPP.h"
#include "BARCPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TmnuFRM *mnuFRM;
//---------------------------------------------------------------------------
__fastcall TmnuFRM::TmnuFRM(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------
bool TmnuFRM::packDBF(AnsiString tbNM)
{
 bool active = dummyTB->Active;
 bool exclusive = dummyTB->Exclusive;
 bool retval = true;

 dummyTB->Close();
 dummyTB->DatabaseName=path_+"\\DATA";
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
//---------------------------------------------------------
void TmnuFRM::tbCUT2()
{
 /*
 cut data less than date entered
 copy data to \YYYY\DATA\
 */
  AnsiString s_="";
  AnsiString myDT=DateToStr(mnuFRM->stDT);
  AnsiString errMSG="";
  AnsiString to_;

  //tanya input date
  if (!InputQuery("Batasan Data",
       "Masukkan tanggal akhir yang dipotong", myDT))return;

  if(MessageDlg("Yakin akan memotong data sampai "+myDT+" ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
   //backup ke YYYY\\DATA\\
   loged=true;
   TbarFRM *bar=new TbarFRM(this);
   bar->gmb->Visible=true;
   bar->gmb->Active=true;
   bar->txt->Caption="Transfer cut-off data";
   bar->Show();


   to_=FormatDateTime("yyyy",StrToDate(myDT));
   to_+="\\DATA";
   bool goes_=true;
   //pastikan lagi kalu udah ada
   if(DirectoryExists(path_+"\\"+to_))
   {
    s_="data cut-off "+myDT+" sudah ada ";
    s_+="\nbenar mau di cut-off lagi?";
    if(MessageDlg(s_, mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)goes_=false;
   }

   if(!goes_)return;

   //backME();
   try
   {
    func->DirCopy(path_+"\\DATA",path_+"\\"+to_);
    /*
    selanjutnya cuma perlu potong
    1.req ; needed
    2.dtlreq ; refno=req.trfno+req.reqno
    terus dipack
    */
      //pake filter table mas
       s_=" select trfno+reqno as nomor from 'req.dbe' ";
       s_+=" where needed<=:DAT and trfdt<=:DAT ";
       dummy->Close();
       dummy->SQL->Clear();
       dummy->SQL->Add(s_);
       dummy->ParamByName("DAT")->AsDateTime=
       StrToDate(myDT);

       try
       {
        bar->txt->Caption="Cutting-off transaction detail";
        bar->Update();

        dummy->Open();
        dummyTB->Close();
        dummyTB->TableName="dtlreq.dbe";
        dummyTB->Open();
        dummyTB->Filtered=true;
        while(!dummy->Eof)
        {
         dummyTB->First();
         dummyTB->Filter="refno='"+Trim(dummy->FieldByName("nomor")->AsString)+"'";
         while(!dummyTB->Eof)
         {
          dummyTB->Delete();
          dummyTB->Next();
         }
         dummy->Next();
        }
        dummy->Close();
        dummyTB->Filtered=false;
        dummyTB->Filter="";
        dummyTB->Close();

        bar->txt->Caption="Compacting transaction detail";
        bar->Update();
        packDBF("dtlreq.dbe");
       }
       catch(...)
       {
        errMSG+="\nerror cutting-off transaction detail,proccess aborted";
       }

       if(errMSG!="")ShowMessage(errMSG);
       /*
       s_=" delete from 'req.dbe' ";
       s_+=" where needed<=:DAT  ";
       dummy->Close();
       dummy->SQL->Clear();
       dummy->SQL->Add(s_);
       dummy->ParamByName("DAT")->AsDateTime=
       StrToDate(myDT);
       */
       dummyTB->Close();
       dummyTB->Filtered=false;
       dummyTB->Filter="";


       AnsiString strDT="";
       strDT=FormatDateTime("dd/mm/yyyy",myDT);

       dummyTB->TableName="req.dbe";
       dummyTB->Filtered=true;
       dummyTB->Filter="needed<='"+strDT+"' or trfdt<='"+strDT+"' ";
       try
       {
        bar->txt->Caption="Cutting-off transaction header ";
        bar->Update();
        /*
        dummy->Prepare();
        dummy->ExecSQL();
        dummy->Close();
        */
        dummyTB->Open();
        while(!dummyTB->Eof)
        {
         dummyTB->Delete();
         dummyTB->Next();
        }

        dummyTB->Close();
        dummyTB->Filtered=false;
        dummyTB->Filter="";

        bar->txt->Caption="Compacting transaction header";
        bar->Update();
        packDBF("req.dbe");
       }
       catch(...)
       {
        errMSG+="\nerror cutting-off transaction header,proccess aborted";
       }

       if(errMSG!="")ShowMessage(errMSG);

    dummy->Close();
   }
   catch(...)
   {
    ShowMessage("Error transfer data, proccess aborted");
   }

   bar->Hide();
   delete bar;
  }
}
//---------------------------------------------------------
void TmnuFRM::chgREQ()
{
  AnsiString s_="";
  s_+=" alter table 'req.dbe' add reqtmp char(20) ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();

  s_="";
  s_+=" update 'req.dbe' set reqtmp=reqno";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();

  s_="";
  s_+=" alter table 'req.dbe' drop reqno ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();

  s_="";
  s_+=" alter table 'req.dbe' add reqno char(20) ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();

  s_="";
  s_+=" update 'req.dbe' set reqno=reqtmp ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();

  s_="";
  s_+=" alter table 'req.dbe' drop reqtmp ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->ExecSQL();
  dummy->Close();



}
//---------------------------------------------------------
void TmnuFRM::chgQRY()
{
  bool chg_;
  AnsiString s_="";
  //ubah field reqno,bila size<20
  s_=" select reqno from 'req.dbe' ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  dummy->Open();
  /*
  if(!dummy->Eof)
    if(dummy->Fields[0]->Size<20)chg_=true;

  dummy->Close();
  if(chg_)chgREQ();
  */

  s_="";
  s_+=" alter table 'vd.dbe' add working char(20) ";
  dummy->Close();
  dummy->SQL->Clear();
  dummy->SQL->Add(s_);
  try
  {
   dummy->ExecSQL();
  }
  catch(...)
  {
   int x;
  }
  dummy->Close();



}

//---------------------------------------------------------
void TmnuFRM::goCHG(TQuery *tempQRY,TSearchRec hasil_,AnsiString SQLDir)
{
 AnsiString asal=hasil_.Name;
 asal=UpperCase(asal);
 tempQRY->Close();
 tempQRY->SQL->Clear();

 tempQRY->SQL->LoadFromFile(SQLDir+"\\"+asal);
 tempQRY->Prepare();
 tempQRY->ExecSQL();
 tempQRY->Close();
 //-----------------------------------------------------------------
 //PROSES PERUBAHAN KARENA AKAN MENGUBAH STRUKTUR
 //sebenarnya harus dicatat field-field yang berbeda
 //oke,bisa pake dummy sama tempQRY
 if(FileExists(tempQRY->DatabaseName+"\\employee.bac"))
  {
   dummy->Close();
   dummy->DatabaseName=tempQRY->DatabaseName;
   dummy->SQL->Clear();
   dummy->SQL->Add("select * from 'employee.bac' ");
   dummy->Open();
   tempQRY->Close();
   tempQRY->SQL->Clear();
   tempQRY->SQL->Add("select * from 'employee.dbf' ");
   tempQRY->Open();
   TStringList *fldSTR=new TStringList();
   fldSTR->Clear();
   for(int i=0;i<dummy->FieldCount;i++)
   {
    /*
    TField *myFLD=tempQRY->FindField(dummy->Fields[i]->FieldName);
    if ( (myFLD->FieldName==dummy->Fields[i]->FieldName) &&
    myFLD->DataType==dummy->Fields[i]->DataType)
    fldSTR->Add(dummy->Fields[i]->FieldName);
    */
    //gua coba cari manual aja yah
    AnsiString cari=dummy->Fields[i]->FieldName;
    bool ada;
    for(int xx=0;xx<tempQRY->FieldCount;xx++)
    {
     if (tempQRY->Fields[xx]->FieldName==cari &&
         tempQRY->Fields[xx]->DataType==dummy->Fields[i]->DataType)
     {
      ada=true;
      fldSTR->Add(dummy->Fields[i]->FieldName);
      break;
     }
    }
   }
   dummy->Close();
   tempQRY->Close() ;
   AnsiString txtQRY=" ";
   for(int i=1;i<=fldSTR->Count;i++)
   {
    txtQRY+=" " ;
    txtQRY+=fldSTR->Strings[i-1];
    txtQRY+=i>=fldSTR->Count?" ":",";
   }
   tempQRY->SQL->Clear();
   tempQRY->SQL->Add("insert into 'employee.dbf' ");
   tempQRY->SQL->Add("("+txtQRY+")");
   tempQRY->SQL->Add(" select "+txtQRY+" from 'employee.bac' ");
   tempQRY->ExecSQL();

   dummy->Close();
   dummy->SQL->Clear();
   tempQRY->Close();
   tempQRY->SQL->Clear();
  }
}
//---------------------------------------------------------
void TmnuFRM::maintain()
{
  TQuery *tempQRY;
  tempQRY=Query1;
  tempQRY->Close();
  tempQRY->DatabaseName="hisDB";
  tempQRY->RequestLive=true;
  //AnsiString SQLDir=GetCurrentDir()+"\\SQLDIR";

  AnsiString SQLDir=Application->ExeName;
  SQLDir=SQLDir.SubString(0,SQLDir.LastDelimiter("\\"));
  SQLDir+="SQLDIR";

  if(DirectoryExists(SQLDir))
   {
    //oke deh mo ngerjakan apa nih mas tom?
    TSearchRec hasil_;
    //AnsiString filenya="\\DB"+FormatDateTime("mmyyyy",mnufrm->DateTimePicker1->Date);
    if (FindFirst(SQLDir+"\\CHG*.SQL", faAnyFile, hasil_) == 0)
      {
       goCHG(tempQRY,hasil_,SQLDir);
       while (FindNext(hasil_) == 0)
       {
        goCHG(tempQRY,hasil_,SQLDir);
       }
      }
     FindClose(hasil_);
     tempQRY->Close();
     dummy->Close();
  }
  tempQRY->Close();
}
//---------------------------------------------------------
AnsiString TmnuFRM::seekTB(TDataSet *mySQL,AnsiString fldNM,AnsiString seekSTR,AnsiString retNM)
{
 AnsiString retu_="";
 TLocateOptions Opts;
 Opts.Clear();
 Opts <<loCaseInsensitive <<loPartialKey;
 mySQL->First();
 if(mySQL->Locate(fldNM,seekSTR,Opts))
  retu_=mySQL->FieldByName(retNM)->AsString;
 return(retu_);
}
//---------------------------------------------------------
void TmnuFRM::closeTB()
{
 divSQL->Close();
 deptSQL->Close();
 reqSQL->Close();
 dtlSQL->Close();
}
//---------------------------------------------------------
void TmnuFRM::openTB()
{
 AnsiString dbPath=path_+"\\DATA";
 divSQL->DatabaseName=dbPath;
 deptSQL->DatabaseName=dbPath;
 reqSQL->DatabaseName=dbPath;
 dtlSQL->DatabaseName=dbPath;
}
//---------------------------------------------------------
void TmnuFRM::uploadPR()
{
 TbarFRM *FRM=new TbarFRM(Application);
 if(dataku->Execute())
  {
   AnsiString seekNM=dataku->FileName.Trim();
   AnsiString dirNM=seekNM.SubString(1,seekNM.LastDelimiter("\\")-1);
   AnsiString fileNM=seekNM.SubString(dirNM.Length()+2,seekNM.Length()-dirNM.Length());
   /*ShowMessage("File Name ="+seekNM+"\n"+
               "Dir Name  ="+dirNM+"\n"+
               "Table Name="+fileNM);*/
   if(MessageDlg("Upload from file "+fileNM+"?", mtConfirmation, 
              TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes) return;

   dummy->Close();
   dummy->DatabaseName=dirNM;
   dummy->SQL->Clear();
   dummy->SQL->Add("select tanggal as reqdt,");
   dummy->SQL->Add("prno as reqno,");
   dummy->SQL->Add("dept as reqdept,");
   dummy->SQL->Add("division as reqdiv,");
   dummy->SQL->Add("descriptio as gdnm,");
   dummy->SQL->Add("unit as uom,");
   dummy->SQL->Add("qty as req,");
   dummy->SQL->Add("cur,");
   dummy->SQL->Add("price,");
   dummy->SQL->Add("total");
   dummy->SQL->Add("from "+Trim(fileNM)+" ");
   dummy->Open();
   ShowMessage("will be proccessing "+IntToStr(dummy->RecordCount)+" records");
   AnsiString reqno;
   if(!dummy->Eof)
   {
    closeTB();
    openTB();
    deptSQL->Open();
    divSQL->Open();
    reqSQL->Open();
    dtlSQL->Open();


    FRM->gmb->Visible=true;
    FRM->gmb->Active=true;

    FRM->bar->Max=12;
    FRM->bar->Step=1;
    FRM->bar->Left=0;

    FRM->txt->Caption="Wait a moment,uploading PR database...";
    FRM->Show();

   }
   AnsiString reqDEPT,reqDIV;
   while(!dummy->Eof)
   {
    FRM->txt->Caption="Uploading PR no "+Trim(dummy->FieldByName("reqno")->AsString);
    FRM->bar->Position+=1;
    FRM->Update();
    //find dept
    reqDEPT=
    seekTB(deptSQL,"name",dummy->FieldByName("reqdept")->AsString,"recid");
    if(Trim(reqDEPT)=="")
    {
     deptSQL->Append();
     deptSQL->FieldByName("idkey")->AsString="DEPT";
     deptSQL->FieldByName("name")->AsString=
     dummy->FieldByName("reqDEPT")->AsString;
     deptSQL->Post();
     reqDEPT=deptSQL->FieldByName("recid")->AsString;
    }
    //find div
    reqDIV=
    seekTB(divSQL,"name",dummy->FieldByName("reqdiv")->AsString,"recid");
    if(Trim(reqDIV)=="")
    {
     divSQL->Append();
     divSQL->FieldByName("idkey")->AsString="DIVISI";
     divSQL->FieldByName("name")->AsString=
     dummy->FieldByName("reqDIV")->AsString;
     divSQL->Post();
     reqDIV=divSQL->FieldByName("recid")->AsString;
    }
    //fill req
    if(reqno!=Trim(dummy->FieldByName("reqno")->AsString))
    {
     reqno=Trim(dummy->FieldByName("reqno")->AsString);
     reqSQL->Append();
     reqSQL->FieldByName("reqDT")->AsString=
     dummy->FieldByName("reqDT")->AsString;
     reqSQL->FieldByName("reqno")->AsString=reqno;
     reqSQL->FieldByName("reqdept")->AsString=
     reqDEPT;
     reqSQL->FieldByName("reqdiv")->AsString=
     reqDIV;
     reqSQL->FieldByName("mode")->AsString=0;
    }
    //fill dtl
    dtlSQL->Append();
    dtlSQL->FieldByName("reqno")->AsString=reqno;
    dtlSQL->FieldByName("gdnm")->AsString=
    dummy->FieldByName("gdnm")->AsString;
    dtlSQL->FieldByName("uom")->AsString=
    dummy->FieldByName("uom")->AsString;
    dtlSQL->FieldByName("req")->AsString=
    dummy->FieldByName("req")->AsString;
    dtlSQL->FieldByName("cur")->AsString=
    dummy->FieldByName("cur")->AsString;
    dtlSQL->FieldByName("price")->AsString=
    dummy->FieldByName("price")->AsString;
    /*dtlSQL->FieldByName("total")->AsString=
    dummy->FieldByName("total")->AsString;*/
    dummy->Next();
   }
   delete FRM;
   closeTB();
   dummy->Close();
  }
}
//---------------------------------------------------------
void TmnuFRM::other()
{
 dummy->Close();
 dummy->SQL->Clear();

 AnsiString mFile="";
 AnsiString uFile="";

 mFile=path_+"\\DATA\\REQ.DBE";
 uFile=path_+"\\DATA\\REQ.DB";


 //tgl 25 mei 2009, cek untuk field reqno columnSize=20

 AnsiString s_="";
 s_=" select distinct reqno from 'req.dbe' ";
 dummy->SQL->Add(s_);
 dummy->Open();
 if(dummy->FieldByName("reqno")->Size!=20)
 {
  dummy->Close();
  if(!RenameFile(mFile,uFile))ShowMessage("Tidak Bisa Mengupdate Data Requisition");
  else
  {

   tbCREATE();
   //append from .bac
   s_="";
   s_+=" insert into 'req.dbe' select * from 'req.db' ";
   dummy->Close();
   dummy->SQL->Clear();
   dummy->SQL->Add(s_);
   dummy->ExecSQL();
   dummy->Close();
  }
 }

 dummy->Close();
}
//---------------------------------------------------------
void TmnuFRM::login()
{
 if(!loged)
  {

   Application->CreateForm(__classid(TsignonFRM), &signonFRM);
   TModalResult hw_=signonFRM->ShowModal();
   if(hw_==mrCancel || hw_==mrNo)
    {
     ShowMessage("Terimakasih sudah mencoba dan menggunakan produk kami");
     Application->Terminate();
     Close();
    }
   user_=signonFRM->passwCMB->Text;
   stDT=signonFRM->stDT->Date;
   lstDT=signonFRM->lstDT->Date;
   loged=true;
  }
}
//---------------------------------------------------------
void TmnuFRM::menuCREATE()
{
 varMNU=new TStringList();
 varDES=new TStringList();
 varPAS=new TStringList();

 //daftar mnunya disini mas
 varMNU->Clear();
 varDES->Clear();
 varPAS->Clear();

 //isi varmenu utama
 TStringList *mnuVAR=new TStringList();
 mnuVAR->Clear();
 for(int i=1;i<=mnuGRP->Items->Count;i++)
 {
  varMNU->Add(func->squeezeLTR(mnuGRP->Items->Strings[i-1]));
  varDES->Add(mnuGRP->Items->Strings[i-1]);
 }

 //isi validasi default
 for(int i=0 ;i<=varMNU->Count;i++)
 {
  varPAS->Add("00000");
 }

}
//---------------------------------------------------------
void TmnuFRM::tbCREATE()
{
 AnsiString tbFILE;
 for(int i=1;i<=tbSTRUCT->Count;i++)
  {
   tbFILE=path_+"\\DATA\\"+Trim(tbSTRUCT->Strings[i-1])+".DBE";
   if(!FileExists(tbFILE))
    {
     ForceDirectories(path_+"\\DATA");
     dummy->Close();
     dummy->DatabaseName=path_+"\\DATA";
     dummy->SQL->Clear();
     dummy->SQL->Add(" create table '"+Trim(tbSTRUCT->Strings[i-1])+".DBE' ");
     dummy->SQL->Add(" ("+sqlSTRUCT->Strings[i-1]+") ");
     //ShowMessage(dummy->SQL->Text);
     dummy->Prepare();
     dummy->ExecSQL();
     dummy->Close();
     dummy->SQL->Clear();
    }
  }
}
//---------------------------------------------------------
void TmnuFRM::createSTRUCT()
{
 //tables creation
 sqlSTRUCT=new TStringList();
 sqlSTRUCT->Clear();
 tbSTRUCT=new TStringList();tbSTRUCT->Clear();
 //structure list
 TStrings *s_=new TStringList();s_->Clear();
 tbSTRUCT->Add("CATITEM");              //category item
 s_->Add(" catrec numeric(10,0),");     //record id
 s_->Add(" catid char(30),");           //id/code category
 s_->Add(" catcld boolean,");           //has sub?
 s_->Add(" catparent char(60),");       //parent id
 s_->Add(" catchild char(30),");        //this id
 s_->Add(" catcomb char(100)");         //to be parent later
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("ITEM");         		//item
 s_->Add(" gdrec numeric(10,0),");		//record id
 s_->Add(" id char(15),");  			//item id
 s_->Add(" nm char(45),");  			//description
 s_->Add(" type char(5),"); 			//type of item [exclusive|inventory|sale|purchase|proccess]
 s_->Add(" ctgry numeric(10,0),");		//good category
 s_->Add(" uom char(10),"); 			//unit of measurement - able for convertion
 s_->Add(" cur numeric(10,0),");  		//currency type if any
 s_->Add(" buy numeric(10,2),");		//buying price
 s_->Add(" sell numeric(10,2)");		//selling price
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("DTLREQ");       		//detail requisition
 s_->Add(" refno char(30),");   		//transaction reference no
 s_->Add(" reqno char(30),");           //for specific request
 s_->Add(" batch char(15),");   		//part ser/batch
 s_->Add(" id char(15),");              //good code
 s_->Add(" gd numeric(10,0),");   		//good's id
 s_->Add(" gdnm char(60),");            //for unlisted goods
 s_->Add(" uom char(10),"); 			//unit of measurement
 s_->Add(" uomrec numeric(10,2),");     //uomrec
 s_->Add(" req numeric(10,2),");		//qty request
 s_->Add(" rel numeric(10,2),");		//qty release
 s_->Add(" bal numeric(10,2),");		//qty balance
 s_->Add(" state boolean,");            //status release or not
 s_->Add(" cur char(10),");             //currency
 s_->Add(" price numeric(15,2),");		//price
 s_->Add(" refdt date");                //inv date
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("REQ");          		//requisition
 s_->Add(" reqno char(20),");			//no
 s_->Add(" reqdt date,");   			//date request
 s_->Add(" needed date,");              //date need
 s_->Add(" reqdept numeric(10,0),"); 	//by departement
 s_->Add(" reqdiv numeric(10,0),");  	//by division
 s_->Add(" trfno char(15),");   		//good's trf
 s_->Add(" trfdt date,");   			//date trf
 s_->Add(" vd numeric(10,0),");         //vendor
 s_->Add(" vdref char(15),");           //invoice reference
 s_->Add(" wono char(15),");			//w/o no
 s_->Add(" job char(15),"); 			//for job id
 s_->Add(" descr char(100),");			//remark
 s_->Add(" refno char(15),");			//transaction reference no
 s_->Add(" reqby char(15),");			//request by
 s_->Add(" reqttl char(15),");  		//title
 s_->Add(" aprby char(15),");			//approvement
 s_->Add(" aprttl char(15),");  		//title
 s_->Add(" rcvby char(15),");			//receiv by
 s_->Add(" rcvttl char(15),");  		//title
 s_->Add(" relby char(15),");			//release by
 s_->Add(" relttl char(15),");  		//title
 s_->Add(" trfrom numeric(10,0),");		//location transfer from
 s_->Add(" trto numeric(10,0),"); 		//location transfer to
 s_->Add(" mode numeric(10,0),");  		//transaction mode
 s_->Add(" note char(255),");           //any note/remark
 s_->Add(" othnote char(255),");        //
 s_->Add(" state char(1),");            //status plan/proccess/partial release/full release
 s_->Add(" org char(15),");             //who makes me
 s_->Add(" mod char(15),");             //who mods me
 s_->Add(" moddt date,");               //when mod made
 s_->Add(" whfrom numeric(10,0),");     //type warehouse
 s_->Add(" whto numeric(10,0)");        //type warehouse
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("TRMODE");       		//transaction mode
 s_->Add(" modrec numeric(10,0),");		//record id
 s_->Add(" descr char(25),");			//description
 s_->Add(" result numeric(2,0)");  		//effect to stock [+|-]
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("VD");       			//vendor
 s_->Add(" vdrec numeric(10,0),");		//record id
 s_->Add(" id char(10),");  			//id
 s_->Add(" nm char(45),");  			//name
 s_->Add(" add1 char(30),");			//address
 s_->Add(" add2 char(30),");			//...
 s_->Add(" cty numeric(10,0),"); 		//city
 s_->Add(" prov numeric(10,0),");		//province
 s_->Add(" cntry numeric(20,0),");		//country
 s_->Add(" mail char(30),");			//e-mail
 s_->Add(" web char(30),"); 			//web site
 s_->Add(" ph char(30),");  			//phone
 s_->Add(" fx char(30),");  			//fax
 s_->Add(" prs char(30),"); 			//contact person
 s_->Add(" ttl char(20),"); 			//title
 s_->Add(" working char(20),"); 			//field/working
 s_->Add(" gm char(20)");   			//manager/owner
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("VDITEM");       		//item by vendor
 s_->Add(" vd numeric(10,0),");			//vendor id
 s_->Add(" gd numeric(10,0),");			//goods id
 s_->Add(" last date,");    			//last transaction
 s_->Add(" price numeric(15,2)");		//last price
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
 tbSTRUCT->Add("BINCRD");       		//stock history
 s_->Add(" wh numeric(10,0),");			//location
 s_->Add(" id char(15),");              //item id
 s_->Add(" unit char(10),");            //unit measurement
 s_->Add(" uom numeric(10,0),");        //uomrec
 s_->Add(" gd numeric(10,0),");			//item no
 s_->Add(" gdnm char(60),");
 s_->Add(" batch char(15),");           //batch
 s_->Add(" refdt date,");               //refdt
 s_->Add(" jobid char(15),");           //jobid
 s_->Add(" cond char(1),"); 			//condition 1.good | 0.broken
 s_->Add(" bal numeric(10,2),");		//do we need inline ballance?
 s_->Add(" price numeric(15,2),");		//price
 s_->Add(" ref char(20),"); 			//transaction reference no.
 s_->Add(" req char(20),");             //if fulfillment of requesition
 s_->Add(" mode numeric(10),"); 		//transaction mode [in|out|ret in|ret out|...]
 s_->Add(" db numeric(10,2),"); 		//debet side
 s_->Add(" cr numeric(10,2)");  		//credit side
 sqlSTRUCT->Add(s_->Text);
 s_->Clear();
}
//---------------------------------------------------------
void TmnuFRM::setVAR()
{
 whCMB=new TStringList();whCMB->Clear();
 whTYPE=new TStringList();whTYPE->Clear();
 whTYPE->Add("0");whCMB->Add("Store/Warehouse");
 whTYPE->Add("1");whCMB->Add("Outlet");

 cmbSTATUS=new TStringList();cmbSTATUS->Clear();
 ivSTATUS=new TStringList();ivSTATUS->Clear();
 ivSTATUS->Add("0");cmbSTATUS->Add("Plan");
 ivSTATUS->Add("1");cmbSTATUS->Add("Proccess");
 ivSTATUS->Add("2");cmbSTATUS->Add("Release");
 ivSTATUS->Add("3");cmbSTATUS->Add("Outstanding");

 ivTYPE=new TStringList();ivTYPE->Clear();
 cmbTYPE=new TStringList();cmbTYPE->Clear();
 ivTYPE->Add("0");cmbTYPE->Add("Requisition");
 ivTYPE->Add("1");cmbTYPE->Add("Purchase Order");
 ivTYPE->Add("2");cmbTYPE->Add("Receiving");
 ivTYPE->Add("3");cmbTYPE->Add("Goods Transfer");
 ivTYPE->Add("4");cmbTYPE->Add("Adjustment");
 ivTYPE->Add("5");cmbTYPE->Add("Stock Opname");

 gdSTATUS=new TStringList();gdSTATUS->Clear();
 gdCMB=new TStringList();gdCMB->Clear();
 gdSTATUS->Add("1");gdCMB->Add("New");
 gdSTATUS->Add("2");gdCMB->Add("Recycled");
 gdSTATUS->Add("0");gdCMB->Add("Broken/Lost");
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::Button4Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::Panduan1Click(TObject *Sender)
{
 AnsiString fiPATH=pathAPP_+"\\HELP";
 AnsiString fiHELP=fiPATH+"\\HELP.MHT";
 //print using shellobj
 ShellExecute(
    Handle,	// handle to parent window
    "open",	// pointer to string that specifies operation to perform
    fiHELP.c_str(),	// pointer to filename or folder name string
    NULL,	// pointer to string that specifies executable-file parameters
    fiPATH.c_str(),	// pointer to string that specifies default directory
    SW_SHOW 	// whether file is shown when opened
   );
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::FormCreate(TObject *Sender)
{
 user_="";
 loged=false;
 pathAPP_=
 ExtractFilePath(Application->ExeName);
 pathAPP_=pathAPP_.SubString(0,pathAPP_.LastDelimiter("\\")-1);
 setVAR();
 createSTRUCT();
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::FormActivate(TObject *Sender)
{
 if(func->isBDE())
 {
 AnsiString tpath_=
 ExtractFilePath(Application->ExeName);
 tpath_=tpath_.SubString(0,tpath_.LastDelimiter("\\"));
 AnsiString back_=tpath_.SubString(tpath_.Length(),1);
 if(back_==AnsiString("\\"))
  tpath_=tpath_.SubString(0,tpath_.Length()-1);
 AnsiString front_=tpath_.SubString(0,tpath_.LastDelimiter("\\")) ;
 int st=front_.Length();
 tpath_=tpath_.SubString(st+1,tpath_.Length()-st);
 //ShowMessage(tpath_);
 ttl=Application->Title; 
 //ttl=tpath_;
 // ---------------------------------------------------*/
 func->savepath(ttl);
 path_=func->path_;
  //cek tables preparation
  func->ceksetup();
  //cektable();
  tbCREATE();
  //create menu
  menuCREATE();
  loged=false;
  login();
  //do other function
  chgQRY();
  other();
 }
 else Close();
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::Keluar1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TmnuFRM::goBTNClick(TObject *Sender)
{
int a=mnuGRP->ItemIndex;
 switch (a)
 {
  case 0:{ if(func->cekPAS(varMNU->Strings[0]))vdFRM->ShowModal();}break;
  case 1:{ if(func->cekPAS(varMNU->Strings[1]))reqFRM->ShowModal();}break;
  case 2:{ if(func->cekPAS(varMNU->Strings[2]))poFRM->ShowModal();}break;
  case 3:{ if(func->cekPAS(varMNU->Strings[3]))seekprFRM->ShowModal();}break;
  case 4:
       {
        if(func->cekPAS(varMNU->Strings[4]))
         {
          passwFRM->setup->Close();
          passwFRM->users->Close();
          passwFRM->setup->DatabaseName=mnuFRM->path_+"\\DATA";
          passwFRM->users->DatabaseName=mnuFRM->path_+"\\DATA";
          passwFRM->ShowModal();
         }
       }break;
  case 5:{ if(func->cekPAS(varMNU->Strings[5]))openFRM->ShowModal();}break;
  case 6:{ if(func->cekPAS(varMNU->Strings[6])) uploadPR();}break;
  case 7:
       {
        loged=false;
        func->del_alias(ttl);
        login();
       }break;
  default:break;
 }

}
//---------------------------------------------------------------------------

void __fastcall TmnuFRM::deptSQLAfterPost(TDataSet *DataSet)
{
if(deptSQL->FieldByName("recid")->IsNull)
{
 deptSQL->Edit();
 deptSQL->FieldByName("recid")->AsString=
 IntToStr(deptSQL->RecNo);
 deptSQL->Post();
}

}
//---------------------------------------------------------------------------

void __fastcall TmnuFRM::divSQLAfterPost(TDataSet *DataSet)
{
if(divSQL->FieldByName("recid")->IsNull)
{
 divSQL->Edit();
 divSQL->FieldByName("recid")->AsString=
 IntToStr(divSQL->RecNo);
 divSQL->Post();
}

}
//---------------------------------------------------------------------------


void __fastcall TmnuFRM::ArsipFile1Click(TObject *Sender)
{
 //func->PathCopy(true,"DATA");
 //copy master-master datas
 TStringList *ffile=new TStringList();
 ffile->Clear();
 AnsiString svrFile=pathAPP_;
 AnsiString orgpath=path_+"\\DATA";
 AnsiString cpypath="";

 svrFile+="\\SERVER.TXT";
 ffile->LoadFromFile(svrFile);

 cpypath=Trim(ffile->Text)+"\\DATA";
 ForceDirectories(cpypath);
 //ShowMessage(cpypath);
 AnsiString fromFile="";
 int pos;
 loged=true;
 bool pack_;
 TSearchRec hasil_;
 //AnsiString filenya="\\DB"+FormatDateTime("mmyyyy",mnufrm->DateTimePicker1->Date);
 if (!DirectoryExists(cpypath)) ShowMessage("failed while creating directory structure "+cpypath);
 if (FindFirst(orgpath+"\\*.DBE*", faAnyFile, hasil_) == 0)
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
   if(func->atSTR(fromFile,AnsiString("."),pos))pack_=false;
   if(pack_ && func->atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

   if(pack_) packDBF(fromFile);

   func->FileCopy(orgpath+"\\"+fromFile,cpypath+"\\"+fromFile);
   while (FindNext(hasil_) == 0)
   {
    fromFile=hasil_.Name;
    FRM->txt->Caption="Copying File ... "+
    fromFile;
    FRM->Update();

    //pack dbf first
   pack_=true;
   if(func->atSTR(fromFile,AnsiString("."),pos))pack_=false;
   if(pack_ && func->atSTR(fromFile,AnsiString(".."),pos) ) pack_=false;

    if(pack_)packDBF(fromFile);

    func->FileCopy(orgpath+"\\"+fromFile,cpypath+"\\"+fromFile);
   }
   FRM->Hide();
   delete FRM;
  }
 FindClose(hasil_);

}
//---------------------------------------------------------------------------

void __fastcall TmnuFRM::FolderLaporan1Click(TObject *Sender)
{
 //ShowMessage("Langsung Buka directory tempat RPT");
 AnsiString fiHELP=pathAPP_+"\\REPORT";
 AnsiString fiPATH=pathAPP_+"\\REPORT";;
 ShellExecute(
 Handle,	// handle to parent window
 "open",	// pointer to string that specifies operation to perform
 fiHELP.c_str(),	// pointer to filename or folder name string
 NULL,	// pointer to string that specifies executable-file parameters
 fiPATH.c_str(),	// pointer to string that specifies default directory
 SW_SHOW 	// whether file is shown when opened
 );    
}
//---------------------------------------------------------------------------

void __fastcall TmnuFRM::PerbaikiDatabase1Click(TObject *Sender)
{
func->DirPack("DATA");    
}
//---------------------------------------------------------------------------

void __fastcall TmnuFRM::PotongData1Click(TObject *Sender)
{
tbCUT2();
}
//---------------------------------------------------------------------------

