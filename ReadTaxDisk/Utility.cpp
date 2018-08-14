#include "StdAfx.h"
#include "Utility.h"
#include <ATLComTime.h>
#include <Winver.h>
#include <WinSock2.h>
#include <sstream>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/locale.hpp>
#include <boost/format.hpp>
#include <tinyxml.h>
#include "resource.h"
#include <boost/algorithm/string.hpp>
//#include "GuidMgr.h"
//#include "Configuration.h"


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Version.lib")

using namespace std;

string CUtility::s_strProtocolersion;
string CUtility::s_strDownloadUrl;
string CUtility::s_strGuid;
string CUtility::s_strOsVersion;
string CUtility::s_strComputerName;
string CUtility::s_strOsLoginUser;
string CUtility::s_strAppVersion;
string CUtility::s_strIpAddress;
string CUtility::s_strWindowsDir;


void CUtility::AddNode(TiXmlNode* pParent, const string &strKey, const string &strValue)
{
	TiXmlElement *pNode = new TiXmlElement(strKey.c_str());
	TiXmlNode *pElement = pParent->LinkEndChild(pNode);
	TiXmlText *pText = new TiXmlText(strValue.c_str());
	pElement->LinkEndChild(pText);
}

bool CUtility::GetNodeText(TiXmlNode* pNode, string &strText)
{
	strText.clear();

	if(pNode != nullptr)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement != nullptr)
		{
			const char *pszText = pElement->GetText();
			if(pszText != nullptr)
			{
				strText = pszText;
				return true;
			}
		}
	}

	return false;
}

bool CUtility::SetNodeText(TiXmlNode* pNode, const string &strText)
{
	if(pNode != nullptr)
	{
		TiXmlNode *pText = pNode->FirstChild();
		if(pText != nullptr)
		{
			pText->SetValue(strText.c_str());
			return true;
		}
		else
		{
			pText = new TiXmlText(strText.c_str());
			pNode->LinkEndChild(pText);
		}
	}

	return false;
}

bool CUtility::GetAttributeText(TiXmlNode* pNode, const string &strAttributeName, string &strText)
{
	strText.clear();

	if(pNode != nullptr)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement != nullptr)
		{
			const char *pszText = pElement->Attribute(strAttributeName.c_str());
			if(pszText != nullptr)
			{
				strText = pszText;
				return true;
			}
		}
	}

	return false;
}

bool CUtility::SetAttributeText(TiXmlNode* pNode, const string &strAttributeName, const string &strText)
{
	if(pNode != nullptr)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement != nullptr)
		{
			pElement->SetAttribute(strAttributeName.c_str(), strText.c_str());
		}
	}

	return false;
}

void CUtility::Utf8ToGbk(string &strUtf8ToGbk)
{
	string const &to_encoding("GBK");
	string const &from_encoding("UTF-8");
	strUtf8ToGbk = boost::locale::conv::between(strUtf8ToGbk.c_str(), 
		strUtf8ToGbk.c_str() + strUtf8ToGbk.size(), 
		to_encoding, 
		from_encoding);
}

void CUtility::GbkToUtf8(string &GbkToUtf8)
{
	string const &to_encoding("UTF-8");
	string const &from_encoding("GBK");
	GbkToUtf8 = boost::locale::conv::between(GbkToUtf8.c_str(), 
		GbkToUtf8.c_str() + GbkToUtf8.size(), 
		to_encoding, 
		from_encoding);
}

void CUtility::GetAppFullPath(string &strFullPath)
{
	strFullPath.clear();
	char szModulePath[MAX_PATH];
	::memset(szModulePath, '\0', MAX_PATH);
	::GetModuleFileName(nullptr, szModulePath, MAX_PATH);
	char* pszTemp = ::_tcsrchr(szModulePath, (unsigned int)'\\');
	if(pszTemp > szModulePath)
	{
		szModulePath[pszTemp - szModulePath + 1] = '\0';
		strFullPath = szModulePath;
	}
}

string CUtility::GetCurrentDateTime(void)
{
	COleDateTime dtCurrent = COleDateTime::GetCurrentTime();
	boost::format format_("%04d-%02d-%02d %02d:%02d:%02d");
	format_ % dtCurrent.GetYear();
	format_ % dtCurrent.GetMonth();
	format_ % dtCurrent.GetDay();
	format_ % dtCurrent.GetHour();
	format_ % dtCurrent.GetMinute();
	format_ % dtCurrent.GetSecond();
	return string(format_.str());
}

bool CUtility::String2Time(const char *pszDateTime, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute, int &nSecond)
{
	if(pszDateTime != nullptr)
	{
		if(strlen(pszDateTime) == 19)
		{
			::sscanf_s(pszDateTime, "%04d-%02d-%02d %02d:%02d:%02d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
			return true;
		}
	}

	return false;
}


bool CUtility::ReadFile(const string &strFileName, string &strReadData)
{
	FILE *pFile = nullptr;
	::fopen_s(&pFile, strFileName.c_str(), "r");
	if(pFile == nullptr)
	{
		return false;
	}

	string strUTF8;
	strUTF8.clear();
	char szBuf[1024];
	while(!feof(pFile))
	{
		::memset(szBuf, '\0', sizeof(szBuf));
		::fread(szBuf, sizeof(char), sizeof(szBuf) - 2, pFile);
		strUTF8 += szBuf;
	}
	::fclose(pFile);

	/// UTF-8 ----> ANSI
	Utf8ToGbk(strUTF8);
	strReadData = strUTF8;

	return true;
}

bool CUtility::WriteFile(const string &strFileName, const string &strWriteData, const char *szMode)
{
	FILE *pFile = nullptr;
	::fopen_s(&pFile, strFileName.c_str(), szMode);
	if(pFile == nullptr)
	{
		return false;
	}

	/// ANSI ----> UTF-8
	string strData = strWriteData;
	GbkToUtf8(strData);

	::fwrite(strData.c_str(), sizeof(char), strData.size(), pFile);

	::fclose(pFile);

	return true;
}

bool CUtility::GetPathFromFileName(const string &strFileName, string &strPath)
{
	strPath.clear();
	size_t st = strFileName.rfind("\\");
	if(st == string::npos)
	{
		return false;
	}

	strPath = strFileName.substr(0, st);
	strPath.append("\\");

	return true;
}

bool CUtility::String2PosixTime(const string &strDateTime, boost::posix_time::ptime &dtDateTime)
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	dtDateTime = from_iso_string(strDateTime);

	return true;
}

bool CUtility::PosixTime2String(const boost::posix_time::ptime &dtDateTime, string &strDateTime)
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	strDateTime.clear();

	strDateTime = to_iso_string(dtDateTime);

	return !strDateTime.empty();
}

bool CUtility::StdTime2String(const std::time_t &tmSrc, string &strDateTime)
{
	tm dtTemp;
	if(0 == ::localtime_s(&dtTemp, &tmSrc))
	{
		string strFormat = "%04d-%02d-%02d %02d:%02d:%02d";
		boost::format formatStr = boost::format(strFormat.c_str())
			% (dtTemp.tm_year + 1900) % (dtTemp.tm_mon + 1) % dtTemp.tm_mday
			% dtTemp.tm_hour % dtTemp.tm_min % dtTemp.tm_sec;

		strDateTime = formatStr.str();

		return !strDateTime.empty();
	}

	return false;
}

bool CUtility::GetOsVersion(string &strOsVersion)
{
	strOsVersion.clear();

	SYSTEM_INFO systemInfo;
	ZeroMemory(&systemInfo, sizeof(SYSTEM_INFO));
	typedef void (WINAPI *FN_GetNativeSystemInfo)(LPSYSTEM_INFO);
	FN_GetNativeSystemInfo pfnGetNativeSystemInfo = nullptr;
	pfnGetNativeSystemInfo = (FN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if(nullptr != pfnGetNativeSystemInfo)
	{
		pfnGetNativeSystemInfo(&systemInfo);
	}
	else
	{
		::GetSystemInfo(&systemInfo);
	}

	OSVERSIONINFOEX osVerInfo;
	ZeroMemory(&osVerInfo, sizeof(OSVERSIONINFOEX));
	osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!::GetVersionEx((OSVERSIONINFO *)&osVerInfo))
	{
		return false;
	}
	if(VER_PLATFORM_WIN32_NT != osVerInfo.dwPlatformId || osVerInfo.dwMajorVersion <= 4)
	{
		return false;
	}
	typedef BOOL (WINAPI *FN_GetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
	FN_GetProductInfo pfnGetProductInfo = nullptr;
	pfnGetProductInfo = (FN_GetProductInfo)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
	if(pfnGetProductInfo == nullptr)
	{
		return false;
	}

	DWORD dwType = 0;
	string strTemp = "Microsoft ";
	if(osVerInfo.dwMajorVersion == 6)
	{
		if(osVerInfo.dwMinorVersion == 0)
		{
			if(osVerInfo.wProductType == VER_NT_WORKSTATION)
			{
				strTemp += "Windows Vista ";
			}
			else
			{
				strTemp += "Windows Server 2008 ";
			}
		}
		else if(osVerInfo.dwMinorVersion == 1)
		{
			if(osVerInfo.wProductType == VER_NT_WORKSTATION)
			{
				strTemp += "Windows 7 ";
			}
			else
			{
				strTemp += "Windows Server 2008 R2 ";
			}
		}

		pfnGetProductInfo(osVerInfo.dwMajorVersion, osVerInfo.dwMinorVersion, 0, 0, &dwType);
		switch(dwType)
		{
		case PRODUCT_ULTIMATE:
			{
				strTemp += "Ultimate Edition";
			}
			break;
		case PRODUCT_PROFESSIONAL:
			{
				strTemp += "Professional";
			}
			break;
		case PRODUCT_HOME_PREMIUM:
			{
				strTemp += "Home Premium Edition";
			}
			break;
		case PRODUCT_HOME_BASIC:
			{
				strTemp += "Home Basic Edition";
			}
			break;
		case PRODUCT_ENTERPRISE:
			{
				strTemp += "Enterprise Edition";
			}
			break;
		case PRODUCT_BUSINESS:
			{
				strTemp += "Business Edition";
			}
			break;
		case PRODUCT_STARTER:
			{
				strTemp += "Starter Edition";
			}
			break;
		case PRODUCT_CLUSTER_SERVER:
			{
				strTemp += "Cluster Server Edition";
			}
			break;
		case PRODUCT_DATACENTER_SERVER:
			{
				strTemp += "Datacenter Edition";
			}
			break;
		case PRODUCT_DATACENTER_SERVER_CORE:
			{
				strTemp += "core installation";
			}
			break;
		case PRODUCT_ENTERPRISE_SERVER:
			{
				strTemp += "Enterprise Edition";
			}
			break;
		case PRODUCT_ENTERPRISE_SERVER_CORE:
			{
				strTemp += "Enterprise Edition (core installation)";
			}
			break;
		case PRODUCT_ENTERPRISE_SERVER_IA64:
			{
				strTemp += "Enterprise Edition for Itanium-based Systems";
			}
			break;
		case PRODUCT_SMALLBUSINESS_SERVER:
			{
				strTemp += "Small Business Server";
			}
			break;
		case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
			{
				strTemp += "Small Business Server Premium Edition";
			}
			break;
		case PRODUCT_STANDARD_SERVER:
			{
				strTemp += "Standard Edition";
			}
			break;
		case PRODUCT_STANDARD_SERVER_CORE:
			{
				strTemp += "Standard Edition (core installation)";
			}
			break;
		case PRODUCT_WEB_SERVER:
			{
				strTemp += "Web Server Edition";
			}
			break;
		default:
			break;
		}
	}
	else if(osVerInfo.dwMajorVersion == 5 && osVerInfo.dwMinorVersion == 2)
	{
		if(::GetSystemMetrics(SM_SERVERR2))
		{
			strTemp += "Windows Server 2003 R2, ";
		}
		else if(osVerInfo.wSuiteMask & VER_SUITE_STORAGE_SERVER)
		{
			strTemp += "Windows Storage Server 2003";
		}
		else if(osVerInfo.wSuiteMask & VER_SUITE_WH_SERVER)
		{
			strTemp += "Windows Home Server";
		}
		else if(osVerInfo.wProductType == VER_NT_WORKSTATION && systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			strTemp += "Windows XP Professional x64 Edition";
		}
		else
		{
			strTemp += "Windows Server 2003, ";
		}

		if(osVerInfo.wProductType != VER_NT_WORKSTATION)
		{
			if(systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			{
				if(osVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
				{
					strTemp += "Datacenter Edition for Itanium-based Systems";
				}
				else if(osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				{
					strTemp += "Enterprise Edition for Itanium-based Systems";
				}
			}
			else if(systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				if(osVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
				{
					strTemp += "Datacenter x64 Edition";
				}
				else if(osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				{
					strTemp += "Datacenter x64 Edition";
				}
				else
				{
					strTemp += "Standard x64 Edition";
				}
			}
			else
			{
				if(osVerInfo.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
				{
					strTemp += "Compute Cluster Edition";
				}
				else if(osVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
				{
					strTemp += "Datacenter Edition";
				}
				else if(osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				{
					strTemp += "Enterprise Edition";
				}
				else if(osVerInfo.wSuiteMask & VER_SUITE_BLADE)
				{
					strTemp += "Web Edition";
				}
				else
				{
					strTemp += "Standard Edition";
				}
			}
		}
	}
	else if(osVerInfo.dwMajorVersion == 5 && osVerInfo.dwMinorVersion == 1)
	{
		strTemp += "Windows XP ";
		if(osVerInfo.wSuiteMask & VER_SUITE_PERSONAL)
		{
			strTemp += "Home Edition";
		}
		else
		{
			strTemp += "Professional";
		}
	}
	else if(osVerInfo.dwMajorVersion == 5 && osVerInfo.dwMinorVersion == 0)
	{
		strTemp += "Windows 2000 ";
		if(osVerInfo.wProductType == VER_NT_WORKSTATION)
		{
			strTemp += "Professional";
		}
		else
		{
			if(osVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
			{
				strTemp += "Datacenter Server";
			}
			else if(osVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
			{
				strTemp += "Advanced Server";
			}
			else
			{
				strTemp += "Server";
			}
		}
	}

	if(_tcslen(osVerInfo.szCSDVersion) > 0)
	{
		strTemp += " ";
		strTemp += osVerInfo.szCSDVersion;
	}

	char szNum[MAX_PATH];
	sprintf_s(szNum, sizeof(szNum), " (build %d)", osVerInfo.dwBuildNumber);
	strTemp.append(szNum);
	if(osVerInfo.dwMajorVersion >= 6)
	{
		if(systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			strTemp += ", 64-bit";
		}
		else if(systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		{
			strTemp += ", 32-bit";
		}
	}

	strOsVersion = strTemp;

	return !strOsVersion.empty();
}

bool CUtility::GetSysInfo(string &strComputerName, string &strOsLoginUser)
{
	DWORD dwBufCharCount = 0;
	const int INFO_BUFFER_SIZE = 32767;
	char szInfoBuf[INFO_BUFFER_SIZE];

	strComputerName.clear();
	ZeroMemory(szInfoBuf, INFO_BUFFER_SIZE);
	dwBufCharCount = INFO_BUFFER_SIZE;
	if(!::GetComputerName(szInfoBuf, &dwBufCharCount))
	{
		return false;
	}
	strComputerName = szInfoBuf;

	// Get the user name
	strOsLoginUser.clear();
	ZeroMemory(szInfoBuf, INFO_BUFFER_SIZE);
	dwBufCharCount = INFO_BUFFER_SIZE;
	if(!::GetUserName(szInfoBuf, &dwBufCharCount))
	{
		return false;
	}
	strOsLoginUser = szInfoBuf;

	return true;
}

bool CUtility::GetFileVersion(string &strAppVersion)
{
	strAppVersion.clear();

	char szPath[MAX_PATH];
	::GetModuleFileName(nullptr, szPath, sizeof(szPath));

	/// 获取文件版本信息大小
	DWORD dwDummy = 0;
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szPath, &dwDummy);
	if(dwVerInfoSize <= 0)
	{
		return false;
	}

	/// 获取文件版本信息.
	LPBYTE lpVersionInfo = new BYTE[dwVerInfoSize];
	if(lpVersionInfo == nullptr)
	{
		return false;
	}
	if(::GetFileVersionInfo(szPath, 0, dwVerInfoSize, lpVersionInfo))
	{
		UINT uLen = 0;
		VS_FIXEDFILEINFO *lpFileInfo = nullptr;
		/// 查询文件版本信息
		if(::VerQueryValue(lpVersionInfo, _T("\\"), (LPVOID *)&lpFileInfo, &uLen))
		{
			if(lpFileInfo != nullptr)
			{
				DWORD dwFileVersionMS = 0;
				DWORD dwFileVersionLS = 0;

				dwFileVersionMS = lpFileInfo->dwFileVersionMS;
				dwFileVersionLS = lpFileInfo->dwFileVersionLS;

				DWORD dwVer1 = HIWORD(dwFileVersionMS);
				DWORD dwVer2 = LOWORD(dwFileVersionMS);
				DWORD dwVer3 = HIWORD(dwFileVersionLS);
				DWORD dwVer4 = LOWORD(dwFileVersionLS);

				char szBuf[MAX_PATH];
				sprintf_s(szBuf, "%d.%d.%d.%d", dwVer1, dwVer2, dwVer3, dwVer4);
				strAppVersion = szBuf;
			}
		}
	}

	delete [] lpVersionInfo;

	return !strAppVersion.empty();
}

bool CUtility::GetIpAddress(const string &strHostName, string &strIpAddress)
{
	strIpAddress.clear();
	if(strHostName.empty())
	{
		return false;
	}

	WSADATA wsaData;
	if(0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}

	struct hostent *remoteHost = ::gethostbyname(strHostName.c_str());
	if(nullptr != remoteHost)
	{
		if(AF_INET == remoteHost->h_addrtype)
		{
			if(nullptr != remoteHost->h_addr_list[0])
			{
				struct in_addr inAddr;
				inAddr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
				strIpAddress = inet_ntoa(inAddr);
			}
		}
	}

	::WSACleanup();

	return !strIpAddress.empty();
}

bool CUtility::GetWindowsDir(std::string& strWindowsDir)
{
	const int BufSize = 32768;
	strWindowsDir.clear();
	strWindowsDir = "Windows";
	char szBuf[BufSize];
	ZeroMemory(szBuf, BufSize);
	int nRtn = ::GetWindowsDirectory(szBuf, BufSize);
	if(nRtn > 0 && nRtn < BufSize)
	{
		strWindowsDir.assign(szBuf, nRtn);
		return true;
	}
	return false;
}

void CUtility::GenerateData(void)
{
	s_strProtocolersion.clear();
	s_strDownloadUrl.clear();
	s_strGuid.clear();
	s_strOsVersion.clear();
	s_strComputerName.clear();
	s_strOsLoginUser.clear();
	s_strAppVersion.clear();
	s_strIpAddress.clear();
	s_strWindowsDir.clear();
	
	//LoadResString(IDS_PROTOCOL_VERSION, s_strProtocolersion);
	//if(s_strProtocolersion.empty())
	//{
	//	s_strProtocolersion = DEFAULT_VERSION;
	//}
	//LoadResString(IDS_DOWNLOAD_URL, s_strDownloadUrl);
	//if(s_strDownloadUrl.empty())
	//{
	//	s_strDownloadUrl = g_pCfg->m_strCheckUpdateUrl;
	//}

	//CGuidMgr guidMgr;
	//guidMgr.GetGuid(s_strGuid);

	string strTemp1, strTemp2;
	CUtility::GetOsVersion(s_strOsVersion);
	CUtility::GetSysInfo(s_strComputerName, s_strOsLoginUser);
	CUtility::GetFileVersion(s_strAppVersion);
	CUtility::GetIpAddress(s_strComputerName, s_strIpAddress);
	CUtility::GetWindowsDir(s_strWindowsDir);
}

bool CUtility::LoadResString(unsigned int nID, string &strDst)
{
	strDst.clear();
	TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf, sizeof(szBuf)/sizeof(szBuf[0]));
	if(::LoadString(::GetModuleHandle(NULL), nID, szBuf, sizeof(szBuf)/sizeof(szBuf[0])) > 0)
	{
		strDst = szBuf;
		return true;
	}
	return false;
}

bool CUtility::StringCompareNoLength(const std::string& str1, const std::string& str2)
{
	bool bEqual = boost::algorithm::icontains(str1, str2);
	bEqual |= boost::algorithm::icontains(str1, str2);
	return bEqual;
}
