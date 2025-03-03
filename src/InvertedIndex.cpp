#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	// ������� ��������� ������� � ������ ���������� ����� �����������:
	freq_dictionary.clear();
	docs.clear();
	docs = input_docs;	// ��������� ����� ���������.

	const size_t numThreads = std::thread::hardware_concurrency(); // ���������� ������� (������ = ���������� ����)
	size_t docsPerThread = docs.size() / numThreads; // ������� ���������� �� ���� �����
	std::mutex MutexFreqDictionary;		// ������ ������� �� ��������� �������.
	std::vector<std::thread> threads;	// ������ �������.

	auto ProccesDocuments = [&](int start, int end) {
		std::map<std::string, std::vector<Entry>> localFreqDictionary;
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
		std::lock_guard<std::mutex> lock(MutexFreqDictionary);
		for (auto& entries : localFreqDictionary) {
			std::string word = entries.first;
			freq_dictionary[word].insert(freq_dictionary[word].end(), entries.second.begin(), entries.second.end());
		}
	};

	// ��������� ������:
	for (int i = 0; i < numThreads; i++) {
		int start = i * docsPerThread;
		int end = (i == numThreads - 1) ? docs.size() : (start + docsPerThread);
		threads.emplace_back(ProccesDocuments, start, end);
	} 

	// ������� ���������� ���� �������:
	for (auto& currentThread : threads) {
		currentThread.join();
	}

	// ��������� ������� Entry ��� ������� ����� �� doc_id:
	for (auto& entries : freq_dictionary) {
		std::sort(entries.second.begin(), entries.second.end(), [](const Entry& a, const Entry& b) {
			return a.doc_id < b.doc_id;
		});
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// ���������, ���� �� ��������� ����� � ��������� �������:
	if (freq_dictionary.find(word) != freq_dictionary.end()) {
		return freq_dictionary[word];	// ���� ����, ���������� ��� ��� ����������.
	}

	return std::vector<Entry>();	// �����, ���������� ������ ������.
}
