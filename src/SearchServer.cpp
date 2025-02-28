#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
   std::vector<std::vector<RelativeIndex>> results; // »тоговый список результатов.

	// –азбивка поискового запроса на отдельные слова и формирование из них список уникальных:
   for (const std::string& requests : queries_input) {
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
            docsRelevance[entry.doc_id] += entry.count;  // —уммируем частоту слов.
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
         size_t doc_id = relevance.first;    // Ќомер документа - переменна€ нужна чисто дл€ €сности.
         rankedResults.push_back({ doc_id, rank });
      }

      // —ортировка документов по убыванию относительной релевантности:
      std::sort(rankedResults.begin(), rankedResults.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
         return a.rank > b.rank;
      });

      // ƒобавление отсортированного результата в общий список:
      results.push_back(rankedResults);
   }

   //for (int i = 0; i < results.size(); i++) {
   //   std::cout << "NEXT REQ:\n";
   //   for (int j = 0; j < results[i].size(); j++) {
   //      std::cout << "doc_id: " << results[i][j].doc_id << " \t-\t rank: " << results[i][j].rank << "\n";
   //   }
   //   std::cout << "\n";
   //}

   return results;
}


std::vector<std::vector<std::pair<int, float>>> SearchServer::ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   std::vector<std::vector<std::pair<int, float>>> convertedResults; // »тоговый вектор с преобразованными данными.

   // ѕроходим по каждому списку документов в searchResults:
   for (const auto& resultList : searchResults) {
      std::vector<std::pair<int, float>> convertedList;  // Ћокальный список дл€ хранени€ пар <doc_id, rank>.

      // ѕроходим по каждому элементу RelativeIndex в текущем списке:
      for (const auto& result : resultList) {
         convertedList.push_back({ result.doc_id, result.rank }); // ƒобавл€ем в список пару <doc_id, rank>.
      }
      // ƒобавл€ем готовый список в общий список результатов:
      convertedResults.push_back(std::move(convertedList));
   }

   return convertedResults;   // ¬озвращаем преобразованный список
}
