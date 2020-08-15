#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "stdafx.h"
struct J_ROIIndividual
{
	int ROI_Affiliation;	//ROI是属于那张图的
	int ROI_Shape;	//形状
	int ROI_Type;	//ROI的类型（干嘛用的）
	std::vector <cv::Point> vecCoordinate;	//构成ROI的坐标点
	J_ROIIndividual()
	{
		ROI_Affiliation = 0;
		ROI_Type = 0;
	}
};
class J_ROIBase{
public:
	J_ROIBase()
	{
		m_shapeName.push_back(_T("Line"));
		std::vector<CString> temp;
		temp.push_back(_T("列1")); temp.push_back(_T("行1")); temp.push_back(_T("列2")); temp.push_back(_T("行2"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Rectangle"));
		temp.clear();
		temp.push_back(_T("列1")); temp.push_back(_T("行1")); temp.push_back(_T("列2")); temp.push_back(_T("行2"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Circle"));
		temp.clear();
		temp.push_back(_T("圆心列")); temp.push_back(_T("圆心行")); temp.push_back(_T("半径"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Curve"));
		temp.clear();
		temp.push_back(_T("点数"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Polygon"));
		temp.clear();
		temp.push_back(_T("点数"));
		m_shapeProperties.push_back(temp);


		m_typeName.push_back(_T("Erase"));
		m_typeName.push_back(_T("Template"));

		m_affiliationName.push_back(_T("DF"));
		m_affiliationName.push_back(_T("LF"));
		m_affiliationName.push_back(_T("TF"));
	};
	~J_ROIBase(){};
	std::vector<CString> m_shapeName;
	std::vector<CString> m_typeName;
	std::vector<CString> m_affiliationName;
	std::vector<std::vector<CString>> m_shapeProperties;
	enum{
		ADarkField = 0,	//暗场图像
		ALightField = 1,//明场图像
		ATransField = 2,//透射场图像
		ALength,
	};
	enum{
		SLine = 0,		//直线
		SRectangle = 1,	//方形
		SCircle = 2,	//圆形
		SCurve = 3,		//任意形状
		SPolygon = 4,	//多边形
		SLength,
	};
	enum{
		TErase = 0,		//ROI区域内部不处理
		TTemplate = 1,		//测试
		TLength,
	};
	void makeRectangleLegal(cv::Point &ptTL, cv::Point &ptBR)
	{
		if (ptTL.x > ptBR.x)
			std::swap(ptTL.x, ptBR.x);
		if (ptTL.y > ptBR.y)
			std::swap(ptTL.y, ptBR.y);
	}
	float CalcDistance(cv::Point pt1, cv::Point pt2)
	{
		float dx = pt1.x - pt2.x;
		float dy = pt1.y - pt2.y;
		return sqrt(dx*dx + dy*dy);
	}
};
