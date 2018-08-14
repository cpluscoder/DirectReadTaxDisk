#include "StdAfx.h"
#include "ReadData.h"
#include <tinyxml.h>
#include "Utility.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

bool WStr2Str(const wstring& wstrSrc, string& strDst)
{
	strDst.clear();
	int nLength = ::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, NULL, 0, NULL, NULL);
	if(nLength <= 0)
	{
		return false;
	}

	nLength += 1;
	char* pszDst = new char[nLength];
	if(NULL == pszDst)
	{
		return false;
	}
	ZeroMemory(pszDst, nLength);
	nLength = ::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, pszDst, nLength, NULL, NULL);

	strDst.assign(pszDst, nLength);
	delete [] pszDst;

	return true;
}

CReadData::CReadData(void)
{
	m_hModule = nullptr;

#if UseOperateDisk
	m_pfnOperateDisk = nullptr;
#else
	m_pfnOperateDiskF = nullptr;
#endif
}

CReadData::~CReadData(void)
{
	m_hModule = nullptr;

#if UseOperateDisk
	m_pfnOperateDisk = nullptr;
#else
	m_pfnOperateDiskF = nullptr;
#endif
}

bool CReadData::Open(void)
{
	m_hModule = nullptr;

#if UseOperateDisk
	m_pfnOperateDisk = nullptr;
#else
	m_pfnOperateDiskF = nullptr;
#endif

	m_hModule = ::LoadLibrary("NISEC_SKPC.dll");
	if(m_hModule)
	{
#if UseOperateDisk
		m_pfnOperateDisk = (func_operate_disk)::GetProcAddress(m_hModule, "OperateDisk");
		return m_pfnOperateDisk != nullptr;
#else
		m_pfnOperateDiskF = (func_operate_disk_f)::GetProcAddress(m_hModule, "OperateDiskF");
		return m_pfnOperateDiskF != nullptr;
#endif
	}

	return false;
}

void CReadData::Close(void)
{
	if(m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = nullptr;
	}

	::SetDllDirectory(nullptr);
}

bool CReadData::ReadTaxDisk(const std::string& strXml, std::string& strResult)
{
	if(strXml.empty())
	{
		return false;
	}
	strResult.clear();

#if UseOperateDisk
	char *pszBuf = new char[1024 * 1024];
	ZeroMemory(pszBuf, 1024 * 1024);
	if(m_pfnOperateDisk)
	{
		int nRet = m_pfnOperateDisk(strXml.c_str(), pszBuf);
		if(nRet > 0)
		{
			strResult = pszBuf;
		}
	}
	delete [] pszBuf;
	pszBuf = nullptr;
	if(strResult.empty())
	{
		return false;
	}
#else
	wstring wstrResult;
	wstrResult.clear();
	wchar_t *pwszXml = nullptr;
	if(m_pfnOperateDiskF)
	{
		pwszXml = m_pfnOperateDiskF(strXml.c_str());
		if(pwszXml)
		{
			wstrResult = pwszXml;
		}
	}
	if(!WStr2Str(wstrResult, strResult))
	{
		return false;
	}
#endif

#if SAVE2FILE
	SaveToFile(strResult);
	return true;
#else
	if(ParseData(strResult))
	{
		return true;
	}
#endif // SAVE2FILE

	return false;
}

#if SAVE2FILE

void CReadData::SaveToFile(const std::string& strXml) const
{
	FILE *pFile = nullptr;
	if(fopen_s(&pFile, "Invoice.xml", "w") == 0)
	{
		if(pFile)
		{
			fwrite(strXml.c_str(), sizeof(char), strXml.size(), pFile);
			fclose(pFile);
		}
	}
}

#else

bool CReadData::ParseInvoiceDetail(TiXmlNode* pNodeInvoiceDetail, CInvoiceItem::Pointer pInvoiceItem) const
{
	if(pNodeInvoiceDetail == nullptr)
	{
		return false;
	}

	string strTemp;
	if(!CUtility::GetAttributeText(pNodeInvoiceDetail, "count", strTemp))
	{
		return false;
	}
	TRACE("MxCount=%s\n", strTemp.c_str());
	
	int nCount = std::stoi(strTemp);
	if(nCount <= 0)
	{
		return false;
	}

	int i = 0;
	TiXmlNode* pNode = nullptr;
	for(TiXmlNode* pGroup = pNodeInvoiceDetail->FirstChild("group"); i < nCount && pGroup != nullptr; i++, pGroup = pGroup->NextSibling("group"))
	{
		pNode = pGroup->FirstChild("FPMXXH");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_Num, strTemp);
			TRACE("明细_序号=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("spmc");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_ProductName, strTemp);
			TRACE("明细_商品名称=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("spsl");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_ProductCnt, strTemp);
			TRACE("明细_商品数量=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("je");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_Sum, strTemp);
			TRACE("明细_金额=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("dj");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_Cost, strTemp);
			TRACE("明细_单价=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("se");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_TaxPrice, strTemp);
			TRACE("明细_税额=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("sl");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_TaxRate, strTemp);
			TRACE("明细_税率=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("ggxh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_Spec, strTemp);
			TRACE("明细_规格型号=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("dw");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_ProductUnit, strTemp);
			TRACE("明细_商品单位=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("spsm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_ProductItem, strTemp);
			TRACE("明细_商品税目=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("spbm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_FLBM, strTemp);
			TRACE("明细_分类编码=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("fphxz");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MX_FPHXZ, strTemp);
			TRACE("明细_FPHXZ=%s\n", strTemp.c_str());
		}
	}

	return true;
}

bool CReadData::ParseInvoice(TiXmlNode* pNodeInvoiceInfo, list<CInvoiceItem::Pointer>& lstInvoice) const
{
	string strTemp;
	if(!CUtility::GetAttributeText(pNodeInvoiceInfo, "count", strTemp))
	{
		return false;
	}
	TRACE("Count=%s\n", strTemp.c_str());

	int nCount = std::stoi(strTemp);
	if(nCount <= 0)
	{
		return false;
	}

	int i = 0;
	TiXmlNode* pNode = nullptr;
	for(TiXmlNode* pGroup = pNodeInvoiceInfo->FirstChild("group"); pGroup != nullptr && i < nCount; pGroup = pGroup->NextSibling("group"), i++)
	{
		if(CUtility::GetAttributeText(pGroup, "xh", strTemp))
		{
			if(std::stoi(strTemp) > nCount)
			{
				break;
			}
		}

		CInvoiceItem::Pointer pInvoiceItem = CInvoiceItem::Create();

		pNode = pGroup->FirstChild("fpdm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("发票代码=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::Code, strTemp);
		}

		strTemp = "007";
		TRACE("发票类型-原始数据=%s\n", strTemp);
		pInvoiceItem->AddField(CFieldMatch::SrcType, strTemp);

		TRACE("发票类型=%s\n", strTemp);
		if(strTemp == "007")
		{
			pInvoiceItem->AddField(CFieldMatch::ParseType, "增值税普票");
		}
		else if(strTemp == "004")
		{
			pInvoiceItem->AddField(CFieldMatch::ParseType, "增值税专票");
		}
		else
		{
			pInvoiceItem->AddField(CFieldMatch::ParseType, "未知发票类型");
		}
	
		pNode = pGroup->FirstChild("fphm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("发票号码=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::Number, strTemp);
		}

		pNode = pGroup->FirstChild("fpzt");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("发票状态=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::CancelStatus, strTemp);
		}

		pNode = pGroup->FirstChild("kpr");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("开票人=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::MakePerson, strTemp);
		}

		pNode = pGroup->FirstChild("CancelPerson");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("作废人=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::CancelPerson, strTemp);
		}

		string strDate;
		pNode = pGroup->FirstChild("kprq");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("开票日期=%s\n", strTemp.c_str());
			strDate = strTemp;
		}
		pNode = pGroup->FirstChild("kpsj");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			if(strTemp.size() == 6 && strDate.size() > 0)
			{
				boost::format _fmt("%s:%s:%s");
				_fmt % strTemp.substr(0, 2);
				_fmt % strTemp.substr(2, 2);
				_fmt % strTemp.substr(4, 2);
				strTemp = _fmt.str();
				strDate.append(strTemp);
				string strCancel;
				pInvoiceItem->GetValue(CFieldMatch::CancelStatus, strCancel);
				if(boost::algorithm::equals(strCancel, "0"))
				{
					pInvoiceItem->AddField(CFieldMatch::MakeDate, strDate);
					TRACE("开票时间=%s\n", strTemp.c_str());
				}
				else
				{
					pInvoiceItem->AddField(CFieldMatch::CancelDate, strDate);
					TRACE("作废时间=%s\n", strTemp.c_str());
				}
			}
		}

/*
		pNode = pGroup->FirstChild("zfrq");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::CancelDate, strTemp);
			TRACE("作废时间=%s\n", strTemp.c_str());
		}
*/

		pNode = pGroup->FirstChild("zyspmc");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("主要商品名称=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::ProductName, strTemp);
		}
		pNode = pGroup->FirstChild("ghdwmc");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("客户单位名称=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::CustomerName, strTemp);
		}

		pNode = pGroup->FirstChild("hjse");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			TRACE("税额合计=%s\n", strTemp.c_str());
			pInvoiceItem->AddField(CFieldMatch::TaxNumberSum, strTemp);
		}

		pNode = pGroup->FirstChild("hjje");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::PriceSum, strTemp);
			TRACE("合计金额=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("jshj");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::PriceTaxSum, strTemp);
			TRACE("价税合计=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("zhsl");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::TaxRate, strTemp);
			TRACE("税率=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("xhdwmc");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::SellerName, strTemp);
			TRACE("销方名称=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("xhdwdzdh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::SellerAddrTel, strTemp);
			TRACE("销方地址电话=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("ghdwmc");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::CustomerName, strTemp);
			TRACE("客户名称=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("ghdwyhzh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::CustomerBankNum, strTemp);
			TRACE("客户银行帐号=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("ghdwdzdh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::CustomerAddrTel, strTemp);
			TRACE("购方地址电话=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("ghdwsbh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::CustomerTaxCode, strTemp);
			TRACE("客户识别号=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("kpjh");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::MakeMachine, strTemp);
			TRACE("开票机号=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("skr");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::Receiver, strTemp);
			TRACE("收款人=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("fhr");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::Reviewer, strTemp);
			TRACE("复核人=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("spsm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::ProductItem, strTemp);
			TRACE("商品税目=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("bz");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::Remark, strTemp);
			TRACE("备注=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("yfpdm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::SrcCode, strTemp);
			TRACE("原发票代码=%s\n", strTemp.c_str());
		}

		pNode = pGroup->FirstChild("yfphm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::SrcNumber, strTemp);
			TRACE("原发票号码=%s\n", strTemp.c_str());
		}
		
		pNode = pGroup->FirstChild("yfphm");
		if(CUtility::GetNodeText(pNode, strTemp))
		{
			pInvoiceItem->AddField(CFieldMatch::NotificationNum, strTemp);
			TRACE("通知单编号=%s\n", strTemp.c_str());
		}

		if(ParseInvoiceDetail(pNode->FirstChild("fyxm"), pInvoiceItem))
		{
			lstInvoice.push_back(pInvoiceItem);
		}
		else if(ParseInvoiceDetail(pNode->FirstChild("qdxm"), pInvoiceItem))
		{
			lstInvoice.push_back(pInvoiceItem);
		}
	}

	return !lstInvoice.empty();
}

bool CReadData::ParseData(const string& strXml)
{
	if(strXml.empty())
	{
		return false;
	}

	TiXmlDocument xmlDoc;
	xmlDoc.Parse(strXml.c_str(), nullptr, TIXML_ENCODING_LEGACY);
	if(xmlDoc.Error())
	{
		TRACE("Xml Parse Error!----Line:%d----Desc:%s", xmlDoc.ErrorRow(), xmlDoc.ErrorDesc());
		return false;
	}

	TiXmlNode* pNodeBusiness = xmlDoc.RootElement();
	if(pNodeBusiness == nullptr)
	{
		return false;
	}

	string strComment, strId;
	CUtility::GetAttributeText(pNodeBusiness, string("comment"), strComment);/// 注释
	CUtility::GetAttributeText(pNodeBusiness, string("id"), strId);/// ID
	TRACE("Comment=%s, Id=%s\n", strComment.c_str(), strId.c_str());

	TiXmlNode* pNodeBody = pNodeBusiness->FirstChild("body");
	if(pNodeBody == nullptr)
	{
		return false;
	}
	string strYylxdm;/// 应用类型代码
	if(CUtility::GetAttributeText(pNodeBusiness, string("yylxdm"), strYylxdm))
	{
		TRACE("Yylxdm=%s\n", strYylxdm.c_str());
	}
	
	TiXmlNode* pNodeOutput = pNodeBody->FirstChild("output");
	if(pNodeOutput == nullptr)
	{
		return false;
	}

	TiXmlNode* pNodeReturnCode = pNodeOutput->FirstChild("returncode");
	string strReturnCode;
	if(CUtility::GetNodeText(pNodeReturnCode, strReturnCode))
	{
		TRACE("ReturnCode=%s\n", strReturnCode.c_str());
	}
	TiXmlNode* pNodeReturnMsg = pNodeOutput->FirstChild("returnmsg");
	string strReturnMsg;
	if(CUtility::GetNodeText(pNodeReturnMsg, strReturnMsg))
	{
		TRACE("ReturnMsg=%s'\n", strReturnMsg.c_str());
	}

	TiXmlNode* pNodeFpxx = pNodeOutput->FirstChild("fpxx");
	if(pNodeFpxx == nullptr)
	{
		return false;
	}

	list<CInvoiceItem::Pointer> lstResult;
	return ParseInvoice(pNodeFpxx, lstResult);
}
#endif
