#pragma once
#include "afxpropertygridctrl.h"
#include "Converter.h"
#include "afxwin.h"

// DrawROIMainDlg 对话框

class DrawROIMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DrawROIMainDlg)

public:
	DrawROIMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DrawROIMainDlg();

// 对话框数据
	enum { IDD = IDD_DrawROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_filePath;		//配置文件路径
	CString m_imagePath;	//图像文件路径
	CMFCPropertyGridCtrl m_propertyGrid;
	J_ROIBase m_ROIBase;
	J_ROIIndividual GetROIIndividual();	//根据当前的情况初始化一个ROI
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();

	std::vector<cv::Mat> m_vecImgSrc;	//原图的各级金字塔图像

	float m_ratioHWofPicCtrl;			//Pic控件的高宽比
	//---------图像缩放、拖动-----------//
	void MatToCImage(cv::Mat &mat, CImage &cImage);
	void DisplayImage(cv::Mat img, UINT nID, CRect* pCRectImgPos);
	CRect m_cRectImgPos;		//显示图像的的位置坐标，其坐标是相对于对话框的
	bool m_flagCanScale;		//1能缩放，0不能缩放
	bool m_flagCanDrag;			//1能拖动，0不能拖动
	bool m_flagisDraging;		//1拖动中，0
	bool m_flagDispDst;			//显示结果图像标志位
	float m_scaleTime;			//缩放总倍数
	cv::Rect m_rectProZoneROI;  //显示区域在原图上的ROI
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void initWithNewImage(cv::Mat& imgNew);	//当读取新的图像时，对所有参数初始化；
	int MyDisplayImage(bool flagWithEraseROI=FALSE);		//根据缩放信息显示图像；
	CPoint m_oriPoint;			//拖动时的上一点坐标
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	//Point相对屏幕
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	//Point相对对话框
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	//Point相对对话框


	//---------画ROI相关--------//
	int m_indexOfSelectedROI;		//选中的ROI
	bool m_flagIsModifying;			//ROI正在修改flag
	//int m_indexOfModifyingROI;		//正在修改的ROI序号
	std::vector<J_ROIIndividual> m_vecROI;

	bool m_flagCanDrawLine;
	bool m_flagIsDrawingLine;

	bool m_flagCanDrawRectangle;
	bool m_flagIsDrawingRectangle;

	bool m_flagCanDrawCircle;
	bool m_flagIsDrawingCircle;
	bool m_flagIsModifyCentre;		//圆ROI修改的两种方式，1移动圆心，0修改半径

	bool m_flagCanDrawPencil;
	bool m_flagIsDrawingPencil;
	bool m_flagIsStopPencil;

	bool m_flagCanDrawPolygon;
	bool m_flagIsDrawingPolygon;
	int m_indexPtPolygon;

	afx_msg void OnBnClickedButtonDrawline();
	CRect GetImageOnScreen();		//获得图像相对于屏幕的显示区域
	cv::Point GetPixelFromMouse(CPoint pt, bool isPtOnScreen = TRUE);	//从相对于屏幕的坐标转换成图像上的坐标
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	//---------属性表显示相关--------//
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);

	void UpdatePropetyGrid();		//画完ROI后显示在列表里
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
