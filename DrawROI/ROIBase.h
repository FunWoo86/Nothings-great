#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "stdafx.h"
struct J_ROIIndividual
{
	int ROI_Affiliation;	//ROI����������ͼ��
	int ROI_Shape;	//��״
	int ROI_Type;	//ROI�����ͣ������õģ�
	std::vector <cv::Point> vecCoordinate;	//����ROI�������
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
		temp.push_back(_T("��1")); temp.push_back(_T("��1")); temp.push_back(_T("��2")); temp.push_back(_T("��2"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Rectangle"));
		temp.clear();
		temp.push_back(_T("��1")); temp.push_back(_T("��1")); temp.push_back(_T("��2")); temp.push_back(_T("��2"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Circle"));
		temp.clear();
		temp.push_back(_T("Բ����")); temp.push_back(_T("Բ����")); temp.push_back(_T("�뾶"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Curve"));
		temp.clear();
		temp.push_back(_T("����"));
		m_shapeProperties.push_back(temp);

		m_shapeName.push_back(_T("Polygon"));
		temp.clear();
		temp.push_back(_T("����"));
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
		ADarkField = 0,	//����ͼ��
		ALightField = 1,//����ͼ��
		ATransField = 2,//͸�䳡ͼ��
		ALength,
	};
	enum{
		SLine = 0,		//ֱ��
		SRectangle = 1,	//����
		SCircle = 2,	//Բ��
		SCurve = 3,		//������״
		SPolygon = 4,	//�����
		SLength,
	};
	enum{
		TErase = 0,		//ROI�����ڲ�������
		TTemplate = 1,		//����
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
