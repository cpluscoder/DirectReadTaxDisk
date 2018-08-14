#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
#include "FieldMatch.h"

#define SAVE2FILE		1
#define UseOperateDisk	0

class CInvoiceItem
{
public:
	typedef boost::shared_ptr<CInvoiceItem> Pointer;
	static CInvoiceItem::Pointer Create(void)
	{
		return CInvoiceItem::Pointer(new CInvoiceItem);
	}
protected:
	CInvoiceItem(const CInvoiceItem &);
	CInvoiceItem &operator= (const CInvoiceItem &);

public:

	virtual ~CInvoiceItem(void) {
		Clear();
	}

	void AddField(CFieldMatch::InvoiceFieldType emType, const std::string &strValue) {
		m_mapType2Value[emType] = strValue;
	}

	bool GetNodeValue(const std::string &strNodeName, std::string &strNodeValue) {
		strNodeValue.clear();
		CFieldMatch::InvoiceFieldType emType;
		if(CFieldMatch::Instance()->GetTypeByXmlNodeName(strNodeName, emType)) {
			auto iter = m_mapType2Value.find(emType);
			if(iter != m_mapType2Value.end()) {
				strNodeValue = iter->second;
				return true;
			}
		}
		return false;
	}

	bool GetValue(CFieldMatch::InvoiceFieldType emType, std::string &strValue) {
		strValue.clear();
		auto iter = m_mapType2Value.find(emType);
		if(iter == m_mapType2Value.end()) {
			return false;
		}
		strValue = iter->second;
		return true;
	}

	void Clear(void) {
		m_mapType2Value.clear();
	}

protected:
	explicit CInvoiceItem(void) {
		Clear();
	}

protected:
	std::map<CFieldMatch::InvoiceFieldType, std::string> m_mapType2Value;

};

class TiXmlNode;

class CReadData
{
public:
	CReadData(void);
	virtual ~CReadData(void);

	bool Open(void);

	bool ReadTaxDisk(const std::string& strXml, std::string& strResult);

	void Close(void);

protected:

#if UseOperateDisk
	typedef int (__stdcall * func_operate_disk)(const char * pszXml, char* pszBuf);
	func_operate_disk m_pfnOperateDisk;
#else
	typedef wchar_t *(__stdcall * func_operate_disk_f)(const char * pszXml);
	func_operate_disk_f m_pfnOperateDiskF;
#endif

#if SAVE2FILE
	void SaveToFile(const std::string& strXml) const;
#else
	bool ParseData(const std::string& strXml);
	bool ParseInvoice(TiXmlNode* pNodeInvoiceInfo, std::list<CInvoiceItem::Pointer>& lstInvoice) const;
	bool ParseInvoiceDetail(TiXmlNode* pNodeInvoiceDetail, CInvoiceItem::Pointer pInvoiceItem) const;
#endif

protected:
	HMODULE m_hModule;
};

