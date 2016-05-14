
// BMPtoDataDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define PROCESS_START 0
#define PROCESS_END   200

// CBMPtoDataDlg �Ի���
class CBMPtoDataDlg : public CDialogEx
{
// ����
public:
	CBMPtoDataDlg(CWnd* pParent = NULL);	// ��׼���캯��
	char BmpNamePath[100];                     // BMP�ļ�·�� 
	int OpenSourceFileSuccess;
	char BmpDataPath[100];                     // ���ݱ����ļ�·�� 
	CProgressCtrl *m_Progress;
	int restart;
	char FileNameBmp[100];
	char FileName[100];
	char FilePathSource[100];
	int Tool_Count( unsigned char data[500][500], int col, int row, int flag );

// �Ի�������
	enum { IDD = IDD_BMPTODATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit3();
	CEdit DemandValue;
};
