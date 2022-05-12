using namespace std;

class NetSettings
{
	private:
		SimpleFileSettings sfs;
		Log lg;
		
	public:
		string ServerAddress;
		string ServerPort;
		
		bool Init() // some checking
		{
			lg.SetFileName("log\\ExecLog.log");
			lg.LogFile=true;
			
			lg.WriteLog("Inizializzazione settaggi.");
			sfs.SetFileName("C:\\PipeAgent\\Client\\NetSettings.ini");
			if(!sfs.CheckFile())
			{
				lg.WriteLog("File \"NetSettings.ini\" assente o inaccessibile.");
				return false;
			}
			if(!sfs.CheckSetting("ServerAddress"))
			{
				lg.WriteLog("Settaggio \"ServerAddress\" assente.");
				return false;
			}
			if(!sfs.CheckSetting("ServerPort"))
			{
				lg.WriteLog("Settaggio \"ServerPort\" assente.");
				return false;
			}
			return true;
		}
		
		void LoadSettings()
		{
			lg.WriteLog("Caricamento NetSettings.");
			ServerAddress = sfs.GetSetting("ServerAddress");
			ServerPort = sfs.GetSetting("ServerPort");
		}
};
