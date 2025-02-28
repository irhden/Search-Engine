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
		int counterRequests = 1;	// Счётчик обработанных запросов.

		// Создаём сервер поиска
		SearchServer server(invIndex);

		// Выполняем поиск
		std::vector<std::vector<RelativeIndex>> searchResults = server.search(requests);

		// Конвертируем результаты в нужный формат:
		std::vector<std::vector<std::pair<int, float>>> resultList = server.ConvertToPairs(searchResults);

		// общее количество результатов:
		size_t total_count = 0;
		for (const auto& query_results : searchResults) {
			total_count += query_results.size();
		}
		std::cout << "Total results found: " << total_count << std::endl;

		// Записываем результаты в answers.json:
		jsonData.putAnswers(resultList);

		//if (counterRequests > maxResponses) {
		//	std::cout << "Request limit exceeded!" << std::endl;
		//}
		//counterRequests++;
	}
	catch (const std::exception& exp) {
		std::cerr << exp.what() << "\n";
	}

	return 0;
}
