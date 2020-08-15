// DrawROIMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawROI.h"
#include "DrawROIMainDlg.h"
#include "afxdialogex.h"


// DrawROIMainDlg 对话框

IMPLEMENT_DYNAMIC(DrawROIMainDlg, CDialogEx)

DrawROIMainDlg::DrawROIMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(DrawROIMainDlg::IDD, pParent)
{

}

DrawROIMainDlg::~DrawROIMainDlg()
{
}

void DrawROIMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_propertyGrid);
	DDX_Control(pDX, IDC_COMBO_Affiliation, m_comboAffiliation);
}


BEGIN_MESSAGE_MAP(DrawROIMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DrawROIMainDlg::OnBnClickedButton1)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_DrawLine, &DrawROIMainDlg::OnBnClickedButtonDrawline)
	ON_WM_RBUTTONUP()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	
	ON_BN_CLICKED(IDC_BUTTON_DrawRectangle, &DrawROIMainDlg::OnBnClickedButtonDrawrectangle)
	ON_STN_CLICKED(IDC_MFCPROPERTYGRID1, &DrawROIMainDlg::OnStnClickedMfcpropertygrid1)
	ON_BN_CLICKED(IDC_BUTTON_Delete1, &DrawROIMainDlg::OnBnClickedButtonDelete1)
	ON_BN_CLICKED(IDC_BUTTON_DeleteAll, &DrawROIMainDlg::OnBnClickedButtonDeleteall)
	ON_BN_CLICKED(IDC_BUTTON_ExportData, &DrawROIMainDlg::OnBnClickedButtonExportdata)
	ON_BN_CLICKED(IDC_BUTTON_ImportData2, &DrawROIMainDlg::OnBnClickedButtonImportdata2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Modify, &DrawROIMainDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DrawPolygon, &DrawROIMainDlg::OnBnClickedButtonDrawpolygon)
//	ON_BN_CLICKED(IDC_BUTTON_DrawPolygon2, &DrawROIMainDlg::OnBnClickedButtonDrawpolygon2)
ON_BN_CLICKED(IDC_BUTTON_DrawPencil, &DrawROIMainDlg::OnBnClickedButtonDrawpencil)
ON_BN_CLICKED(IDC_BUTTON_DrawCircle, &DrawROIMainDlg::OnBnClickedButtonDrawcircle)
ON_WM_MBUTTONUP()
ON_WM_MBUTTONDOWN()
ON_BN_CLICKED(IDC_BUTTON_SaveData, &DrawROIMainDlg::OnBnClickedButtonSavedata)
//ON_BN_CLICKED(OpenImage, &DrawROIMainDlg::OnBnClickedOpenimage)
ON_CBN_SELCHANGE(IDC_COMBO_Affiliation, &DrawROIMainDlg::OnCbnSelchangeComboAffiliation)
END_MESSAGE_MAP()


// DrawROIMainDlg 消息处理程序
void DrawROIMainDlg::MatToCImage(cv::Mat &mat, CImage &cImage)
{
	/*****************************************************************
	** 函数名:  MatToCImage
	** 输入: mat,cImage
	** mat---输入的Mat矩阵
	** cImage---转换后的CImage图像
	** 输出: 无
	** 功能描述:将Opencv中的Mat图像矩阵转换成CImage图像
	** 限制：mat矩阵中的数据类型必须为uchar(0~255)类型（不是的话，则需转换成此类型）
	** 作者:谢世斌
	** 日期:2015.1.15
	** 修改: 谢世斌
	** 日期:  20150327
	** 版本
	****************************************************************/

	//create new CImage  
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();
	if (!cImage.IsNull())
	{
		cImage.Destroy(); //clear 
	}

	cImage.Create(width,
		height, //positive: left-bottom-up   or negative: left-top-down  
		8 * channels); //numbers of bits per pixel  
	if (1 == channels)
	{
		RGBQUAD* ColorTable;
		int MaxColors = 256;
		ColorTable = new RGBQUAD[MaxColors];
		cImage.GetColorTable(0, MaxColors, ColorTable);//这里是取得指针
		for (int i = 0; i<MaxColors; ++i)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			//BYTE和uchar一回事，但MFC中都用它
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cImage.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;
	}
	//copy values  
	uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //A pointer to the bitmap buffer  

	//The pitch is the distance, in bytes. represent the beginning of   
	// one bitmap line and the beginning of the next bitmap line  
	int step = cImage.GetPitch();

	for (int i = 0; i < height; ++i)
	{
		ps = (mat.ptr<uchar>(i));
		for (int j = 0; j < width; ++j)
		{
			if (channels == 1) //gray  
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if (channels == 3) //color  
			{
				for (int k = 0; k < 3; ++k)
				{
					*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
				}
			}
		}
	}

}
void DrawROIMainDlg::DisplayImage(cv::Mat img, UINT nID, CRect* pCRectImgPos)
{

	/*****************************************************************
	** 函数名:  DisplayImage
	** 输入: img,nID，pCRectImgPos
	** img---待显示Mat图像
	** nID---图像窗口ID
	** pCRectImgPos--CRect对象指针，用于保存显示图像的的位置坐标，其坐标是相对于对话框的,如果不需要保存图像位置，可缺省此参数的输入
	** 输出: 无
	** 功能描述:
	** 功能描述:将图像显示在指定ID的窗口
	** 全局变量:无
	** 调用模块:调用函数MatToCImage(img,cimg);
	** 限制：无
	** 作者:谢世斌
	** 日期:2015.1.19
	** 修改:
	** 日期:
	** 版本：
	****************************************************************/


	//获取用于显示图片窗口的位置
	CImage cimg;      //将Mat图像转换成CImage图像;
	MatToCImage(img, cimg);

	CWnd* pWnd = GetDlgItem(nID);
	CRect rect;
	//pWnd->GetClientRect(&rect);  //获取图像窗口的矩形大小，rect.top=0,rect.left=0;
	pWnd->GetWindowRect(&rect);//获取图像窗口相对于屏幕的位置
	ScreenToClient(rect); //将其转换成相对于对话框的位置

	//保持图像比例，让图像居中显示在图像窗口
	int img_width = cimg.GetWidth(), img_height = cimg.GetHeight();//图像宽度和高度
	int win_width = rect.Width(), win_height = rect.Height();  //图像窗口的宽度和高度
	float win_ratio = (float)win_width / win_height, img_ratio = (float)img_width / img_height;//图像窗口和图像的宽高比
	CRect CRectImgPos; //图像在对话框中的位置
	if (win_ratio>img_ratio) //如果图像窗口宽高比大于图像宽高比，则显示的高度保持不变，改变宽度
	{
		CRectImgPos.top = rect.top;
		CRectImgPos.bottom = rect.bottom;
		CRectImgPos.left = rect.left + (win_width - (img_ratio*win_height)) / 2;
		CRectImgPos.right = CRectImgPos.left + (img_ratio*win_height);
	}
	else if (win_ratio<img_ratio)//如果图像窗口宽高比小于图像宽高比，则显示的宽度保持不变，改变高度
	{
		CRectImgPos.top = rect.top + (win_height - (win_width / img_ratio)) / 2;
		CRectImgPos.bottom = CRectImgPos.top + (win_width / img_ratio);
		CRectImgPos.left = rect.left;
		CRectImgPos.right = rect.right;
	}
	else//如果图像窗口宽高比等于图像宽高比，则显示的矩形位置保持不变
	{
		CRectImgPos = rect;
	}

	//显示图片
	CDC* pDC = this->GetDC();	//获取设备环境	
	SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);//设置拉伸模式为保留彩色位图中的颜色
	cimg.Draw(pDC->m_hDC, CRectImgPos);  //显示图片
	ReleaseDC(pDC);//释放DC

	if (pCRectImgPos != NULL)//保存图像在对话框中的位置
	{
		*pCRectImgPos = CRectImgPos;
	}
}

BOOL DrawROIMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_flagisDraging = FALSE;	
	m_flagCanScale = FALSE;
	m_flagCanDrag = FALSE;

	m_flagIsModifying = FALSE;
	m_flagCanDrawLine = FALSE;
	m_flagIsDrawingLine = FALSE;
	m_flagCanDrawRectangle = FALSE;
	m_flagIsDrawingRectangle = FALSE;
	m_flagCanDrawCircle = FALSE;
	m_flagIsDrawingCircle = FALSE;
	m_flagCanDrawPencil = FALSE;
	m_flagIsDrawingPencil = FALSE;
	m_flagCanDrawPolygon = FALSE;
	m_flagIsDrawingPolygon = FALSE;
	m_rectProZoneROI.x = -1;
	m_indexOfSelectedROI = -1;
	HDITEM item;
	item.cxy = 120;
	item.mask = HDI_WIDTH;
	m_propertyGrid.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	
	GetDlgItem(IDC_MFCPROPERTYGRID1)->EnableWindow(TRUE);
	SetTimer(1, 500, NULL);

	CWnd* pWnd = GetDlgItem(IDC_PictureC);
	CRect rect;
	pWnd->GetWindowRect(&rect);//获取图像窗口相对于屏幕的位置
	float win_width = rect.Width(), win_height = rect.Height();  //图像窗口的宽度和高度
	m_ratioHWofPicCtrl = win_height / win_width;

	for (int i = 0; i < J_ROIBase::ALength; i++)
		m_comboAffiliation.AddString(m_ROIBase.m_affiliationName[i]);
	m_comboAffiliation.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void DrawROIMainDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString cstrImgPath;  //原始采集图象全路径
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Image Files (*.bmp,*.jpg,*.tif)|*.bmp;*.jpg;*.tif|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal() == IDOK){
		CString cstrFolderPath = dlgFile.GetPathName();
		std::string strTemp = CStringA(cstrFolderPath);
		cv::Mat src0 = cv::imread(strTemp, 1);
		if (!src0.empty())
		{
			initWithNewImage(src0);
			m_imagePath = cstrFolderPath;
			GetDlgItem(IDC_STATIC_PicName)->SetWindowTextW(m_imagePath);
		}
	}	
}


BOOL DrawROIMainDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd* pWnd = GetDlgItem(IDC_PictureC);
	CRect rect = GetImageOnScreen();
	if (m_flagCanScale == 1 && pt.x >= rect.left && pt.x <= rect.right && pt.y >= rect.top && pt.y <= rect.bottom)
	{		
		if (m_vecImgSrc[0].empty()) return 0;
		cv::Mat m_Image = m_vecImgSrc[0];	//初始原图

		//算出目前的总放大倍数
		float k = 1.2;
		if (zDelta > 0)
			m_scaleTime = m_scaleTime * k;
		if (zDelta < 0)
			m_scaleTime = m_scaleTime / k < 1 ? 1 : m_scaleTime / k;

		//限制最大放大倍数
		if (8 * m_Image.cols / m_scaleTime < rect.right - rect.left
			|| 8 * m_Image.rows / m_scaleTime < rect.bottom - rect.top)
			m_scaleTime = m_scaleTime / k;

		//计算出缩放的中心在初始图像上的坐标
		cv::Point CenterPt = GetPixelFromMouse(pt);
		int CenterX = CenterPt.x;
		int CenterY = CenterPt.y;

		//计算新的显示ROI
		m_rectProZoneROI.height = m_Image.rows / m_scaleTime;
		//m_rectProZoneROI.width = m_Image.cols / m_scaleTime;
		m_rectProZoneROI.width = m_rectProZoneROI.height/m_ratioHWofPicCtrl;
		//m_rectProZoneROI.height = m_Image.rows / m_scaleTime;
		m_rectProZoneROI.x = CenterX - (pt.x - rect.left) * m_rectProZoneROI.width / (rect.right - rect.left);
		m_rectProZoneROI.y = CenterY - (pt.y - rect.top) * m_rectProZoneROI.height / (rect.bottom - rect.top);
		if (m_rectProZoneROI.width > m_Image.cols)	m_rectProZoneROI.width = m_Image.cols;
		if (m_rectProZoneROI.height > m_Image.rows)	m_rectProZoneROI.height = m_Image.rows;
		if (m_rectProZoneROI.x < 0) m_rectProZoneROI.x = 0;
		if (m_rectProZoneROI.y < 0) m_rectProZoneROI.y = 0;
		if (m_rectProZoneROI.x + m_rectProZoneROI.width > m_Image.cols) m_rectProZoneROI.x = m_Image.cols - m_rectProZoneROI.width;
		if (m_rectProZoneROI.y + m_rectProZoneROI.height > m_Image.rows) m_rectProZoneROI.y = m_Image.rows - m_rectProZoneROI.height;
		if (m_rectProZoneROI.x < 0 || m_rectProZoneROI.y < 0 || m_rectProZoneROI.x >= m_Image.cols || m_rectProZoneROI.y >= m_Image.rows)
			return 0;
		if (m_rectProZoneROI.br().x < 0 || m_rectProZoneROI.br().y < 0 || m_rectProZoneROI.br().x > m_Image.cols || m_rectProZoneROI.br().y > m_Image.rows)
			return 0;
		if (m_rectProZoneROI.width <= 0 || m_rectProZoneROI.height <= 0)
			return 0;

		UpdateData(true);   //将控件的值更新至变量
		if (zDelta < 0)
		{
			CRect rect00;
			pWnd->GetWindowRect(&rect00);//获取图像窗口相对于屏幕的位置
			ScreenToClient(rect00); //将其转换成相对于对话框的位置
			InvalidateRect(rect00);  //使得图像窗口无效，重绘	
		}
		MyDisplayImage();
		ValidateRect(m_cRectImgPos);//使得图像位置有效，不重绘,显示图像
		
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
void DrawROIMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//显示鼠标在点的坐标
	if (m_rectProZoneROI.x >= 0)
	{
		if (point.x >= m_cRectImgPos.left && point.x <= m_cRectImgPos.right && point.y >= m_cRectImgPos.top && point.y <= m_cRectImgPos.bottom)
		{
			cv::Point pt = GetPixelFromMouse(point, FALSE);
			CString str;
			str.Format(_T("(%d, %d)"), pt.x, pt.y);
			GetDlgItem(IDC_STATIC_PIXELCOR)->SetWindowText(str);
		}
	}
	//画直线
	if (m_flagIsDrawingLine)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);
		
		//if (!m_flagIsModifying)
		//	m_vecROI[m_vecROI.size() - 1].vecCoordinate[1] = (ptEnd);
		//else
		m_vecROI[m_indexOfSelectedROI].vecCoordinate[1] = (ptEnd);
		MyDisplayImage(TRUE);
	}
	//画方
	else if (m_flagIsDrawingRectangle)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);

		//if (!m_flagIsModifying)
		//	m_vecROI[m_vecROI.size() - 1].vecCoordinate[1] = (ptEnd);
		//else
			m_vecROI[m_indexOfSelectedROI].vecCoordinate[1] = (ptEnd);
		MyDisplayImage(TRUE);
	}
	//画圆
	else if (m_flagIsDrawingCircle)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);
		int radius = m_ROIBase.CalcDistance(ptEnd, m_vecROI[m_vecROI.size() - 1].vecCoordinate[0]);
		if (!m_flagIsModifying)
			m_vecROI[m_indexOfSelectedROI].vecCoordinate[1].x = radius;
		else
		{
			if (!m_flagIsModifyCentre)
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[1].x = m_ROIBase.CalcDistance(ptEnd, m_vecROI[m_indexOfSelectedROI].vecCoordinate[0]);
			else
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[0] = (ptEnd);
		}
		MyDisplayImage(TRUE);
	}
	//任意区域
	else if (m_flagIsDrawingPencil)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);

		if ((!m_flagIsModifying) && 2 <= m_ROIBase.CalcDistance(m_vecROI[m_indexOfSelectedROI].vecCoordinate[m_vecROI[m_indexOfSelectedROI].vecCoordinate.size() - 1], ptEnd))
			m_vecROI[m_indexOfSelectedROI].vecCoordinate.push_back(ptEnd);
		else
			;
		MyDisplayImage(TRUE);
	}
	//多边形
	else if (m_flagIsDrawingPolygon)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);

		m_vecROI[m_indexOfSelectedROI].vecCoordinate[m_indexPtPolygon]=(ptEnd);
		if (m_flagIsModifying)	//修改模式下要保证最后一个点和一个点重合
		{
			if (m_indexPtPolygon == 0)
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[m_vecROI[m_indexOfSelectedROI].vecCoordinate.size()-1] = (ptEnd);
			//if (m_indexPtPolygon == m_vecROI[m_indexOfSelectedROI].vecCoordinate.size() - 1);
			//	m_vecROI[m_indexOfSelectedROI].vecCoordinate[0] = (ptEnd);
		}
		MyDisplayImage(TRUE);
	}
	else if (m_flagisDraging)
	{
		int dx, dy;	//拖动的实际像素距离
		dx = (point - m_oriPoint).cx * m_rectProZoneROI.width / m_cRectImgPos.Width();
		dy = (point - m_oriPoint).cy * m_rectProZoneROI.width / m_cRectImgPos.Width();
		m_oriPoint = point;
		{
			if (m_vecImgSrc[0].empty()) return;
			//确定新的显示ROI
			cv::Mat m_Image = m_vecImgSrc[0];
			int Px = m_rectProZoneROI.x;
			int Py = m_rectProZoneROI.y;
			Px -= dx;
			if (Px<0) Px = 0;
			if (Px + m_rectProZoneROI.width>m_Image.cols) Px = m_Image.cols - m_rectProZoneROI.width;
			Py -= dy;
			if (Py<0) Py = 0;
			if (Py + m_rectProZoneROI.height>m_Image.rows) Py = m_Image.rows - m_rectProZoneROI.height;
			m_rectProZoneROI.x = Px;
			m_rectProZoneROI.y = Py;
			UpdateData(true);   //将控件的值更新至变量

			MyDisplayImage();
			ValidateRect(m_cRectImgPos);//使得图像位置有效，不重绘,显示图像
			//MessageBox(_T("滚动！"));
		}
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}
void DrawROIMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	////点击图像区域来取消选择某一项
	if (m_rectProZoneROI.x >= 0 && m_flagCanScale)
	{
		if (m_indexOfSelectedROI >= 0 && point.x >= m_cRectImgPos.left && point.x <= m_cRectImgPos.right && point.y >= m_cRectImgPos.top && point.y <= m_cRectImgPos.bottom)
		{
			m_indexOfSelectedROI = -1;
			MyDisplayImage();
			GetDlgItem(IDC_BUTTON_DrawLine)->SetFocus();
		}
	}

	if (m_flagIsDrawingLine || m_flagIsDrawingRectangle || m_flagIsDrawingCircle)
	{
		//cv::Point ptEnd = GetPixelFromMouse(point,FALSE);

		//m_vecROI[indexT].vecCoordinate[1] = (ptEnd);
		if (m_flagIsDrawingRectangle)
		{
			int indexT = m_indexOfSelectedROI;
			m_ROIBase.makeRectangleLegal(m_vecROI[indexT].vecCoordinate[0], m_vecROI[indexT].vecCoordinate[1]);
			MyDisplayImage(TRUE);
		}
		m_flagIsDrawingLine = FALSE;
		m_flagIsDrawingRectangle = FALSE;
		m_flagIsDrawingCircle = FALSE;
		//左键松开进入修改模式
		if (!m_flagIsModifying)
		{
			m_flagIsModifying = TRUE;
			//m_indexOfModifyingROI = m_indexOfSelectedROI;
		}
	}
	else if (m_flagIsDrawingPencil)
	{
		cv::Point ptEnd = GetPixelFromMouse(point, FALSE);
		int indexT = m_indexOfSelectedROI;
		m_vecROI[indexT].vecCoordinate.push_back(ptEnd);
		m_flagIsDrawingPencil = FALSE;
		MyDisplayImage(TRUE);
		//左键松开进入修改模式
		if (!m_flagIsModifying)
		{
			m_flagIsStopPencil = TRUE;
		}
	}
	else if (m_flagCanDrawPolygon)
	{
		cv::Point ptStart = GetPixelFromMouse(point, FALSE);
		if (m_flagIsModifying)	//修改模式下，左键松开取消鼠标移动响应
		{
			m_flagIsDrawingPolygon = FALSE;
		}
		else                    //画ROI模式下，第一个点和其他点的区别
		{
			if (!m_flagIsDrawingPolygon)
			{
				J_ROIIndividual roiIndividual = GetROIIndividual();
				roiIndividual.ROI_Shape = J_ROIBase::SPolygon;
				roiIndividual.vecCoordinate.push_back(ptStart);
				roiIndividual.vecCoordinate.push_back(ptStart);
				m_vecROI.push_back(roiIndividual);
				m_indexOfSelectedROI = m_vecROI.size() - 1;
				m_indexPtPolygon = 1;
				m_flagIsDrawingPolygon = TRUE;
			}
			else
			{
				m_vecROI[m_indexOfSelectedROI].vecCoordinate.push_back(ptStart);
				m_indexPtPolygon++;
			}
			MyDisplayImage(TRUE);
		}
		
	}
	else if (m_flagCanDrag)
	{
		ReleaseCapture();
		m_flagisDraging = 0;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}
void DrawROIMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_flagCanDrawLine)
	{
		cv::Point ptStart = GetPixelFromMouse(point,FALSE);
		if (!m_flagIsModifying)
		{
			J_ROIIndividual roiIndividual = GetROIIndividual();
			roiIndividual.ROI_Shape = J_ROIBase::SLine;
			roiIndividual.vecCoordinate.push_back(ptStart);
			roiIndividual.vecCoordinate.push_back(ptStart);
			m_vecROI.push_back(roiIndividual);
			m_indexOfSelectedROI = m_vecROI.size() - 1;
		}
		else//修改模式下
		{
			cv::Point ptTL = m_vecROI[m_indexOfSelectedROI].vecCoordinate[0];
			cv::Point ptBR = m_vecROI[m_indexOfSelectedROI].vecCoordinate[1];
			////按鼠标点离哪个近，就动哪个点
			if (abs((ptStart - ptTL).y) < abs((ptStart-ptBR).y))
			{
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[1] = ptTL;
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[0] = ptBR;
			}
		}
		m_flagIsDrawingLine = TRUE;
	}
	else if (m_flagCanDrawRectangle)
	{
		cv::Point ptStart = GetPixelFromMouse(point, FALSE);
		if (!m_flagIsModifying)
		{
			J_ROIIndividual roiIndividual = GetROIIndividual();
			roiIndividual.ROI_Shape = J_ROIBase::SRectangle;
			roiIndividual.vecCoordinate.push_back(ptStart);
			roiIndividual.vecCoordinate.push_back(ptStart);
			m_vecROI.push_back(roiIndividual);
			m_indexOfSelectedROI = m_vecROI.size() - 1;
		}
		else//修改模式下
		{
			cv::Point ptTL = m_vecROI[m_indexOfSelectedROI].vecCoordinate[0];
			cv::Point ptBR = m_vecROI[m_indexOfSelectedROI].vecCoordinate[1];
			////按鼠标点离哪个近，就动哪个点
			if (abs((ptStart - ptTL).y) < abs((ptStart - ptBR).y))
			{
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[1] = ptTL;
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[0] = ptBR;
			}
			//m_indexOfSelectedROI = m_indexOfModifyingROI;
		}
		m_flagIsDrawingRectangle = TRUE;
	}
	else if (m_flagCanDrawCircle)
	{
		cv::Point ptStart = GetPixelFromMouse(point, FALSE);
		if (!m_flagIsModifying)
		{
			J_ROIIndividual roiIndividual = GetROIIndividual();
			roiIndividual.ROI_Shape = J_ROIBase::SCircle;
			roiIndividual.vecCoordinate.push_back(ptStart);
			roiIndividual.vecCoordinate.push_back(cv::Point(10,10));
			m_vecROI.push_back(roiIndividual);
			m_indexOfSelectedROI = m_vecROI.size() - 1;
		}
		else//修改模式下
		{
			cv::Point centre = m_vecROI[m_indexOfSelectedROI].vecCoordinate[0];
			int radius = m_vecROI[m_indexOfSelectedROI].vecCoordinate[1].x;
			////按鼠标点离哪个近，就动哪个点
			if (m_ROIBase.CalcDistance(centre, ptStart) < radius / 2)
				m_flagIsModifyCentre = TRUE;
			else
				m_flagIsModifyCentre = FALSE;
			//m_indexOfSelectedROI = m_indexOfModifyingROI;
		}
		m_flagIsDrawingCircle = TRUE;
	}
	else if (m_flagCanDrawPencil)
	{
		cv::Point ptStart = GetPixelFromMouse(point, FALSE);
		if (!m_flagIsStopPencil)
		{
			J_ROIIndividual roiIndividual = GetROIIndividual();
			roiIndividual.ROI_Shape = J_ROIBase::SCurve;
			roiIndividual.vecCoordinate.push_back(ptStart);
			roiIndividual.vecCoordinate.push_back(ptStart);
			m_vecROI.push_back(roiIndividual);
			m_indexOfSelectedROI = m_vecROI.size() - 1;
		}
		else
		{
			m_vecROI[m_vecROI.size() - 1].vecCoordinate.push_back(ptStart);
		}
		m_flagIsDrawingPencil = TRUE;
	}
	else if (m_flagCanDrawPolygon)
	{
		cv::Point ptStart = GetPixelFromMouse(point, FALSE);
		if (m_flagIsModifying)  //修改模式下，选中离点击点最近的顶点拖动
		{
			m_indexPtPolygon = 0;
			float minDist = m_ROIBase.CalcDistance(ptStart, m_vecROI[m_indexOfSelectedROI].vecCoordinate[0]);
			for (int jj = 1; jj < m_vecROI[m_indexOfSelectedROI].vecCoordinate.size(); jj++)
			{
				if (minDist>m_ROIBase.CalcDistance(ptStart, m_vecROI[m_indexOfSelectedROI].vecCoordinate[jj]))
				{
					m_indexPtPolygon = jj;
					minDist = m_ROIBase.CalcDistance(ptStart, m_vecROI[m_indexOfSelectedROI].vecCoordinate[jj]);
				}
			}
			m_flagIsDrawingPolygon = TRUE;
		}
	}
	else if (m_flagCanDrag)
	{
		CWnd* pWnd = GetDlgItem(IDC_PictureC);
		if (m_flagCanScale == 1 && m_scaleTime > 1.f && point.x >= m_cRectImgPos.left && point.x <= m_cRectImgPos.right && point.y >= m_cRectImgPos.top && point.y <= m_cRectImgPos.bottom)
			//if(nFlags == MK_LBUTTON )
		{
			SetCapture();
			m_flagisDraging = 1;
			m_oriPoint = point;
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}
int DrawROIMainDlg::MyDisplayImage(bool flagWithEraseROI)
{
	if (m_vecImgSrc.size()<1) return 1;
	
	{
		CWnd* pWnd = GetDlgItem(IDC_PictureC);
		CRect rect;
		pWnd->GetWindowRect(&rect);//获取图像窗口相对于屏幕的位置
		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;
		float win_ratio = (float)windowWidth / windowHeight, img_ratio = (float)m_rectProZoneROI.width / m_rectProZoneROI.height;//图像窗口和图像的宽高比
		
		CRect CRectImgPos; //图像在对话框中的位置
		if (win_ratio > img_ratio) //如果图像窗口宽高比大于图像宽高比，则显示的高度保持不变，改变宽度
		{
			CRectImgPos.top = rect.top;
			CRectImgPos.bottom = rect.bottom;
			CRectImgPos.left = rect.left + (windowWidth - (img_ratio*windowHeight)) / 2;
			CRectImgPos.right = CRectImgPos.left + (img_ratio*windowHeight);
		}
		else if (win_ratio < img_ratio)//如果图像窗口宽高比小于图像宽高比，则显示的宽度保持不变，改变高度
		{
			CRectImgPos.top = rect.top + (windowHeight - (windowWidth / img_ratio)) / 2;
			CRectImgPos.bottom = CRectImgPos.top + (windowWidth / img_ratio);
			CRectImgPos.left = rect.left;
			CRectImgPos.right = rect.right;
		}
		else//如果图像窗口宽高比等于图像宽高比，则显示的矩形位置保持不变
		{
			CRectImgPos = rect;
		}
		windowWidth = CRectImgPos.right - CRectImgPos.left;
		windowHeight = CRectImgPos.bottom - CRectImgPos.top;
		//选择适合大小的金字塔图像用于显示
		cv::Rect r0 = m_rectProZoneROI;	
		int imgIndex = 0;
		float tScale = 1.f;
		while (r0.width > windowWidth * 2 && r0.height > windowHeight * 2)
		{
			imgIndex++;
			r0.x /= 2;
			r0.y /= 2;
			r0.width /= 2;
			r0.height /= 2;
			tScale *= 2;
			if (imgIndex>=m_vecImgSrc.size()-1)
				break;
		}
		cv::Mat imgShow = m_vecImgSrc[imgIndex](r0).clone();
		//将生成的ROI也进行缩放然后画出来
		for (int i = 0; i < m_vecROI.size(); i++)
		{
			int iChannel = m_comboAffiliation.GetCurSel();
			if (iChannel != m_vecROI[i].ROI_Affiliation) continue;
			std::vector<cv::Point> vecPoints;
			vecPoints.reserve(m_vecROI[i].vecCoordinate.size());
			for (int j = 0; j < m_vecROI[i].vecCoordinate.size(); j++)
			{
				vecPoints.push_back(cv::Point(m_vecROI[i].vecCoordinate[j].x / tScale - r0.x,
					m_vecROI[i].vecCoordinate[j].y / tScale - r0.y));
			}

			cv::Scalar color(255, 0, 0);
			if (i == m_indexOfSelectedROI)
			{
				color = cv::Scalar(0, 255, 255);
			}
			switch (m_vecROI[i].ROI_Shape)
			{				
			case J_ROIBase::SLine:
				cv::line(imgShow, vecPoints[0], vecPoints[1], color, 2);
				break;
			case J_ROIBase::SRectangle:
				cv::rectangle(imgShow, vecPoints[0], vecPoints[1], color, 2);
				break;
			case J_ROIBase::SCircle:
				cv::circle(imgShow, vecPoints[0], m_vecROI[i].vecCoordinate[1].x / tScale, color, 2);
				break;
			case J_ROIBase::SCurve:
				for (int iDraw = 0; iDraw < vecPoints.size()-1;iDraw++)
					cv::line(imgShow, vecPoints[iDraw], vecPoints[iDraw+1], color, 2);
				break;
			case J_ROIBase::SPolygon:
				for (int iDraw = 0; iDraw < vecPoints.size() - 1; iDraw++)
					cv::line(imgShow, vecPoints[iDraw], vecPoints[iDraw + 1], color, 2);
				break;
			default:
				break;
			}
			if (i == m_indexOfSelectedROI && m_vecROI[i].ROI_Shape != J_ROIBase::SCurve)
			{
				for (int iP = 0; iP < vecPoints.size(); iP++)
				{
					cv::circle(imgShow, vecPoints[iP], 7, cv::Scalar(0, 0, 255), -1);
					if (m_vecROI[i].ROI_Shape == J_ROIBase::SCircle)
						break;						//圆只用画出一点圆心点即可
				}
			}
		}
		DisplayImage(imgShow, IDC_PictureC, &m_cRectImgPos);
		ValidateRect(m_cRectImgPos);//使得图像位置有效，不重绘,显示图像
	}
	return 0;
}
void DrawROIMainDlg::initWithNewImage(cv::Mat& src0)
{
	int deg = 7;
	m_vecImgSrc.clear();
	m_vecImgSrc.reserve(deg);
	for (int i = 0; i < deg; i++)
	{
		cv::Mat dst;
		if (i == 0)
			dst = src0;
		else
			cv::pyrDown(m_vecImgSrc[i - 1], dst);
		m_vecImgSrc.push_back(dst);
	}
	m_scaleTime = 1.f;
	m_rectProZoneROI = cv::Rect(0, 0, src0.cols, src0.rows);
	m_flagCanScale = TRUE;
	m_flagCanDrag = TRUE;
	m_flagisDraging = FALSE;
	MyDisplayImage();
}


CRect DrawROIMainDlg::GetImageOnScreen()
{
	CWnd* pWnd = GetDlgItem(IDC_PictureC);
	CRect rect;
	//pWnd->GetClientRect(&rect);  //获取图像窗口的矩形大小，rect.top=0,rect.left=0;
	pWnd->GetWindowRect(&rect);//获取图像窗口相对于屏幕的位置
	CRect rect2;
	pWnd->GetWindowRect(&rect2);  //获取图像窗口的矩形大小，rect.top=0,rect.left=0;
	ScreenToClient(rect2); //将其转换成相对于对话框的位置

	rect.top += m_cRectImgPos.top - rect2.top;		//算出显示图像在屏幕上的位置
	rect.bottom += m_cRectImgPos.bottom - rect2.bottom;
	rect.left += m_cRectImgPos.left - rect2.left;
	rect.right += m_cRectImgPos.right - rect2.right;
	return rect;
}
cv::Point DrawROIMainDlg::GetPixelFromMouse(CPoint pt, bool isPtOnScreen)
{
	CWnd* pWnd = GetDlgItem(IDC_PictureC);
	
	CRect rect = GetImageOnScreen();
	if (!isPtOnScreen)
		rect = m_cRectImgPos;
	int CenterX = m_rectProZoneROI.width * (pt.x - rect.left) / (rect.right - rect.left) + m_rectProZoneROI.x;
	int CenterY = m_rectProZoneROI.height * (pt.y - rect.top) / (rect.bottom - rect.top) + m_rectProZoneROI.y;

	return cv::Point(CenterX, CenterY);
}

void DrawROIMainDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//点击右键表示当前ROI画完或者修改完
	if ((m_flagIsDrawingLine == FALSE && m_flagCanDrawLine == TRUE)
		||(m_flagIsDrawingRectangle == FALSE && m_flagCanDrawRectangle == TRUE)
		|| (m_flagIsDrawingCircle == FALSE && m_flagCanDrawCircle == TRUE)
		|| (m_flagIsDrawingPencil == FALSE && m_flagCanDrawPencil == TRUE)
		|| (m_flagCanDrawPolygon == TRUE))
	{
		//根据当前的通道设置该ROI的通道
		int iChannel = m_comboAffiliation.GetCurSel();
		m_vecROI[m_indexOfSelectedROI].ROI_Affiliation = iChannel;

		m_flagCanDrawLine = FALSE;
		m_flagCanDrawRectangle = FALSE;
		m_flagCanDrawCircle = FALSE;

		if (m_flagCanDrawPencil)
		{
			m_vecROI[m_indexOfSelectedROI].vecCoordinate.push_back(m_vecROI[m_indexOfSelectedROI].vecCoordinate[0]);
			m_flagCanDrawPencil = FALSE;
		}
		if (m_flagCanDrawPolygon)
		{
			if (!m_flagIsModifying)
				m_vecROI[m_indexOfSelectedROI].vecCoordinate[m_indexPtPolygon] = m_vecROI[m_indexOfSelectedROI].vecCoordinate[0];
			m_flagCanDrawPolygon = FALSE;
			m_flagIsDrawingPolygon = FALSE;
		}
		m_flagCanScale = TRUE;
		m_flagCanDrag = TRUE;
		m_flagIsModifying = FALSE;
		m_indexOfSelectedROI = -1;
		UpdatePropetyGrid();
		MyDisplayImage();
		ClipCursor(NULL);

	}
	


	CDialogEx::OnRButtonUp(nFlags, point);
}

void DrawROIMainDlg::UpdatePropetyGrid()
{
	int iChannel = m_comboAffiliation.GetCurSel();

	m_propertyGrid.RemoveAll();
	m_propertyGrid.RedrawWindow();
	
	for (int i = 0; i < m_vecROI.size(); i++)
	{
		if (m_vecROI[i].ROI_Affiliation != iChannel) continue;
		CString strDescription;
		strDescription.Format(_T("%d"), i);
		std::vector<CString> shapeProperties = m_ROIBase.m_shapeProperties[m_vecROI[i].ROI_Shape];

		//ROI类型列，可修改
		CMFCPropertyGridProperty* pPropROIType = NULL;
		pPropROIType = new CMFCPropertyGridProperty(_T("Type"), m_ROIBase.m_typeName[m_vecROI[i].ROI_Type], _T("001"));
		for (int iType = 0; iType < J_ROIBase::TLength; iType++)
			pPropROIType->AddOption(m_ROIBase.m_typeName[iType]);
		
		switch (m_vecROI[i].ROI_Shape)
		{
		case J_ROIBase::SLine:
		case J_ROIBase::SRectangle:
		{
								 CMFCPropertyGridProperty* group = NULL;
								 group = new CMFCPropertyGridProperty(m_ROIBase.m_shapeName[m_vecROI[i].ROI_Shape]);
								 group->SetDescription(strDescription);
								 CMFCPropertyGridProperty* pProp1 = NULL;
								 pProp1 = new CMFCPropertyGridProperty(shapeProperties[0], (_variant_t)(int)m_vecROI[i].vecCoordinate[0].x, _T("001"));
								 
								 CMFCPropertyGridProperty* pProp2 = NULL;
								 pProp2 = new CMFCPropertyGridProperty(shapeProperties[1], (_variant_t)(int)m_vecROI[i].vecCoordinate[0].y, _T("002"));
								 CMFCPropertyGridProperty* pProp3 = NULL;
								 pProp3 = new CMFCPropertyGridProperty(shapeProperties[2], (_variant_t)(int)m_vecROI[i].vecCoordinate[1].x, _T("003"));
								 CMFCPropertyGridProperty* pProp4 = NULL;
								 pProp4 = new CMFCPropertyGridProperty(shapeProperties[3], (_variant_t)(int)m_vecROI[i].vecCoordinate[1].y, _T("004"));
								 //pProp11->SetValue((_variant_t)(int)100);
								 
								 group->AddSubItem(pProp1); group->AddSubItem(pProp2); group->AddSubItem(pProp3); group->AddSubItem(pProp4);
								 group->AddSubItem(pPropROIType); m_propertyGrid.AddProperty(group);
								 break;

		}
		case J_ROIBase::SCircle:
		{
								  CMFCPropertyGridProperty* group = NULL;
								  group = new CMFCPropertyGridProperty(m_ROIBase.m_shapeName[m_vecROI[i].ROI_Shape]);
								  group->SetDescription(strDescription);
								  CMFCPropertyGridProperty* pProp1 = NULL;
								  pProp1 = new CMFCPropertyGridProperty(shapeProperties[0], (_variant_t)(int)m_vecROI[i].vecCoordinate[0].x, _T("001"));
								  CMFCPropertyGridProperty* pProp2 = NULL;
								  pProp2 = new CMFCPropertyGridProperty(shapeProperties[1], (_variant_t)(int)m_vecROI[i].vecCoordinate[0].y, _T("001"));
								  CMFCPropertyGridProperty* pProp3 = NULL;
								  pProp3 = new CMFCPropertyGridProperty(shapeProperties[2], (_variant_t)(int)m_vecROI[i].vecCoordinate[1].x, _T("001"));

								  group->AddSubItem(pProp1); group->AddSubItem(pProp2); group->AddSubItem(pProp3);
								  group->AddSubItem(pPropROIType); m_propertyGrid.AddProperty(group);
								  break;

		}
		case J_ROIBase::SCurve:
		case J_ROIBase::SPolygon:
		{
									  CMFCPropertyGridProperty* group = NULL;
									  group = new CMFCPropertyGridProperty(m_ROIBase.m_shapeName[m_vecROI[i].ROI_Shape]);
									  group->SetDescription(strDescription);
									  CMFCPropertyGridProperty* pProp1 = NULL;
									  pProp1 = new CMFCPropertyGridProperty(shapeProperties[0], (_variant_t)(int)(m_vecROI[i].vecCoordinate.size()-1), _T("001"));
									  pProp1->AllowEdit(FALSE);
									  group->AddSubItem(pProp1); 
									  group->AddSubItem(pPropROIType);
									  m_propertyGrid.AddProperty(group);
									  break;

		}
		default:
			break;
		}

	}

}
//消息处理， lParam是Property属性项， wParam是ctrl的id
LRESULT DrawROIMainDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CString parentDescription = pProp->GetParent()->GetDescription();
	int indexROI = _ttoi(parentDescription);
	if (indexROI >= m_vecROI.size()) return 1;

	CString str = pProp->GetName();
	if (str == _T("Type"))
	{
		CString val = pProp->GetValue().bstrVal;
		for (int iType = 0; iType < J_ROIBase::TLength; iType++)
		{
			if (m_ROIBase.m_typeName[iType] == val)
			{
				m_vecROI[indexROI].ROI_Type = iType;
				break;
			}
		}

	}
	else
	{
		int val = pProp->GetValue().intVal;

		std::vector<CString> shapeProperties = m_ROIBase.m_shapeProperties[m_vecROI[indexROI].ROI_Shape];
		switch (m_vecROI[indexROI].ROI_Shape)
		{
		case J_ROIBase::SLine:
		case J_ROIBase::SRectangle:
		{
									  if (str == shapeProperties[0])
										  m_vecROI[indexROI].vecCoordinate[0].x = val;
									  else if (str == shapeProperties[1])
										  m_vecROI[indexROI].vecCoordinate[0].y = val;
									  else if (str == shapeProperties[2])
										  m_vecROI[indexROI].vecCoordinate[1].x = val;
									  else if (str == shapeProperties[3])
										  m_vecROI[indexROI].vecCoordinate[1].y = val;
									  break;

		}
		case J_ROIBase::SCircle:
		{
								   if (str == shapeProperties[0])
									   m_vecROI[indexROI].vecCoordinate[0].x = val;
								   else if (str == shapeProperties[1])
									   m_vecROI[indexROI].vecCoordinate[0].y = val;
								   else if (str == shapeProperties[2])
									   m_vecROI[indexROI].vecCoordinate[1].x = val;
								   break;

		}
		default:
			break;
		}
		if (m_vecROI[indexROI].ROI_Shape == J_ROIBase::SRectangle)
			m_ROIBase.makeRectangleLegal(m_vecROI[indexROI].vecCoordinate[0], m_vecROI[indexROI].vecCoordinate[1]);
		UpdatePropetyGrid();
		MyDisplayImage();
	}
	return 0;
}

void DrawROIMainDlg::OnBnClickedButtonDrawline()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_flagCanScale)
	{
		m_flagCanDrawLine = TRUE;
		m_flagCanScale = FALSE;
		m_flagCanDrag = FALSE;
		ClipCursor(&GetImageOnScreen());
	}
}
void DrawROIMainDlg::OnBnClickedButtonDrawrectangle()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_flagCanScale)
	{
		m_flagCanDrawRectangle = TRUE;
		m_flagCanScale = FALSE;
		m_flagCanDrag = FALSE;
		ClipCursor(&GetImageOnScreen());
	}
}


void DrawROIMainDlg::OnStnClickedMfcpropertygrid1()
{
	// TODO:  在此添加控件通知处理程序代码
	////获取当前鼠标的坐标
	CPoint pointC;
	GetCursorPos(&pointC);
	CWnd* pWnd = GetDlgItem(IDC_MFCPROPERTYGRID1);
	CRect rect;
	pWnd->GetWindowRect(&rect);//获取图像窗口相对于屏幕的位置
	pointC.x -= rect.left;
	pointC.y -= rect.top;

	////算出鼠标在哪个ROI里
	bool isFind = 0;
	int Count = m_propertyGrid.GetPropertyCount();
	int i = 0;
	for (; i < Count; i++)
	{
		CMFCPropertyGridProperty* pProp = m_propertyGrid.GetProperty(i);
		CRect cRect = pProp->GetRect();
		if (pointC.x >= cRect.left && pointC.x <= cRect.right && pointC.y >= cRect.top && pointC.y <= cRect.bottom)
		{
			isFind = 1;
			break;
		}
		
		for (int j = 0; j < pProp->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty* pChildProp = pProp->GetSubItem(j);
			CRect cRect = pChildProp->GetRect();
			if (pointC.x >= cRect.left && pointC.x <= cRect.right && pointC.y >= cRect.top && pointC.y <= cRect.bottom)
			{
				isFind = 1;
				break;
			}
		}
		if (isFind)
			break;
	}
	if (isFind)
	{
		CMFCPropertyGridProperty* pProp = m_propertyGrid.GetProperty(i);
		CString parentDescription = pProp->GetDescription();
		int indexROI = _ttoi(parentDescription);
		if (indexROI >= m_vecROI.size()) return;
		if (m_indexOfSelectedROI != indexROI)
		{	
			m_indexOfSelectedROI = indexROI;
			MyDisplayImage();
		}
		//m_indexOfModifyingROI = m_indexOfSelectedROI;
		//m_flagIsModifying = TRUE;

	}
	return;
}


void DrawROIMainDlg::OnBnClickedButtonDelete1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_indexOfSelectedROI >= 0 && m_indexOfSelectedROI < m_vecROI.size())
	{
		m_vecROI.erase(m_vecROI.begin() + m_indexOfSelectedROI);
		m_indexOfSelectedROI = -1;
		UpdatePropetyGrid();
		MyDisplayImage();
		
	}
}


void DrawROIMainDlg::OnBnClickedButtonDeleteall()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecROI.clear();
	m_indexOfSelectedROI = -1;
	UpdatePropetyGrid();
	MyDisplayImage();
	
}


void DrawROIMainDlg::OnBnClickedButtonExportdata()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = FALSE;		//是否打开(否则为保存)
	CString defaultDir = L"E:\\FileTest";	//默认打开的文件路径
	CString fileName = L"Export.txt";			//默认打开的文件名
	CString filter = L"文件 (*.txt)|*.txt||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, NULL, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	//openFileDlg.GetOFN().lpstrInitialDir = L"E:\\FileTest\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK) {
		if (openFileDlg.GetFileName().Find(_T(".txt")) == -1)
			MessageBox(_T("文件名有误！"));
		else
		{
			filePath = openFileDlg.GetPathName();
			std::string strFP = CStringA(filePath);
			Converter cvt;
			cvt.saveFile(strFP, m_vecROI);
			MessageBox(_T("导出成功"));

			m_filePath = filePath;
			GetDlgItem(IDC_STATIC_Filepath)->SetWindowTextW(m_filePath);
		}
		MyDisplayImage();	//防止box框涂抹了窗口图像控件
	}
	
	return;
}


void DrawROIMainDlg::OnBnClickedButtonImportdata2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString cstrImgPath;  //原始采集图象全路径
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Image Files (*.txt)|*.txt||"), NULL);
	if (dlgFile.DoModal() == IDOK){
		cstrImgPath = dlgFile.GetPathName();
		std::string strPath = CStringA(cstrImgPath);
		
		Converter cvt;
		m_vecROI.clear();
		if (cvt.loadFile(strPath, m_vecROI) == 1)
		{
			UpdatePropetyGrid();
			MyDisplayImage();
			m_filePath = cstrImgPath;
			GetDlgItem(IDC_STATIC_Filepath)->SetWindowTextW(m_filePath);
		}
		else
		{
			MessageBox(_T("文件有误"));
			MyDisplayImage();
		}
	}


	
	return;
}


void DrawROIMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString str;
	str.Format(_T("%d"), m_indexOfSelectedROI);
	switch (nIDEvent)//nIDEvent 为定时器事件ID，1，2，3  
	{
	case 1:
		GetDlgItem(IDC_STATIC_IndexSelected)->SetWindowText(str);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void DrawROIMainDlg::OnBnClickedButtonModify()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_indexOfSelectedROI >= 0 && m_indexOfSelectedROI < m_vecROI.size())
	{
		switch (m_vecROI[m_indexOfSelectedROI].ROI_Shape)
		{
		case J_ROIBase::SLine:
			OnBnClickedButtonDrawline();
			break;
		case J_ROIBase::SRectangle:
			OnBnClickedButtonDrawrectangle();
			break;
		case J_ROIBase::SCircle:
			OnBnClickedButtonDrawcircle();
			break;
		case J_ROIBase::SCurve:
			return;
		case J_ROIBase::SPolygon:
			OnBnClickedButtonDrawpolygon();
			break;
		default:
			break;
		}
		//m_indexOfModifyingROI = m_indexOfSelectedROI;
		m_flagIsModifying = TRUE;
	}
}


void DrawROIMainDlg::OnBnClickedButtonDrawpolygon()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_flagCanScale)
	{
		m_flagCanDrawPolygon = TRUE;
		m_flagCanScale = FALSE;
		m_flagCanDrag = FALSE;
		m_flagIsDrawingPolygon = FALSE;
		ClipCursor(&GetImageOnScreen());
	}

}


void DrawROIMainDlg::OnBnClickedButtonDrawpencil()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_flagCanScale)
	{
		m_flagCanDrawPencil = TRUE;
		m_flagCanScale = FALSE;
		m_flagCanDrag = FALSE;
		m_flagIsStopPencil = FALSE;
		ClipCursor(&GetImageOnScreen());
	}
}


void DrawROIMainDlg::OnBnClickedButtonDrawcircle()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_flagCanScale)
	{
		m_flagCanDrawCircle = TRUE;
		m_flagCanScale = FALSE;
		m_flagCanDrag = FALSE;
		ClipCursor(&GetImageOnScreen());
	}
}


void DrawROIMainDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnMButtonUp(nFlags, point);
}


void DrawROIMainDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//----如果在画多边形，中键弹起后删除一个点
	if (m_flagCanDrawPolygon && m_flagIsDrawingPolygon && !m_flagIsModifying)
	{
		int tSize = m_vecROI[m_indexOfSelectedROI].vecCoordinate.size();
		if (tSize > 2)
		{
			m_vecROI[m_indexOfSelectedROI].vecCoordinate.erase(m_vecROI[m_indexOfSelectedROI].vecCoordinate.begin() + tSize - 2);
			m_indexPtPolygon--;
		}
	}
	CDialogEx::OnMButtonDown(nFlags, point);
}


void DrawROIMainDlg::OnBnClickedButtonSavedata()
{
	// TODO:  在此添加控件通知处理程序代码
	std::string strPath = CStringA(m_filePath);
	if (m_filePath.Find(_T(".txt"))!=-1)
	{
		Converter cvt;
		cvt.saveFile(strPath, m_vecROI);
		MessageBox(_T("保存成功"));
		MyDisplayImage();	//防止box框涂抹了窗口图像控件
	}
	else
	{
		OnBnClickedButtonExportdata();
	}
}




void DrawROIMainDlg::OnCbnSelchangeComboAffiliation()
{
	// TODO:  在此添加控件通知处理程序代码
	//int iChannel = m_comboAffiliation.GetCurSel();
	UpdatePropetyGrid();
	MyDisplayImage();
}
J_ROIIndividual DrawROIMainDlg::GetROIIndividual()
{
	J_ROIIndividual jri;
	jri.ROI_Affiliation = m_comboAffiliation.GetCurSel();
	return jri;
}