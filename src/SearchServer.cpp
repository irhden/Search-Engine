#include "SearchServer.h"
#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string>& queriesInput, const int& maxResponses) {
   std::vector<std::vector<RelativeIndex>> results; // �������� ������ �����������.

	// �������� ���������� ������� �� ��������� ����� � ������������ �� ��� ������ ����������:
   for (const std::string& requests : queriesInput) {
      std::vector<std::string> uniqueWords;     // ������ ���������� ����.
      std::map<size_t, size_t> docsRelevance;   // ������������� ����������.

      // ��������� ������ ������� �� ��������� �����:
      std::istringstream stream(requests);	// ����� ��� ��������� ������ ��������� �� �����.
      std::string word;		// ��� ���������� �������� ����, ������� ����� ������� �� ������.

      while (stream >> word) {
         // ���� ����� ���������� � �������, ������ ��� ����������, � �� ��������� ��� � �������:
         if (std::find(uniqueWords.begin(), uniqueWords.end(), word) == uniqueWords.end()) {
            uniqueWords.push_back(word);
         }
      }

      // �������� ������� ������������� ������� ����� � ���������� (����� GetWordCount):
      std::vector<std::pair<std::string, size_t>> wordFrequencies;
      for (auto& uniqWord : uniqueWords) {
         auto entries = _index.GetWordCount(uniqWord);
         wordFrequencies.push_back({ uniqWord, entries.size()});
      }

      // ���� ����� ���������� � ������, ���������� �������� � ����������, � ��������� ������ �������:
      if (wordFrequencies.empty()) {
         std::vector<RelativeIndex> empty;
         results.push_back(empty);
         continue;
      }

      // ���������� ���� �� �������� (�� ���������� ����������, � ������� ��� �����������):
      std::sort(wordFrequencies.begin(), wordFrequencies.end(),
         [](const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b) {
            return a.second < b.second;
         });

      // ������������ ���������� ������������� ������� ��������� (����� count):
      for (auto& _word : wordFrequencies) {
         for (auto& entry : _index.GetWordCount(_word.first)) {
            docsRelevance[entry.docID] += entry.count;  // ��������� ������� ����.
         }
      }

      // ������� ������������ ���������� ������������� ����� ���� ����������:
      size_t maxRelevance = 0;
      for (auto& relevance : docsRelevance) {
         if (relevance.second > maxRelevance) {
            maxRelevance = relevance.second;
         }
      }

      // ��������� ������������� ������������� ������� ���������:
      std::vector<RelativeIndex> rankedResults;
      for (auto& relevance : docsRelevance) {
         float rank = (float)relevance.second / maxRelevance;
         size_t docID = relevance.first;    // ����� ��������� - ���������� ����� ����� ��� �������.
         rankedResults.push_back({ docID, rank });
      }

      // ���������� ���������� �� �������� ������������� �������������:
      std::sort(rankedResults.begin(), rankedResults.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
         return a.rank > b.rank;
      });

      // ������������ ���������� ����������� �� maxResponses:
      if (rankedResults.size() > maxResponses) {
         rankedResults.resize(maxResponses);  // �������� �� maxResponses:
      }

      // ���������� ���������������� ���������� � ����� ������:
      results.push_back(rankedResults);
   }

//    ����� � ������� (��� �������):
   //for (int i = 0; i < results.size(); i++) {
   //   std::cout << "NEXT REQ:\n";
   //   for (int j = 0; j < results[i].size(); j++) {
   //      std::cout << "docID: " << results[i][j].docID << " \t-\t rank: " << results[i][j].rank << "\n";
   //   }
   //   std::cout << "\n";
   //}

   return results;
}


std::vector<std::vector<std::pair<int, float>>> SearchServer::ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   std::vector<std::vector<std::pair<int, float>>> convertedResults; // �������� ������ � ���������������� �������.

   // �������� �� ������� ������ ���������� � searchResults:
   for (const auto& resultList : searchResults) {
      std::vector<std::pair<int, float>> convertedList;  // ��������� ������ ��� �������� ��� <docID, rank>.

      // �������� �� ������� �������� RelativeIndex � ������� ������:
      for (const auto& result : resultList) {
         convertedList.push_back({ result.docID, result.rank }); // ��������� � ������ ���� <docID, rank.
      }
      // ��������� ������� ������ � ����� ������ �����������:
      convertedResults.push_back(std::move(convertedList));
   }
   
   return convertedResults;   // ���������� ��������������� ������.
}

int SearchServer::CountSearchResults(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   int totalResults = 0;
   for (const auto& query_results : searchResults) {
      totalResults += query_results.size();
   }
//   std::cout << "Total results: " << totalResults << std::endl;    // ��� �������.
   return totalResults;
}
