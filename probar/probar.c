#include"probar.h"

void probar()
{

    int cin=0;
    char arr[101];
    char arr2[5]="/|-\\";
    memset(arr,0,sizeof(arr));
    while(cin<=100)
    {
     arr[cin]=SYMBOL;
     printf("[%-100s]%3d%[%c] \r",arr,cin,arr2[cin%4]);
     fflush(stdout);
     usleep(50000);
     cin++;
    }
    printf("\n");
}
