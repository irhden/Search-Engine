#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	// Очищаем частотный словарь и список документов перед обновлением:
	freq_dictionary.clear();
	docs.clear();
	docs = input_docs;	// Загружаем новые документы.

	// Перебираем все документы, добавляя их в индекс:
	for (size_t docID = 0; docID < docs.size(); docID++) {
		std::map<std::string, int> wordsCounter; // Локальный словарь для подсчёта слов в документе.
		
		std::istringstream stream(docs[docID]);	// Поток для разбиения текста документа на слова.
		std::string word;		// Для временного хранения слов, которые будут браться из потока.

		// Разбиение документа на слова и их подсчёт:
		while (stream >> word) {
			wordsCounter[word]++;	// Увеличение счётчика текущего слова.
		}

		// Заполнение частотного словаря:
		for (auto& currentWord : wordsCounter) {
			Entry tempEntry = {docID, currentWord.second};
			freq_dictionary[currentWord.first].push_back(tempEntry);
		}
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// Проверяем, есть ли указанное слово в частотном словаре:
	if (freq_dictionary.find(word) != freq_dictionary.end()) {
		return freq_dictionary[word];	// Если есть, возвращаем все его нахождения.
	}

	return std::vector<Entry>();	// Иначе, возвращаем пустой вектор.
}
