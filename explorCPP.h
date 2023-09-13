//---------------------------------------------------------------------------
#ifndef explorCPPH
#define explorCPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Db.hpp>
//---------------------------------------------------------------------------
class Texplor : public TForm
{
__published:	// IDE-managed Components
    TDBGrid *grid;
    TDataSource *gridDS;
private:	// User declarations
public:		// User declarations
    __fastcall Texplor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Texplor *explor;
//---------------------------------------------------------------------------
#endif
