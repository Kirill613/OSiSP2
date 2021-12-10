#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class Table {
public:
	static void LoadDataFromFile(const char * filePath);
	static void Draw(HDC hdc, RECT rect);
private:
	static std::vector<std::vector<std::string>> values;
	static int columnCount;
	static int rowCount;
};