#include "FieldMatch.h"


using namespace std;

static const struct
{
	CFieldMatch::InvoiceFieldType emType;
	string strXmlNodeName;
	string strHxField;
	string strBwField;
}
InvoiceFieldMatch[] = {
	{CFieldMatch::Code,				"发票代码",			"FPDM",		"FPDM"},
	{CFieldMatch::Number,			"发票号码",			"FPHM",		"FPHM"},
	{CFieldMatch::SrcType,			"发票类型-原始数据","FPZL",		"FPZL_DM"},
	{CFieldMatch::ParseType,		"发票类型",			"",			""},
	{CFieldMatch::MakeDate,			"开票日期",			"KPRQ",		"KPRQ"},
	{CFieldMatch::MakePerson,		"开票人",			"KPR",		"KPR"},
	{CFieldMatch::CancelStatus,		"作废状态",			"ZFBZ",		"FPZT"},
	{CFieldMatch::CancelDate,		"作废日期",			"ZFRQ",		"ZFRQ"},
	{CFieldMatch::CancelPerson,		"作废人",			"",			"ZFR"},
	{CFieldMatch::TaxNumberSum,		"税额合计",			"HJSE",		"SE"},
	{CFieldMatch::PriceSum,			"合计金额",			"HJJE",		"HJJE"},
	{CFieldMatch::PriceTaxSum,		"价税合计",			"",			"JSHJ"},
	{CFieldMatch::TaxRate,			"税率",				"SLV",		"SL"},
	{CFieldMatch::ProductName,		"商品名称",			"ZYSPMC",	"ZYSPMC"},
	{CFieldMatch::SellerName,		"销方名称",			"XFMC",		"XF_NSRMC"},
	{CFieldMatch::SellerAddrTel,	"销方地址电话",		"XFDZDH",	"XF_DZDH"},
	{CFieldMatch::SellerBankNum,	"销方银行账号",		"XFYHZH",	"XF_YHZH"},
	{CFieldMatch::CustomerName,		"客户名称",			"GFMC",		"GF_NSRMC"},
	{CFieldMatch::CustomerBankNum,	"客户银行帐号",		"GFYHZH",	"GF_YHZH"},
	{CFieldMatch::CustomerAddrTel,	"购方地址电话",		"GFDZDH",	"GF_DZDH"},
	{CFieldMatch::CustomerTaxCode,	"客户识别号",		"GFSH",		"GF_NSRSBH"},
	{CFieldMatch::MakeMachine,		"开票机号",			"KPJH",		"KPJH"},
	{CFieldMatch::ComputerNum,		"机器编号",			"JQBH",		""},
	{CFieldMatch::Receiver,			"收款人",			"SKR",		"SKR"},
	{CFieldMatch::Reviewer,			"复核人",			"FHR",		"FHR"},
	{CFieldMatch::ProductItem,		"商品税目",			"SPSM",		"SPSM"},
	{CFieldMatch::Remark,			"备注",				"BZ",		"BZ"},
	{CFieldMatch::Cryptograph,		"密文",				"MW",		""},
	{CFieldMatch::SrcCode,			"原发票代码",		"",			"YFPDM"},
	{CFieldMatch::SrcNumber,		"原发票号码",		"",			"YFPHM"},
	{CFieldMatch::NotificationNum,	"通知单编号",		"",			"TZDBH"},

	/// 明细表数据（CFieldMatch::MX_Num表示开始）
	{CFieldMatch::MX_Num,			"明细-序号",		"FPMXXH",	"FPMXXH"},
	{CFieldMatch::MX_ProductName,	"明细-商品名称",	"SPMC",		"SPMC"},
	{CFieldMatch::MX_ProductCnt,	"明细-商品数量",	"SL",		"SPSL"},
	{CFieldMatch::MX_Sum,			"明细-金额",		"JE",		"JE"},
	{CFieldMatch::MX_Cost,			"明细-单价",		"DJ",		"SPDJ"},
	{CFieldMatch::MX_TaxPrice,		"明细-税额",		"SE",		"SE"},
	{CFieldMatch::MX_TaxRate,		"明细-税率",		"SLV",		"SL"},
	{CFieldMatch::MX_Spec,			"明细-规格型号",	"GGXH",		"GGXH"},
	{CFieldMatch::MX_ProductUnit,	"明细-商品单位",	"JLDW",		"JLDW"},
	{CFieldMatch::MX_ProductItem,	"明细-商品税目",	"SPSM",		"SPSM"},
	{CFieldMatch::MX_FLBM,			"明细-分类编码",	"FLBM",		"SCBM"},
	{CFieldMatch::MX_FPHXZ,			"明细-FPHXZ",		"FPHXZ",	"FPHXZ"}

};

vector<string> CFieldMatch::s_vctXmlField;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapXmlNodeName2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapHxZbField2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapHxMxField2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapHxQdField2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapBwZbField2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapBwMxField2Type;
map<string, CFieldMatch::InvoiceFieldType> CFieldMatch::s_mapBwQdField2Type;


CFieldMatch::CFieldMatch(void)
{
	Clear();
	CreateMatch();
}

CFieldMatch::~CFieldMatch(void)
{
	Clear();
}

void CFieldMatch::Clear(void)
{
	s_vctXmlField.clear();
	s_mapXmlNodeName2Type.clear();

	s_mapHxZbField2Type.clear();
	s_mapHxMxField2Type.clear();
	s_mapHxQdField2Type.clear();
	s_mapBwZbField2Type.clear();
	s_mapBwMxField2Type.clear();
	s_mapBwQdField2Type.clear();
}

void CFieldMatch::CreateMatch(void)
{
	for(int i = 0; i < UnknownField; i++)
	{
		if(InvoiceFieldMatch[i].strXmlNodeName != "销方名称")
		{
			s_vctXmlField.push_back(InvoiceFieldMatch[i].strXmlNodeName);
		}
		
		s_mapXmlNodeName2Type[InvoiceFieldMatch[i].strXmlNodeName] = InvoiceFieldMatch[i].emType;

		if(i < CFieldMatch::MX_Num)
		{
			if(!InvoiceFieldMatch[i].strHxField.empty())
			{
				s_mapHxZbField2Type[InvoiceFieldMatch[i].strHxField] = InvoiceFieldMatch[i].emType;
			}
			if(!InvoiceFieldMatch[i].strBwField.empty())
			{
				s_mapBwZbField2Type[InvoiceFieldMatch[i].strBwField] = InvoiceFieldMatch[i].emType;
			}
		}
		else
		{
			if(!InvoiceFieldMatch[i].strHxField.empty())
			{
				s_mapHxMxField2Type[InvoiceFieldMatch[i].strHxField] = InvoiceFieldMatch[i].emType;
				s_mapHxQdField2Type[InvoiceFieldMatch[i].strHxField] = InvoiceFieldMatch[i].emType;
			}
			if(!InvoiceFieldMatch[i].strBwField.empty())
			{
				s_mapBwMxField2Type[InvoiceFieldMatch[i].strBwField] = InvoiceFieldMatch[i].emType;
				s_mapBwQdField2Type[InvoiceFieldMatch[i].strBwField] = InvoiceFieldMatch[i].emType;
			}
		}
	}
}

bool CFieldMatch::GetTypeByHxZbField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapHxZbField2Type.find(strField);
	if(iter != s_mapHxZbField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}

bool CFieldMatch::GetTypeByHxMxField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapHxMxField2Type.find(strField);
	if(iter != s_mapHxMxField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}

bool CFieldMatch::GetTypeByHxQdField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapHxQdField2Type.find(strField);
	if(iter != s_mapHxQdField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}


bool CFieldMatch::GetTypeByBwZbField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapBwZbField2Type.find(strField);
	if(iter != s_mapBwZbField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}

bool CFieldMatch::GetTypeByBwMxField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapBwMxField2Type.find(strField);
	if(iter != s_mapBwMxField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}

bool CFieldMatch::GetTypeByBwQdField(const string &strField, InvoiceFieldType &emType)
{
	if(strField.empty())
	{
		return false;
	}

	auto iter = s_mapBwQdField2Type.find(strField);
	if(iter != s_mapBwQdField2Type.end())
	{
		emType = iter->second;
		return true;
	}

	return false;
}
