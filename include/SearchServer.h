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
	* @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
	* чтобы SearchServer мог узнать частоту слов встречаемых в запросе
	*/
	SearchServer(InvertedIndex& idx) : _index(idx) { };

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла requests.json
	* @return возвращает отсортированный список релевантных ответов для заданных запросов
	*/
	std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput, const int& maxResponses);

	/**
	* Конвертирование результатов метода search, для передачи в функцию ConverterJSON::putAnswers
	* @param searchResults - список результатов поиска, полученных с метода search, где каждый внутренний вектор представляет результаты поиска для одного запроса
	* @return возвращает конвертированный список результатов
	*/
	std::vector<std::vector<std::pair<int, float>>> ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults);

	/**
	* Считает общее количество результатов после поиска в файлах
	* @param searchResults - список результатов поиска, полученных с метода search, где каждый внутренний вектор представляет результаты поиска для одного запроса
	* @return Общее количество результатов
	*/
	static int CountSearchResults(const std::vector<std::vector<RelativeIndex>>& searchResults);

private:
	InvertedIndex _index;
};