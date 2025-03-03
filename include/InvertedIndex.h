#pragma once
#include <vector>
#include <map>
#include <sstream>  // Для std::istringstream
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>


struct Entry {
	size_t doc_id;		// Номер элемента в векторе docs, по которому хранится текст.
	size_t count;	// Число, которое обозначает, сколько раз ключевое слово встретилось в документе doc_id.

	// Данный оператор необходим для проведения тестовых сценариев
	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id &&
			count == other.count);
	}
};


class InvertedIndex {
public:
	InvertedIndex() = default;

	/**
	* Обновить или заполнить базу документов, по которой будем совершать поиск
	* @param input_docs содержимое документов
	*/
	void UpdateDocumentBase(std::vector<std::string> input_docs);

	/**
	* Метод определяет количество вхождений слова word в загруженной базе документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> GetWordCount(const std::string& word);

private:
	// Коллекция для хранения текстов документов, в которой номер элемента в векторе
	// определяет doc_id для формирования результата запроса:
	std::vector<std::string> docs;

	// Частотный словарь, это коллекция для хранения частоты слов, встречаемых в тексте:
	std::map<std::string, std::vector<Entry>> freq_dictionary;
};

