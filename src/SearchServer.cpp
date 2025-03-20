#include "SearchServer.h"
#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string>& queriesInput, const int& maxResponses) {
   std::vector<std::vector<RelativeIndex>> results; // Итоговый список результатов.

	// Разбивка поискового запроса на отдельные слова и формирование из них список уникальных:
   for (const std::string& requests : queriesInput) {
      std::vector<std::string> uniqueWords;     // Список уникальных слов.
      std::map<size_t, size_t> docsRelevance;   // Релевантность документов.

      // Разбиение строки запроса на отдельные слова:
      std::istringstream stream(requests);	// Поток для разбиения текста документа на слова.
      std::string word;		// Для временного хранения слов, которые будут браться из потока.

      while (stream >> word) {
         // Если слово отсутсвует в словаре, значит оно уникальное, и мы добавляем его в словарь:
         if (std::find(uniqueWords.begin(), uniqueWords.end(), word) == uniqueWords.end()) {
            uniqueWords.push_back(word);
         }
      }

      // Получаем частоту встречаемости каждого слова в документах (через GetWordCount):
      std::vector<std::pair<std::string, size_t>> wordFrequencies;
      for (auto& uniqWord : uniqueWords) {
         auto entries = _index.GetWordCount(uniqWord);
         wordFrequencies.push_back({ uniqWord, entries.size()});
      }

      // Если слово отсутсвует в файлах, пропускаем проверки и сортировки, и добавляем пустой элемент:
      if (wordFrequencies.empty()) {
         std::vector<RelativeIndex> empty;
         results.push_back(empty);
         continue;
      }

      // Сортировка слов по редкости (по количеству документов, в которых они встречаются):
      std::sort(wordFrequencies.begin(), wordFrequencies.end(),
         [](const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b) {
            return a.second < b.second;
         });

      // Подсчитываем абсолютную релевантность каждого документа (сумма count):
      for (auto& _word : wordFrequencies) {
         for (auto& entry : _index.GetWordCount(_word.first)) {
            docsRelevance[entry.docID] += entry.count;  // Суммируем частоту слов.
         }
      }

      // Находим максимальную абсолютную релевантность среди всех документов:
      size_t maxRelevance = 0;
      for (auto& relevance : docsRelevance) {
         if (relevance.second > maxRelevance) {
            maxRelevance = relevance.second;
         }
      }

      // Вычисляем относительную релевантность каждого документа:
      std::vector<RelativeIndex> rankedResults;
      for (auto& relevance : docsRelevance) {
         float rank = (float)relevance.second / maxRelevance;
         size_t docID = relevance.first;    // Номер документа - переменная нужна чисто для ясности.
         rankedResults.push_back({ docID, rank });
      }

      // Сортировка документов по убыванию относительной релевантности:
      std::sort(rankedResults.begin(), rankedResults.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
         return a.rank > b.rank;
      });

      // Ограничиваем количество результатов по maxResponses:
      if (rankedResults.size() > maxResponses) {
         rankedResults.resize(maxResponses);  // Обрезаем до maxResponses:
      }

      // Добавление отсортированного результата в общий список:
      results.push_back(rankedResults);
   }

   return results;
}


std::vector<std::vector<std::pair<int, float>>> SearchServer::ConvertToPairs(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   std::vector<std::vector<std::pair<int, float>>> convertedResults; // Итоговый вектор с преобразованными данными.

   // Проходим по каждому списку документов в searchResults:
   for (const auto& resultList : searchResults) {
      std::vector<std::pair<int, float>> convertedList;  // Локальный список для хранения пар <docID, rank>.

      // Проходим по каждому элементу RelativeIndex в текущем списке:
      for (const auto& result : resultList) {
         convertedList.push_back({ result.docID, result.rank }); // Добавляем в список пару <docID, rank.
      }
      // Добавляем готовый список в общий список результатов:
      convertedResults.push_back(std::move(convertedList));
   }
   
   return convertedResults;   // Возвращаем преобразованный список.
}

int SearchServer::CountSearchResults(const std::vector<std::vector<RelativeIndex>>& searchResults) {
   int totalResults = 0;
   for (const auto& query_results : searchResults) {
      totalResults += query_results.size();
   }

   return totalResults;
}
