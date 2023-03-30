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
const int ERR = -1;
const int ACP = 99;
int idx = 0;
bool cERR = false;
string tok = "";
string lex = "";
bool bPrinc = false;
int ren = 1;
int colu = 0;
string tipo[5] = {"nulo", "entero", "decimal", "palabra", "logico"};
string opl[3] = {"no", "y", "o"};
string ctl[2] = {"verdadero", "falso"};
string key[20] = {"constante", "desde", "si", "hasta", "mientras", "entero", "decimal", "regresa", "hacer",
      "palabra", "logico", "nulo", "sino", "incr" "imprime", "imprimenl", "lee", "repite", "que"};
char opar[6] = {'+', '-', '*', '/', '%', '^'};
char deli[9] = {';', ',', '(',')', '{', '}', '[', ']', ':'};
char delu[3] = {' ', '\t', '\n'};
string opRl[5] = {"<", ">", "<=", ">=", "<>"};
string tkCts[4] = {"Ent", "Dec", "CtA", "CtL"};
string entrada = "";
int matran[14][9] = {
    //let  dig  del  opa   <    >    =    .   "
    {1,   2,   6,   5,    10,  8,   7,  ERR, 12}, //0
    {1,   1,   ACP, ACP, ACP, ACP, ACP, ACP,ACP}, //1
    {ACP, 2,   ACP, ACP, ACP, ACP, ACP,  3, ACP}, //2
    {ERR, 4,   ERR, ERR, ERR, ERR, ERR, ERR,ERR}, //3
    {ACP, 4,   ACP, ACP, ACP, ACP, ACP, ACP,ACP}, //4
    {ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP}, //5
    {ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP}, //6
    {ACP, ACP, ACP, ACP, ACP, ACP,  9 ,ACP ,ACP}, //7
    {ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,9 ,ACP ,ACP}, //8
    {ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP}, //9
    {ACP ,ACP ,ACP ,ACP ,ACP ,11 ,9 ,ACP ,ACP}, //10
    {ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP}, //11
    {12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,13}, //12
    {ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP ,ACP} //13
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
    cout << '[' << ren << ']' << '[' << colu << ']' << terr << desc << endl;
    cERR = true;
}

int colCar(char x) {
    if (x == '_' || isalpha(x)) return 0;
    if (isdigit(x)) return 1;
    if (find(deli, deli+9, x) != deli+9) return 2;
    if (find(opar, opar+6, x) != opar+6) return 3;
    if (x == '<') return 4;
    if (x == '>') return 5;
    if (x == '=') return 6;
    if (x == '.') return 7;
    if (x == '"') return 8;
    if (find(delu, delu+3, x) != delu+3) return 15;
    erra("Error Lexico", x + " simbolo no valido en Alfabeto");
    return ERR;
}

pair<string, string> scanner() {
    int estado = 0;
    string lexema = "";
    int col = 0;
    while (idx < entrada.length() && estado != ERR && estado != ACP) {
        char c = entrada[idx];
        idx = idx + 1;
        if (c == '\n') {
            colu = 0;
            ren = ren + 1;
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
                estado = matran[estado][col];
            }
            if (estado != ERR && estado != ACP && col != 15 || col == 15 && estado == 12) {
                int estA = estado;
                lexema = lexema + c;
            }

            if (c != '\n') colu = colu + 1;
        }
    }
    if (estado != ACP && estado != ERR) int estA = estado;
    string token = "Ntk";
    if (estado == ACP && col != 15) {
        idx = idx - 1;
        colu = colu - 1;
    }

    if (estado != ERR && estado != ACP) {
        int estA = estado;
    }

    if (find(key, key+20, lexema) != key+20) token = "Res";
    else if (find(opl, opl+3, lexema) != opl+3) token = "OpL";
    else if (find(ctl, ctl+2, lexema) != ctl+2) token = "CtL";
    else token = "Ide";

    int estA = estado;

    if (estA == 2) token = "Ent";
    else if (estA == 4) token = "Dec";
    else if (estA == 5) token = "OpA";
    else if (estA == 6) token = "Del";
    else if (estA == 7) token = "OpS";
    else if (estA >= 8 && estA <= 11) token = "OpR";
    else if (estA == 13) token = "CtA";

    if (token == "Ntk") {
        cout << "estA=" << estA << "estado=" << estado << endl;
    }

    return make_pair(token, lexema);
}

void cte() {
    if (find(tkCts, tkCts+4, tok) == tkCts+4) {
        erra("Error de sintaxis", "se esperaba Cte y llego " + lex);
    }
}

void termino() {
    if (lex != "(" && tok != "Ide" && tok != "CtA" && tok != "CtL" && tok != "Ent" && tok != "Dec") {
        tie(tok, lex) = scanner();
    }
    if (lex == "(") {
        tie(tok, lex) = scanner();
        expr();
        if (lex != ")") {
            erra("Error de Sintaxis", "se espera cerrar ) y llego " + lex);
        }
    }
    else if (tok == "Ide") {
        tie(tok, lex) = scanner();
        if (lex == "[") {
            tie(tok, lex) = scanner();
            expr();
            if (lex != "]") {
                erra("Error Sintaxis", "se esperaba cerrar ] y llego " + lex);
            }
        }
    }
    else if (tok == "CtL" || tok == "CtA" || tok == "Dec" || tok == "Ent") {
        cte();
    }
    if (lex != ")") {
        tie(tok, lex) = scanner();
    }
}

void signo() {
    if (lex == "-") {
        tie(tok, lex) = scanner();
    }
    termino();
}

void expo() {
    string opr = "^";
    while (opr == "^") {
        signo();
        opr = lex;
    }
}

void multi() {
    string opr = "*";
    while (opr == "*" || opr == "/" || opr == "%") {
        expo();
        opr = lex;
    }
}

void suma() {
    string opr = "+";
    while (opr == "+" || opr == "-") {
        multi();
        opr = lex;
    }
}

void oprel() {
    string opr = "<";
    while (find(opRl, opRl+5, opr) != opRl+5) {
        suma();
        opr = lex;
    }
}

void opno() {
    if (lex == "no") {
        tie(tok, lex) = scanner();
    }
    oprel();
}

void opy() {
    string opr = "y";
    while (opr == "y") {
        opno();
        opr = lex;
    }
}

void expr() {
    string opr = "o";
    while (opr == "o") {
        opy();
        opr = lex;
    }
}

void constVars() {
    tie(tok, lex) = scanner();
}

void params() {
    tie(tok, lex) = scanner();
}

void gpoExp() {
    if (lex != ")") {
        string deli = ",";
        while (deli == ",") {
            if (lex == ",") {
                tie(tok, lex) = scanner();
            }
            expr();
            deli = lex;
        }
    }
}

void leer() {
    tie(tok, lex) = scanner();
    if (lex != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lex);
    }
    tie(tok, lex) = scanner();
    if (tok != "Ide") {
        erra("Error de Sintaxis", "se esperaba un identificador y llego " + lex);
    }
    string var = lex; // store the variable name
    tie(tok, lex) = scanner();
    if (lex != ")") {
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lex);
    }
}

void imprime() {
    tie(tok, lex) = scanner();
    if (lex != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lex);
    }
    tie(tok, lex) = scanner();
    if (lex != ")") gpoExp();
    if (lex != ")") tie(tok, lex) = scanner();
    if (lex != ")") {
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lex);
    }
}

void imprimenl() {
    tie(tok, lex) = scanner();
    if (lex != "(") {
        erra("Error de Sintaxis", "se esperaba abrir ( y llego " + lex);
    }
    tie(tok, lex) = scanner();
    if (lex != ")") gpoExp();
    if (lex != ")") tie(tok, lex) = scanner();
    if (lex != ")") {
        erra("Error de Sintaxis", "se esperaba cerrar ) y llego " + lex);
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
    //if (tok == "Ide") asigLfunc();
    if (lex == "lee") leer();
    else if (lex == "imprime") imprime();
    else if (lex == "imprimenl") imprimenl();
    else if (lex == "desde") desde();
    else if (lex == "mientras") mientras();
    else if (lex == "si") si();
    else if (lex == "repite") repite();
    else if (lex == "lmp") lmp();
    else if (lex == "regresa") regresa();
    else erra("Error de Sintaxis", "comando no definido " + lex);
    tie(tok, lex) = scanner();
}

void blkcmd() {
    tie(tok, lex) = scanner();
    if (lex != ";" && lex != "{") {
        comando();
        tie(tok, lex) = scanner();
        if (lex != ";") erra("Error de Sintaxis", "se esperaba ; y llego " + lex);
    }
    else if (lex == "{") {
        estatutos();
        if (lex != "}") erra("Error de Sintaxis", "se esperaba cerrar block \"}\" y llego " + lex);
    }
}

void estatutos() {
    string cbk = "{";
    while (cbk != "}") {
        if (lex != ";") comando();
        if (lex != ";") erra("Error de Sintaxis", "se esperaba ; y llego " + lex);
        tie(tok, lex) = scanner();
        cbk = lex;
    }
}

void blkFunc() {
    if (lex != "{") erra("Error de Sintaxis", "se esperaba abrir \"{\" y llego " + lex);
    tie(tok, lex) = scanner();
    if (lex != "}") estatutos();
    if (lex != "}") erra("Error de Sintaxis", "se esperaba cerrar \"}\" y llego " + lex);
}

void funcs() {
    if (find(tipo, tipo+5, lex) == tipo+5) {
        erra("Error Sintactico", "Se esperaba tipo");
    }
    tie(tok, lex) = scanner();
    if (tok != "Ide") erra("Error Sintaxis", "Se esperaba Nombre Funcion y llego " + lex);
    if (bPrinc) erra("Error de Semantica", "la Funcion Principal ya esta definida");
    if (lex == "principal") bPrinc = true;
    tie(tok, lex) = scanner();
    if (lex != "(") erra("Error de Sintaxis", "se esperaba parentisis abierto \"(\" y llego " + lex);
    tie(tok, lex) = scanner();
    if (lex != ")") params();
    if (lex != ")") erra("Error de Sintaxis", "se esperaba parentisis cerrado \")\"");
    tie(tok, lex) = scanner();
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