#include <iostream>
#include <string>
#include <vector>
#include "eval.h"
#include "check_correct.h"

int main(int argc, char* argv[]) {
    std::map<std::string, int64_t> db;
    db["pi"] = 3;
    db["e"] = 2;



    try{
        ExamOnInCorrect("*");
    } catch (const std::invalid_argument& ex) {
        std::cout << "Поймано исключение: " << ex.what() << std::endl;
    }

    auto result = EvaluateExpression("27+6.5*2+2.5*2.5");
    (void)result;
}