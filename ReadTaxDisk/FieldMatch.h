#pragma once

#include <map>
#include <string>
#include <vector>

/*!
	字段映射关系
	1 内存索引
	2 XML节点
	3 航信数据库字段
	4 百旺数据库字段
*/
class CFieldMatch
{
public:
	typedef enum _InvoiceFieldType
	{
		Code = 0,		///	发票代码
		Number,			///	发票号码
		SrcType,		///	发票类型-原始数据
		ParseType,		///	发票类型
		MakeDate,		///	开票日期
		MakePerson,		///	开票人
		CancelStatus,	///	作废状态
		CancelDate,		///	作废日期
		CancelPerson,	///	作废人
		TaxNumberSum,	///	税额合计
		PriceSum,		///	合计金额
		PriceTaxSum,	///	价税合计
		TaxRate,		///	税率
		ProductName,	///	商品名称
		SellerName,		///	销方名称
		SellerAddrTel,	///	销方地址电话
		SellerBankNum,	///	销方银行账号
		CustomerName,	///	客户名称
		CustomerBankNum,///	客户银行帐号
		CustomerAddrTel,///	购方地址电话
		CustomerTaxCode,///	客户识别号
		MakeMachine,	///	开票机号
		ComputerNum,	///	机器编号
		Receiver,		///	收款人
		Reviewer,		///	复核人
		ProductItem,	///	商品税目
		Remark,			///	备注
		Cryptograph,	///	密文
		SrcCode,		///	原发票代码
		SrcNumber,		///	原发票号码
		NotificationNum,///	通知单编号

		MX_Num,			///	明细_序号
		MX_ProductName,	///	明细_商品名称
		MX_ProductCnt,	///	明细_商品数量
		MX_Sum,			///	明细_金额
		MX_Cost,		///	明细_单价
		MX_TaxPrice,	///	明细_税额
		MX_TaxRate,		///	明细_税率
		MX_Spec,		///	明细_规格型号
		MX_ProductUnit,	///	明细_商品单位
		MX_ProductItem,	///	明细_商品税目
		MX_FLBM,		///	明细_分类编码
		MX_FPHXZ,		///	明细_FPHXZ

		UnknownField	///	未知字段类型
	}InvoiceFieldType;

	static CFieldMatch * Instance(void) {
		static CFieldMatch obj;
		return &obj;
	}

	virtual ~CFieldMatch(void);

	///	根据类型获取XML节点名称
	///	@param[in]	emNodeName	XML节点名称
	///	@param[out]	emType		发票字段内存索引
	///	@retval		查找成功
	static bool GetTypeByXmlNodeName(const std::string &strXmlNodeName, InvoiceFieldType &emType) {
		auto iter = s_mapXmlNodeName2Type.find(strXmlNodeName);
		if(iter != s_mapXmlNodeName2Type.end()) {
			emType = iter->second;
			return true;
		}
		return false;
	}

	static bool GetTypeByHxZbField(const std::string &strField, InvoiceFieldType &emType);

	static bool GetTypeByHxMxField(const std::string &strField, InvoiceFieldType &emType);

	static bool GetTypeByHxQdField(const std::string &strField, InvoiceFieldType &emType);

	static bool GetTypeByBwZbField(const std::string &strBwField, InvoiceFieldType &emType);

	static bool GetTypeByBwMxField(const std::string &strField, InvoiceFieldType &emType);

	static bool GetTypeByBwQdField(const std::string &strField, InvoiceFieldType &emType);

	static const std::vector<std::string> &GetXmlNodeVector(void) { return s_vctXmlField; }

protected:
	explicit CFieldMatch(void);

	///	创建映射关系
	static void CreateMatch(void);

	static void Clear(void);

	static std::vector<std::string> s_vctXmlField;	///	XML节点名称
	static std::map<std::string, InvoiceFieldType> s_mapXmlNodeName2Type;	/// XML节点 映射 发票字段内存索引
	static std::map<std::string, InvoiceFieldType> s_mapHxZbField2Type;		/// 航信发票主表字段   映射 字段类型
	static std::map<std::string, InvoiceFieldType> s_mapHxMxField2Type;		/// 航信发票明细表字段 映射 字段类型
	static std::map<std::string, InvoiceFieldType> s_mapHxQdField2Type;		/// 航信发票清单表字段 映射 字段类型
	static std::map<std::string, InvoiceFieldType> s_mapBwZbField2Type;		/// 百旺发票主表字段   映射 字段类型
	static std::map<std::string, InvoiceFieldType> s_mapBwMxField2Type;		/// 百旺发票明细表字段 映射 字段类型
	static std::map<std::string, InvoiceFieldType> s_mapBwQdField2Type;		/// 百旺发票清单表字段 映射 字段类型
};
