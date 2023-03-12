#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <tuple> 

const int ERR = -1;
const int ACP = 99;
unsigned int idx = 0;
bool cERR = false;
std::string tok = "";
std::string lex = "";
bool check_bool_main = false;
int line = 1;
int colu = 0;
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
std::string entry;

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

// Prototipos de funcion
void compileError(std::string error_type, std::string desc);
int colChar(char x);
std::pair<std::string, std::string> scanner();
void opno();
void opy();
void opo();
void constVars();
void params();
void leer();
void imprime();
void imprimenl();
void desde();
void mientras();
void si();
void repite();
void lmp();
void regresa();
void comand();
void blockCommand();
void statements();
void blockFunction();
void functions();
void program();
void parser();

int main(){

    std::string arche;
    std::cout << "File (.icc) [.]=salir: ";
    std::cin >> arche;
    
    if(arche == ".") return 0;
    
    std::ifstream archivo(arche);

    std::string linea;
    while (std::getline(archivo,linea)){
        entry += linea + "\n";
    }

    std::cout << entry << std::endl;
    parser();
    if(!cERR) std::cout << "El Programa se ha Compilado con Exito" << std::endl;

    return 0;
}

// Definici�n de las declaraciones de prototipo de funcion
void compileError(std::string error_type, std::string desc){
    std::cout << "[" << line << "]" << "[" << colu << "]" << error_type << " " << desc << std::endl;
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
    int col = 0;
    std::string lexema = "";

    while (idx < entry.length() && status != ERR && status != ACP) {
        char c = entry[idx];
        idx++;

        if (c == '\n') {
            colu = 0;
            line++;
        } else {
            col++;
        }

        col = colChar(c);

        if ((status == 0) && (col == 15)) {
            continue;
        }

        if ((col >= 0 && col <= 8) || (col == 15)) {
            if (col == 15 && status != 12) {
                status = ACP;
            }

            if (col >= 0 && col <= 8) {
                status = transition_table[status][col];
            }

            if (status != ERR && status != ACP && (col != 15 || (col == 15 && status == 12))) {
                int current_status = status;
                lexema += c;
            }
            if(c != '\n'){
                colu++;
            }
        }
    }

    if (status != ACP && status != ERR) {
        int current_status = status;
    }

    std::string token = "Ntk";

    if (status == ACP && col != 15) {
        idx--;
        colu--;
    }

    if (status != ERR && status != ACP) {
        int current_status = status;
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

    return std::make_pair(token, lexema);
}

void opno(){

}

void opy(){
    std::string opr = "y";
    while(opr == "y"){
        opno();
        opr = lex;
    }
}

void expr(){
    std::string opr = "o";
    while(opr == "o"){
        opy();
        opr = lex;
    }
}

void constVars(){
    std::tie(tok,lex) = scanner();
}

void params(){
    std::tie(tok,lex) = scanner();
}

void leer() {
    // Código para leer datos desde la entrada estándar
}

void imprime() {
    // Código para imprimir un mensaje en la salida estándar sin salto de línea
}

void imprimenl() {
    // Código para imprimir un mensaje en la salida estándar con salto de línea
}

void desde() {
    //void desde(int inicio, int fin, int paso);
    // Código para realizar un ciclo "desde" con inicio, fin y paso
}

void mientras() {
    //void mientras(bool condicion);
    // Código para realizar un ciclo "mientras" con una condición
}

void si() {
    //void si(bool condicion);
    // Código para realizar una estructura "si" con una condición
}

void repite() {
    // Código para realizar un ciclo "repite"
}

void lmp() {
    // Código para mostrar el contenido de una lista
}

void regresa() {
    // Código para devolver un valor de una función
}

void comando() {
    if (lex == "lee") {
        leer();
    } else if (lex == "imprime") {
        imprime();
    } else if (lex == "imprimenl") {
        imprimenl();
    } else if (lex == "desde") {
        desde();
    } else if (lex == "mientras") {
        mientras();
    } else if (lex == "si") {
        si();
    } else if (lex == "repite") {
        repite();
    } else if (lex == "lmp") {
        lmp();
    } else if (lex == "regresa") {
        regresa();
    } else {
        compileError("Error de Sintaxis", "comando no definido " + lex);
    }
}

void blockCommand() {
    std::tie(tok, lex) = scanner();
    if (lex != ";" && lex != "{") {
        comando();
        std::tie(tok, lex) = scanner();
        if (lex != ";") compileError("Error de Sintaxis", "se esperaba ; y llego " + lex);
    } else if (lex == "{") {
        statements();
        if (lex != "}") compileError("Error de Sintaxis", "se esperaba cerrar block \"}\" y llego " + lex);
        std::tie(tok, lex) = scanner();
    }
}

void statements() {
    std::tie(tok, lex) = scanner();
    while (lex != ";") {
        if (lex != ";") comando();
        std::tie(tok, lex) = scanner();
        if (lex != ";") compileError("Error de Sintaxis", "se esperaba ; y llego " + lex);
        std::tie(tok, lex) = scanner();
    }
}

void blockFunction() {
    if (lex != "{") compileError("Error de Sintaxis", "se esperaba abrir \"{\"");
    std::tie(tok, lex) = scanner();
    if (lex != "}") statements();
    if (lex != "}") compileError("Error de Sintaxis", "se esperaba cerrar \"}\"");
}

void functions() {
    std::cout << "Entro a Funcs" << std::endl;;
    if (!(std::find(data_type.begin(), data_type.end(), lex) != data_type.end())) {
        compileError("Error Sintactico", "Se esperaba tipo" + lex);
    }
    std::tie(tok, lex) = scanner();
    if (tok != "Ide") {
        compileError("Error Sintaxis", "Se esperaba Nombre Funcion y llego " + lex);
    }
    if (check_bool_main) {
        compileError("Error de Semantica", "La Funcion Principal ya esta definida");
    }
    if (lex == "principal") {
        check_bool_main = true;
    }
    std::tie(tok, lex) = scanner();
    if (lex != "(") {
        compileError("Error de Sintaxis", "Se esperaba parentesis abierto \"(\"");
    }
    std::tie(tok, lex) = scanner();
    if (lex != ")") {
        params();
    }
    if (lex != ")") {
        compileError("Error de Sintaxis", "Se esperaba parentesis cerrado \")\"");
    }
    std::tie(tok, lex) = scanner();
    blockFunction();
}

void program(){
    while (!entry.empty() && idx < entry.size()) {
        constVars();
        functions();
    }
}

void parser(){
    program();
}