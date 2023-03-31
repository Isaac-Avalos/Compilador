#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <tuple>

using namespace std;

// Todas las variables necesarias

string cTipo[] = {"E=E", "A=A", "R=R", "L=L", "R=E",
        "E+E", "E+R", "R+E", "R+R", "A+A",
        "E-E", "E-R", "R-E", "R-R",
        "E*E", "E*R", "R*E", "R*R",
        "E/E", "E/R", "R/E", "R/R",
        "E\37E", "-E", "-R",
        "LyL", "LoL", "noL",
        "E>E", "R>E", "E>R", "R>R",
        "E<E", "R<E", "E<R", "R<R",
        "E>=E", "R>=E", "E>=R", "R>=R",
        "E<=E", "R<=E", "E<=R", "R<=R",
        "E<>E", "R<>E", "E<>R", "R<>R", "A<>A",
        "E==E", "R==E", "E==R", "R==R", "A==A"
};

string tipoR[] = {"",  "",  "",  "",  "",
        "E", "R", "R", "R", "A",
        "E", "R", "R", "R",
        "E", "R", "R", "R",
        "R", "R", "R", "R",
                                          "E", "E", "R",
                                          "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L",
                                          "L", "L", "L", "L", "L",
                                          "L", "L", "L", "L", "L",
                                          "L", "L", "L", "L", "L"
};

const int ERR = -1;
const int ACP = 99;
int idx = 0;
bool cERR = false;
string token = "";
string lexema = "";
bool boolPrinc = false;
int renglon = 1;
int columna = 0;
string tipo[5] = {"nulo", "entero", "decimal", "palabra", "logico"};
string opl[3] = {"no", "y", "o"};
string ctl[2] = {"verdadero", "falso"};
string key[20] = {"constante", "desde", "si", "hasta", "mientras", "entero", "decimal", "regresa", "hacer",
                  "palabra", "logico", "nulo", "sino", "incr" "imprime", "imprimenl", "lee", "repite", "que"};
char opAritmetico[6] = {'+', '-', '*', '/', '%', '^'};
char delimitador[9] = {';', ',', '(',')', '{', '}', '[', ']', ':'};
char delUniversal[3] = {' ', '\t', '\n'};
string opRelacional[5] = {"<", ">", "<=", ">=", "<>"};
string tkCts[4] = {"Ent", "Dec", "CtA", "CtL"};
string entrada = "";
int matrizTransicion[14][9] = {
    // let  dig del   opa   <    >    =     .    "
    {   1,   2,   6,   5,   10,  8,   7,   ERR, 12 },  //0
    {   1,   1,   ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 1
    {   ACP, 2,   ACP, ACP, ACP, ACP, ACP, 3,   ACP},  // 2
    {   ERR, 4,   ERR, ERR, ERR, ERR, ERR, ERR, ERR},  // 3
    {   ACP, 4,   ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 4
    {   ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 5
    {   ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 6
    {   ACP, ACP, ACP, ACP, ACP, ACP, 9,   ACP, ACP},  // 7
    {   ACP, ACP, ACP, ACP, ACP, ACP, 9,   ACP, ACP},  // 8
    {   ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 9
    {   ACP, ACP, ACP, ACP, ACP, 11,  9 ,  ACP, ACP},  // 10
    {   ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP},  // 11
    {   12,  12,  12,  12,  12,  12,  12,  12,  13 },  // 12
    {   ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP}   // 13
};

// Prototipado de funciones

void erra(string terr, string desc);
int colCar(char x);
pair<string, string> scanner();
void cte();
void termino();
void signo();
void expo();
void multi();
void suma();
void oprel();
void opno();
void opy();
void expr();
void constVars();
void params();
void gpoExp();
void leer(); // no
void imprime(); // no
void imprimenl(); // no
void desde(); // no
void mientras(); // no
void si(); // no
void repite(); // no
void lmp(); // no
void regresa(); // no
void comando();
void blkcmd();
void estatutos();
void blkFunc();
void funcs();
void prgm();
void parser();

// -------------------------------------------------------------
// Funcion principal
int main() {
    string arche = "";
    cout << "Archivo (.icc) [.] = salir: ";
    cin >> arche;
    if (arche == ".") exit(0);
    ifstream archivo(arche);

    // Carga de archivo en entrada
    string entrada = "";
    string linea = "";
    while (getline(archivo, linea)) {
        entrada += linea + "\n";
    }

    cout << entrada << endl;
    parser();
    if (!cERR) cout << "Programa COMPILO con EXITO" << endl;

    return 0;
}
// Funcion principal
// -------------------------------------------------------------

// Definicion de las funciones

void erra(string terr, string desc) {
    cERR;
    cout << '[' << renglon << ']' << '[' << columna << ']' << terr << ": " << desc << endl;
    cERR = true;
}

int colCar(char x) {
    if (x == '_' || isalpha(x))
        return 0;
    if (isdigit(x))
        return 1;
    if (find(delimitador, delimitador+9, x) != delimitador+9)
        return 2;
    if (find(opAritmetico, opAritmetico+6, x) != opAritmetico+6)
        return 3;
    if (x == '<')
        return 4;
    if (x == '>')
        return 5;
    if (x == '=')
        return 6;
    if (x == '.')
        return 7;
    if (x == '"')
        return 8;
    if (find(delUniversal, delUniversal+3, x) != delUniversal+3)
        return 15;

    erra("Error lexemaico", x + " simbolo no valido en Alfabeto");
    return ERR;
}

pair<string, string> scanner() {
    // entrada, ERR, ACP, idx, ren, colu
    int estado = 0;
    string lexema = "";
    int col = 0;
    while (idx < entrada.length() && estado != ERR && estado != ACP) {
        char c = entrada[idx];
        idx += 1;

        // Para brincar de renglon
        if (c == '\n') {
            columna = 0;
            renglon += 1;
        }

        col = colCar(c);
        if (estado == 0 && col == 15) {
            continue;
        }
        if (col >= 0 && col <= 8 || col == 15) {
            if (col == 15 && estado != 12) {
                estado = ACP;
            }
            if (col >=0 && col <= 8) {
                estado = matrizTransicion[estado][col];
            }
            if (estado != ERR && estado != ACP && col != 15 || col == 15 && estado == 12) {
                int estA = estado;
                lexema += c;
            }

            if (c != '\n')
                columna += 1;
        }
    }

    if (estado != ACP && estado != ERR)
        int estA = estado;
    string token = "Ntk";

    if (estado == ACP && col != 15) {
        idx -= 1;
        columna -= 1;
    }

    if (estado != ERR && estado != ACP) {
        int estA = estado;
    }

    if (find(key, key+20, lexema) != key+20)
        token = "Res";
    else if (find(opl, opl+3, lexema) != opl+3)
        token = "OpL";
    else if (find(ctl, ctl+2, lexema) != ctl+2)
        token = "CtL";
    else token = "Ide";

    int estA = estado;

    if (estA == 2)
        token = "Ent";
    else if (estA == 4)
        token = "Dec";
    else if (estA == 5)
        token = "OpA";
    else if (estA == 6)
        token = "Del";
    else if (estA == 7)
        token = "OpS";
    else if (estA >= 8 && estA <= 11)
        token = "OpR";
    else if (estA == 13)
        token = "CtA";

    if (token == "Ntk") {
        cout << "estA=" << estA << "estado=" << estado << endl;
    }

    return make_pair(token, lexema);
}

void cte() {
    if (find(tkCts, tkCts+4, token) == tkCts+4) {
        erra("Error de sintaxis", "se esperaba Cte y llego " + lexema);
    }
}

void termino() {
    if (lexema != "(" && token != "Ide" && token != "CtA" && token != "CtL" && token != "Ent" && token != "Dec") {
        tie(token, lexema) = scanner();
    }
    if (lexema == "(") {
        tie(token, lexema) = scanner();
        expr();
        if (lexema != ")") {
            erra("Error de Sintaxis", "se espera cerrar ) y llego " + lexema);
        }
    }
    else if (token == "Ide") {
        tie(token, lexema) = scanner();
        if (lexema == "[") {
            tie(token, lexema) = scanner();
            expr();
            if (lexema != "]") {
                erra("Error Sintaxis", "se esperaba cerrar ] y llego " + lexema);
            }
        }
    }
    else if (token == "CtL" || token == "CtA" || token == "Dec" || token == "Ent") {
        cte();
    }
    if (lexema != ")") {
        tie(token, lexema) = scanner();
    }
}

void signo() {
    if (lexema == "-") {
        tie(token, lexema) = scanner();
    }
    termino();
}

void expo() {
    string opr = "^";
    while (opr == "^") {
        signo();
        opr = lexema;
    }
}

void multi() {
    string opr = "*";
    while (opr == "*" || opr == "/" || opr == "%") {
        expo();
        opr = lexema;
    }
}

void suma() {
    string opr = "+";
    while (opr == "+" || opr == "-") {
        multi();
        opr = lexema;
    }
}

void oprel() {
    string opr = "<";
    while (find(opRelacional, opRelacional+5, opr) != opRelacional+5) {
        suma();
        opr = lexema;
    }
}

void opno() {
    if (lexema == "no") {
        tie(token, lexema) = scanner();
    }
    oprel();
}

void opy() {
    string opr = "y";
    while (opr == "y") {
        opno();
        opr = lexema;
    }
}

void expr() {
    string opr = "o";
    while (opr == "o") {
        opy();
        opr = lexema;
    }
}

void constVars() {
    tie(token, lexema) = scanner();
}

void params() {
    tie(token, lexema) = scanner();
}

void gpoExp() {
    if (lexema != ")") {
        string delimitador = ",";
        while (delimitador == ",") {
            if (lexema == ",") {
                tie(token, lexema) = scanner();
            }
            expr();
            delimitador = lexema;
        }
    }
}

void leer() {
    tie(token, lexema) = scanner();
    if (lexema != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lexema);
    }
    tie(token, lexema) = scanner();
    if (token != "Ide") {
        erra("Error de Sintaxis", "se esperaba un identificador y llego " + lexema);
    }
    string var = lexema; // store the variable name
    tie(token, lexema) = scanner();
    if (lexema != ")") {
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lexema);
    }
}

void imprime() {
    tie(token, lexema) = scanner();
    if (lexema != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lexema);
    }
    tie(token, lexema) = scanner();
    if (lexema != ")")
        gpoExp();
    if (lexema != ")")
        tie(token, lexema) = scanner();
    if (lexema != ")")
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lexema);
}

void imprimenl() {
    tie(token, lexema) = scanner();
    if (lexema != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lexema);
    }

    tie(token, lexema) = scanner();
    if (lexema != ")") gpoExp();
    if (lexema != ")") tie(token, lexema) = scanner();
    if (lexema != ")") {
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lexema);
    }
}

void desde() {

}

void mientras() {

}

void si() {

}

void repite() {

}

void lmp() {

}

void regresa() {

}

void comando() {
    //if (token == "Ide") asigLfunc();
    if (lexema == "lee") leer();
    else if (lexema == "imprime") imprime();
    else if (lexema == "imprimenl") imprimenl();
    else if (lexema == "desde") desde();
    else if (lexema == "mientras") mientras();
    else if (lexema == "si") si();
    else if (lexema == "repite") repite();
    else if (lexema == "lmp") lmp();
    else if (lexema == "regresa") regresa();
    else erra("Error de Sintaxis", "comando no definido " + lexema);
    tie(token, lexema) = scanner();
}

void blkcmd() {
    tie(token, lexema) = scanner();
    if (lexema != ";" && lexema != "{") {
        comando();
        tie(token, lexema) = scanner();
        if (lexema != ";") erra("Error de Sintaxis", "se esperaba ; y llego " + lexema);
    }
    else if (lexema == "{") {
        estatutos();
        if (lexema != "}") erra("Error de Sintaxis", "se esperaba cerrar block \"}\" y llego " + lexema);
    }
}

void estatutos() {
    string cbk = "{";
    while (cbk != "}") {
        if (lexema != ";") comando();
        if (lexema != ";") erra("Error de Sintaxis", "se esperaba ; y llego " + lexema);
        tie(token, lexema) = scanner();
        cbk = lexema;
    }
}

void blkFunc() {
    if (lexema != "{") erra("Error de Sintaxis", "se esperaba abrir \"{\" y llego " + lexema);
    tie(token, lexema) = scanner();
    if (lexema != "}") estatutos();
    if (lexema != "}") erra("Error de Sintaxis", "se esperaba cerrar \"}\" y llego " + lexema);
}

void funcs() {
    if (find(tipo, tipo+5, lexema) == tipo+5) {
        erra("Error Sintactico", "Se esperaba tipo");
    }
    tie(token, lexema) = scanner();
    if (token != "Ide") erra("Error Sintaxis", "Se esperaba Nombre Funcion y llego " + lexema);
    if (boolPrinc) erra("Error de Semantica", "la Funcion Principal ya esta definida");
    if (lexema == "principal") boolPrinc = true;
    tie(token, lexema) = scanner();
    if (lexema != "(") erra("Error de Sintaxis", "se esperaba parenglontisis abierto \"(\" y llego " + lexema);
    tie(token, lexema) = scanner();
    if (lexema != ")") params();
    if (lexema != ")") erra("Error de Sintaxis", "se esperaba parenglontisis cerrado \")\"");
    tie(token, lexema) = scanner();
    blkFunc();
}

void prgm() {
    while (entrada.length() > 0 && idx < entrada.length()) {
        constVars();
        funcs();
    }
}

void parser() {
    prgm();
}
