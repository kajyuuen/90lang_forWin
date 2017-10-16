#include <stdio.h>
#include <stdlib.h>

void loadText(int argc, const char **argv, unsigned char *t, int siz)
{
    FILE *fp;
    int i;
    if(argc < 2){
        printf("usage>%s source-file\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rt");
    if(fp ==0){
        printf("fopen error: %s\n", argv[1]);
        exit(1);
    }
    i = fread(t, 1, siz - 1, fp);
    fclose(fp);
    t[i] = 0;
    return;
}

int var[256];

int getNumber(unsigned char c)
{
    if('0' <= c && c <= '9')
        return c - '0';
    return var[c];
}

int main(int argc, const char **argv)
{
    unsigned char txt[10000], *p;
    loadText(argc, argv, txt, 10000);

    p = txt;
    for(;;){
        if(p[0] == 0){
            exit(0);
        }
        if(p[0] == '\n'){
            p++;
            continue;
        }
        if(p[1] == '='){
            if(p[3] == ';'){
                var[p[0]] = getNumber(p[2]);
            }else if(p[3] == '+' && p[5] == ';'){
                var[p[0]] = getNumber(p[2]) + getNumber(p[4]);
            }else if(p[3] == '-' && p[5] == ';'){
                var[p[0]] = getNumber(p[2]) - getNumber(p[4]);
            }else{
                goto err;
            }
        }else if(p[0] == 'p' && p[1] == 'r' && p[5] == ' ' && p[7] == ';'){
            printf("%d\n", var[p[6]]);
        }else{
            goto err;
        }
        while(p[0] != ';')
            p++;
        p++;
    }
    err:
        printf("syntax error: %.10s\n", p);
        exit(1);
}