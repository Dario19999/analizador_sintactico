#ifndef SINTACTICO_H_INCLUDED
#define SINTACTICO_H_INCLUDED
#include "lexico.h"
#include "conio.h"
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <stack>
#include <iostream>

#define ES_IF 6
#define ENT_IF 5
#define ES_ELSE 4
#define ENT_ELSE 3
#define ES_ELSE_IF 8
#define ENT_ELSE_IF 7
#define ES_WHILE 6
#define ENT_WHILE 5
#define ES_SWITCH 6
#define ENT_SWITCH 5

using namespace std;

//IF-ELSE IF-ELSE
enum { IF_3 = 0, PARENTESIS_A_3, CONDICION_3, PARENTESIS_B_3, LLAVE_3 } e_if;
enum { LLAVE1_4 = 0, ELSE_4, LLAVE2_4 } e_else;
enum { LLAVE2_5 = 0, ELSE_5, IF_5, PARENTESIS_A_5, CONDICION_5, PARENTESIS_B_5, LLAVE1_5 } e_else_if;
enum { WHILE_8 = 0, PARENTESIS_A_8, CONDICION_8, PARENTESIS_B_8, LLAVE_8 } e_while;
enum { SWITCH_6 = 0, PARENTESIS_A_6, CONDICION_6, PARENTESIS_B_6, LLAVE_6 } e_switch;
enum { Q0 = 0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11 } estados;

int declaracionIf[ES_IF][ENT_IF];
int declaracionElse[ES_ELSE][ENT_ELSE];
int declaracionElseIf[ES_ELSE_IF][ENT_ELSE_IF];
int declaracionWhile[ES_WHILE][ENT_WHILE];
int declaracionSwitch[ES_SWITCH][ENT_SWITCH];

int posFinalIf = 0;

void llenarAutomatas();
void llenarDeclaracionIf();
void llenarDeclaracionElse();
void llenarDeclaracionElseIf();
void llenarDeclaracionWhile();
void llenarDeclaracionSwitch();
bool analizarDeclaracionIf(string cadena);
bool analizarDeclaracionElseIf(string cadena);
bool analizarDeclaracionElse(string cadena);
bool analizarDeclaracionWhile(string cadena);
bool analizarDeclaracionSwitch(string cadena);
void analizar_cadena_sintactico(string cadena);

void llenarAutomatas() {
    llenarDeclaracionIf();
    llenarDeclaracionElse();
    llenarDeclaracionElseIf();
    llenarDeclaracionWhile();
    llenarDeclaracionSwitch();
}

struct Comparacion {
    string comparadorIzquierda;
    string comparadorDerecha;
    string operador;
    int num;
};

struct Condicion {
    string cadena;
    vector<Comparacion> comparaciones;
    string operador;
    int num;
    bool esVariableOFuncion = false;
};

struct Linea {
    int num;
    string contenido;
    int longitud;
    int tipo;
    int posFinal;
    bool esIf = false;
    bool ifActivado = false;
    Condicion condicion;
};

vector<Linea> lineas;
string codigo;
string archivo;
int numLinea;

bool validarPilaLlaves(string codigo) {
    int contador = 0;
    for (int i = 0; i < (int)codigo.length(); i++) {
        if (codigo[i] == '{') {
            contador++;
        }
        else if (codigo[i] == '}') {
            if (contador == 0) {
                return false;
            }
            else {
                contador--;
            }
        }
    }
    return contador == 0;
}

string leerArchivo(string nombreArchivo) {
    string cadena;
    ifstream leer;
    char temp;
    cadena = "";
    leer.open(nombreArchivo);
    if (leer.good()) {
        while (!leer.eof()) {
            leer.read((char*)&temp, 1);
            if (leer.eof()) break;
            cadena += temp;
        }
        leer.close();
    }
    return cadena;
}

void leerLineas(string codigo) {
    int tamanoCodigo = codigo.length();
    string linea;
    Linea l;
    for (int i = 0; i < tamanoCodigo; i++) {
        if ((int)codigo[i] != 10) {
            linea += codigo[i];
        }
        else {
            l.contenido = linea;
            l.longitud = linea.length();
            l.num = lineas.size() + 1;
            lineas.push_back(l);
            linea = "";
        }
    }
    l.contenido = codigo[tamanoCodigo - 1];
    l.longitud = l.contenido.length();
    l.num = lineas.size() + 1;
    lineas.push_back(l);
}

void analizarLinea(int pos) {
    Linea linea = lineas[pos];
    string contenido = linea.contenido;
    analizar_cadena_sintactico(contenido);
}

void analizarLineas(int lineaInicial, int lineaFinal) {
    for (numLinea = lineaInicial; numLinea < lineaFinal; numLinea++) {
        analizarLinea(numLinea);
    }
}

bool analizarArchivo(string nombreAchivo) {
    ifstream test;
    test.open(nombreAchivo);
    lineas.clear();
    if (test.good()) {
        codigo = leerArchivo(nombreAchivo);
        if (validarPilaLlaves(codigo)) {
            archivo = nombreAchivo;
            leerLineas(codigo);
            numLinea = 0;
            analizarLineas(numLinea, lineas.size());
        }
        else {
            cout << endl << "\t[No hay equivalencia de llaves {}]" << endl;
            return false;
        }
    }
    else {
        cout << "\n\t[No se pudo abrir el archivo]" << endl;
        return false;
    }
}


void sintactico_cout(string cadena, int i) {
    if (cadena.size() < 10) {
        cout << "\t[Error de Sintaxis]" << endl;
        return;
    }
    i++;
    if (cadena[i] == '<') {
        i++;
        if (cadena[i] == '<') {
            i++;
            if (cadena[i] == '"') {
                i = cadena.size() - 1;
                if (cadena[i] == ';') {
                    i--;
                    if (cadena[i] == '"') {
                        cout << "\t[IMPRESION DE PANTALLA]" << endl;
                    }
                    else {
                        cout << "\t[Error de Sintaxis]" << endl;
                    }
                }
                else {
                    cout << "\t[Error de Sintaxis]" << endl;;
                }
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
            }
        }
        else {
            cout << "\t[Error de Sintaxis]" << endl;
        }
    }
    else {
        cout << "\t[Error de Sintaxis]" << endl;
    }
    return;
}

void sintactico_cin(string cadena, int i) {
    if (cadena.size() < 8) {
        cout << "\t[Error de Sintaxis]" << endl;
        return;
    }

    i++;
    if (cadena[i] == '>') {
        i++;
        if (cadena[i] == '>') {
            int x = cadena.size() - 1;
            if (cadena[x] == ';') {
                i++;
                char copia_cadena[1000];
                strcpy_s(copia_cadena, cadena.c_str());
                char reserva[1000];
                x = 0;
                while (cadena[i] != ';') {
                    reserva[x] = copia_cadena[i];
                    x++;
                    i++;
                }
                reserva[x] = '\'0';
                if (es_identificador(reserva)) {
                    cout << "\t[ENTRADA DE DATOS]" << endl;
                }
                else {
                    cout << "\t[Error de Sintaxis]" << endl;
                }
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
            }
        }
        else {
            cout << "\t[Error de Sintaxis]" << endl;
        }
    }
    else {
        cout << "\t[Error de Sintaxis]" << endl;
    }
    return;
}

bool parametros(string cadena) {
    int i;
    char reservada[1000];
    char copia_cadena[1000];
    bool encuentra = false;
    string primer_palabra;
    strcpy_s(copia_cadena, cadena.c_str());
    for (i = 0; i < cadena.size(); i++) {
        if (cadena[i] == ' ') {
            encuentra = true;
            break;
        }
        reservada[i] = copia_cadena[i];
    }
    if (encuentra == false) {
        return false;
    }
    reservada[i] = '\0';
    primer_palabra = reservada;
    string tipo;
    if (analizar_cadena_lexico(primer_palabra)) {
        tipo = retorna_tipo(primer_palabra);
        if (tipo == "Tipo de Dato") {
            if (primer_palabra == "void") {
                return false;
            }
            else {
                reservada[0] = '\0';
                int b = 0;
                i++;
                for (; i < cadena.size(); i++, b++) {
                    reservada[b] = copia_cadena[i];
                }
                reservada[b] = '\0';
                tipo = reservada;
                if (es_identificador(tipo)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void sintactico_prototipo(string cadena) {
    char copia_cadena[1000];
    int i = 0;
    while (cadena[i] != '(') {
        i++;
    }
    i++;
    int i2 = 0;
    while (cadena[i] != ')') {
        copia_cadena[i2] = cadena[i];
        i++;
        i2++;
    }
    copia_cadena[i2] = '\0';
    if (strlen(copia_cadena) > 0) {
        int tam = strlen(copia_cadena) - 1;
        if (copia_cadena[tam] == ',') {
            cout << "\t[Error de Sintaxis]" << endl;
            return;
        }
    }
    else {
        cout << "[DECLARACION DE PROTOTIPO DE FUNCION]" << endl;
        return;
    }
    int x = 0;
    char sub_cadena[100];
    for (i = 0; i < strlen(copia_cadena) + 1; i++) {
        if (copia_cadena[i] != ',' && copia_cadena[i] != '\0') {
            sub_cadena[x] = copia_cadena[i];
            x++;
        }
        else {
            sub_cadena[x] = '\0';
            if (parametros(sub_cadena)) {
                x = 0;
                sub_cadena[x] = '\0';
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
                return;
            }
        }
    }
    cout << "[DECLARACION DE PROTOTIPO DE FUNCION]" << endl;
    return;
}

void sintactico_funciones(string cadena) {
    cout << "[FUNCION]" << endl;
    return;
}

void sintactico_asignaciones(string cadena, int i) {

    i++;
    if (cadena[i] == '=') {
        i++;
        if (cadena[i] == ' ') {
            i++;
            int a = cadena.size() - 1;
            if (cadena[a] == ';') {
                a = i;
                char copia_cadena[100];
                char reserva[100];
                strcpy_s(reserva, cadena.c_str());
                int j;
                for (j = 0; a < cadena.size(); a++, j++) {
                    copia_cadena[j] = reserva[a];
                }
                copia_cadena[j - 1] = '\0';
                int cuenta_operadores = 0;
                for (int x = 0; x < strlen(copia_cadena); x++) {
                    if (copia_cadena[x] == '+' or copia_cadena[x] == '-' or copia_cadena[x] == '*' or copia_cadena[x] == '=' or copia_cadena[x] == '/' or copia_cadena[x] == '%') {
                        cuenta_operadores++;
                    }
                }
                if (cuenta_operadores == 0) {
                    string identificador = copia_cadena;
                    if (es_identificador(identificador)) {
                        cout << "\t[ASIGNACIONES]" << endl;
                    }
                    else if (es_numero(identificador)) {
                        cout << "\t[ASIGNACIONES]" << endl;
                    }
                    else if (es_funcion(identificador)) {
                        cout << "\t[ASIGNACIONES]" << endl;
                    }
                    else {
                        cout << "\t[Error de Sintaxis]" << endl;
                    }
                }
                else {
                    int x = 0;
                    char sub_cadena[100];
                    for (i = 0; i < strlen(copia_cadena) + 1; i++) {
                        if ((copia_cadena[i] != '+' and copia_cadena[i] != '-' and copia_cadena[i] != '*' and copia_cadena[i] != '=' and copia_cadena[i] != '/' and copia_cadena[i] != '%') && (copia_cadena[i] != '\0')) {
                            sub_cadena[x] = copia_cadena[i];
                            x++;
                        }
                        else {
                            sub_cadena[x] = '\0';
                            if (es_identificador(sub_cadena)) {
                                x = 0;
                                sub_cadena[x] = '\0';
                            }
                            else if (es_numero(sub_cadena)) {
                                x = 0;
                                sub_cadena[x] = '\0';
                            }
                            else if (es_funcion(sub_cadena)) {
                                x = 0;
                                sub_cadena[x] = '\0';
                            }
                            else {
                                cout << "\t[Error de Sintaxis]" << endl;
                                return;
                            }
                        }
                    }
                    cout << "\t[ASIGNACIONES]" << endl;
                }
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
            }
        }
        else {
            cout << "\t[Error de Sintaxis]" << endl;
        }
    }
    else {
        cout << "\t[Error de Sintaxis]" << endl;
    }
}

void sintactico_declaraciones(string cadena, string primer_palabra, int i) { // Terminado
    bool encuentra;
    char copia_cadena[1000];
    int b;
    bool hay_llave = false;
    for (b = 0; b < cadena.size(); b++) { //buscar llave que abre '{'
        if (cadena[b] == '{') {
            hay_llave = true;
            break;
        }
    }
    if (hay_llave) { // si esta
        hay_llave = false;
        b--;
        for (; b < cadena.size(); b++) { //buscar llave que cierra
            if (cadena[b] == '}') {
                hay_llave = true;
            }
            else {
                hay_llave = false;
            }
        }
        if (hay_llave) { //si esta
            sintactico_funciones(cadena);
        }
        else {
            cout << "\t[Error de Sintaxis]" << endl;
            return;
        }
    }
    else {
        encuentra = false;
        int espacio = i, parentesis = i;
        char identificador[1000];
        // validar que no haya solo ';' despues de espacio
        if (cadena[i + 1] == ';') {
            cout << "\t[Error de Sintaxis]" << endl;
            return;
        }
        // validar que termine en ; si hay caracteres despues de espacio
        for (; i < cadena.size(); i++) {
            if (cadena[i] == ';' && cadena[i + 1] == '\0') {
                encuentra = true;
            }
        }
        if (encuentra == false) {
            cout << "\t[Error de Sintaxis]" << endl;
            return;
        }
        //validar que la palabra que hay entre el espacio y el ';' sea un identificador valido
        int x;
        espacio++;
        for (x = 0; espacio < cadena.size(); espacio++, x++) {
            if (cadena[espacio] == ';') {
                break;
            }
            identificador[x] = cadena[espacio];
        }
        identificador[x] = '\0';
        string ide = identificador;
        if (es_identificador(ide)) { // (tipo de dato) (identificador) ;
            cout << "\t[DECLARACION DE VARIABLE]" << endl;
        }
        else { // puede tener parentesis
            bool bandera = false;
            int a;
            for (a = 0; a < ide.size(); a++) { //buscar parentesis que abre '('
                if (ide[a] == '(') {
                    bandera = true;
                    break;
                }
                else if (ide[a] == ')') {
                    bandera = false;
                    break;
                }
            }
            if (bandera) { // si esta
                a++;
                bandera = false;
                int uno = 0;
                for (; a < ide.size(); a++) { //buscar parentesis que cierra
                    if (ide[a] == ')') {
                        bandera = true;
                        uno++;
                    }
                    else if (ide[a] == '(') {
                        bandera = false;
                        break;
                    }
                }
                if (uno > 1) {
                    bandera = false;
                }
                if (bandera) { //si esta
                    strcpy_s(copia_cadena, ide.c_str());
                    char ide_2[1000];
                    for (a = 0; a < ide.size(); a++) {
                        if (copia_cadena[a] == '(') {
                            break;
                        }
                        ide_2[a] = copia_cadena[a];
                    }
                    ide_2[a] = '\0';
                    ide = ide_2;
                    if (es_identificador(ide)) {
                        sintactico_prototipo(cadena);
                        return;
                    }
                    else {
                        cout << "\t[Error de Sintaxis]" << endl;
                        return;
                    }
                }
                else {
                    cout << "\t[Error de Sintaxis]" << endl;
                    return;
                }
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
                return;
            }
        }
    }
}

bool haySoloLlaves(string linea) {
    int tamano = linea.length();
    for (int i = 0; i < tamano; i++) {
        if ((linea[i] != '}') && (linea[i] != ' ')) {
            if ((int)linea[i] != 10) {
                if ((int)linea[i] != 9) {
                    return false;
                }
            }
        }
    }
    return true;
}

void imprimirCondicion(Linea linea, bool tieneCondicion) {
    string condicion = linea.condicion.cadena;
    condicion[condicion.length() - 1] = '\0';
}

void analizar_cadena_sintactico(string cadena) {
    Linea linea;
    cout << endl << numLinea << "\t" << cadena;
    if (cadena.length() == 0) {
        cout << "[LINEA EN BLANCO]";
        return;
    }
    if (haySoloLlaves(cadena)) {
        cout << "\t[LLAVE DE CIERRE]";
        return;
    }
    if (analizarDeclaracionIf(cadena)) {
        lineas[numLinea].posFinal = posFinalIf;
        lineas[numLinea].esIf = true;
        linea = lineas[numLinea];
        posFinalIf = 0;
        cout << "\t\t[DECLARACION DE IF]";
        imprimirCondicion(linea, true);
        return;
    }
    if (analizarDeclaracionElse(cadena)) {
        lineas[numLinea].posFinal = posFinalIf;
        lineas[numLinea].esIf = false;
        linea = lineas[numLinea];
        posFinalIf = 0;
        cout << "\t\t[DECLARACION DE ELSE]";
        imprimirCondicion(linea, false);
        return;
    }
    if (analizarDeclaracionElseIf(cadena)) {
        lineas[numLinea].posFinal = posFinalIf;
        lineas[numLinea].esIf = true;
        linea = lineas[numLinea];
        posFinalIf = 0;
        cout << "\t\t[DECLARACION DE ELSE IF]";
        imprimirCondicion(linea, true);
        return;
    }
    if (analizarDeclaracionWhile(cadena)) {
        lineas[numLinea].posFinal = posFinalIf;
        lineas[numLinea].esIf = false;
        linea = lineas[numLinea];
        posFinalIf = 0;
        cout << "\t\t[DECLARACION DE WHILE]";
        imprimirCondicion(linea, true);
        return;
    }
    if (analizarDeclaracionSwitch(cadena)) {
        lineas[numLinea].posFinal = posFinalIf;
        lineas[numLinea].esIf = true;
        linea = lineas[numLinea];
        posFinalIf = 0;
        cout << "\t\t[DECLARACION DE SWITCH]";
        imprimirCondicion(linea, true);
        return;
    }
    if (es_funcion(cadena)) {
        cout << "\t\t[LLAMADA DE FUNCION]";
        return;
    }
    int i;
    char reservada[1000];
    char copia_cadena[1000];
    bool encuentra = false;
    string primer_palabra;
    strcpy_s(copia_cadena, cadena.c_str());
    for (i = 0; i < cadena.size(); i++) {
        if (cadena[i] == ' ') {
            encuentra = true;
            break;
        }
        reservada[i] = copia_cadena[i];
    }
    if (encuentra == false) {
        cout << "\t[Error de Sintaxis]" << endl;
        return;
    }
    reservada[i] = '\0';
    primer_palabra = reservada;
    string tipo;
    if (analizar_cadena_lexico(primer_palabra)) {
        tipo = retorna_tipo(primer_palabra);
        if (tipo == "Tipo de Dato") { // declaraciones de variables o funciones
            if (primer_palabra != "void") {
                sintactico_declaraciones(cadena, primer_palabra, i);
            }
            else {
                for (i = 0; i < cadena.size(); i++) {
                    if (cadena[i] == ';') {
                        sintactico_prototipo(cadena);
                        return;
                    }
                }
                sintactico_funciones(cadena);
            }
        }

        else if (tipo == "Palabra reservada") { // ciclos o condicionales o structs

            if (primer_palabra == "cout") {
                sintactico_cout(cadena, i);
            }
            else if (primer_palabra == "cin") {
                sintactico_cin(cadena, i);
            }
            else {
                cout << "\t[Error de Sintaxis]" << endl;
            }
        }
        else if (tipo == "identificador") { // asignaciones o llamadas a funciones
            sintactico_asignaciones(cadena, i);
        }
        else {                    // numeros
            cout << "\t[Error de Sintaxis]" << endl;
            return;
        }
    }
    else {
        cout << "\t[Error de Sintaxis]" << endl;
    }
    return;
}

void llenarDeclaracionWhile() {
    //Forma : while(condicion) {
    declaracionWhile[Q0][WHILE_8] = Q1;
    declaracionWhile[Q0][PARENTESIS_A_8] = Q6;
    declaracionWhile[Q0][CONDICION_8] = Q6;
    declaracionWhile[Q0][PARENTESIS_B_8] = Q6;
    declaracionWhile[Q0][LLAVE_8] = Q6;

    declaracionWhile[Q1][WHILE_8] = Q6;
    declaracionWhile[Q1][PARENTESIS_A_8] = Q2;
    declaracionWhile[Q1][CONDICION_8] = Q6;
    declaracionWhile[Q1][PARENTESIS_B_8] = Q6;
    declaracionWhile[Q1][LLAVE_8] = Q6;

    declaracionWhile[Q2][WHILE_8] = Q6;
    declaracionWhile[Q2][PARENTESIS_A_8] = Q6;
    declaracionWhile[Q2][CONDICION_8] = Q3;
    declaracionWhile[Q2][PARENTESIS_B_8] = Q6;
    declaracionWhile[Q2][LLAVE_8] = Q6;

    declaracionWhile[Q3][WHILE_8] = Q6;
    declaracionWhile[Q3][PARENTESIS_A_8] = Q6;
    declaracionWhile[Q3][CONDICION_8] = Q6;
    declaracionWhile[Q3][PARENTESIS_B_8] = Q4;
    declaracionWhile[Q3][LLAVE_8] = Q6;

    declaracionWhile[Q4][WHILE_8] = Q6;
    declaracionWhile[Q4][PARENTESIS_A_8] = Q6;
    declaracionWhile[Q4][CONDICION_8] = Q6;
    declaracionWhile[Q4][PARENTESIS_B_8] = Q6;
    declaracionWhile[Q4][LLAVE_8] = Q5;

    declaracionWhile[Q5][WHILE_8] = Q6;
    declaracionWhile[Q5][PARENTESIS_A_8] = Q6;
    declaracionWhile[Q5][CONDICION_8] = Q6;
    declaracionWhile[Q5][PARENTESIS_B_8] = Q6;
    declaracionWhile[Q5][LLAVE_8] = Q6;
}

void llenarDeclaracionIf() {
    //Forma : if(condicion) {
    declaracionIf[Q0][IF_3] = Q1;
    declaracionIf[Q0][PARENTESIS_A_3] = Q6;
    declaracionIf[Q0][CONDICION_3] = Q6;
    declaracionIf[Q0][PARENTESIS_B_3] = Q6;
    declaracionIf[Q0][LLAVE_3] = Q6;

    declaracionIf[Q1][IF_3] = Q6;
    declaracionIf[Q1][PARENTESIS_A_3] = Q2;
    declaracionIf[Q1][CONDICION_3] = Q6;
    declaracionIf[Q1][PARENTESIS_B_3] = Q6;
    declaracionIf[Q1][LLAVE_3] = Q6;

    declaracionIf[Q2][IF_3] = Q6;
    declaracionIf[Q2][PARENTESIS_A_3] = Q6;
    declaracionIf[Q2][CONDICION_3] = Q3;
    declaracionIf[Q2][PARENTESIS_B_3] = Q6;
    declaracionIf[Q2][LLAVE_3] = Q6;

    declaracionIf[Q3][IF_3] = Q6;
    declaracionIf[Q3][PARENTESIS_A_3] = Q6;
    declaracionIf[Q3][CONDICION_3] = Q6;
    declaracionIf[Q3][PARENTESIS_B_3] = Q4;
    declaracionIf[Q3][LLAVE_3] = Q6;

    declaracionIf[Q4][IF_3] = Q6;
    declaracionIf[Q4][PARENTESIS_A_3] = Q6;
    declaracionIf[Q4][CONDICION_3] = Q6;
    declaracionIf[Q4][PARENTESIS_B_3] = Q6;
    declaracionIf[Q4][LLAVE_3] = Q5;

    declaracionIf[Q5][IF_3] = Q6;
    declaracionIf[Q5][PARENTESIS_A_3] = Q6;
    declaracionIf[Q5][CONDICION_3] = Q6;
    declaracionIf[Q5][PARENTESIS_B_3] = Q6;
    declaracionIf[Q5][LLAVE_3] = Q6;
}

void llenarDeclaracionElse() {
    //Foma : } else {
    declaracionElse[Q0][LLAVE2_4] = Q1;
    declaracionElse[Q0][ELSE_4] = Q4;
    declaracionElse[Q0][LLAVE1_4] = Q4;

    declaracionElse[Q1][LLAVE2_4] = Q4;
    declaracionElse[Q1][ELSE_4] = Q2;
    declaracionElse[Q1][LLAVE1_4] = Q4;

    declaracionElse[Q2][LLAVE2_4] = Q4;
    declaracionElse[Q2][ELSE_4] = Q4;
    declaracionElse[Q2][LLAVE1_4] = Q3;

    declaracionElse[Q3][LLAVE2_4] = Q4;
    declaracionElse[Q3][ELSE_4] = Q4;
    declaracionElse[Q3][LLAVE1_4] = Q4;
}
void llenarDeclaracionElseIf() {
    //Forma : } else if(condicion) {
    declaracionElseIf[Q0][LLAVE2_5] = Q1;
    declaracionElseIf[Q0][ELSE_5] = Q8;
    declaracionElseIf[Q0][IF_5] = Q8;
    declaracionElseIf[Q0][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q0][CONDICION_5] = Q8;
    declaracionElseIf[Q0][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q0][LLAVE1_5] = Q8;

    declaracionElseIf[Q1][LLAVE2_5] = Q8;
    declaracionElseIf[Q1][ELSE_5] = Q2;
    declaracionElseIf[Q1][IF_5] = Q8;
    declaracionElseIf[Q1][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q1][CONDICION_5] = Q8;
    declaracionElseIf[Q1][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q1][LLAVE1_5] = Q8;

    declaracionElseIf[Q2][LLAVE2_5] = Q8;
    declaracionElseIf[Q2][ELSE_5] = Q8;
    declaracionElseIf[Q2][IF_5] = Q3;
    declaracionElseIf[Q2][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q2][CONDICION_5] = Q8;
    declaracionElseIf[Q2][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q2][LLAVE1_5] = Q8;

    declaracionElseIf[Q3][LLAVE2_5] = Q8;
    declaracionElseIf[Q3][ELSE_5] = Q8;
    declaracionElseIf[Q3][IF_5] = Q8;
    declaracionElseIf[Q3][PARENTESIS_A_5] = Q4;
    declaracionElseIf[Q3][CONDICION_5] = Q8;
    declaracionElseIf[Q3][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q3][LLAVE1_5] = Q8;

    declaracionElseIf[Q4][LLAVE2_5] = Q8;
    declaracionElseIf[Q4][ELSE_5] = Q8;
    declaracionElseIf[Q4][IF_5] = Q8;
    declaracionElseIf[Q4][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q4][CONDICION_5] = Q5;
    declaracionElseIf[Q4][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q4][LLAVE1_5] = Q8;

    declaracionElseIf[Q5][LLAVE2_5] = Q8;
    declaracionElseIf[Q5][ELSE_5] = Q8;
    declaracionElseIf[Q5][IF_5] = Q8;
    declaracionElseIf[Q5][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q5][CONDICION_5] = Q8;
    declaracionElseIf[Q5][PARENTESIS_B_5] = Q6;
    declaracionElseIf[Q5][LLAVE1_5] = Q8;

    declaracionElseIf[Q6][LLAVE2_5] = Q7;
    declaracionElseIf[Q6][ELSE_5] = Q7;
    declaracionElseIf[Q6][IF_5] = Q7;
    declaracionElseIf[Q6][PARENTESIS_A_5] = Q7;
    declaracionElseIf[Q6][CONDICION_5] = Q7;
    declaracionElseIf[Q6][PARENTESIS_B_5] = Q7;
    declaracionElseIf[Q6][LLAVE1_5] = Q7;

    declaracionElseIf[Q7][LLAVE2_5] = Q8;
    declaracionElseIf[Q7][ELSE_5] = Q8;
    declaracionElseIf[Q7][IF_5] = Q8;
    declaracionElseIf[Q7][PARENTESIS_A_5] = Q8;
    declaracionElseIf[Q7][CONDICION_5] = Q8;
    declaracionElseIf[Q7][PARENTESIS_B_5] = Q8;
    declaracionElseIf[Q7][LLAVE1_5] = Q8;
}

void llenarDeclaracionSwitch() {
    //Forma : if(condicion) {
    declaracionSwitch[Q0][SWITCH_6] = Q1;
    declaracionSwitch[Q0][PARENTESIS_A_6] = Q6;
    declaracionSwitch[Q0][CONDICION_6] = Q6;
    declaracionSwitch[Q0][PARENTESIS_B_6] = Q6;
    declaracionSwitch[Q0][LLAVE_6] = Q6;

    declaracionSwitch[Q1][SWITCH_6] = Q6;
    declaracionSwitch[Q1][PARENTESIS_A_6] = Q2;
    declaracionSwitch[Q1][CONDICION_6] = Q6;
    declaracionSwitch[Q1][PARENTESIS_B_6] = Q6;
    declaracionSwitch[Q1][LLAVE_6] = Q6;

    declaracionSwitch[Q2][SWITCH_6] = Q6;
    declaracionSwitch[Q2][PARENTESIS_A_6] = Q6;
    declaracionSwitch[Q2][CONDICION_6] = Q3;
    declaracionSwitch[Q2][PARENTESIS_B_6] = Q6;
    declaracionSwitch[Q2][LLAVE_6] = Q6;

    declaracionSwitch[Q3][SWITCH_6] = Q6;
    declaracionSwitch[Q3][PARENTESIS_A_6] = Q6;
    declaracionSwitch[Q3][CONDICION_6] = Q6;
    declaracionSwitch[Q3][PARENTESIS_B_6] = Q4;
    declaracionSwitch[Q3][LLAVE_6] = Q6;

    declaracionSwitch[Q4][SWITCH_6] = Q6;
    declaracionSwitch[Q4][PARENTESIS_A_6] = Q6;
    declaracionSwitch[Q4][CONDICION_6] = Q6;
    declaracionSwitch[Q4][PARENTESIS_B_6] = Q6;
    declaracionSwitch[Q4][LLAVE_6] = Q5;

    declaracionSwitch[Q5][SWITCH_6] = Q6;
    declaracionSwitch[Q5][PARENTESIS_A_6] = Q6;
    declaracionSwitch[Q5][CONDICION_6] = Q6;
    declaracionSwitch[Q5][PARENTESIS_B_6] = Q6;
    declaracionSwitch[Q5][LLAVE_6] = Q6;
}
int estadoDeclaracionIf(int estadoActual, int simbolo) {
    return declaracionIf[estadoActual][simbolo];
}
int estadoDeclaracionElse(int estadoActual, int simbolo) {
    return declaracionElse[estadoActual][simbolo];
}
int estadoDeclaracionElseIf(int estadoActual, int simbolo) {
    return declaracionElseIf[estadoActual][simbolo];
}

int estadoDeclaracionWhile(int estadoActual, int simbolo) {
    return declaracionWhile[estadoActual][simbolo];
}
int estadoDeclaracionSwitch(int estadoActual, int simbolo) {
    return declaracionSwitch[estadoActual][simbolo];
}

bool existeCadena(string linea, int pos, string cadena, int& indiceFinal) {
    int tamanoLinea = linea.length();
    stringstream cadenaEncontrada;
    for (int i = pos; i < tamanoLinea; i++) {
        if ((linea[i] != ' ') && ((int)linea[i] != 10) && ((int)linea[i] != 9)) {
            cadenaEncontrada << linea[i];
            if (cadenaEncontrada.str() == cadena) {
                indiceFinal = i + 1;
                return true;
            }
        }
    }
    indiceFinal = -1;
    return false;
}

string obtenerSubCadena(string cadena, int pos1, int pos2) {
    stringstream cadenaCorta;
    int caracteresALaDererecha = pos2 - pos1;
    switch (caracteresALaDererecha) {
    case 0:
        cadenaCorta << cadena[pos1];
        return cadenaCorta.str();
    case 1:
        cadenaCorta << cadena[pos1];
        return cadenaCorta.str();
    default:
        return cadena.substr(pos1, caracteresALaDererecha);
    }
}

int obtenerIndiceSubcadena(string linea, int indice, char caracter) {
    int tamanoLinea = linea.length();
    if (tamanoLinea) {
        for (int i = indice; i < tamanoLinea; i++) {
            if (linea[i] == caracter) {
                return i;
            }
        }
        return 0;
    }
    else {
        return -1;
    }
}

int encontrarLlaves(string linea, int contLlaves) {
    int tamanoLinea = linea.length();
    int cont = contLlaves;
    for (int i = 0; i < tamanoLinea; i++) {
        if (linea[i] == '{') {
            cont++;
            if (cont == 0) {
                break;
            }
        }
        else if (linea[i] == '}') {
            cont--;
            if (cont == 0) {
                break;
            }
        }
    }
    return cont;
}

int obtenerIndiceBloqueInstrucciones(int linea, bool esIf, bool esElseIf) {
    int contLineas = lineas.size();
    int contLlaves = 0;
    if (esElseIf) {
        esIf = true;
        contLlaves = 1;
        linea++;
    }
    for (int i = linea; i < contLineas; i++) {
        contLlaves = encontrarLlaves(lineas[i].contenido, contLlaves);

        if (contLlaves == 0) {
            if (esIf) {
                posFinalIf = i + 1;
            }
            return 0;
        }
        else if (contLlaves < 1) {
            return -1;
        }
    }
    return 0;
}

string dameOperadoresLinea(string linea, char caracter_1, char caracter_2) {
    int tamanoLinea = linea.length();
    char simbolo;
    string cadena = "";
    for (int i = 0; i < tamanoLinea; i++) {
        simbolo = linea[i];
        if ((simbolo == caracter_1) || (simbolo == caracter_2)) {
            cadena += simbolo;
        }
    }
    if (cadena.length() > 0) {
        return cadena;
    }
    return "";
}

bool estaBalanceadaLaPila(string cadena, char caracterApertura) {
    stack<char> pila;
    int tamanoLinea = cadena.length();
    char simbolo;
    bool balanceada = true;
    for (int i = 0; i < tamanoLinea; i++) {
        simbolo = cadena[i];
        if (simbolo == caracterApertura) {
            pila.push(simbolo);
        }
        else {
            if (pila.empty()) {
                balanceada = false;
                break;
            }
            else {
                pila.pop();
            }
        }
    }
    if ((balanceada) && (pila.empty())) {
        return true;
    }
    return false;
}

bool tieneIf(int posElse) {
    int cantLineas = lineas.size();
    for (int i = 0; i < cantLineas; i++) {
        if (lineas[i].esIf) {
            if (!lineas[i].ifActivado) {
                if (lineas[i].posFinal == posElse) {
                    lineas[i].ifActivado = true;
                    return true;
                }
            }
        }
    }
    return false;
}

bool analizarDeclaracionIf(string linea) {
    int tamanoLinea = linea.length();
    int entrada, i, indice, estado;
    string subCadena, condicion;
    int lineaFinalBloque;

    i = indice = estado = 0;
    if (existeCadena(linea, i, "if", indice)) {
        subCadena = obtenerSubCadena(linea, indice - 2, indice);
        i = indice - 2;
    }
    else {
        return false;
    }
    while (i < tamanoLinea) {
        /*cout << endl << "*" << subCadena << "*" << endl;
        system("pause");*/
        if (subCadena == "if") {
            entrada = IF_3;
            i += 2;
            if (!existeCadena(linea, i, "(", indice)) {
                return false;
            }
            subCadena = obtenerSubCadena(linea, indice - 1, indice);
        }
        else if (subCadena == "(") {
            entrada = PARENTESIS_A_3;
            i += subCadena.length() - 1;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = obtenerSubCadena(linea, i, indice);
            if (!estaBalanceadaLaPila(dameOperadoresLinea(subCadena, '(', ')'), '(')) {
                cout << endl << "\t[Error: No hay equivalencia de parentesis]";
                return false;
            }
            condicion = subCadena.substr(1, subCadena.length() - 3);
            estado = estadoDeclaracionIf(estado, entrada);
            entrada = CONDICION_3;
            i += condicion.length() + 1;
            subCadena = linea[i];
            i += subCadena.length();
        }
        else if (subCadena == ")") {
            entrada = PARENTESIS_B_3;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = linea[indice];
            i += subCadena.length();
        }
        else if (subCadena == "{") {
            entrada = LLAVE_3;
            i++;
        }
        else {
            break;
        }
        estado = estadoDeclaracionIf(estado, entrada);
    }
    if (estado == Q5) {
        string c2 = "(";
        condicion.push_back(')');
        c2 += condicion;
        if (!es_condicional_valido(c2)) {
            cout << endl << "\t[Error en Condici�n del IF]";
        }
        lineaFinalBloque = obtenerIndiceBloqueInstrucciones(numLinea, true, false);
        if (lineaFinalBloque == -1) {
            return false;
        }
        for (; numLinea < lineaFinalBloque; numLinea++) {
            analizarLinea(numLinea);
        }
        return true;
    }
    else {
        return false;
    }
}

bool analizarDeclaracionElse(string linea) {
    int tamanoLinea = linea.length();
    int entrada, i, indice, estado;
    string subCadena;
    int lineaFinalBloque;

    i = indice = estado = 0;
    i = obtenerIndiceSubcadena(linea, i, '}');
    subCadena = linea[i];
    while (i < tamanoLinea) {
        //cout << "." << subCadena << "." << endl;
        if (subCadena == "}") {
            i += 2;
            entrada = LLAVE2_4;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            indice--;
            subCadena = obtenerSubCadena(linea, i, indice);
        }
        else if (subCadena == "else") {
            entrada = ELSE_4;
            i += subCadena.length() + 1;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = obtenerSubCadena(linea, i, indice);
        }
        else if (subCadena == "{") {
            entrada = LLAVE1_4;
            i++;
        }
        else {
            break;
        }
        estado = estadoDeclaracionElse(estado, entrada);
    }
    if (estado == Q3) {
        if (!tieneIf(numLinea + 1)) {
            cout << endl << endl << "\t[Error: ELSE sin IF]" << endl;
            return false;
        }
        lineaFinalBloque = obtenerIndiceBloqueInstrucciones(numLinea, false, true);
        if (lineaFinalBloque == -1) {
            return false;
        }
        for (; numLinea < lineaFinalBloque; numLinea++) {
            analizarLinea(numLinea);
        }
        return true;
    }
    return false;
}
bool analizarDeclaracionElseIf(string linea) {
    int tamanoLinea = linea.length();
    int entrada, i, indice, estado;
    string subCadena, condicion;
    int lineaFinalBloque;

    i = indice = estado = 0;
    i = obtenerIndiceSubcadena(linea, i, '}');
    subCadena = linea[i];
    while (i < tamanoLinea) {
        //cout << endl << "*" << subCadena << "*" << endl;
        if (subCadena == "}") {
            entrada = LLAVE2_5;
            i += 2;
            if (!existeCadena(linea, i, "else", indice)) {
                return false;
            }
            subCadena = obtenerSubCadena(linea, indice - 4, indice);
        }
        else if (subCadena == "else") {
            entrada = ELSE_5;
            i += subCadena.length() + 1;
            if (!existeCadena(linea, i, "if", indice)) {
                return false;
            }
            subCadena = obtenerSubCadena(linea, indice - 2, indice);
        }
        else if (subCadena == "if") {
            entrada = IF_5;
            i += subCadena.length();
            if (!existeCadena(linea, i, "(", indice)) {
                return false;
            }
            subCadena = linea[indice - 1];
        }
        else if (subCadena == "(") {
            entrada = PARENTESIS_A_5;
            i += subCadena.length() - 1;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = obtenerSubCadena(linea, i, indice);
            if (!estaBalanceadaLaPila(dameOperadoresLinea(subCadena, '(', ')'), '(')) {
                return false;
            }
            condicion = subCadena.substr(1, subCadena.length() - 3);
            estado = estadoDeclaracionIf(estado, entrada);
            entrada = CONDICION_5;
            i += condicion.length() + 1;
            subCadena = linea[i];
            i += subCadena.length();
        }
        else if (subCadena == ")") {
            entrada = PARENTESIS_B_5;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = linea[indice];
            i += subCadena.length();
        }
        else if (subCadena == "{") {
            entrada = LLAVE1_5;
            i++;
        }
        else {
            break;
        }
        estado = estadoDeclaracionElseIf(estado, entrada);
    }
    if (estado == Q7) {
        if (!tieneIf(numLinea + 1)) {
            cout << endl << endl << "\t[Error: ELSE IF sin IF]" << endl;
        }
        string c2 = "(";
        condicion.push_back(')');
        c2 += condicion;
        if (!es_condicional_valido(c2)) {
            cout << endl << "\t[Error en Condici�n del ELSE IF]";
            return false;
        }
        lineaFinalBloque = obtenerIndiceBloqueInstrucciones(numLinea, false, true);
        if (lineaFinalBloque == -1) {
            return false;
        }
        for (; numLinea < lineaFinalBloque; numLinea++) {
            analizarLinea(numLinea);
        }
        return true;
    }
    return false;
}

int buscarPrimerCaracter(string linea, int posInicial, char caracter) {
    int tamanoLinea = linea.length();
    for (int i = posInicial; i < tamanoLinea; i++) {
        if (linea[i] == caracter) {
            return i;
        }
    }
    return tamanoLinea;
}

bool analizarDeclaracionWhile(string linea) {
    int tamanoLinea = linea.length();
    int entrada, i, j, indice_1, indice_2, estado, lineaFinalBloque;
    string subCadena, condicion;

    i = j = indice_1 = indice_2 = estado = 0;
    indice_1 = buscarPrimerCaracter(linea, j, 'w');
    i += indice_1;
    indice_2 = buscarPrimerCaracter(linea, indice_1, '(');
    subCadena = obtenerSubCadena(linea, indice_1, indice_2);
    i += subCadena.length();
    while (i < tamanoLinea) {
        //cout << endl << "*" << subCadena << "*" << endl;
        if (subCadena == "while") {
            entrada = WHILE_8;
            j = i;
            indice_1 = buscarPrimerCaracter(linea, j, '(');
            subCadena = obtenerSubCadena(linea, indice_1, indice_1);
            i++;
        }
        else if (subCadena == "(") {
            entrada = PARENTESIS_A_8;
            j = i;
            indice_1 = buscarPrimerCaracter(linea, j, '{');
            indice_1 -= 2; //Para encontrar )
            subCadena = obtenerSubCadena(linea, j, indice_1);
            condicion = subCadena;
            estado = estadoDeclaracionWhile(estado, entrada);
            entrada = CONDICION_8;
            i = j + subCadena.length();
            subCadena = obtenerSubCadena(linea, indice_1, indice_1);
            i++;
        }
        else if (subCadena == ")") {
            entrada = PARENTESIS_B_8;
            j = i;
            indice_1 = buscarPrimerCaracter(linea, j, '{');
            subCadena = obtenerSubCadena(linea, indice_1, indice_1);
            i = j + subCadena.length();
        }
        else if (subCadena == "{") {
            entrada = LLAVE_8;
            i++;
        }
        else {
            break;
        }
        estado = estadoDeclaracionWhile(estado, entrada);
    }
    if (estado == Q5) {
        string c2 = "(";
        condicion.push_back(')');
        c2 += condicion;
        if (!es_condicional_valido(c2)) {
            cout << endl << "\[tError en Condici�n del WHILE]";
        }
        lineaFinalBloque = obtenerIndiceBloqueInstrucciones(numLinea, true, false);
        if (lineaFinalBloque == -1) {
            return false;
        }
        for (; numLinea < lineaFinalBloque; numLinea++) {
            analizarLinea(numLinea);
        }
        return true;
    }
    return false;
}

bool analizarDeclaracionSwitch(string linea) {
    int tamanoLinea = linea.length();
    int entrada, i, indice, estado;
    string subCadena, condicion;
    int lineaFinalBloque;

    i = indice = estado = 0;
    if (existeCadena(linea, i, "switch", indice)) {
        subCadena = obtenerSubCadena(linea, indice - 2, indice);
        i = indice - 2;
    }
    else {
        return false;
    }
    while (i < tamanoLinea) {
        /*cout << endl << "*" << subCadena << "*" << endl;
        system("pause");*/
        if (subCadena == "switch") {
            entrada = SWITCH_6;
            i += 2;
            if (!existeCadena(linea, i, "(", indice)) {
                return false;
            }
            subCadena = obtenerSubCadena(linea, indice - 1, indice);
        }
        else if (subCadena == "(") {
            entrada = PARENTESIS_A_6;
            i += subCadena.length() - 1;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = obtenerSubCadena(linea, i, indice);
            if (!estaBalanceadaLaPila(dameOperadoresLinea(subCadena, '(', ')'), '(')) {
                cout << endl << "\t[Error: No hay equivalencia de parentesis]";
                return false;
            }
            condicion = subCadena.substr(1, subCadena.length() - 3);
            estado = estadoDeclaracionSwitch(estado, entrada);
            entrada = CONDICION_6;
            i += condicion.length() + 1;
            subCadena = linea[i];
            i += subCadena.length();
        }
        else if (subCadena == ")") {
            entrada = PARENTESIS_B_6;
            indice = obtenerIndiceSubcadena(linea, i, '{');
            subCadena = linea[indice];
            i += subCadena.length();
        }
        else if (subCadena == "{") {
            entrada = LLAVE_6;
            i++;
        }
        else {
            break;
        }
        estado = estadoDeclaracionSwitch(estado, entrada);
    }
    if (estado == Q5) {
        string c2 = "(";
        condicion.push_back(')');
        c2 += condicion;
        if (!es_condicional_valido(c2)) {
            cout << endl << "\t[Error en Condici�n del SWITCH]";
        }
        lineaFinalBloque = obtenerIndiceBloqueInstrucciones(numLinea, true, false);
        if (lineaFinalBloque == -1) {
            return false;
        }
        for (; numLinea < lineaFinalBloque; numLinea++) {
            analizarLinea(numLinea);
        }
        return true;
    }
    else {
        return false;
    }
}

#endif // LEXICO_H_INCLUDED
