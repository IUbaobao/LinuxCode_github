#include "myStdio.h"


int main()
{
    FILE_* fp=fopen_("log.txt","w");
    assert(fp);

    const char* str="hello linux!\n";
    int cnt=5;
    while(cnt--)
    {
         fwrite_(str,strlen(str),fp);
    }
    fclose_(fp);
    return 0;
}
