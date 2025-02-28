#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip> // ��� setw � setfill
#include <sstream>

// ����� ��� ������ � JSON-�������
class ConverterJSON {
public:
	ConverterJSON() = default;

	/**
	* ����� ��������� ����������� ������
	* @return ���������� ������ � ���������� ������ �������������
	* � config.json
	*/
	std::vector<std::string> GetTextDocuments();

	/**
	* ����� ��������� ���� max_responses ��� ����������� �����������
	* ���������� ������� �� ���� ������
	* @return
	*/
	int GetResponsesLimit();

	/**
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	std::vector<std::string> GetRequests();

	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
	const std::string& configFile = "config.json";
	const std::string& requestsFile = "requests.json";
	const std::string& answersFile = "answers.json";
};
