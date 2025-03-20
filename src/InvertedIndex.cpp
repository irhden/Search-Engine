#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs) {
	// Очищаем частотный словарь и список документов перед обновлением:
	freqDictionary.clear();
	docs.clear();
	docs = inputDocs;	// Загружаем новые документы.

	const int numThreads = std::thread::hardware_concurrency(); // Количество потоков (обычно = количеству ядер)
	int docsPerThread = docs.size() / numThreads; // Сколько документов на один поток
	std::mutex mutexFreqDictionary;		// Защита словаря от множества потоков.
	std::vector<std::thread> threads;	// Список потоков.

	// Лямбда выражение для распределения индексирования документов по потокам:
	// start - номер документа, с которого будет работать поток;
	// end - номер документа, до которого работает поток;
	auto ProccesDocuments = [&](int start, int end) {
		std::map<std::string, std::vector<Entry>> localFreqDictionary;	// Локальный частотный словарь для каждого потока.

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
		std::lock_guard<std::mutex> lock(mutexFreqDictionary);
		for (auto& entries : localFreqDictionary) {
			std::string word = entries.first;
			freqDictionary[word].insert(freqDictionary[word].end(), entries.second.begin(), entries.second.end());
		}
	};

	// Запускаем потоки:
	for (int i = 0; i < numThreads; i++) {
		// Номер документа, с которого будет начинать работать поток:
		int start = i * docsPerThread;

		// Номер документа, до которого работает поток (если это последний поток, то до конца списка документов):
		int end = (i == numThreads - 1) ? docs.size() : (start + docsPerThread);

		// Создаём и запускаем новый поток, который обрабатывает документы с номера start до end:
		threads.emplace_back(ProccesDocuments, start, end);
	} 

	// Ожидаем завершения всех потоков:
	for (auto& currentThread : threads) {
		currentThread.join();
	}

	// Сортируем вектора Entry для каждого слова по docID:
	for (auto& entries : freqDictionary) {
		std::sort(entries.second.begin(), entries.second.end(), [](const Entry& a, const Entry& b) {
			return a.docID < b.docID;
		});
	}
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// Проверяем, есть ли указанное слово в частотном словаре:
	if (freqDictionary.find(word) != freqDictionary.end()) {
		return freqDictionary[word];	// Если есть, возвращаем все его нахождения.
	}

	return std::vector<Entry>();	// Иначе, возвращаем пустой вектор.
}
