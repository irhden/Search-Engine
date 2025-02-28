#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip> // Для setw и setfill
#include <sstream>

// Класс для работы с JSON-файлами
class ConverterJSON {
public:
	ConverterJSON() = default;

	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	std::vector<std::string> GetTextDocuments();

	/**
	* Метод считывает поле max_responses для определения предельного
	* количества ответов на один запрос
	* @return
	*/
	int GetResponsesLimit();

	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> GetRequests();

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
	const std::string& configFile = "config.json";
	const std::string& requestsFile = "requests.json";
	const std::string& answersFile = "answers.json";
};
