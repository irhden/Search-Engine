#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
	try {
		ConverterJSON jsonData;		// Предоставляет доступ к JSON файлам, для работы с их содержимым.
		InvertedIndex invIndex;

		// Загружаем документы и создаём индекс:
		std::vector<std::string> documents = jsonData.GetTextDocuments();
		invIndex.UpdateDocumentBase(documents);

		std::vector<std::string> requests = jsonData.GetRequests();	 // Получаем запросы.
		int maxResponses = jsonData.GetResponsesLimit();	// Ограничение количества запросов.

		// Создаём сервер поиска:
		SearchServer server(invIndex);

		// Выполняем поиск:
		std::vector<std::vector<RelativeIndex>> searchResults = server.Search(requests, maxResponses);

		// Конвертируем результаты в нужный формат:
		std::vector<std::vector<std::pair<int, float>>> resultList = server.ConvertToPairs(searchResults);

		// Общее количество результатов:
		int totalResults = SearchServer::CountSearchResults(searchResults);

		// Выводим общее количество найденных результатов:
		std::cout << totalResults << " results found.\n";
		int numRequest = 1;	// Счётчик для нумерации запросов.

		// Проходим по всем результатам поиска для каждого запроса:
		for (auto& entries : searchResults) {
			// Выводим номер текущего запроса:
			std::cout << "Request #" << numRequest << ":\n";
			numRequest++;	// Увеличиваем счётчик запросов.

			// Перебираем найденные документы для текущего запроса:
			for (int i = 0; i < entries.size(); i++) {
				// Выводим идентификатор документа и его релевантность:
				std::cout << "Doc ID: " << entries[i].docID << " \tRank: " << entries[i].rank << "\n";
			}

			std::cout << "\n";
		}

		// Записываем результаты в answers.json:
		jsonData.PutAnswers(resultList);
	}
	catch (const std::exception& exp) {
		std::cerr << exp.what() << "\n";
	}

	return 0;
}
