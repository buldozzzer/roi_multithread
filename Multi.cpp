#include "My_functions.h"

mutex main_mutex;

int main(int argc, char** argv) {

	queue<vector<vector<int>>> vectors_queue; //очередь наборов векторов
	queue<vector<pair<int, int>>> all_pairs; //пары
	vector<thread> threads_for_pairs;  //вектор потоков обработки
	vector<thread> computational_threads;
	queue<vector<pair<int, int>>> ranges;

	vector<vector<double>> result;

	int current_hardware = _Thrd_hardware_concurrency(); //число допустимых потоков
	int fileNum = 1;
	bool flag = true;
	ofstream fout;
	string inputFileName;

	do {
		char numBuf[4];
		_itoa_s(fileNum++, numBuf, sizeof(int), 10);
		inputFileName = "output" + (string)numBuf + ".txt";
		flag = reader(inputFileName, vectors_queue);
		if (!flag) {
			break;
		}
	} while (true);

	queue<vector<vector<int>>> vectors_queue_seccond = vectors_queue; //копия наборов векторов для дальнейшего их перебора;
	int size = vectors_queue.size();

	int number_of_sets = vectors_queue.size();
	
	fout.open("out-data.csv", ios_base::app);

	for(int i = 0; i < number_of_sets; ++i) {
		threads_for_pairs.push_back(thread(get_all_pairs, ref(vectors_queue), ref(all_pairs)));
	}
	for (auto& thread : threads_for_pairs){
		thread.join();
	}

	queue<vector<pair<int, int>>> all_pairs_seccond = all_pairs;
	threads_for_pairs.clear();

	while(all_pairs.size()>0){
		auto set = all_pairs.front();
		splitting_of_sets(set, current_hardware, ranges);
		all_pairs.pop();
	}

	while(ranges.size() > 0) {
		main_mutex.lock();
		auto set_of_ranges = ranges.front();
		auto set_of_vectors = vectors_queue_seccond.front();
		auto set_of_all_pairs = all_pairs_seccond.front();
		ranges.pop();
		vectors_queue_seccond.pop();
		all_pairs_seccond.pop();
		main_mutex.unlock();
		auto start = chrono::steady_clock::now();
		for (auto pair : set_of_ranges) {
			computational_threads.push_back(thread(calculation, set_of_vectors, set_of_all_pairs, pair.first, pair.second));
		}
		for (auto& thread : computational_threads) {
			thread.join();
		}
		auto stop = chrono::steady_clock::now();
		auto elapsed = chrono::duration_cast<chrono::milliseconds>(stop - start);
		fout << elapsed.count() << endl;
		computational_threads.clear();
	}
	fout.close();
	return 0;
}