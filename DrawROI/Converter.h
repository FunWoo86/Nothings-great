#pragma once
#include "ROIBase.h"
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string>

class Converter
{
private:
	std::unordered_map<std::string, int> ShapeToNum;
	std::unordered_map<int, std::string> NumToShape;
	std::unordered_map<std::string, int> TypeToNum;
	std::unordered_map<int, std::string> NumToType;
	std::unordered_map<std::string, int> FiledToNum;
	std::unordered_map<int, std::string> NumToFiled;
public:
	void saveFile(const std::string& fileName, const std::vector<J_ROIIndividual>& ROIIndividuals); // ��ROI����������ΪfileName���ļ���
	int loadFile(const std::string& fileName, std::vector<J_ROIIndividual>& ROIindividuals); //����ΪfileName���ļ��ж�ȡROI����
	Converter(const std::string& changeRule = "ChangeRule.csv"); // ���캯�������������ֶ�Ӧ��Shape������S��ͷ Type������T��ͷ�� ������ʽ��A��ͷ
};