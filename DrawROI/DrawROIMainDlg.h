#pragma once
#include "afxpropertygridctrl.h"
#include "Converter.h"
#include "afxwin.h"

// DrawROIMainDlg �Ի���

class DrawROIMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DrawROIMainDlg)

public:
	DrawROIMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DrawROIMainDlg();

// �Ի�������
	enum { IDD = IDD_DrawROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_filePath;		//�����ļ�·��
	CString m_imagePath;	//ͼ���ļ�·��
	CMFCPropertyGridCtrl m_propertyGrid;
	J_ROIBase m_ROIBase;
	J_ROIIndividual GetROIIndividual();	//���ݵ�ǰ�������ʼ��һ��ROI
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();

	std::vector<cv::Mat> m_vecImgSrc;	//ԭͼ�ĸ���������ͼ��

	float m_ratioHWofPicCtrl;			//Pic�ؼ��ĸ߿��
	//---------ͼ�����š��϶�-----------//
	void MatToCImage(cv::Mat &mat, CImage &cImage);
	void DisplayImage(cv::Mat img, UINT nID, CRect* pCRectImgPos);
	CRect m_cRectImgPos;		//��ʾͼ��ĵ�λ�����꣬������������ڶԻ����
	bool m_flagCanScale;		//1�����ţ�0��������
	bool m_flagCanDrag;			//1���϶���0�����϶�
	bool m_flagisDraging;		//1�϶��У�0
	bool m_flagDispDst;			//��ʾ���ͼ���־λ
	float m_scaleTime;			//�����ܱ���
	cv::Rect m_rectProZoneROI;  //��ʾ������ԭͼ�ϵ�ROI
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void initWithNewImage(cv::Mat& imgNew);	//����ȡ�µ�ͼ��ʱ�������в�����ʼ����
	int MyDisplayImage(bool flagWithEraseROI=FALSE);		//����������Ϣ��ʾͼ��
	CPoint m_oriPoint;			//�϶�ʱ����һ������
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	//Point�����Ļ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	//Point��ԶԻ���
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	//Point��ԶԻ���


	//---------��ROI���--------//
	int m_indexOfSelectedROI;		//ѡ�е�ROI
	bool m_flagIsModifying;			//ROI�����޸�flag
	//int m_indexOfModifyingROI;		//�����޸ĵ�ROI���
	std::vector<J_ROIIndividual> m_vecROI;

	bool m_flagCanDrawLine;
	bool m_flagIsDrawingLine;

	bool m_flagCanDrawRectangle;
	bool m_flagIsDrawingRectangle;

	bool m_flagCanDrawCircle;
	bool m_flagIsDrawingCircle;
	bool m_flagIsModifyCentre;		//ԲROI�޸ĵ����ַ�ʽ��1�ƶ�Բ�ģ�0�޸İ뾶

	bool m_flagCanDrawPencil;
	bool m_flagIsDrawingPencil;
	bool m_flagIsStopPencil;

	bool m_flagCanDrawPolygon;
	bool m_flagIsDrawingPolygon;
	int m_indexPtPolygon;

	afx_msg void OnBnClickedButtonDrawline();
	CRect GetImageOnScreen();		//���ͼ���������Ļ����ʾ����
	cv::Point GetPixelFromMouse(CPoint pt, bool isPtOnScreen = TRUE);	//���������Ļ������ת����ͼ���ϵ�����
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	//---------���Ա���ʾ���--------//
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);

	void UpdatePropetyGrid();		//����ROI����ʾ���б���
	afx_msg void OnBnClickedButtonDrawrectangle();
	afx_msg void OnStnClickedMfcpropertygrid1();
	afx_msg void OnBnClickedButtonDelete1();
	afx_msg void OnBnClickedButtonDeleteall();
	afx_msg void OnBnClickedButtonExportdata();
	afx_msg void OnBnClickedButtonImportdata2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDrawpolygon();
//	afx_msg void OnBnClickedButtonDrawpolygon2();
	afx_msg void OnBnClickedButtonDrawpencil();
	afx_msg void OnBnClickedButtonDrawcircle();
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSavedata();
//	afx_msg void OnBnClickedOpenimage();
	CComboBox m_comboAffiliation;
	afx_msg void OnCbnSelchangeComboAffiliation();
};
