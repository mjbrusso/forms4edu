#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "forms4edu.h"

using namespace std;

void read_pwd()
{
    string senha = form.read_password("Digite o c�digo de acesso: ");
    if(form.canceled()) exit(1);
    if(senha!="123") {
        form.message("C�digo Inv�lido!\nFinalizando");
        exit(2);
    }
}

void show_imc_form()
{
    form.set_title("C�lculo de IMC");
    form.add_float_input("Altura em cm: ");
    form.add_float_input("Peso em kg: ");
    form.add_output("Seu IMC: ");

    while(true) {
        form.read(true, "Calcular", "Sair");
        if(form.canceled()) break;
        float  altura = form.field_float_value(0)/100.0,
               peso   = form.field_float_value(1),
               imc    = peso/(altura*altura);

        form.set_field_value(2, imc);
    }
}

int main ()
{
    read_pwd();
    show_imc_form();
    return 0;
}
