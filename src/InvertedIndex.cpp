#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs) {
	// ќчищаем частотный словарь и список документов перед обновлением:
	freqDictionary.clear();
	docs.clear();
	docs = inputDocs;	// «агружаем новые документы.

	const int numThreads = std::thread::hardware_concurrency(); //  оличество потоков (обычно = количеству €дер)
	int docsPerThread = docs.size() / numThreads; // —колько документов на один поток
	std::mutex mutexFreqDictionary;		// «ащита словар€ от множества потоков.
	std::vector<std::thread> threads;	// —писок потоков.

	// Ћ€мбда выражение дл€ распределени€ индексировани€ документов по потокам:
	// start - номер документа, с которого будет работать поток;
	// end - номер документа, до которого работает поток;
	auto ProccesDocuments = [&](int start, int end) {
		std::map<std::string, std::vector<Entry>> localFreqDictionary;	// Ћокальный частотный словарь дл€ каждого потока.

		// ѕеребираем все документы, добавл€€ их в индекс:
		for (size_t docID = start; docID < end; ++docID){
			std::map<std::string, int> wordsCounter; // Ћокальный словарь дл€ подсчЄта слов в документе.

			std::istringstream stream(docs[docID]);	// ѕоток дл€ разбиени€ текста документа на слова.
			std::string word;		// ƒл€ временного хранени€ слов, которые будут братьс€ из потока.

			// –азбиение документа на слова и их подсчЄт:
			while (stream >> word) {
				wordsCounter[word]++;	// ”величение счЄтчика текущего слова.
			}

			// «аполнение частотного словар€:
			for (auto& currentWord : wordsCounter) {
				Entry tempEntry = { docID, currentWord.second };
				localFreqDictionary[currentWord.first].push_back(tempEntry);
			}
		}

		// ќбъедин€ем локальные частотные словари в общий:
		std::lock_guard<std::mutex> lock(mutexFreqDictionary);
		for (auto& entries : localFreqDictionary) {
			std::string word = entries.first;
			freqDictionary[word].insert(freqDictionary[word].end(), entries.second.begin(), entries.second.end());
		}
	};

	// «апускаем потоки:
	for (int i = 0; i < numThreads; i++) {
		// Ќомер документа, с которого будет начинать работать поток:
		int start = i * docsPerThread;

		// Ќомер документа, до которого работает поток (если это последний поток, то до конца списка документов):
		int end = (i == numThreads - 1) ? docs.size() : (start + docsPerThread);

		// —оздаЄм и запускаем новый поток, который обрабатывает документы с номера start до end:
		threads.emplace_back(ProccesDocuments, start, end);
	} 

	// ќжидаем завершени€ всех потоков:
	for (auto& currentThread : threads) {
		currentThread.join();
	}

	// —ортируем вектора Entry дл€ каждого слова по docID:
	for (auto& entries : freqDictionary) {
		std::sort(entries.second.begin(), entries.second.end(), [](const Entry& a, const Entry& b) {
			return a.docID < b.docID;
		});
	}
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	// ѕровер€ем, есть ли указанное слово в частотном словаре:
	if (freqDictionary.find(word) != freqDictionary.end()) {
		return freqDictionary[word];	// ≈сли есть, возвращаем все его нахождени€.
	}

	return std::vector<Entry>();	// »наче, возвращаем пустой вектор.
}
