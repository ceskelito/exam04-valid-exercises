#include <stdlib.h>

int	picoshell(char **cmds[]);

int main(void)
{
    picoshell((char **[]){
        (char *[]){"ls", "-l", NULL},
        (char *[]){"grep", "pico", NULL},
        NULL
    });
}
