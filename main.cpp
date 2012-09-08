#include <iostream>
#include <iomanip>
#include "forms4edu.h"

using namespace forms4edu;
using namespace std;

int main ()
{
    int matr;
    string nome;
    float salario;

    nome = form.select_input_file();
    if(!form.canceled()) form.message(nome);

    form.read_int("Matricula: ");
    cin >> matr;
    form.read_string("Nome: ");
    getline(cin, nome);
    form.read_float("Salario: ");
    cin >> salario;

    cout << "Matricula: " << matr << endl
         << "Nome:" << nome << endl
        << "Salario:" << fixed << setprecision(2) << salario << endl;

    form.show_cout();


    bool bolsa;
    int estado;
    string senha;
    float nota;
    int idade;

    form.set_title("Dados do aluno");
    form.add_int_input("Número de matrícula:");
    form.add_string_input("Nome:");
    form.add_float_input("Salario:");
    form.add_bool_input("Bolsista?");
    form.add_choice_input("Estado civil: ", "Casado|Solteiro|Viuvo");
    form.add_secret_input("Senha:");
    form.add_float_spinner("Nota:", 0.0, 10.0, 0.1);
    form.add_int_spinner("Idade:", 1, 120);
    form.add_output("Resultado:");

    do {
        form.read(true);
        if(form.canceled()) break;;

//    matr = form.field_int_value(0);
//    nome = form.field_string_value(1);
//    salario = form.field_float_value(2);
//    bolsa = form.field_bool_value(3);
//    estado = form.field_int_value(4);
//    senha = form.field_string_value(5);
//    nota = form.field_float_value(6);
//    idade = form.field_int_value(7);

        cin >> matr;
        cin.ignore();
        getline(cin, nome);
        cin >> salario;
        cin >> bolsa;
        cin >> estado;
        cin.ignore();
        getline(cin, senha);
        cin >> nota;
        cin >> idade;

        cout << "Matricula: "<<  matr << endl
             << "Nome: " << nome << endl
             << "Salário: " << salario << endl
             << "Nota: " << nota << endl
             << "Bolsa: " << bolsa << endl
             << "Estado Civil: " << estado << form.choice_selected_text(4) << endl
             << "Senha:" << senha << endl
             << "Nota: " << nota << endl
             << "Idade: " << idade << endl;

        form.show_cout();
    } while(form.confirm("Desejas continuar?"));
    return 0;

}
