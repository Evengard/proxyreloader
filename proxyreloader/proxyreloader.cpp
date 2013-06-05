// proxyreloader.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>


int AutoProxyDiscovery(bool enable)
    {
        _TCHAR *szSubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections");

		BYTE pBuffer[372];
		DWORD nMaxLength = 372;
        DWORD   rc = 0; 
        DWORD   dwType; 
        HKEY    hOpenedKey;

        rc = RegOpenKeyEx (
                HKEY_CURRENT_USER, // handle of open key 
                szSubKey,               // address of name of subkey to open 
                0,                  // reserved 
                KEY_READ | KEY_WRITE ,       // security access mask 
                &hOpenedKey            // address of handle of open key 
                );
		if( ERROR_SUCCESS == rc)
        {
            rc = RegQueryValueEx( 
                hOpenedKey, 
                _T("DefaultConnectionSettings"), 
                0, 
                &dwType, 
                (LPBYTE)pBuffer, 
                &nMaxLength ); 
            if( rc != ERROR_SUCCESS ) 
            { 
                return (int)-1;
            }

			if(enable)
			{
				pBuffer[8] = pBuffer[8] | 8;
			}
			else
			{
				pBuffer[8] = pBuffer[8] ^ 8;
			}

			rc = RegSetValueEx(
				hOpenedKey,
				_T("DefaultConnectionSettings"),
				0,
				dwType,
				pBuffer,
				nMaxLength
				);

			if( rc != ERROR_SUCCESS ) 
            { 
                return (int)-1;
            } 

            RegCloseKey( hOpenedKey );
            return 0; 
        }
        else
        {
            return (DWORD)-1;
        }   
    }

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc >= 2)
	{
		if (0 == _tcscmp(argv[1], _T("autodiscovery=true")))
		{
			AutoProxyDiscovery(true);
		}
		else if (0 == _tcscmp(argv[1], _T("autodiscovery=false")))
		{
			AutoProxyDiscovery(false);
		}
	}
	InternetSetOption(0, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(0, INTERNET_OPTION_REFRESH, NULL, 0);

	return 0;
}

