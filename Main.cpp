#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <random>
#include <set>
#include <cstdlib>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
	ifstream fin;
	int length, fileNum, countsOfVectors;
	fin.open("parameters.cfg");
	if (fin.is_open()) {
		while (!fin.eof()) {
			string tempString;
			int tempNum;
			fin >> tempString >> tempNum;
			if (tempString == "OutputFileNumber:") {
				fileNum = tempNum;
			}
			else if (tempString == "LengthOfVectors:") {
				length = tempNum;
			}
			else if (tempString == "CountOfVectors:") {
				countsOfVectors = tempNum;
			}
		}
	}
	else {
		cout << "Can`t open config file for reading" << endl;

		return -1;
	}
	fin.close();
	string outputFileName;
	ofstream fout;
	char itoaBuf[4];
	for (int i = 1; i <= fileNum; i++) {
		_itoa_s(i, itoaBuf, sizeof(int));
		outputFileName = "output" + (string)itoaBuf + ".txt";
		srand(time(NULL));
		fout.open(outputFileName);
		if (fout.is_open()) {
			for (int j = 0; j < countsOfVectors; j++) {
				for (int k = 0; k < length; k++) {
					fout << (rand() % 100) << ' ';
				}
				fout << '\n';
			}
		}
		fout.close();
		countsOfVectors += countsOfVectors;
	}
	return 0;
}