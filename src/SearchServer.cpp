#include "SearchServer.h"
#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string>& queriesInput, const int& maxResponses) {
   std::vector<std::vector<RelativeIndex>> results; // »тоговый список результатов.

	// –азбивка поискового запроса на отдельные слова и формирование из них список уникальных:
   for (const std::string& requests : queriesInput) {
      std::vector<std::string> uniqueWords;     // —писок уникальных слов.
      std::map<size_t, size_t> docsRelevance;   // –елевантность документов.

      // –азбиение строки запроса на отдельные слова:
      std::istringstream stream(requests);	// ѕоток дл€ разбиени€ текста документа на слова.
      std::string word;		// ƒл€ временного хранени€ слов, которые будут братьс€ из потока.

      while (stream >> word) {
         // ≈сли слово отсутсвует в словаре, значит оно уникальное, и мы добавл€ем его в словарь:
         if (std::find(uniqueWords.begin(), uniqueWords.end(), word) == uniqueWords.end()) {
            uniqueWords.push_back(word);
         }
      }

      // ѕолучаем частоту встречаемости каждого слова в документах (через GetWordCount):
      std::vector<std::pair<std::string, size_t>> wordFrequencies;
      for (auto& uniqWord : uniqueWords) {
         auto entries = _index.GetWordCount(uniqWord);
         wordFrequencies.push_back({ uniqWord, entries.size()});
      }

      // ≈сли слово отсутсвует в файлах, пропускаем проверки и сортировки, и добавл€ем пустой элемент:
      if (wordFrequencies.empty()) {
         std::vector<RelativeIndex> empty;
         results.push_back(empty);
         continue;
      }

      // —ортировка слов по редкости (по количеству документов, в которых они встречаютс€):
      std::sort(wordFrequencies.begin(), wordFrequencies.end(),
         [](const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b) {
            return a.second < b.second;
         });

      // ѕодсчитываем абсолютную релевантность каждого документа (сумма count):
      for (auto& _word : wordFrequencies) {
         for (auto& entry : _index.GetWordCount(_word.first)) {
            docsRelevance[entry.docID] += entry.count;  // —уммируем частоту слов.
         }
      }

      // Ќаходим максимальную абсолютную релевантность среди всех документов:
      size_t maxRelevance = 0;
      for (auto& relevance : docsRelevance) {
         if (relevance.second > maxRelevance) {
            maxRelevance = relevance.second;
         }
      }

      // ¬ычисл€ем относительную релевантность каждого документа:
      std::vector<RelativeIndex> rankedResults;
      for (auto& relevance : docsRelevance) {
         float rank = (float)relevance.second / maxRelevance;
         size_t docID = relevance.first;    // Ќомер документа - переменна€ нужна чисто дл€ €сности.
         rankedResults.push_back({ docID, rank });
      }

      // —ортировка документов по убыванию относительной релевантности:
      std::sort(rankedResults.begin(), rankedResults.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
         return a.rank > b.rank;
      });

      // ќграничиваем количество результатов по maxResponses:
      if (rankedResults.size() > maxResponses) {
         rankedResults.resize(maxResponses);  // ќбрезаем до maxResponses:
      }

      // ƒобавление отсортированного результата в общий список:
      results.push_back(rankedResults);
   }

//    ¬ывод в консоль (дл€ отладки):
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
   std::vector<std::vector<std::pair<int, float>>> convertedResults; // »тоговый вектор с преобразованными данными.

   // ѕроходим по каждому списку документов в searchResults:
   for (const auto& resultList : searchResults) {
      std::vector<std::pair<int, float>> convertedList;  // Ћокальный список дл€ хранени€ пар <docID, rank>.

      // ѕроходим по каждому элементу RelativeIndex в текущем списке:
      for (const auto& result : resultList) {
         convertedList.push_back({ result.docID, result.rank }); // ƒобавл€ем в список пару <docID, rank.
      }
      // ƒобавл€ем готовый список в общий список результатов:
      convertedResults.push_back(std::move(convertedList));
   }
   
   return convertedResults;   // ¬озвращаем преобразованный список.
}

int SearchServer::CountSearchResults(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   int totalResults = 0;
   for (const auto& query_results : searchResults) {
      totalResults += query_results.size();
   }
//   std::cout << "Total results: " << totalResults << std::endl;    // ƒл€ отладки.
   return totalResults;
}
