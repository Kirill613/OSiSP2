#include "Table.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::vector<std::string>> Table::values;
int Table::rowCount;
int Table::columnCount;

void Table::LoadDataFromFile(const char* filePath) {
	std::ifstream fin;
	fin.open(filePath);

	std::string line;
	while (std::getline(fin, line)) {
		std::vector<std::string> lineVec;
		std::stringstream lineStr(line);
		std::string cell;
		while (std::getline(lineStr, cell, ';')) {
			lineVec.push_back(cell);
		}
		values.push_back(lineVec);
	}

	rowCount = values.size();
	if (rowCount) {
		columnCount = values[0].size();
	}

	fin.close();
}

void Table::Draw(HDC hdc, RECT clientRect) {

	const int horizontalPadding = 4;
	const int separatorWidth = 4;
	const int lineSpacing = 2;

	HPEN pen = CreatePen(PS_SOLID, separatorWidth, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	const float cellWidth = (float)(clientRect.right - separatorWidth) / columnCount;
	const int cellAvalibleWidth = (int)cellWidth - horizontalPadding * 2 - separatorWidth;

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	MoveToEx(hdc, 0, separatorWidth / 2, nullptr);
	LineTo(hdc, clientRect.right, separatorWidth / 2);

	float incrementX = cellAvalibleWidth + horizontalPadding + separatorWidth;

	int tableHeight = separatorWidth;
	for (int row = 0; row < rowCount; row++) {
		int nextTableHeight = tableHeight;
		int columnX = separatorWidth + horizontalPadding;
		for (int column = 0; column < columnCount; column++) {
			int columY = tableHeight + lineSpacing;
			int fittedCharactersCount = 0;
			LPCSTR pStr = values[row][column].c_str();
			while (fittedCharactersCount != values[row][column].size()) {
				int fittingCharactersCount;
				SIZE size;
				GetTextExtentExPointA(hdc, pStr, values[row][column].size() - fittedCharactersCount,
					cellAvalibleWidth, &fittingCharactersCount, nullptr, &size);

				if (fittingCharactersCount <= 0) {
					break;
				}

				TextOutA(hdc, columnX, columY, pStr, fittingCharactersCount);

				pStr += fittingCharactersCount;
				fittedCharactersCount += fittingCharactersCount;
				columY += tm.tmHeight + lineSpacing;
			}
			columnX += incrementX;

			int curNextTableHeight = columY + lineSpacing + separatorWidth;
			if (curNextTableHeight > nextTableHeight) {
				nextTableHeight = curNextTableHeight;
			}

		}

		tableHeight = nextTableHeight;

		int separatorY = tableHeight - separatorWidth / 2;
		MoveToEx(hdc, 0, separatorY, nullptr);
		LineTo(hdc, clientRect.right, separatorY);
	}


	float separatorX = (float)separatorWidth / 2;
	while (separatorX < clientRect.right - incrementX) {
		MoveToEx(hdc, (int)separatorX, 0, nullptr);
		LineTo(hdc, (int)separatorX, tableHeight - separatorWidth);
		separatorX += incrementX;
	}
	MoveToEx(hdc, clientRect.right - (separatorWidth - 1) / 2, 0, nullptr);
	LineTo(hdc, clientRect.right - (separatorWidth - 1) / 2, tableHeight - separatorWidth);


	SelectObject(hdc, oldPen);
	DeleteObject(pen);
}

