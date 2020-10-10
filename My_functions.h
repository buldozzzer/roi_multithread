#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <map>
#include <algorithm>

using namespace std;

mutex my_mutex;

int counter(string fileName) {
	int count = 0;
	string temp;
	ifstream fin;
	fin.open(fileName);
	if (fin.is_open()) {
		while (!fin.eof()) {
			getline(fin, temp);
			count++;
		}
	}
	return count;
}

bool reader(string filename, queue<vector<vector<int>>>& vectors_queue) {
	ifstream fin;
	fin.open(filename);
	vector<vector<int>> vectors_from_file;
	if (fin.is_open()) {
		while (!fin.eof()) {
			vector<int> myVector;
			string fileString;
			string stringForDigit;
			stringstream streamString;
			getline(fin, fileString);
			if (fileString == "" | fileString.empty()) {
				break;
			}
			fileString.erase(fileString.rfind(' '));
			streamString << fileString;
			while (!streamString.eof()) {
				streamString >> stringForDigit;
				myVector.push_back(atoi(stringForDigit.c_str()));
			}

			vectors_from_file.push_back(myVector);


		}
		vectors_queue.push(vectors_from_file);
		return true;
	}
	else {
		fin.close();
		return false;
	}
}

double modul(vector<int> vector) {
	double result = 0;
	for (unsigned int i = 0; i < vector.size(); ++i) {
		result += pow(vector[i], 2);
	}
	return sqrt(result);
}

double sum(vector<int> a, vector<int> b) {
	double result = 0;
	for (unsigned int i = 0; i < a.size(); ++i) {
		result += a.at(i) * b.at(i);
	}
	return result;
}

vector<double> get_result(vector<vector<int>> vectors, int left, int right) {
	double result = 0;
	vector<double> cos_for_all_pairs;
	for (unsigned int i = left; i < right-1; ++i) {
		double modulA = modul(vectors[i]);
		for (unsigned int j = i + 1; j < right; ++j) {
			double modulB = modul(vectors[j]);
			double s = sum(vectors[i], vectors[j]);
			result = s / (modulA*modulB);
			cos_for_all_pairs.push_back(result);
			result = 0;
		}
	}
	return cos_for_all_pairs;
}

queue<vector<pair<int, int>>> get_all_pairs( //метод формирования всех пар элемнтов в наборе
	queue<vector<vector<int>>>& vectors_queue,
	queue<vector<pair<int, int>>>& pairs_queue) {

	my_mutex.lock();
	vector<vector<int>> vectors = vectors_queue.front();
	vectors_queue.pop();
	my_mutex.unlock();

	vector<pair<int, int>> all_pairs;

	for (unsigned int i = 0; i < vectors.size() - 1; ++i) {
		for (unsigned int j = i + 1; j < vectors.size(); ++j) {
			all_pairs.push_back(make_pair(i, j));
		}
	}

	my_mutex.lock();
	pairs_queue.push(all_pairs);
	my_mutex.unlock();
	return pairs_queue;
}

queue<vector<pair<int, int>>> splitting_of_sets(vector<pair<int, int>> all_pairs, int current_hardware, queue<vector<pair<int, int>>>& ranges) {
	vector<pair<int, int>> set;
	int count_pairs_in_range = all_pairs.size() / current_hardware;
	int left = 0;
	int right;
	for (int i = 0; i < current_hardware - 1; ++i) {
		right = left + count_pairs_in_range;
		set.push_back(make_pair(left, right));
		left = right + 1;
	}
	set.push_back(make_pair(left, all_pairs.size() - 1));
	ranges.push(set);
	return ranges;
}

vector<int> merge(const vector<double>& a,
	const vector<double>& b)
{
	vector<int> result;
	std::set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
	return result;
}

vector<double> calculation(vector<vector<int>> vectors, vector<pair<int, int>> pairs, int left, int right) {
	double result = 0;
	vector<double> cos_for_all_pairs;
	for (int i = left; i <= right; ++i) {
		double modulA = modul(vectors[pairs[i].first]);
		double modulB = modul(vectors[pairs[i].second]);
		double s = sum(vectors[pairs[i].first], vectors[pairs[i].second]);
		result = s / (modulA*modulB);
		cos_for_all_pairs.push_back(result);
	}
	return cos_for_all_pairs;
}

void calculation_of_cosine(vector<vector<int>> vectors, vector<pair<int, int>> pairs, int left, int right/*, vector<vector<double>>& result*/) {

	vector<double> result_cos = get_result(vectors, left, right);

	calculation(vectors, pairs, left, right);

	/*my_mutex.lock();
	result.push_back(result_cos);
	my_mutex.unlock();*/

}

