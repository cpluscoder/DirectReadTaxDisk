#pragma once

#include <map>
#include <string>
#include <vector>

/*!
	�ֶ�ӳ���ϵ
	1 �ڴ�����
	2 XML�ڵ�
	3 �������ݿ��ֶ�
	4 �������ݿ��ֶ�
*/
class CFieldMatch
{
public:
	typedef enum _InvoiceFieldType
	{
		Code = 0,		///	��Ʊ����
		Number,			///	��Ʊ����
		SrcType,		///	��Ʊ����-ԭʼ����
		ParseType,		///	��Ʊ����
		MakeDate,		///	��Ʊ����
		MakePerson,		///	��Ʊ��
		CancelStatus,	///	����״̬
		CancelDate,		///	��������
		CancelPerson,	///	������
		TaxNumberSum,	///	˰��ϼ�
		PriceSum,		///	�ϼƽ��
		PriceTaxSum,	///	��˰�ϼ�
		TaxRate,		///	˰��
		ProductName,	///	��Ʒ����
		SellerName,		///	��������
		SellerAddrTel,	///	������ַ�绰
		SellerBankNum,	///	���������˺�
		CustomerName,	///	�ͻ�����
		CustomerBankNum,///	�ͻ������ʺ�
		CustomerAddrTel,///	������ַ�绰
		CustomerTaxCode,///	�ͻ�ʶ���
		MakeMachine,	///	��Ʊ����
		ComputerNum,	///	�������
		Receiver,		///	�տ���
		Reviewer,		///	������
		ProductItem,	///	��Ʒ˰Ŀ
		Remark,			///	��ע
		Cryptograph,	///	����
		SrcCode,		///	ԭ��Ʊ����
		SrcNumber,		///	ԭ��Ʊ����
		NotificationNum,///	֪ͨ�����

		MX_Num,			///	��ϸ_���
		MX_ProductName,	///	��ϸ_��Ʒ����
		MX_ProductCnt,	///	��ϸ_��Ʒ����
		MX_Sum,			///	��ϸ_���
		MX_Cost,		///	��ϸ_����
		MX_TaxPrice,	///	��ϸ_˰��
		MX_TaxRate,		///	��ϸ_˰��
		MX_Spec,		///	��ϸ_����ͺ�
		MX_ProductUnit,	///	��ϸ_��Ʒ��λ
		MX_ProductItem,	///	��ϸ_��Ʒ˰Ŀ
		MX_FLBM,		///	��ϸ_�������
		MX_FPHXZ,		///	��ϸ_FPHXZ

		UnknownField	///	δ֪�ֶ�����
	}InvoiceFieldType;

	static CFieldMatch * Instance(void) {
		static CFieldMatch obj;
		return &obj;
	}

	virtual ~CFieldMatch(void);

	///	�������ͻ�ȡXML�ڵ�����
	///	@param[in]	emNodeName	XML�ڵ�����
	///	@param[out]	emType		��Ʊ�ֶ��ڴ�����
	///	@retval		���ҳɹ�
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

	///	����ӳ���ϵ
	static void CreateMatch(void);

	static void Clear(void);

	static std::vector<std::string> s_vctXmlField;	///	XML�ڵ�����
	static std::map<std::string, InvoiceFieldType> s_mapXmlNodeName2Type;	/// XML�ڵ� ӳ�� ��Ʊ�ֶ��ڴ�����
	static std::map<std::string, InvoiceFieldType> s_mapHxZbField2Type;		/// ���ŷ�Ʊ�����ֶ�   ӳ�� �ֶ�����
	static std::map<std::string, InvoiceFieldType> s_mapHxMxField2Type;		/// ���ŷ�Ʊ��ϸ���ֶ� ӳ�� �ֶ�����
	static std::map<std::string, InvoiceFieldType> s_mapHxQdField2Type;		/// ���ŷ�Ʊ�嵥���ֶ� ӳ�� �ֶ�����
	static std::map<std::string, InvoiceFieldType> s_mapBwZbField2Type;		/// ������Ʊ�����ֶ�   ӳ�� �ֶ�����
	static std::map<std::string, InvoiceFieldType> s_mapBwMxField2Type;		/// ������Ʊ��ϸ���ֶ� ӳ�� �ֶ�����
	static std::map<std::string, InvoiceFieldType> s_mapBwQdField2Type;		/// ������Ʊ�嵥���ֶ� ӳ�� �ֶ�����
};
