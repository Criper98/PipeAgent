using namespace std;

class CLIENT
{
	private:
		TcpIP clnt;
		Log lg;
		DirUtils du;
		
	public:
		CLIENT() : lg(du.GetCurrentFilePath()+"log\\NetLog.log" ,true) {}
		
		bool Init(string Host, string Port)
		{
			lg.RWFile();
			lg.WriteLog("Inizializzazione Client.");
			
			clnt.Host = Host;
			clnt.Port = stoi(Port);
			short i=clnt.StartClient();
			
			if(i != 0)
			{
				lg.WriteLog("Errore nell'inizializzazione: Codice-"+i);
				return false; // Errore
			}
			return true; // OK
		}
};

class NetSettings
{
	private:
		SimpleFileSettings sfs;
		Log lg;
		DirUtils du;
		
	public:
		string ServerAddress;
		string ServerPort;
		
		NetSettings() : lg(du.GetCurrentFilePath()+"log\\ExecLog.log", true) {}
		
		bool Init() // some checking
		{
			lg.WriteLog("Inizializzazione settaggi.");
			sfs.SetFileName(du.GetCurrentFilePath()+"NetSettings.ini");
			
			if(!sfs.CheckFile())
			{
				lg.WriteLog("File \"NetSettings.ini\" assente o inaccessibile.");
				return false; // Errore
			}
			if(!sfs.CheckSetting("ServerAddress"))
			{
				lg.WriteLog("Settaggio \"ServerAddress\" assente.");
				return false; // Errore
			}
			if(!sfs.CheckSetting("ServerPort"))
			{
				lg.WriteLog("Settaggio \"ServerPort\" assente.");
				return false; // Errore
			}
			
			lg.WriteLog("Caricamento NetSettings.");
			ServerAddress = sfs.GetSetting("ServerAddress");
			ServerPort = sfs.GetSetting("ServerPort");
			
			return true; // OK
		}
};
