#include <string>
#include "sync.h"
#include "config.h"

#define LD_VER "0.9.5"
#define CODENAME "Bambi"

int main()
{
    printf("***************************************\n");
    printf("\tLiquidDeer %s %s\n", LD_VER, CODENAME);
    cout<<"***************************************"<<endl;

    warning();
    synchro();


}
