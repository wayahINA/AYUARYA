//---------------------------------------------------------------------------
#ifndef validCPPH
#define validCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TvalFRM : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *byEDT;
    TEdit *byttlEDT;
    TEdit *ackEDT;
    TEdit *ackttlEDT;
    TEdit *appEDT;
    TEdit *appttlEDT;
    TButton *Button1;
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall byEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall byttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall ackEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall ackttlEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall appEDTKeyPress(TObject *Sender, char &Key);
    void __fastcall appttlEDTKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    void TvalFRM::key13(TObject *Sender,char &Key);
public:		// User declarations
    __fastcall TvalFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TvalFRM *valFRM;
//---------------------------------------------------------------------------
#endif
