
// ReadTaxDiskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadTaxDisk.h"
#include "ReadTaxDiskDlg.h"
#include "afxdialogex.h"

#include "ReadData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

#define INVOICE_QUERY_CMD	" \
<?xml version=\"1.0\" encoding=\"gbk\"?>	\r\n\
<business id=\"FPCX\" comment=\"��Ʊ��ѯ\">	\r\n\
	<body yylxdm=\"1\">	\r\n\
		<input>	\r\n\
			<nsrsbh>91320104302767442H</nsrsbh>	\r\n\
			<skpbh>539905411217</skpbh>	\r\n\
			<skpkl>88888888</skpkl>	\r\n\
			<fplxdm>007</fplxdm>	\r\n\
			<cxfs>1</cxfs>	\r\n\
			<cxtj>2017062820180629</cxtj>	\r\n\
			<cxlx>0</cxlx>	\r\n\
		</input>	\r\n\
	</body>	\r\n\
</business>"

//	<verify>75089301F85D1B02740C852A0320A85B3AA2BF2CCCC53FE0207E522CBF7D735D</verify>	\r\n\

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CReadTaxDiskDlg �Ի���




CReadTaxDiskDlg::CReadTaxDiskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadTaxDiskDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadTaxDiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReadTaxDiskDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CReadTaxDiskDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CReadTaxDiskDlg ��Ϣ�������

BOOL CReadTaxDiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CString strText = "<?xml version=\"1.0\" encoding=\"gbk\"?>";
	strText.Append("\r\n<business comment=\"˰������Ϣ��ѯ\" id=\"SKPXXCX\">");
	strText.Append("\r\n<body yylxdm=\"1\">");
	strText.Append("\r\n<input><skpkl>88888888</skpkl></input></body></business>\r\n");

	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(INVOICE_QUERY_CMD/*strText*/);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CReadTaxDiskDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReadTaxDiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CReadTaxDiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReadTaxDiskDlg::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText("");
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);

	CString strText;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strText);

	CReadData _readData;
	if(_readData.Open())
	{
		string strOutput;
		if(_readData.ReadTaxDisk(string(strText), strOutput))
		{
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText("�ɹ�"/*strOutput.c_str()*/);
		}
	}
	_readData.Close();

	GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
}
