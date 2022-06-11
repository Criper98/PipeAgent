using namespace std;

bool CheckInst() // controlla se il software è già installato
{
	RegUtils ru;
	DirUtils du;
	EasyMSGB msgb;
	
	ru.MasterKey=ru.HKLM;
	
	/*if(du.CheckFile("InstallLog.log"))
	{
		msgb.Ok("Per eseguire PipeAgent si prega di riavviare il sistema, a meno che non sia gia' stato avviato a seguito dell'installazione.", msgb.Info);
		exit(0);
	}*/
	
	if(ru.RegRead("SOFTWARE\\PipeAgent", "ClientInstall", REG_SZ) == "1")
		return true;	// è già installato
	else
		return false;	// non è installato
}

bool Install() // procedura di installazione del programma
{
	TextColor tc;
	Log lg("InstallLog.log", true);
	EasyMenu em;
	LoadingBar lb;
	DirUtils du;
	RegUtils ru;
	SimpleFileSettings sfs;
	GeneralUtils gu;
	EasyMSGB msgb;
	
	ru.MasterKey=ru.HKLM;
	lg.RWFile(); // Sbianco il file di log
	
	lg.WriteLog("Inizializzazione dell'installazione.");
	
	msgb.DefTitle="PipeAgent";
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
		cout<<"Annullamento installazione..."<<endl;
		return false;
	}
	
	if(!gu.CheckUAC())
	{
		lg.WriteLog("No UAC.");
		msgb.Ok("PipeAgent necessita dei permessi di amministratore per essere installato.\nSi prega di riavviare il programma fornendo tali permessi.", msgb.Error);
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
		if(!du.MakeDir("C:\\PipeAgent\\Client\\log"))
		{
			lb.StopBar(100);
			lg.WriteLog("Creazione percorso \"C:\\PipeAgent\\Client\\log\" fallita.");
			return false;
		}
		lg.WriteLog("Percorso \"C:\\PipeAgent\\Client\\log\" creato.");
	}
	lb.Percent=56; // [---------------------------------------------------------------------------]
	
	lg.WriteLog("Copia di PipeAgent.exe in \"C:\\PipeAgent\\Client\".");
	if(!du.CopyPasteFile(du.GetFullFilePath(), "C:\\PipeAgent\\Client\\PipeAgent.exe"))
	{
		lb.StopBar(100);
		lg.WriteLog("Copia fallita.");
		return false;
	}
	lb.Percent=70; // [---------------------------------------------------------------------------]
	
	/*lg.WriteLog("Scrittura chiave di registro per avvio automatico.");
	if(!ru.RegWrite("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "ClientPipeAgent", REG_SZ, "\"C:\\PipeAgent\\Client\\PipeAgent.exe\""))
	{
		lb.StopBar(100);
		lg.WriteLog("Scrittura chiave di registro fallita.");
		return false;
	}*/
	lg.WriteLog("Creazione Task di avvio automatico.");
	if(gu.NoOutputCMD("schtasks /create /sc ONSTART /tn \"PipeAgent\" /tr \"C:\\PipeAgent\\Client\\PipeAgent.exe\" /ru System") != 0)
	{
		lb.StopBar(100);
		lg.WriteLog("Creazione Task fallita.");
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
	if(!ru.RegWrite("SOFTWARE\\PipeAgent", "ClientInstall", REG_SZ, "1"))
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
		du.RunExe("C:\\PipeAgent\\Client\\PipeAgent.exe");
	}
	
	lg.WriteLog("Installazione terminata con successo.");
	cout<<"Grazie per aver scelto PipeAgent!"<<endl;
	
	Sleep(2000);
	
	return true;
}
