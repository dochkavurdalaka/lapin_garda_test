#include <iostream>
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
        std::cout << answer["res"] << "\n";

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
        std::cout << "not enough arguments in command";
        return 0;
    }

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

// json j;
// j["name"] = "John";
// auto json_str = j.dump(0);

// json s = json::parse(json_str);
// std::string result = s["name"];
// std::cout << result + '\n';

// std::cout << EvaluateExpression("(2+3)*(4+5*1234)")<< "\n";