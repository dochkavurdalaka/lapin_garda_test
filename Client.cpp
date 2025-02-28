#include <iostream>
#include <algorithm>
#include "json.hpp"
#include "httplib.h"

using json = nlohmann::json;

void SendCommand(httplib::Client& client, const json& msg) {
    std::string message = msg.dump();
    std::cout << message << "\n";
    auto res = client.Post("/message", message, "text/plain");
    if (res && res->status == 200) {
        std::cout << "Server response: " << res->body << "\n";
        json answer = json::parse(res->body);
        if (answer.contains("res")) {
            std::cout << "response: " + std::string(answer["res"]) << "\n";
        } else if (answer.contains("err")) {
            std::cout << "error: " + std::string(answer["err"]) << "\n";
        } else {
            std::cout << "empty response" << "\n";
        }

    } else {
        std::cerr << "Failed to send message or receive response." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }

    if (args.size() < 3) {
        std::cout << "not enough arguments in command\n";
        return 0;
    }

    args[2].erase(std::remove(args[2].begin(), args[2].end(), ' '), args[2].end());

    httplib::Client client("http://0.0.0.0:8080");
    json msg;
    if (args[1] == "-c") {
        msg["cmd"] = args[2];
    } else if (args[1] == "-e") {
        msg["exp"] = args[2];
    }
    SendCommand(client, msg);
    return 0;
}
