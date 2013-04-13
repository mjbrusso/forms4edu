#include <iostream>
#include <iomanip>
#include "forms4edu.h"

using namespace std;

int main ()
{
    int matr;
    string nome;
    float salario;

    form.read_string("Nome: ");
    getline(cin, nome);
    form.read_int("Matricula: ");
    cin >> matr;
    form.read_float("Salario: ");
    cin >> salario;

    cout << "Matricula: " << matr << endl
         << "Nome:" << nome << endl
         << "Salario:" << fixed << setprecision(2) << salario << endl;

    form.show_cout();

//    nome = form.select_input_file();
//    if(!form.canceled()) form.message(nome);
}
