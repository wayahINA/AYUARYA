//---------------------------------------------------------------------------
#include <vcl.h>
#include "BARCPP.h"
#pragma hdrstop
USERES("POIDP.res");
USEFORM("mnuCPP.cpp", mnuFRM);
USEFORM("reqCPP.cpp", reqFRM);
USEFORM("funcCPP.cpp", func);
USEFORM("PASSWCPP.CPP", passwFRM);
USEFORM("SIGNONCPP.CPP", signonFRM);
USEFORM("validCPP.cpp", valFRM);
USEFORM("PICKCPP.CPP", pickform);
USEFORM("vdCPP.cpp", vdFRM);
USEFORM("poCPP.cpp", poFRM);
USEFORM("openCPP.cpp", openFRM);
USEFORM("seekCPP.cpp", seekprFRM);
USEFORM("filterCPP.cpp", prtFRM);
USEFORM("BARCPP.CPP", barFRM);
USEFORM("explorCPP.cpp", explor);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Title="PO Recording System";
        Application->Initialize();

        TbarFRM *FRM=new TbarFRM(Application);
        FRM->gmb->Visible=true;
        FRM->gmb->Active=true;

        FRM->bar->Max=12;
        FRM->bar->Step=1;
        FRM->bar->Left=0;

        FRM->txt->Caption="Wait a moment,uploading database and forms...";
        FRM->Show();
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TmnuFRM), &mnuFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TreqFRM), &reqFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(Tfunc), &func);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TpasswFRM), &passwFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TsignonFRM), &signonFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(Tpickform), &pickform);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TvdFRM), &vdFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TpoFRM), &poFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TopenFRM), &openFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TseekprFRM), &seekprFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(TprtFRM), &prtFRM);
        FRM->bar->Position+=1;
        FRM->Update();

        Application->CreateForm(__classid(Texplor), &explor);
        delete FRM;
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
