#pragma once

#include <string>
#include <boost/date_time.hpp>

class TiXmlNode;

/// 全局方法
class CUtility
{
	CUtility(const CUtility &);
	CUtility &operator= (const CUtility &);
public:
	explicit CUtility(void) {}
	virtual ~CUtility(void) {}

	static bool LoadResString(unsigned int nID, std::string &strDst);

	static void GenerateData(void);

	static std::string s_strProtocolersion;
	static std::string s_strDownloadUrl;
	static std::string s_strGuid;
	static std::string s_strOsVersion;
	static std::string s_strComputerName;
	static std::string s_strOsLoginUser;
	static std::string s_strAppVersion;
	static std::string s_strIpAddress;
	static std::string s_strWindowsDir;

	static void GetAppFullPath(std::string &strFullPath);

//////////////////////////////////////////////////////////////////////////
/// date time
	//static void GetFileTime(const std::string &strDateTime, COleDateTime &dt);

	static std::string GetCurrentDateTime(void);

	static bool String2Time(const char *pszDateTime, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute, int &nSecond);

	/// @param[out]		strDateTime		Format:YYYY-MM-DDTHH:MI:SS
	//static bool FileTime2String(const FILETIME &tmFile, std::string &strDateTime);

	/// @param[in]		strDateTime		Format:YYYY-MM-DDTHH:MI:SS
	static bool String2PosixTime(const std::string &strDateTime, boost::posix_time::ptime &dtDateTime);

	/// @param[out]		strDateTime		Format:YYYY-MM-DDTHH:MI:SS
	static bool PosixTime2String(const boost::posix_time::ptime &dtDateTime, std::string &strDateTime);

	/// @param[out]		strDateTime		Format:YYYY-MM-DD HH:MI:SS
	static bool StdTime2String(const std::time_t &tmSrc, std::string &strDateTime);
///
//////////////////////////////////////////////////////////////////////////

	static bool ReadFile(const std::string &strFileName, std::string &strReadData);
	static bool WriteFile(const std::string &strFileName, const std::string &strWriteData, const char *szMode = "a+");

	static void AddNode(TiXmlNode* pParent, const std::string &strKey, const std::string &strValue);

	static bool GetNodeText(TiXmlNode* pNode, std::string &strText);

	static bool SetNodeText(TiXmlNode* pNode, const std::string &strText);

	static bool GetAttributeText(TiXmlNode* pNode, const std::string &strAttributeName, std::string &strText);

	static bool SetAttributeText(TiXmlNode* pNode, const std::string &strAttributeName, const std::string &strText);

	static void Utf8ToGbk(std::string &strUtf8ToGbk);

	static void GbkToUtf8(std::string &strGbkToUtf8);

	/// 从文件名（全路径）获取文件路径
	static bool GetPathFromFileName(const std::string &strFileName, std::string &strPath);

	static bool GetOsVersion(std::string &strOsVersion);

	static bool GetSysInfo(std::string &strComputerName, std::string &strOsLoginUser);

	static bool GetFileVersion(std::string &strAppVersion);

	static bool GetIpAddress(const std::string &strHostName, std::string &strIpAddress);

	static bool GetWindowsDir(std::string& strWindowsDir);

	static bool StringCompareNoLength(const std::string& str1, const std::string& str2);
};
