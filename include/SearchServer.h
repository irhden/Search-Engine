#pragma once
#include "InvertedIndex.h"

struct RelativeIndex {
	size_t doc_id;
	float rank;

	bool operator ==(const RelativeIndex& other) const {
		return (doc_id == other.doc_id && rank == other.rank);
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
	std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

	/**
	* Конвертирование результатов метода search, для передачи в функцию ConverterJSON::putAnswers
	* @param searchResults список результатов поиска, полученные с метода search
	* @return возвращает конвертированный список результатов
	*/
	std::vector<std::vector<std::pair<int, float>>> ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults);

private:
	InvertedIndex _index;
};