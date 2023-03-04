#include <stdlib.h>
#include <iostream>

using namespace std;

#include "lexico.h"
#include "sintactico.h"
#include "stdio.h"

void analizarSintacticoArchivo();

int main() {
    llenarAutomatas();
    system("cls");
    analizarSintacticoArchivo();
    return 0;
}

string escanearNombreArchivo() {
    string archivo;
    system("CLS");
    setlocale(LC_CTYPE, "Spanish");
    cout << endl << "\t-----------Analizador Sintactico-----------" << endl;
    cout << endl << "\tIngrese nombre del archivo: ";
    getline(cin, archivo);
    cout << endl << "---------------------------------------------";
    return archivo;
}

void analizarSintacticoArchivo() {
    char opc;
    fflush(stdin);
    analizarArchivo(escanearNombreArchivo());
}