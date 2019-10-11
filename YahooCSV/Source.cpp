//Brendan Nguyen
//Yahoo CSV Project
//============ References =============
//WEB geeksforgeeks.org/stringstream-c-applications //stringstream to split getline into individual elements.
//WEB geeksforgeeks.org/substring-in-cpp/ // substr to remove .csv ticker in filename string.
//BOOK Tony Gaddis - Starting Out with C++ from Control Structures to Objects
//=====================================

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "days.h"

using namespace std;

void Compute_1_Filename(string &filename) //finds .csv file
{
	WIN32_FIND_DATAA findfiledata;
	HANDLE hfind;
	string file_extension = "*.csv";
	
	string directory_file_ext = file_extension;
	hfind = FindFirstFileA(directory_file_ext.c_str(), &findfiledata);

	if (hfind != INVALID_HANDLE_VALUE)
		filename = findfiledata.cFileName;
	else
		cout << "No file was present" << endl;
}

void readfile(string filename, vector <vector<string>> &data) //reads file and stores them into a 2d vector string
{
	ifstream indata;
	string line;
	indata.open(filename);

	getline(indata, line);

	while (!getline(indata,line).eof())
	{
		stringstream seperate(line);
		string cell;
		vector <string> row;

		for (cell; getline(seperate, cell, ','); row.push_back(cell));
		data.push_back(row);
	}
	indata.close();
}

vector <days>days_sorted(const vector <vector<string>> data, vector <days> &trades)  //takes the 2d string vector and stores data into a 2d vector object; then sorts 2d vector object by percent change
{
	days day;
	int size = data.size();

	for (int i = 0; i < size; i++)
	{
		day.date = data[i][0];
		day.open = (stold(data[i][1]));
		day.adjclose = (stold(data[i][5]));
		day.percentchange = (((day.adjclose - day.open) / day.open) * 100);

		trades.push_back(day);
	}

	for (int i = 0; i < size; i++)
	{
		int index = i;
		for (int j = 0; j < size; j++)
			if (trades[j].percentchange < trades[i].percentchange)
			{
				index = j;
				days temp = trades[i];
				trades[i] = trades[j];
				trades[j] = temp;
			}

	}
	return trades;
}

void display(const vector <days> trades, int num) //displays number of records prompted
{
	int size = num;
	cout << "Date" << '\t' << '\t' << "Open" << '\t' << "A.Close" << '\t' << "Percent Change" << endl;
	for (int row = 0; row < size; row++)
	{
		cout << trades[row].date << '\t' << trades[row].open << '\t' << trades[row].adjclose << '\t' << trades[row].percentchange << endl;
	}
	cout << endl;
}

int main()
{
	string filename;
	vector <vector<string>> data;
	vector <days> trades;

	Compute_1_Filename(filename);
	readfile(filename, data);
	days_sorted(data, trades);

	int size = data.size();
	int numofrecords;

	cout << "This program will anaylze " << filename.substr(0, filename.size() - 4) << " from " << trades[0].date << " to " << trades[size-1].date << endl;
	cout << size << " trading day(s) exist in the file." << endl;
	cout << "How many increase records do you want to find? ";
	cin >> numofrecords;
	cout << endl;
	while (numofrecords > size)
	{
		cout << "The file does not contain enough trading days" << endl;
		cout << "There are only " << size << " trading days." << endl;
		cout << "How many increase records do you want to find? ";
		cin >> numofrecords;
		cout << endl;
	}
	display(trades, numofrecords);

	system("pause");
	return 0;
}