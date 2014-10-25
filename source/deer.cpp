#include <string>
#include "sync.h"
#include "config.h"

#define LD_VER "0.9.5"
#define CODENAME "Bambi"

int main()
{
    printf("***************************************\n");
    cout<<"\tLiquidDeer "<<LD_VER<<" "<<CODENAME<<endl;
    cout<<"***************************************"<<endl;

    warning();
    synchro();


}
