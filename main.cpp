#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
	try {
		ConverterJSON jsonData;		// ������������� ������ � JSON ������, ��� ������ � �� ����������.
		InvertedIndex invIndex;

		// ��������� ��������� � ������ ������:
		std::vector<std::string> documents = jsonData.GetTextDocuments();
		invIndex.UpdateDocumentBase(documents);

		std::vector<std::string> requests = jsonData.GetRequests();	 // �������� �������.
		int maxResponses = jsonData.GetResponsesLimit();	// ����������� ���������� ��������.
		int counterRequests = 1;	// ������� ������������ ��������.

		// ������ ������ ������
		SearchServer server(invIndex);

		// ��������� �����
		std::vector<std::vector<RelativeIndex>> searchResults = server.search(requests);

		// ������������ ���������� � ������ ������:
		std::vector<std::vector<std::pair<int, float>>> resultList = server.ConvertToPairs(searchResults);

		// ����� ���������� �����������:
		size_t total_count = 0;
		for (const auto& query_results : searchResults) {
			total_count += query_results.size();
		}
		std::cout << "Total results found: " << total_count << std::endl;

		// ���������� ���������� � answers.json:
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
