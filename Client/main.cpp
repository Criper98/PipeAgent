#include <iostream>
#include <windows.h>
#include <essentials.h>
#include <NetEss.h>
#include "Classes.h"
#include "Functions.h"

using namespace std;

int main()
{
	TextColor tc;
	Log lg;
	NetSettings ns;
	EasyMSGB msgb;
	msgb.DefTitle="PipeAgent";
	
	if(!CheckInst())
	{
		if(!Install())
		{
			msgb.Ok("Installazione fallita.\nPer maggiori informazioni consultare il file \"InstallLog.log\".", msgb.Error);
		}
		return 0;
	}
	
	lg.LogFile=true;
	lg.SetFileName("log\\ExecLog.log");
	lg.RWFile();
	
	if(!ns.Init())
	{
		msgb.Ok("Errore durante il controllo dei settaggi, impossibile procede.\nPer maggiori informazioni consultare il file \"ExecLog.log\".", msgb.Error);
	}
	
	TcpIP Client;
	//Client.Port=
	
	system("pause");
	return 0;
}
