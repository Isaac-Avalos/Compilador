#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdio>

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

// Prototipos de función
void CompileError(std::string error_type, std::string desc);
int ColChar(char x);

int main(){
    std::cout << "hola mundo!";

    return 0;
}

// Definición de las declaraciones de prototipo de función
void CompileError(std::string error_type, std::string desc){
    std::cout << "[" << line << "]" << "[" << "]" << error_type << " " << desc << std::endl;
    cERR = true;
}

int ColChar(char x) {
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
        CompileError("Error Lexico", std::string(1, x) + " simbolo no valido en alfabeto");
        return ERR;
    }
}

std::pair<std::string, std::string> Scanner() {
    int estado = 0;
    char c = '';

    while (idx < entry.length() && estado != ERR && estado != ACP) {
        c = entry[idx];
        idx++;

        if (c == '\n') {
            col = 1;
            line++;
        } else {
            col++;
        }

        col = colCar(c);

        if (estado == 0 && col == 15) {
            continue;
        }

        if (col >= 0 && col <= 8 || col == 15) {
            if (col == 15 && estado != 12) {
                estado = ACP;
            }

            if (col >= 0 && col <= 8) {
                estado = transition_table[estado][col];
            }

            if (estado != ERR && estado != ACP && (col != 15 || (col == 15 && estado == 12))) {
                int estA = estado;
                lexema += c;
            }
        }
    }

    if (estado != ACP && estado != ERR) {
        int estA = estado;
    }

    std::string token = "Ntk";

    if (estado == ACP && col != 15) {
        idx--;
    }

    if (estado != ERR && estado != ACP) {
        int estA = estado;
    }

    if (key.find(lexema) != key.end()) {
        token = "Res";
    } else if (opl.find(lexema) != opl.end()) {
        token = "OpL";
    } else if (ctl.find(lexema) != ctl.end()) {
        token = "CtL";
    } else {
        token = "Ide";
    }

    int estA = estado;

    if (estA == 2) {
        token = "Ent";
    } else if (estA == 4) {
        token = "Dec";
    } else if (estA == 5) {
        token = "OpA";
    } else if (estA == 6) {
        token = "Del";
    } else if (estA == 7) {
        token = "OpS";
    } else if (estA >= 8 && estA <= 11) {
        token = "OpR";
    } else if (estA == 13) {
        token = "CtA";
    }

    if (token == "Ntk") {
        cerr << "estA=" << estA << "estado=" << estado << std::endl;
    }

    return make_pair(token, lexema);
}
