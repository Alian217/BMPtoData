
// BMPtoDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BMPtoData.h"
#include "BMPtoDataDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMPtoDataDlg 对话框




CBMPtoDataDlg::CBMPtoDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBMPtoDataDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMPtoDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, DemandValue);
}

BEGIN_MESSAGE_MAP(CBMPtoDataDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CBMPtoDataDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBMPtoDataDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT2, &CBMPtoDataDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CBMPtoDataDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, &CBMPtoDataDlg::OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT3, &CBMPtoDataDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CBMPtoDataDlg 消息处理程序

BOOL CBMPtoDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset( BmpNamePath, 0, sizeof(BmpNamePath) );
	memset( BmpDataPath, 0, sizeof(BmpDataPath) );

	//进度条
	m_Progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1); 
	m_Progress->SetRange(PROCESS_START,PROCESS_END); 
	m_Progress->SetPos(PROCESS_START);
	restart = 0;

	OpenSourceFileSuccess = 1;
	memset( FileNameBmp, 0, sizeof(FileNameBmp));
	memset( FileName, 0, sizeof(FileName));
	memset( FilePathSource, 0, sizeof(FilePathSource));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBMPtoDataDlg::OnPaint()
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
HCURSOR CBMPtoDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBMPtoDataDlg::OnBnClickedButton1()
{
	//这段可以考虑缩减
	BOOL isOpen = TRUE;					//是否打开(否则为保存)  
	CString defaultDir = L"C:\\";		//默认打开的文件路径  
	CString fileName = L"";			//默认打开的文件名  
	CString PromptError = L"Path error,or Pic error!";
	CString PromptNull = L"";
	CString filter = L"文件 (*.bmp)|*.bmp||";   //文件过虑的类型  
	CFileDialog openFileDlg( isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL );  
	openFileDlg.GetOFN().lpstrInitialDir = L"C:\\";  
	openFileDlg.GetOFN().lpstrInitialDir = defaultDir; 
	INT_PTR result = openFileDlg.DoModal();  
	CString filePath = defaultDir;  
	m_Progress->SetPos(PROCESS_START); 
	if(result == IDOK) 
	{  
		filePath = openFileDlg.GetPathName(); 
		defaultDir = filePath;
	}
	CWnd::SetDlgItemTextW( IDC_EDIT1, filePath ); 

	//将源文件路径保存在成员元素中
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,NULL,NULL,0,NULL);
	WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,BmpNamePath,dwNum,0,NULL);

	//显示图片到pic显示窗中，可以考虑自己成为一个成员函数
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;
	//根据路径载入图片  
	if( E_FAIL == image.Load(filePath) )
	{
		CWnd::SetDlgItemTextW( IDC_EDIT3, PromptError );
		OpenSourceFileSuccess = 0;
		CWnd* pWnd1 = GetDlgItem(ID_WINDOW_NEW);
		pWnd1->ShowWindow(FALSE);
		return;
	}
	else
	{
		CWnd::SetDlgItemTextW( IDC_EDIT3, PromptNull );
		OpenSourceFileSuccess = 1;
		int SourceLen = strlen(BmpNamePath);
		for( iCount=SourceLen-1; iCount>=0; iCount-- )
		{
			if( '\\' == BmpNamePath[iCount] )
				break;
		}
		strcpy( FileNameBmp, &BmpNamePath[iCount+1] );
		strncpy( FilePathSource, BmpNamePath, iCount+1);
		for( iCount=0; iCount<strlen(FileNameBmp); iCount++ )
		{
			if( '.' == FileNameBmp[iCount] )
				break;
		}
		strncpy(FileName,FileNameBmp,iCount);
		strncpy( &FilePathSource[strlen(FilePathSource)], FileName, iCount);
		strncpy( &FilePathSource[strlen(FilePathSource)], ".xls", 4);
	}
	//获取图片的宽 高度  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//获取Picture Control控件的大小  
	GetDlgItem(ID_WINDOW_NEW)->GetWindowRect(&rect);  
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);  
	//窗口移动到控件表示的区域  
	GetDlgItem(ID_WINDOW_NEW)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(ID_WINDOW_NEW);//获取控件句柄 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//获取picture的DC      
	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内       
	ReleaseDC(pDc); 
}


void CBMPtoDataDlg::OnBnClickedButton3()
{
	//这段可以考虑缩减
	BOOL isOpen = FALSE;				//是否打开(否则为保存)  
	CString defaultDir = L"C:\\";		//默认打开的文件路径  
	CString fileName = L"";			//默认打开的文件名  
	CString filter = L"文件 (*.xls;*.xlsx)|*.xls;*.xlsx||";   //文件过虑的类型  
	CFileDialog openFileDlg( isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL );  
	openFileDlg.GetOFN().lpstrInitialDir = L"C:\\";  
	openFileDlg.GetOFN().lpstrInitialDir = defaultDir;
	INT_PTR result = openFileDlg.DoModal();  
	CString filePath = defaultDir + FileName + L".xls";  
	if(result == IDOK) 
	{  
		filePath = openFileDlg.GetPathName(); 
		defaultDir = filePath;
	}
	else
	{
		filePath = (CString)(FilePathSource);
	}
	CWnd::SetDlgItemTextW( IDC_EDIT2, filePath );  

	//将源文件路径保存在成员元素中
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,NULL,NULL,0,NULL);
	WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,BmpDataPath,dwNum,0,NULL);
}


void CBMPtoDataDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CBMPtoDataDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CBMPtoDataDlg::OnBnClickedButton4()
{
	BITMAPFILEHEADER bf;			//BMP文件头结构体  
	BITMAPINFOHEADER bi;			//BMP信息头结构体  
	FILE* fp;						//指向文件的指针  
	RGBQUAD *ipRGB;					//
	DWORD LineByte,ImgSize;       
	unsigned char **Imgdata;   
	CString PromptTransSuccess = L"Transform Sucess!";
	CString PromptValueError = L"Damend Value Error!";
	int i,j;    
	char SetData[500][500] = {0};

	m_Progress->SetPos(PROCESS_START); 

	if( 1 == OpenSourceFileSuccess )
	{
		//打开文件  
		fp=fopen(BmpNamePath,"rb");    
		if(fp == NULL)
		{   
			exit(0);   
		}   

		//读取信息头、文件头  
		fread(&bf,sizeof(BITMAPFILEHEADER),1,fp); //把指针fp所指向的文件的头信息写入bf（地址）  
		fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);    
		LineByte=bi.biSizeImage / bi.biHeight; //计算位图的实际宽度并确保它为的倍数  
		ImgSize=(DWORD)LineByte*bi.biHeight;               
		Imgdata=new unsigned char*[bi.biHeight]; //声明一个指针数组  
		char WriteData[500][500] = {0};
		char HeadData[4][100] = {0};
		int pix = 0;
		unsigned char WriteData2[500][500] = {0};
		int row = 0,row2 = 0;
		int iCount = 0;

		//获取文件名
		char *FileName_flag = "File Name";
		char *TH_flag = "TH";
		int TH = 250;
		char *Width_flag = "Width";
		char *Height_flag = "Height";
		char *Count_flag = "Count";
		int count = 0;
		CString str;
		char Value[20] = {0};
		int iCK=0;
		int iCKres = 0;
		DemandValue.GetWindowTextW(str);
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,str,-1,NULL,NULL,0,NULL);
		WideCharToMultiByte(CP_OEMCP,NULL,str,-1,Value,dwNum,0,NULL);
		if (1==dwNum)
		{
			TH = 250;
		} 
		else
		{
			for ( iCK=0; iCK<dwNum-1; iCK++ )
			{
				if( 48>Value[iCK]||Value[iCK]>57 )
				{
					iCKres = 1;
					break;
				}
			}
			if( 1 == iCKres )
			{
				CWnd::SetDlgItemTextW( IDC_EDIT3, PromptValueError );
				fclose(fp);  
				return;
			}
			else
			{
				TH = atoi(Value);
			}
		}

		sprintf(HeadData[0],"\n\n\t%s\t%s\t%s\t%03d\n",FileName_flag,FileNameBmp,TH_flag,TH);
		sprintf(HeadData[1],"\t\t\t%s\t%03d\n",Width_flag,bi.biWidth);
		sprintf(HeadData[2],"\t\t\t%s\t%03d\n",Height_flag,bi.biHeight);

		if(bi.biBitCount==24)
		{    
			for ( i=bi.biHeight-1;i>=0;i-- )
			{
				Imgdata[i]=new unsigned char[(bi.biWidth*3+3)/4*4]; //每个数组元素也是一个指针数组  
			}
			for ( i=bi.biHeight-1;i>=0;i-- )
			{
				for(j=0;j<(bi.biWidth*3+3)/4*4;j++)    
				{
					fread(&Imgdata[i][j],1,1,fp);//每次只读取一个字节，存入数组
					//WriteData[i][j] = Imgdata[i][j];
					if( 0 == j%3 )
					{
						sprintf(&WriteData[i][row],"%3d,",Imgdata[i][j]);
						WriteData2[i][row2] = Imgdata[i][j];
						row = row + 4;
						row2 = row2 + 1;
					}
				}
				WriteData[i][row] = '\0';
				WriteData2[i][row2] = '\0';
				row = 0;
				row2 = 0;
			}
			count = Tool_Count(WriteData2,bi.biHeight,bi.biWidth,TH);
			sprintf(HeadData[3],"\t\t\t%s\t%03d\t",Count_flag,count);
		} 
		//TOOL_WriteDataToFile("bmpdata.txt", WriteData);
		//printf("灰度数据:\n");
		fclose(fp);  

		FILE *fk = NULL;
		fk=fopen(BmpDataPath,"w");
		if (fp != NULL) 
		{
			int p,g;

			for(p=0;p<4;p++)
			{
				fprintf(fk,"%s",&HeadData[p]);
			}
			for(p=0;p<bi.biHeight;p++)
			{
				for(g=0;g<bi.biWidth;g++)
				{
					fprintf(fk,"%3d\t",WriteData2[p][g]);
				}
				fprintf(fk,"\n\t\t\t\t\t");
			}
		}
		fclose(fk);  
		m_Progress->SetPos(PROCESS_END); 
		//CWnd::SetDlgItemTextW( IDC_EDIT3, PromptTransSuccess );
		MessageBox(PromptTransSuccess,_T("程序运行结果"),MB_OK);
	}
}

int CBMPtoDataDlg::Tool_Count( unsigned char data[500][500], int col, int row, int flag )
{
	int res = 0;
	int icol=0,irow=0;
	for ( icol=0; icol<col; icol++ )
	{
		for ( irow=0; irow<row; irow++ )
		{
			if( flag<=data[icol][irow] )
			{
				res = res + 1;
			}
		}
	}
	return res;
}



void CBMPtoDataDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
