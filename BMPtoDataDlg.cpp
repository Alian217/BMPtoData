
// BMPtoDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BMPtoData.h"
#include "BMPtoDataDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMPtoDataDlg �Ի���




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


// CBMPtoDataDlg ��Ϣ�������

BOOL CBMPtoDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset( BmpNamePath, 0, sizeof(BmpNamePath) );
	memset( BmpDataPath, 0, sizeof(BmpDataPath) );

	//������
	m_Progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1); 
	m_Progress->SetRange(PROCESS_START,PROCESS_END); 
	m_Progress->SetPos(PROCESS_START);
	restart = 0;

	OpenSourceFileSuccess = 1;
	memset( FileNameBmp, 0, sizeof(FileNameBmp));
	memset( FileName, 0, sizeof(FileName));
	memset( FilePathSource, 0, sizeof(FilePathSource));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBMPtoDataDlg::OnPaint()
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
HCURSOR CBMPtoDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBMPtoDataDlg::OnBnClickedButton1()
{
	//��ο��Կ�������
	BOOL isOpen = TRUE;					//�Ƿ��(����Ϊ����)  
	CString defaultDir = L"C:\\";		//Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = L"";			//Ĭ�ϴ򿪵��ļ���  
	CString PromptError = L"Path error,or Pic error!";
	CString PromptNull = L"";
	CString filter = L"�ļ� (*.bmp)|*.bmp||";   //�ļ����ǵ�����  
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

	//��Դ�ļ�·�������ڳ�ԱԪ����
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,NULL,NULL,0,NULL);
	WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,BmpNamePath,dwNum,0,NULL);

	//��ʾͼƬ��pic��ʾ���У����Կ����Լ���Ϊһ����Ա����
	int cx, cy;  
	CImage  image;  
	CRect   rect; 
	char pPath[100] = {0};
	int iCount = 0;
	//����·������ͼƬ  
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
	//��ȡͼƬ�Ŀ� �߶�  
	cx  = image.GetWidth();  
	cy  = image.GetHeight();       
	//��ȡPicture Control�ؼ��Ĵ�С  
	GetDlgItem(ID_WINDOW_NEW)->GetWindowRect(&rect);  
	//���ͻ���ѡ�е��ؼ���ʾ�ľ���������  
	ScreenToClient(&rect);  
	//�����ƶ����ؼ���ʾ������  
	GetDlgItem(ID_WINDOW_NEW)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(ID_WINDOW_NEW);//��ȡ�ؼ���� 
	pWnd->ShowWindow(TRUE);
	pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С       
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();//��ȡpicture��DC      
	image.Draw(pDc->m_hDC, rect);//��ͼƬ���Ƶ�picture��ʾ��������       
	ReleaseDC(pDc); 
}


void CBMPtoDataDlg::OnBnClickedButton3()
{
	//��ο��Կ�������
	BOOL isOpen = FALSE;				//�Ƿ��(����Ϊ����)  
	CString defaultDir = L"C:\\";		//Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = L"";			//Ĭ�ϴ򿪵��ļ���  
	CString filter = L"�ļ� (*.xls;*.xlsx)|*.xls;*.xlsx||";   //�ļ����ǵ�����  
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

	//��Դ�ļ�·�������ڳ�ԱԪ����
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,NULL,NULL,0,NULL);
	WideCharToMultiByte(CP_OEMCP,NULL,filePath,-1,BmpDataPath,dwNum,0,NULL);
}


void CBMPtoDataDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CBMPtoDataDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CBMPtoDataDlg::OnBnClickedButton4()
{
	BITMAPFILEHEADER bf;			//BMP�ļ�ͷ�ṹ��  
	BITMAPINFOHEADER bi;			//BMP��Ϣͷ�ṹ��  
	FILE* fp;						//ָ���ļ���ָ��  
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
		//���ļ�  
		fp=fopen(BmpNamePath,"rb");    
		if(fp == NULL)
		{   
			exit(0);   
		}   

		//��ȡ��Ϣͷ���ļ�ͷ  
		fread(&bf,sizeof(BITMAPFILEHEADER),1,fp); //��ָ��fp��ָ����ļ���ͷ��Ϣд��bf����ַ��  
		fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);    
		LineByte=bi.biSizeImage / bi.biHeight; //����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ�ı���  
		ImgSize=(DWORD)LineByte*bi.biHeight;               
		Imgdata=new unsigned char*[bi.biHeight]; //����һ��ָ������  
		char WriteData[500][500] = {0};
		char HeadData[4][100] = {0};
		int pix = 0;
		unsigned char WriteData2[500][500] = {0};
		int row = 0,row2 = 0;
		int iCount = 0;

		//��ȡ�ļ���
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
				Imgdata[i]=new unsigned char[(bi.biWidth*3+3)/4*4]; //ÿ������Ԫ��Ҳ��һ��ָ������  
			}
			for ( i=bi.biHeight-1;i>=0;i-- )
			{
				for(j=0;j<(bi.biWidth*3+3)/4*4;j++)    
				{
					fread(&Imgdata[i][j],1,1,fp);//ÿ��ֻ��ȡһ���ֽڣ���������
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
		//printf("�Ҷ�����:\n");
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
		MessageBox(PromptTransSuccess,_T("�������н��"),MB_OK);
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
