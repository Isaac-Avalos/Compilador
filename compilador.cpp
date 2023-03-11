#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <tuple> 

const int ERR = -1;
const int ACP = 99;
int idx = 0;
bool cERR = false;
std::string token = "";
std::string lexema = "";
bool check_bool_main = false;
int line = 1;
int col = 1;
std::vector<std::string> data_type = {"nulo", "ent", "dec", "palabra", "log"};
std::vector<std::string> logic_operator = {"no", "y", "o"};
std::vector<std::string> logic_controller = {"verdadero", "falso"};
std::vector<std::string> key = {"const", "desde", "si", "hasta", "mientras",
                                "ent", "dec", "regresa", "hacer", "palabra",
                                "log", "nulo", "sino", "incr", "imprime",
                                "imprimenl", "lee", "repite", "que"};
std::vector<std::string> arithmetic_operator = {"+", "-", "*", "/", "%", "^"};
std::vector<std::string> delimiter = {";", ",", "(", ")", "{", "}", "[", "]", ":"};
std::vector<std::string> uni_delimiter = {" ", "\t", "\n"};
std::string entry = "";

std::vector<std::vector<int>> transition_table = {
    // let  dig del opa <   >   =   .   "
    {   1,  2,  6,  5,  10, 8,  7, ERR, 12}, //0
    {   1,  1, ACP, ACP,ACP,ACP,ACP,ACP,ACP}, // 1
    {   ACP,2, ACP, ACP,ACP,ACP,ACP, 3, ACP}, // 2
    {   ERR, 4,ERR,ERR,ERR,ERR,ERR,ERR, ERR}, // 3
    {   ACP, 4, ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 4
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 5
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 6
    {   ACP,ACP,ACP,ACP,ACP,ACP, 9, ACP, ACP}, // 7
    {   ACP,ACP,ACP,ACP,ACP,ACP, 9, ACP, ACP}, // 8
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 9
    {   ACP,ACP,ACP,ACP,ACP, 11, 9, ACP, ACP}, // 10
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP}, // 11
    {   12,  12, 12, 12, 12, 12, 12, 12, 13}, // 12
    {   ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP, ACP} // 13
};

// Prototipos de funci�n
void compileError(std::string error_type, std::string desc);
int colChar(char x);
void constVars();
void params();

int main(){
    std::cout << "hola mundo!";

    return 0;
}

// Definici�n de las declaraciones de prototipo de funci�n
void compileError(std::string error_type, std::string desc){
    std::cout << "[" << line << "]" << "[" << "]" << error_type << " " << desc << std::endl;
    cERR = true;
}

int colChar(char x) {
    if (x == '_' || isalpha(x)) {
        return 0;
    } else if (isdigit(x)) {
        return 1;
    } else if (std::find(delimiter.begin(), delimiter.end(), std::string(1, x)) != delimiter.end()) {
        return 2;
    } else if (std::find(arithmetic_operator.begin(), arithmetic_operator.end(), std::string(1, x)) != arithmetic_operator.end()) {
        return 3;
    } else if (x == '<') {
        return 4;
    } else if (x == '>') {
        return 5;
    } else if (x == '=') {
        return 6;
    } else if (x == '.') {
        return 7;
    } else if (x == '"') {
        return 8;
    } else if (std::find(uni_delimiter.begin(), uni_delimiter.end(), std::string(1, x)) != uni_delimiter.end()) {
        return 15;
    } else {
        compileError("Error Lexico", std::string(1, x) + " simbolo no valido en alfabeto");
        return ERR;
    }
}

std::pair<std::string, std::string> scanner() {
    int status = 0;

    while (idx < entry.length() && status != ERR && status != ACP) {
        char c = entry[idx];
        idx++;

        if (c == '\n') {
            col = 1;
            line++;
        } else {
            col++;
        }

        col = colChar(c);

        if (status == 0 && col == 15) {
            continue;
        }

        if (col >= 0 && col <= 8 || col == 15) {
            if (col == 15 && status != 12) {
                status = ACP;
            }

            if (col >= 0 && col <= 8) {
                status = transition_table[status][col];
            }

            if (status != ERR && status != ACP && (col != 15 || (col == 15 && status == 12))) {
                int estA = status;
                lexema += c;
            }
        }
    }

    if (status != ACP && status != ERR) {
        int estA = status;
    }

    std::string token = "Ntk";

    if (status == ACP && col != 15) {
        idx--;
    }

    if (status != ERR && status != ACP) {
        int estA = status;
    }

    if (std::find(key.begin(), key.end(), lexema) != key.end()) {
        token = "Res";
    } else if (std::find(logic_operator.begin(), logic_operator.end(), lexema) != logic_operator.end()) {
        token = "OpL";
    } else if (std::find(logic_controller.begin(), logic_controller.end(), lexema) != logic_controller.end()) {
        token = "CtL";
    } else {
        token = "Ide";
    }

    int current_status = status;

    if (current_status == 2) {
        token = "Ent";
    } else if (current_status == 4) {
        token = "Dec";
    } else if (current_status == 5) {
        token = "OpA";
    } else if (current_status == 6) {
        token = "Del";
    } else if (current_status == 7) {
        token = "OpS";
    } else if (current_status >= 8 && current_status <= 11) {
        token = "OpR";
    } else if (current_status == 13) {
        token = "CtA";
    }

    if (token == "Ntk") {
        std::cout << "current status=" << current_status << "status=" << status << std::endl;
    }

    return make_pair(token, lexema);
}

void constVars(){
    std::tie(token,lexema) = scanner();
}

void params(){
    std::tie(token,lexema) = scanner();
}