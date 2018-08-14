
// ReadTaxDiskDlg.cpp : 实现文件
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
<business id=\"FPCX\" comment=\"发票查询\">	\r\n\
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CReadTaxDiskDlg 对话框




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


// CReadTaxDiskDlg 消息处理程序

BOOL CReadTaxDiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CString strText = "<?xml version=\"1.0\" encoding=\"gbk\"?>";
	strText.Append("\r\n<business comment=\"税控盘信息查询\" id=\"SKPXXCX\">");
	strText.Append("\r\n<body yylxdm=\"1\">");
	strText.Append("\r\n<input><skpkl>88888888</skpkl></input></body></business>\r\n");

	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(INVOICE_QUERY_CMD/*strText*/);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CReadTaxDiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CReadTaxDiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReadTaxDiskDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
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
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText("成功"/*strOutput.c_str()*/);
		}
	}
	_readData.Close();

	GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
}
