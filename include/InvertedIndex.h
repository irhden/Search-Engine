#pragma once
#include <vector>
#include <map>
#include <sstream>  // ��� std::istringstream
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>


struct Entry {
	size_t docID;		// ����� �������� � ������� docs, �� �������� �������� �����.
	size_t count;	// �����, ������� ����������, ������� ��� �������� ����� ����������� � ��������� docID.

	// ������ �������� ��������� ��� ���������� �������� ���������
	bool operator ==(const Entry& other) const {
		return (docID == other.docID &&
			count == other.count);
	}
};


class InvertedIndex {
public:
	InvertedIndex() = default;

	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ��������� �����
	* @param input_docs ���������� ����������
	*/
	void UpdateDocumentBase(std::vector<std::string> inputDocs);

	/**
	* ����� ���������� ���������� ��������� ����� word � ����������� ���� ����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/
	std::vector<Entry> GetWordCount(const std::string& word);

private:
	// ��������� ��� �������� ������� ����������, � ������� ����� �������� � �������
	// ���������� doc_id ��� ������������ ���������� �������:
	std::vector<std::string> docs;

	// ��������� �������, ��� ��������� ��� �������� ������� ����, ����������� � ������:
	std::map<std::string, std::vector<Entry>> freqDictionary;
};

