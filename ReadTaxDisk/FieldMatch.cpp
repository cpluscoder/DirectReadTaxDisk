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
	{CFieldMatch::Code,				"��Ʊ����",			"FPDM",		"FPDM"},
	{CFieldMatch::Number,			"��Ʊ����",			"FPHM",		"FPHM"},
	{CFieldMatch::SrcType,			"��Ʊ����-ԭʼ����","FPZL",		"FPZL_DM"},
	{CFieldMatch::ParseType,		"��Ʊ����",			"",			""},
	{CFieldMatch::MakeDate,			"��Ʊ����",			"KPRQ",		"KPRQ"},
	{CFieldMatch::MakePerson,		"��Ʊ��",			"KPR",		"KPR"},
	{CFieldMatch::CancelStatus,		"����״̬",			"ZFBZ",		"FPZT"},
	{CFieldMatch::CancelDate,		"��������",			"ZFRQ",		"ZFRQ"},
	{CFieldMatch::CancelPerson,		"������",			"",			"ZFR"},
	{CFieldMatch::TaxNumberSum,		"˰��ϼ�",			"HJSE",		"SE"},
	{CFieldMatch::PriceSum,			"�ϼƽ��",			"HJJE",		"HJJE"},
	{CFieldMatch::PriceTaxSum,		"��˰�ϼ�",			"",			"JSHJ"},
	{CFieldMatch::TaxRate,			"˰��",				"SLV",		"SL"},
	{CFieldMatch::ProductName,		"��Ʒ����",			"ZYSPMC",	"ZYSPMC"},
	{CFieldMatch::SellerName,		"��������",			"XFMC",		"XF_NSRMC"},
	{CFieldMatch::SellerAddrTel,	"������ַ�绰",		"XFDZDH",	"XF_DZDH"},
	{CFieldMatch::SellerBankNum,	"���������˺�",		"XFYHZH",	"XF_YHZH"},
	{CFieldMatch::CustomerName,		"�ͻ�����",			"GFMC",		"GF_NSRMC"},
	{CFieldMatch::CustomerBankNum,	"�ͻ������ʺ�",		"GFYHZH",	"GF_YHZH"},
	{CFieldMatch::CustomerAddrTel,	"������ַ�绰",		"GFDZDH",	"GF_DZDH"},
	{CFieldMatch::CustomerTaxCode,	"�ͻ�ʶ���",		"GFSH",		"GF_NSRSBH"},
	{CFieldMatch::MakeMachine,		"��Ʊ����",			"KPJH",		"KPJH"},
	{CFieldMatch::ComputerNum,		"�������",			"JQBH",		""},
	{CFieldMatch::Receiver,			"�տ���",			"SKR",		"SKR"},
	{CFieldMatch::Reviewer,			"������",			"FHR",		"FHR"},
	{CFieldMatch::ProductItem,		"��Ʒ˰Ŀ",			"SPSM",		"SPSM"},
	{CFieldMatch::Remark,			"��ע",				"BZ",		"BZ"},
	{CFieldMatch::Cryptograph,		"����",				"MW",		""},
	{CFieldMatch::SrcCode,			"ԭ��Ʊ����",		"",			"YFPDM"},
	{CFieldMatch::SrcNumber,		"ԭ��Ʊ����",		"",			"YFPHM"},
	{CFieldMatch::NotificationNum,	"֪ͨ�����",		"",			"TZDBH"},

	/// ��ϸ�����ݣ�CFieldMatch::MX_Num��ʾ��ʼ��
	{CFieldMatch::MX_Num,			"��ϸ-���",		"FPMXXH",	"FPMXXH"},
	{CFieldMatch::MX_ProductName,	"��ϸ-��Ʒ����",	"SPMC",		"SPMC"},
	{CFieldMatch::MX_ProductCnt,	"��ϸ-��Ʒ����",	"SL",		"SPSL"},
	{CFieldMatch::MX_Sum,			"��ϸ-���",		"JE",		"JE"},
	{CFieldMatch::MX_Cost,			"��ϸ-����",		"DJ",		"SPDJ"},
	{CFieldMatch::MX_TaxPrice,		"��ϸ-˰��",		"SE",		"SE"},
	{CFieldMatch::MX_TaxRate,		"��ϸ-˰��",		"SLV",		"SL"},
	{CFieldMatch::MX_Spec,			"��ϸ-����ͺ�",	"GGXH",		"GGXH"},
	{CFieldMatch::MX_ProductUnit,	"��ϸ-��Ʒ��λ",	"JLDW",		"JLDW"},
	{CFieldMatch::MX_ProductItem,	"��ϸ-��Ʒ˰Ŀ",	"SPSM",		"SPSM"},
	{CFieldMatch::MX_FLBM,			"��ϸ-�������",	"FLBM",		"SCBM"},
	{CFieldMatch::MX_FPHXZ,			"��ϸ-FPHXZ",		"FPHXZ",	"FPHXZ"}

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
		if(InvoiceFieldMatch[i].strXmlNodeName != "��������")
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
