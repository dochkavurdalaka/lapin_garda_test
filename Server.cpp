#include <iostream>
#include <string>
#include "eval.h"
#include "json.hpp"
#include "httplib.h"
#include "check_correct.h"

using json = nlohmann::json;

std::vector<std::string> Split(const std::string& string,
                               const std::vector<std::string>& delimiters = {" "});

std::string GenerateAnswer(const httplib::Request& req,
                           std::unordered_map<std::string, NumberType>& database) {
    std::string message = req.body;
    json msg = json::parse(message);

    std::string result;
    std::string tag;
    if (msg.contains("cmd")) {
        if (msg["cmd"] == "echo") {
            tag = "res";
            result = "echo";
        } else if (msg["cmd"] == "clean") {
            database.clear();
            tag = "";
            result = "";
        } else {
            tag = "err";
            result = "unknown command";
        }
    } else if (msg.contains("exp")) {
        std::vector<std::string> instructions = std::move(Split(msg["exp"], {";"}));
        try {
            for (size_t i = 0; i < instructions.size(); ++i) {
                if (instructions[i].find('=') != std::string::npos) {
                    std::vector<std::string> parts = std::move(Split(instructions[i], {"="}));
                    ExamOnInCorrect(parts[1]);
                    database[parts[0]] = EvaluateExpression(parts[1], database);
                    result = "";
                    tag = "";
                } else {
                    ExamOnInCorrect(instructions[i]);
                    NumberType res_of_eval = EvaluateExpression(instructions[i], database);
                    tag = "res";
                    if (std::holds_alternative<int64_t>(res_of_eval)) {
                        result = std::to_string(std::get<int64_t>(res_of_eval));
                    } else {
                        result = std::to_string(std::get<double>(res_of_eval));
                    }
                }
            }
        } catch (const std::invalid_argument& ex) {
            result = "get exception : ";
            result += ex.what();
            tag = "err";
        };
    }
    json answer;
    answer[tag] = result;
    return answer.dump();
}

int main() {

    // Создаем HTTP-сервер
    httplib::Server svr;

    std::unordered_map<std::string, NumberType> database;

    svr.Post("/message", [&database](const httplib::Request& req, httplib::Response& res) {
        // Получаем тело запроса (сообщение от клиента)
        res.set_content(GenerateAnswer(req, database), "text/plain");

        // Выводим сообщение в консоль сервера
        std::cout << "Received message: " << req.body << std::endl;
    });

    std::cout << "Server started at http://0.0.0.0:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}

// функция, которая делает сплит строки, разделители передаются в векторе delimiters
std::vector<std::string> Split(const std::string& string,
                               const std::vector<std::string>& delimiters) {
    if (string.empty()) {
        return {};
    }

    for (size_t i = 0; i < delimiters.size(); ++i) {
        if (string == delimiters[i]) {
            return {};
        }
    }

    std::string::size_type iter = 0;
    std::string::size_type n = 0;

    std::string delimiter = delimiters[0];
    n = string.find(delimiters[0]);
    for (size_t i = 1; i < delimiters.size(); ++i) {
        std::string::size_type find_result = string.find(delimiters[i]);
        if (find_result < n) {
            n = find_result;
            delimiter = delimiters[i];
        }
    }

    std::vector<std::string> result;
    while (n != std::string::npos) {
        if (iter != n) {
            result.push_back(string.substr(iter, n - iter));
        }
        iter = n + delimiter.size();
        delimiter = delimiters[0];
        n = string.find(delimiters[0], iter);
        for (size_t i = 1; i < delimiters.size(); ++i) {
            std::string::size_type find_result = string.find(delimiters[i], iter);
            if (find_result < n) {
                n = find_result;
                delimiter = delimiters[i];
            }
        }
    }
    if (iter != string.size()) {
        result.push_back(string.substr(iter, string.size() - iter));
    }
    return result;
}