#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	// ��������� ���� config.json:
	std::ifstream file("config.json");
	if (!file.is_open()) {
		// ������, ���� ���� �����������:
		throw std::runtime_error("config file is missing.");
	}
	
	nlohmann::json jsonData;	// ��� �������� ������ �� ����� JSON.
	file >> jsonData;				// ��������� ���������� ����� � jsonData. 
	file.close();					// �� ��������� ����� ��������� ���� JSON.

	// ���������, ���� �� ���� "config":
	if (!jsonData.contains("config")) {
		// ������, ���� ���� "config" �����������:
		throw std::runtime_error("config file is empty");
	}

	// ���������, ���������� �� ������ "files" � JSON
	if (!jsonData.contains("files")) {
		// ������, ���� ������� ���:
		throw std::runtime_error("key 'files' is missing");
	}

	auto& fileList = jsonData["files"];
	std::vector<std::string> documents;	// ������ ��� �������� ����������� ������.
	documents.reserve(fileList.size()); // ����������� �����

	// ���������� ��� �������� ������� "files" �� �����:
	for (auto& filePath : fileList) {
		std::ifstream currentDocument(filePath.get<std::string>());

		// ��������� �����, ��������� � JSON:
		if (!currentDocument.is_open()) {
			// ������, ���� ���� �����������:
			std::cout << "Can't open file: " << filePath << "\n";
			documents.pop_back();	// ����� ����, � ������ � ����� � �������, ��� ���� �� �����.
		}

		// �������� ���� ���� � ������:
		std::string data((std::istreambuf_iterator<char>(currentDocument)), std::istreambuf_iterator<char>());
		currentDocument.close();
		documents.push_back(data);	// ��������� ���������� ����� � ������.
	}

	return documents;	// ���������� ������ ��������.
}


int ConverterJSON::GetResponsesLimit() {
	// ��������� ���� config.json:
	std::ifstream file(configFile);

	if (!file.is_open()) {
		throw std::runtime_error("config file is missing."); 	// ������, ���� ���� �����������.
	}

	nlohmann::json jsonData;	// ��� �������� ������ �� ����� JSON.
	file >> jsonData;				// ��������� ���������� ����� � jsonData.
	file.close();					// �� ��������� ����� ��������� ���� JSON.

	// ���������, ���� �� ���� "config":
	if (!jsonData.contains("config")) {
		throw std::runtime_error("config file is empty");	// ������, ���� ���� "config" �����������.
	}

	// ���������, ������� �� �������� ��� max_responses:
	if (!jsonData["config"].contains("max_responses")) {
		return 5;  // ���� ���, ���������� �������� �� ���������.
	}

	// ���������� ���������� �������� ������������� ���������� ��������:
	return jsonData["config"]["max_responses"].get<int>();
}


std::vector<std::string> ConverterJSON::GetRequests() {
	// ��������� ���� requests.json:
	std::ifstream file(requestsFile);

	if (!file.is_open()) {
		throw std::runtime_error("requests file is missing.");	// ������, ���� ���� �����������.
	}

	nlohmann::json jsonData;	// ��� �������� ������ �� ����� JSON.
	file >> jsonData;				// ��������� ���������� ����� � jsonData. 
	file.close();					// �� ��������� ����� ��������� ���� JSON.

	// ���������, ���� �� ���� "requests":
	if (!jsonData.contains("requests")) {
		throw std::runtime_error("requests file is empty");	// ������, ���� ���� "requests" �����������.
	}

	std::vector<std::string> requests;	// ������ ��� �������� ��������.

	// ���������� ��� �������� ������� "requests" �� �����:
	for (auto& req : jsonData["requests"]) {
		requests.push_back(req);	// � ��������� � ��� ������ � ���������.
	}

	return requests;	// ���������� ������ ��������.
}


void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
	nlohmann::json resultJson;
	int requestID = 1;	// ������� ����� �������.

	// ��������� ������� �������
	for (auto& answer : answers) {
		nlohmann::json answerJson;	// Json c ������������ ������.

		// ���� ����� ������:
		if (answer.empty()) {
			answerJson["result"] = "false";
		}
		else {
			answerJson["result"] = "true";

			// ���� ������� ������ ������ ���������
			if (answer.size() > 1) {
				nlohmann::json relevanceData;

				for (const auto& currentAnswer : answer) {
					relevanceData.push_back({ {"docid", currentAnswer.first}, {"rank", currentAnswer.second} });
				}
				answerJson["relevance"] = relevanceData;
			}
			// ���� ������ ������ ���� ��������
			else {
				answerJson["docid"] = answer[0].first;
				answerJson["rank"] = answer[0].second;
			}
		}

		// ������ ��������� ����� ��� �������������� ������ �������:
		std::ostringstream formattedID;
		
		// ���������� � ����� ����� "request", ����� ����� �������, 
		// ���������� ��� �� 3 �������� � �������� �������� ������
		formattedID << "request" << std::setw(3) << std::setfill('0') << requestID;

		// ����������� ����� � ������ � ��������� ����� � JSON � ��������������� requestID:
		resultJson["answers"][formattedID.str()] = answerJson;
		requestID++;	// ����������� ��� ���������� �������.
	}

	// ��������� ���� answers.json � ��������� ���������:
	std::ofstream file(answersFile);
	file << resultJson.dump(4);  // ��������� � ��������� ��� ����������.
}
