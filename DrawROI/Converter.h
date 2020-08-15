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
	void saveFile(const std::string& fileName, const std::vector<J_ROIIndividual>& ROIIndividuals); // 将ROI向量存入名为fileName的文件中
	int loadFile(const std::string& fileName, std::vector<J_ROIIndividual>& ROIindividuals); //从名为fileName的文件中读取ROI向量
	Converter(const std::string& changeRule = "ChangeRule.csv"); // 构造函数传入名称数字对应表。Shape必须以S开头 Type必须以T开头， 照明方式以A开头
};