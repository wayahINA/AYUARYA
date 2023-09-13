//---------------------------------------------------------------------------
#ifndef openCPPH
#define openCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TopenFRM : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label6;
    TDateTimePicker *stDT;
    TLabel *Label1;
    TDateTimePicker *lstDT;
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall stDTKeyPress(TObject *Sender, char &Key);
    void __fastcall stDTExit(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TopenFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TopenFRM *openFRM;
//---------------------------------------------------------------------------
#endif
