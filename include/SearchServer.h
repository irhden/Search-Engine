#pragma once
#include "InvertedIndex.h"

struct RelativeIndex {
	size_t docID;
	float rank;

	bool operator ==(const RelativeIndex& other) const {
		return (docID == other.docID && rank == other.rank);
	}
};

class SearchServer {
public:
	/**
	* @param idx � ����������� ������ ��������� ������ �� ����� InvertedIndex,
	* ����� SearchServer ��� ������ ������� ���� ����������� � �������
	*/
	SearchServer(InvertedIndex& idx) : _index(idx) { };

	/**
	* ����� ��������� ��������� ��������
	* @param queries_input ��������� ������� ������ �� ����� requests.json
	* @return ���������� ��������������� ������ ����������� ������� ��� �������� ��������
	*/
	std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput, const int& maxResponses);

	/**
	* ��������������� ����������� ������ search, ��� �������� � ������� ConverterJSON::putAnswers
	* @param searchResults - ������ ����������� ������, ���������� � ������ search, ��� ������ ���������� ������ ������������ ���������� ������ ��� ������ �������
	* @return ���������� ���������������� ������ �����������
	*/
	std::vector<std::vector<std::pair<int, float>>> ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults);

	/**
	* ������� ����� ���������� ����������� ����� ������ � ������
	* @param searchResults - ������ ����������� ������, ���������� � ������ search, ��� ������ ���������� ������ ������������ ���������� ������ ��� ������ �������
	* @return ����� ���������� �����������
	*/
	static int CountSearchResults(const std::vector<std::vector<RelativeIndex>>& searchResults);

private:
	InvertedIndex _index;
};