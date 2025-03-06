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

		// ������ ������ ������:
		SearchServer server(invIndex);

		// ��������� �����:
		std::vector<std::vector<RelativeIndex>> searchResults = server.Search(requests, maxResponses);

		// ������������ ���������� � ������ ������:
		std::vector<std::vector<std::pair<int, float>>> resultList = server.ConvertToPairs(searchResults);

		// ����� ���������� �����������:
		int totalResults = SearchServer::CountSearchResults(searchResults);

		// ���������� ���������� � answers.json:
		jsonData.PutAnswers(resultList);
	}
	catch (const std::exception& exp) {
		std::cerr << exp.what() << "\n";
	}

	return 0;
}
