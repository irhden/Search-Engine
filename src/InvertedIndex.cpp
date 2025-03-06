#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs) {
	// ������� ��������� ������� � ������ ���������� ����� �����������:
	freqDictionary.clear();
	docs.clear();
	docs = inputDocs;	// ��������� ����� ���������.

	const int numThreads = std::thread::hardware_concurrency(); // ���������� ������� (������ = ���������� ����)
	int docsPerThread = docs.size() / numThreads; // ������� ���������� �� ���� �����
	std::mutex mutexFreqDictionary;		// ������ ������� �� ��������� �������.
	std::vector<std::thread> threads;	// ������ �������.

	// ������ ��������� ��� ������������� �������������� ���������� �� �������:
	// start - ����� ���������, � �������� ����� �������� �����;
	// end - ����� ���������, �� �������� �������� �����;
	auto ProccesDocuments = [&](int start, int end) {
		std::map<std::string, std::vector<Entry>> localFreqDictionary;	// ��������� ��������� ������� ��� ������� ������.

		// ���������� ��� ���������, �������� �� � ������:
		for (size_t docID = start; docID < end; ++docID){
			std::map<std::string, int> wordsCounter; // ��������� ������� ��� �������� ���� � ���������.

			std::istringstream stream(docs[docID]);	// ����� ��� ��������� ������ ��������� �� �����.
			std::string word;		// ��� ���������� �������� ����, ������� ����� ������� �� ������.

			// ��������� ��������� �� ����� � �� �������:
			while (stream >> word) {
				wordsCounter[word]++;	// ���������� �������� �������� �����.
			}

			// ���������� ���������� �������:
			for (auto& currentWord : wordsCounter) {
				Entry tempEntry = { docID, currentWord.second };
				localFreqDictionary[currentWord.first].push_back(tempEntry);
			}
		}

		// ���������� ��������� ��������� ������� � �����:
		std::lock_guard<std::mutex> lock(mutexFreqDictionary);
		for (auto& entries : localFreqDictionary) {
			std::string word = entries.first;
			freqDictionary[word].insert(freqDictionary[word].end(), entries.second.begin(), entries.second.end());
		}
	};

	// ��������� ������:
	for (int i = 0; i < numThreads; i++) {
		// ����� ���������, � �������� ����� �������� �������� �����:
		int start = i * docsPerThread;

		// ����� ���������, �� �������� �������� ����� (���� ��� ��������� �����, �� �� ����� ������ ����������):
		int end = (i == numThreads - 1) ? docs.size() : (start + docsPerThread);

		// ������ � ��������� ����� �����, ������� ������������ ��������� � ������ start �� end:
		threads.emplace_back(ProccesDocuments, start, end);
	} 

	// ������� ���������� ���� �������:
	for (auto& currentThread : threads) {
		currentThread.join();
	}

	// ��������� ������� Entry ��� ������� ����� �� docID:
	for (auto& entries : freqDictionary) {
		std::sort(entries.second.begin(), entries.second.end(), [](const Entry& a, const Entry& b) {
			return a.docID < b.docID;
		});
	}
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// ���������, ���� �� ��������� ����� � ��������� �������:
	if (freqDictionary.find(word) != freqDictionary.end()) {
		return freqDictionary[word];	// ���� ����, ���������� ��� ��� ����������.
	}

	return std::vector<Entry>();	// �����, ���������� ������ ������.
}
