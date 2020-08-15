#include "Converter.h"
Converter::Converter(const std::string& changeRule)
{
	std::ifstream fin(changeRule);
	std::string strTemp;
	while (std::getline(fin, strTemp))
	{
		std::istringstream sin(strTemp);
		std::string s1, s2;
		std::getline(sin, s1, ','); std::getline(sin, s2, ',');
		char ch = s1[0];
		switch (ch)
		{
		case 'A':
			FiledToNum[s1] = std::stoi(s2);
			NumToFiled[std::stoi(s2)] = s1;
			break;
		case 'S':
			ShapeToNum[s1] = std::stoi(s2);
			NumToShape[std::stoi(s2)] = s1;
			break;
		case'T':
			TypeToNum[s1] = std::stoi(s2);
			NumToType[std::stoi(s2)] = s1;
			break;
		default:
			break;
		}
	}
}

void Converter::saveFile(const std::string& fileName, const std::vector<J_ROIIndividual>& ROIIndividuals)
{
	std::ofstream fout;
	fout.open(fileName);
	for (auto it = ROIIndividuals.begin(); it != ROIIndividuals.end(); ++it)
	{
		fout << NumToFiled[it->ROI_Affiliation] << " " << NumToShape[it->ROI_Shape] << " " << NumToType[it->ROI_Type] << " " << it->vecCoordinate.size() << std::endl;
		for (auto vecIt = it->vecCoordinate.begin(); vecIt != it->vecCoordinate.end(); ++vecIt)
			fout << "(" << vecIt->x << ", " << vecIt->y << ") ";
		fout << std::endl;
	}
	fout.close();
}

int Converter::loadFile(const std::string& fileName, std::vector<J_ROIIndividual>& ROIindividuals)
{
	std::ifstream fin;
	fin.open(fileName);
	if (!fin) return 0;
	ROIindividuals.clear();
	std::string strLine;
	bool odd = true;
	J_ROIIndividual temp;
	while (std::getline(fin, strLine))
	{
		std::istringstream sin(strLine);
		if (odd)
		{
			std::string s1, s2, s3;
			sin >> s1 >> s2 >> s3;
			if (FiledToNum.find(s1) == FiledToNum.end() || ShapeToNum.find(s2) == ShapeToNum.end() || TypeToNum.find(s3) == TypeToNum.end())
				return 0;
			temp.ROI_Affiliation = FiledToNum[s1];
			temp.ROI_Shape = ShapeToNum[s2];
			temp.ROI_Type = TypeToNum[s3];
		}
		else
		{
			std::vector<cv::Point> points;
			std::string numStr;
			int x = 0, y = 0;
			for (int i = 0, len = strLine.size(); i < len; ++i)
			{
				char ch = strLine[i];
				if (ch == '(' || ch == ' ')
					continue;
				if (isdigit(ch))
					numStr += ch;
				else if (ch == ',')
				{
					x = std::stoi(numStr);
					numStr = "";
				}
				else if (ch == ')')
				{
					y = std::stoi(numStr);
					numStr = "";
					points.push_back(cv::Point(x, y));
				}
				else
					return 0;
			}
			temp.vecCoordinate = points;
			ROIindividuals.push_back(temp);
		}
		odd = !odd;
	}
	return 1;
}
