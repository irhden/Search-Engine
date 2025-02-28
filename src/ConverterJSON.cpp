#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	// Открываем файл config.json:
	std::ifstream file("config.json");
	if (!file.is_open()) {
		// Ошибка, если файл отсутствует:
		throw std::runtime_error("config file is missing.");
	}
	
	nlohmann::json jsonData;	// Для хранение данных из файла JSON.
	file >> jsonData;				// Загружаем содержимое файла в jsonData. 
	file.close();					// Со спокойной душой закрываем файл JSON.

	// Проверяем, есть ли поле "config":
	if (!jsonData.contains("config")) {
		// Ошибка, если поле "config" отсутствует:
		throw std::runtime_error("config file is empty");
	}

	// Проверяем, содержится ли массив "files" в JSON
	if (!jsonData.contains("files")) {
		// Ошибка, если массива нет:
		throw std::runtime_error("key 'files' is missing");
	}

	auto& fileList = jsonData["files"];
	std::vector<std::string> documents;	// Вектор для хранения содержимого файлов.
	documents.reserve(fileList.size()); // Резервируем место

	// Перебираем все элементы массива "files" из файла:
	for (auto& filePath : fileList) {
		std::ifstream currentDocument(filePath.get<std::string>());

		// Открываем файлы, указанные в JSON:
		if (!currentDocument.is_open()) {
			// Ошибка, если файл отсутствует:
			std::cout << "Can't open file: " << filePath << "\n";
			documents.pop_back();	// Файла нету, а значит и место в векторе, для него не нужно.
		}

		// Помещаем весь файл в строку:
		std::string data((std::istreambuf_iterator<char>(currentDocument)), std::istreambuf_iterator<char>());
		currentDocument.close();
		documents.push_back(data);	// Добавляем содержимое файла в вектор.
	}

	return documents;	// Возвращаем список запросов.
}


int ConverterJSON::GetResponsesLimit() {
	// Открываем файл config.json:
	std::ifstream file(configFile);

	if (!file.is_open()) {
		throw std::runtime_error("config file is missing."); 	// Ошибка, если файл отсутствует.
	}

	nlohmann::json jsonData;	// Для хранение данных из файла JSON.
	file >> jsonData;				// Загружаем содержимое файла в jsonData.
	file.close();					// Со спокойной душой закрываем файл JSON.

	// Проверяем, есть ли поле "config":
	if (!jsonData.contains("config")) {
		throw std::runtime_error("config file is empty");	// Ошибка, если поле "config" отсутствует.
	}

	// Проверяем, указано ли значение для max_responses:
	if (!jsonData["config"].contains("max_responses")) {
		return 5;  // Если нет, возвращаем значение по умолчанию.
	}

	// Возвращаем полученное значение максимального количества запросов:
	return jsonData["config"]["max_responses"].get<int>();
}


std::vector<std::string> ConverterJSON::GetRequests() {
	// Открываем файл requests.json:
	std::ifstream file(requestsFile);

	if (!file.is_open()) {
		throw std::runtime_error("requests file is missing.");	// Ошибка, если файл отсутствует.
	}

	nlohmann::json jsonData;	// Для хранение данных из файла JSON.
	file >> jsonData;				// Загружаем содержимое файла в jsonData. 
	file.close();					// Со спокойной душой закрываем файл JSON.

	// Проверяем, есть ли поле "requests":
	if (!jsonData.contains("requests")) {
		throw std::runtime_error("requests file is empty");	// Ошибка, если поле "requests" отсутствует.
	}

	std::vector<std::string> requests;	// Вектор для хранения запросов.

	// Перебираем все элементы массива "requests" из файла:
	for (auto& req : jsonData["requests"]) {
		requests.push_back(req);	// И добавляем в наш вектор с запросами.
	}

	return requests;	// Возвращаем список запросов.
}


void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
	nlohmann::json resultJson;
	int requestID = 1;	// Текущий номер запроса.

	// Обработка каждого запроса
	for (auto& answer : answers) {
		nlohmann::json answerJson;	// Json c результатами поиска.

		// Если ответ пустой:
		if (answer.empty()) {
			answerJson["result"] = "false";
		}
		else {
			answerJson["result"] = "true";

			// Если найдено больше одного документа
			if (answer.size() > 1) {
				nlohmann::json relevanceData;

				for (const auto& currentAnswer : answer) {
					relevanceData.push_back({ {"docid", currentAnswer.first}, {"rank", currentAnswer.second} });
				}
				answerJson["relevance"] = relevanceData;
			}
			// Если найден только один документ
			else {
				answerJson["docid"] = answer[0].first;
				answerJson["rank"] = answer[0].second;
			}
		}

		// Создаём строковый поток для форматирования номера запроса:
		std::ostringstream formattedID;
		
		// Записываем в поток слово "request", затем номер запроса, 
		// выравнивая его по 3 символам и заполняя ведущими нулями
		formattedID << "request" << std::setw(3) << std::setfill('0') << requestID;

		// Преобразуем поток в строку и добавляем ответ в JSON с соответствующим requestID:
		resultJson["answers"][formattedID.str()] = answerJson;
		requestID++;	// Увеличиваем для следующего запроса.
	}

	// Открываем файл answers.json и сохраняем результат:
	std::ofstream file(answersFile);
	file << resultJson.dump(4);  // Сохраняем с отступами для читаемости.
}
