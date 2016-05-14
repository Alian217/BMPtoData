
// BMPtoDataDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define PROCESS_START 0
#define PROCESS_END   200

// CBMPtoDataDlg 对话框
class CBMPtoDataDlg : public CDialogEx
{
// 构造
public:
	CBMPtoDataDlg(CWnd* pParent = NULL);	// 标准构造函数
	char BmpNamePath[100];                     // BMP文件路径 
	int OpenSourceFileSuccess;
	char BmpDataPath[100];                     // 数据保存文件路径 
	CProgressCtrl *m_Progress;
	int restart;
	char FileNameBmp[100];
	char FileName[100];
	char FilePathSource[100];
	int Tool_Count( unsigned char data[500][500], int col, int row, int flag );

// 对话框数据
	enum { IDD = IDD_BMPTODATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
