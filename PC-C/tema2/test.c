#include <string.h>
#include <stdio.h>

char get_input(char *type){
    char inp;
    do{
        inp = getchar();
    } while (!strchr(type, inp));
    return inp;
}

int char_to_int(char c){
    return (int)c - 48;
}

int main(){
    char numbers[50] = "0123456789";
    char c='a';
    int x = 1;
    strncat(numbers, &c, 49 - strlen(numbers));
    printf("%s\n", numbers);
    // while (x != 0){
    //     x = get_input(numbers);
    //     printf("cod ascii:%d  numar:%d\n", x, char_to_int(x));
    // }
    return 0;
}