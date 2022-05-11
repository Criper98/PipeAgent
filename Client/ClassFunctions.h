using namespace std;

bool CheckInst() // controlla se il software è già installato
{
	RegUtils ru;
	ru.MasterKey=ru.HKCU;
	
	if(ru.RegRead("SOFTWARE\\PipeAgent", "ClientInstall", REG_SZ) == "1")
		return false;	// è già installato
	else
		return true;	// non è installato
}

bool Install() // procedura di installazione del programma
{
	TextColor tc;
	Log lg;
	EasyMenu em;
	LoadingBar lb;
	DirUtils du;
	RegUtils ru;
	SimpleFileSettings sfs;
	
	lg.LogFile=true;
	lg.SetFileName("InstallLog.log");
	lg.RWFile();
	
	lg.WriteLog("Inizializzazione dell'installazione.");
	
	ru.MasterKey=ru.HKCU;
	lb.Color=10;
	sfs.SetFileName("C:\\PipeAgent\\Client\\NetSettings.ini");
	
	vector<string> Menu;
	int sel=0;
	string netinfo;
	
	Menu.push_back("No");
	Menu.push_back("Si");
	
	cout<<"PipeAgent non e' installato su questa postazione.\nSi desidera installare PipeAgent ora?"<<endl;
	sel=em.SingleSelMenu(Menu, em.Quadre);
	
	if(sel == 0)
	{
		lg.WriteLog("Installazione annullata dall'utente.");
		cout<<"Chiusura del programma..."<<endl;
		Sleep(1000);
		return false;
	}
	
	cout<<"Scegli indirizzo e porta del Server. (es. 127.0.0.1:5555)\n> ";
	for(bool i=true; i;)
	{
		cin>>netinfo;
		if(netinfo.find(':') != string::npos)
			i=false;
		else
			cout<<"L'indirizzo \""<<netinfo<<"\" non e' nel corretto formato, si prega di rispettare il formato \"indirizzo:porta\".\n> ";
	}
	netinfo+=';';
	
	cout<<"Installazione in corso..."<<endl;
	lb.Percent=0;
	lb.FullBar(50);
	
	lg.WriteLog("Controllo percorso \"C:\\PipeAgent\".");
	if(!du.CheckPath("C:\\PipeAgent"))
	{
		lg.WriteLog("Il percorso \"C:\\PipeAgent\" non esiste.");
		if(!du.MakeDir("C:\\PipeAgent"))
		{
			lb.StopBar(100);
			lg.WriteLog("Creazione percorso \"C:\\PipeAgent\" fallita.");
			return false;
		}
		lg.WriteLog("Percorso \"C:\\PipeAgent\" creato.");
	}
	lb.Percent=14; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Controllo percorso \"C:\\PipeAgent\\Client\".");
	if(!du.CheckPath("C:\\PipeAgent\\Client"))
	{
		lg.WriteLog("Il percorso \"C:\\PipeAgent\\Client\" non esiste.");
		if(!du.MakeDir("C:\\PipeAgent\\Client"))
		{
			lb.StopBar(100);
			lg.WriteLog("Creazione percorso \"C:\\PipeAgent\\Client\" fallita.");
			return false;
		}
		lg.WriteLog("Percorso \"C:\\PipeAgent\\Client\" creato.");
	}
	lb.Percent=28; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Creazione del file di configurazione.");
	if(!sfs.MakeFile())
	{
		lg.WriteLog("Impossibile creare il file di configurazione.");
		return false;
	}
	sfs.SetSetting("ServerAddress", netinfo.substr(0, netinfo.find(':')));
	sfs.SetSetting("ServerPort", SimpleFind(netinfo, ":", ";"));
	lb.Percent=42; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Controllo percorso \"C:\\PipeAgent\\Client\\log\".");
	if(!du.CheckPath("C:\\PipeAgent\\Client\\log"))
	{
		lg.WriteLog("Il percorso \"C:\\PipeAgent\\Client\\log\" non esiste.");
		if(!du.MakeDir("C:\\PipeAgent\\Client\\log"))
		{
			lb.StopBar(100);
			lg.WriteLog("Creazione percorso \"C:\\PipeAgent\\Client\\log\" fallita.");
			return false;
		}
		lg.WriteLog("Percorso \"C:\\PipeAgent\\Client\\log\" creato.");
	}
	lb.Percent=56; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Copio PipeAgent.exe in \"C:\\PipeAgent\\Client\".");
	if(!du.CopyPasteFile(du.GetCurrentFilePath(), "C:\\PipeAgent\\Client\\PipeAgent.exe"))
	{
		lb.StopBar(100);
		lg.WriteLog("Copia fallita.");
		return false;
	}
	lb.Percent=70; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Scrittura chiave di registro per avvio automatico.");
	if(!ru.RegWriteValue("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "ClientAgent", REG_SZ, "\"C:\\PipeAgent\\Client\\PipeAgent.exe\""))
	{
		lb.StopBar(100);
		lg.WriteLog("Scrittura chiave di registro fallita.");
		return false;
	}
	lb.Percent=84; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Creazione chiave di registro PipeAgent.");
	if(!ru.RegMakeKey("SOFTWARE\\PipeAgent"))
	{
		lb.StopBar(100);
		lg.WriteLog("Creazione chiave di registro fallita.");
		return false;
	}
	lb.Percent=100; // [---------------------------------------------------------------------------]
	lb.StopBar(100);
	lg.WriteLog("Installazione terminata.");
	
	lg.WriteLog("Scrittura chiave di registro per installazione terminata.");
	if(!ru.RegWriteValue("SOFTWARE\\PipeAgent", "ClientInstall", REG_SZ, "1"))
	{
		lg.WriteLog("Scrittura chiave di registro fallita.");
		return false;
	}
	
	tc.SetColor(tc.Green);
	cout<<"Installazione completata!"<<endl;
	tc.SetColor(tc.Default);
	cout<<"Si desidera eseguire ora PipeAgent?"<<endl;
	sel=em.SingleSelMenu(Menu, em.Quadre);
	
	if(sel == 1)
	{
		lg.WriteLog("Esecuzione di PipeAgent.exe.");
		du.RunExe("C:\\PipeAgent\\PipeAgent.exe");
	}
	
	lg.WriteLog("Installazione terminata con successo.");
	cout<<"Grazie per aver scelto PipeAgent!"<<endl;
	
	Sleep(2000);
	
	return true;
}
