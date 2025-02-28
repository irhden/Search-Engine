#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	// ������� ��������� ������� � ������ ���������� ����� �����������:
	freq_dictionary.clear();
	docs.clear();
	docs = input_docs;	// ��������� ����� ���������.

	// ���������� ��� ���������, �������� �� � ������:
	for (size_t docID = 0; docID < docs.size(); docID++) {
		std::map<std::string, int> wordsCounter; // ��������� ������� ��� �������� ���� � ���������.
		
		std::istringstream stream(docs[docID]);	// ����� ��� ��������� ������ ��������� �� �����.
		std::string word;		// ��� ���������� �������� ����, ������� ����� ������� �� ������.

		// ��������� ��������� �� ����� � �� �������:
		while (stream >> word) {
			wordsCounter[word]++;	// ���������� �������� �������� �����.
		}

		// ���������� ���������� �������:
		for (auto& currentWord : wordsCounter) {
			Entry tempEntry = {docID, currentWord.second};
			freq_dictionary[currentWord.first].push_back(tempEntry);
		}
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// ���������, ���� �� ��������� ����� � ��������� �������:
	if (freq_dictionary.find(word) != freq_dictionary.end()) {
		return freq_dictionary[word];	// ���� ����, ���������� ��� ��� ����������.
	}

	return std::vector<Entry>();	// �����, ���������� ������ ������.
}
