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

		// ������� ����� ���������� ��������� �����������:
		std::cout << totalResults << " results found.\n";
		int numRequest = 1;	// ������� ��� ��������� ��������.

		// �������� �� ���� ����������� ������ ��� ������� �������:
		for (auto& entries : searchResults) {
			// ������� ����� �������� �������:
			std::cout << "Request #" << numRequest << ":\n";
			numRequest++;	// ����������� ������� ��������.

			// ���������� ��������� ��������� ��� �������� �������:
			for (int i = 0; i < entries.size(); i++) {
				// ������� ������������� ��������� � ��� �������������:
				std::cout << "Doc ID: " << entries[i].docID << " \tRank: " << entries[i].rank << "\n";
			}

			std::cout << "\n";
		}

		// ���������� ���������� � answers.json:
		jsonData.PutAnswers(resultList);
	}
	catch (const std::exception& exp) {
		std::cerr << exp.what() << "\n";
	}

	return 0;
}
