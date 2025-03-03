#pragma once
#include <vector>
#include <map>
#include <sstream>  // ��� std::istringstream
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>


struct Entry {
	size_t doc_id;		// ����� �������� � ������� docs, �� �������� �������� �����.
	size_t count;	// �����, ������� ����������, ������� ��� �������� ����� ����������� � ��������� doc_id.

	// ������ �������� ��������� ��� ���������� �������� ���������
	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id &&
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
	void UpdateDocumentBase(std::vector<std::string> input_docs);

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
	std::map<std::string, std::vector<Entry>> freq_dictionary;
};

