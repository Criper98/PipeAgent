#include <iostream>
#include <windows.h>
#include <essentials.h>
#include <NetEss.h>
#include "ClassFunctions.h"

using namespace std;

int main()
{
	TextColor tc;
	
	if(CheckInst())
	{
		if(!Install())
		{
			tc.SetColor(tc.Red);
			cout<<"Installazione fallita."<<endl;
			tc.SetColor(tc.Default);
			cout<<"Per maggiori informazioni consultare il file \"InstallLog.log\"."<<endl;
			system("pause");
		}
		return 0;
	}
	
	Log lg;
	lg.LogFile=true;
	lg.SetFileName("log\\ExecLog.log");
	
	TcpIP Client;
	//Client.Port=
	
	system("pause");
	return 0;
}
