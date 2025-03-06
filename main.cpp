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

		// Записываем результаты в answers.json:
		jsonData.PutAnswers(resultList);
	}
	catch (const std::exception& exp) {
		std::cerr << exp.what() << "\n";
	}

	return 0;
}
