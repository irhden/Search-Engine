#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	// Очищаем частотный словарь и список документов перед обновлением:
	freq_dictionary.clear();
	docs.clear();
	docs = input_docs;	// Загружаем новые документы.

	const size_t numThreads = std::thread::hardware_concurrency(); // Количество потоков (обычно = количеству ядер)
	size_t docsPerThread = docs.size() / numThreads; // Сколько документов на один поток
	std::mutex MutexFreqDictionary;		// Защита словаря от множества потоков.
	std::vector<std::thread> threads;	// Список потоков.

	auto ProccesDocuments = [&](int start, int end) {
		std::map<std::string, std::vector<Entry>> localFreqDictionary;
		// Перебираем все документы, добавляя их в индекс:
		for (size_t docID = start; docID < end; ++docID){
			std::map<std::string, int> wordsCounter; // Локальный словарь для подсчёта слов в документе.

			std::istringstream stream(docs[docID]);	// Поток для разбиения текста документа на слова.
			std::string word;		// Для временного хранения слов, которые будут браться из потока.

			// Разбиение документа на слова и их подсчёт:
			while (stream >> word) {
				wordsCounter[word]++;	// Увеличение счётчика текущего слова.
			}

			// Заполнение частотного словаря:
			for (auto& currentWord : wordsCounter) {
				Entry tempEntry = { docID, currentWord.second };
				localFreqDictionary[currentWord.first].push_back(tempEntry);
			}
		}

		// Объединяем локальные частотные словари в общий:
		std::lock_guard<std::mutex> lock(MutexFreqDictionary);
		for (auto& entries : localFreqDictionary) {
			std::string word = entries.first;
			freq_dictionary[word].insert(freq_dictionary[word].end(), entries.second.begin(), entries.second.end());
		}
	};

	// Запускаем потоки:
	for (int i = 0; i < numThreads; i++) {
		int start = i * docsPerThread;
		int end = (i == numThreads - 1) ? docs.size() : (start + docsPerThread);
		threads.emplace_back(ProccesDocuments, start, end);
	} 

	// Ожидаем завершения всех потоков:
	for (auto& currentThread : threads) {
		currentThread.join();
	}

	// Сортируем вектора Entry для каждого слова по doc_id:
	for (auto& entries : freq_dictionary) {
		std::sort(entries.second.begin(), entries.second.end(), [](const Entry& a, const Entry& b) {
			return a.doc_id < b.doc_id;
		});
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// Проверяем, есть ли указанное слово в частотном словаре:
	if (freq_dictionary.find(word) != freq_dictionary.end()) {
		return freq_dictionary[word];	// Если есть, возвращаем все его нахождения.
	}

	return std::vector<Entry>();	// Иначе, возвращаем пустой вектор.
}
