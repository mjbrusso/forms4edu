#include <forms4edu.h>
#include <iostream>

using namespace std;

int main (int argc, char ** argv)
{
    int n=5; //form.read_int("N: ");

    for(int i=1; i<=10; ++i)
        cout << n  << " x " << i << " = " <<  i*n << endl;

    form.set_title("Tabuada");
    form.show_cout();
    return 0;
}
