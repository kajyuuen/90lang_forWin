#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UC;

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

int isNumber(UC c)
{
    if('0' <= c && c<= '9'){
        return 1;
    }
    return 0;
}

int isAlphabet(UC c){
    if('a' <= c && c <= 'z'){
        return 1;
    }
    if('A' <= c && c <= 'Z'){
        return 1;
    }
    if(c == '_'){
        return 1;
    }
    return 0;
}

int isSymbol(UC c)
{
    if(strchr("=+-*/%&~|<>?:.", c) != 0){
        return 1;
    }
    return 0;
}

int isSymbol1(UC c)
{
    if(strchr("(){}[];,", c) != 0){
        return 1;
    }
    return 0;
}

UC **lexer(UC *s, UC *b, UC **t)
{
    for(;;){
        if(s[0] == ' ' || s[0] == '\t' || s[0] == '\n'){
            s++;
            continue;
        }
        *t++ = b;
        if(s[0] == 0){
            *b++ = 0;
            return t;
        }
        if(isSymbol1(s[0]) != 0){
            *b++ = s[0];
            s++;
        }else if(isNumber(s[0]) != 0){
            while(isNumber(s[0]) != 0){
                *b++ = s[0];
                s++;
            }
        }else if(isAlphabet(s[0]) != 0){
            while(isAlphabet(s[0]) != 0 || isNumber(s[0]) != 0){
                *b++ = s[0];
                s++;
            }
        }else if(isSymbol(s[0]) != 0){
            while(isSymbol(s[0]) != 0){
                *b++ = s[0];
                s++;
            }
        }else{
            printf("syntax error : %.10s\n", s);
            exit(1);
        }
        *b++ = 0;
    }
}

int var[1000];
UC *varName[1000];

int varNumber(UC *t)
{
    int i = 0;
    while(varName[i] != 0){
        if(strcmp(t, varName[i]) == 0){
            return i;
        }
        i++;
    }
    varName[i] = t;
    return i;
}

UC **t, **t0;

void errExit(const char *m){
    printf("syntax error : %s %s %s %s\n", t[0], t[1], t[2], t[3]);
    exit(1);
}

void skipToken(const char *s){
    char m[100];
    if(strcmp(t[0], s) != 0){
        sprintf(m, "need %s", s);
        errExit(m);
    }
    t++;
    return;
}

int calcNumber();

int getNumber()
{
    int i = 0;
    if(strcmp(t[0], "(") == 0){
        t++;
        i = calcNumber();
        skipToken(")");
    }else if(strcmp(t[0], "-") == 0){
        i = - getNumber();
    }else if(isNumber(t[0][0]) != 0){
        sscanf(t[0], "%d", &i);
        t++;
    }else if(isAlphabet(t[0][0]) != 0){
        i = var[varNumber(t[0])];
        t++;
    }
    return i;
}

int calcNumber()
{
    int i;
    i = getNumber();
    for(;;){
        if(strcmp(t[0], "+") == 0){
            t++;
            i += getNumber();
        }else if(strcmp(t[0], "-") == 0){
            t++;
            i -= getNumber();
        }else if(strcmp(t[0], "*") == 0){
            t++;
            i *= getNumber();
        }else{
            break;
        }
    }
    return i;
}

int main(int argc, const char **argv)
{
    UC txt[10000], buf[10000], *token[10000];
    int i;
    loadText(argc, argv, txt, 10000);
    t = lexer(txt, buf, token);
    t[0] = "";
    t[1] = "";
    t[2] = "";

    t = token;
    for(;;){
        t0 = t;
        if(strcmp(t[0], "") == 0){
            exit(0);
        }
        if(isAlphabet(t[0][0]) != 0 && strcmp(t[1], "=") == 0){
            i = varNumber(t[0]);
            t += 2;
            var[i] = calcNumber();
        }else if(strcmp(t[0], "print") == 0){
            t++;
            printf("%d\n", calcNumber());
        }else{
            errExit("syntax error");
        }
        skipToken(";");
    }
}