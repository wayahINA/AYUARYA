//---------------------------------------------------------------------------
#ifndef barcppH
#define barcppH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TbarFRM : public TForm
{
__published:	// IDE-managed Components
    TStaticText *txt;
    TProgressBar *bar;
    TAnimate *gmb;
private:	// User declarations
public:		// User declarations
    __fastcall TbarFRM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TbarFRM *barFRM;
//---------------------------------------------------------------------------
#endif
