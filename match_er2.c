#include <stdio.h>
#include <stdlib.h>


struct afd {
    unsigned int n;
    unsigned int **transitions;
};

struct afd create_afd(unsigned int n);

char match(char *string, int start, int final, struct afd a);


int main(void) {
    struct afd a = create_afd(10);
    
    for(int i = 0; i < a.n-1; i++) {
        if(i != 5) {
            for(int c = '0'; c <= '9'; c++) {
                a.transitions[i][c] = i+1;
            }
        } else {
            a.transitions[i]['-'] = i+1;
        }
    }

    char *string = malloc(10 * sizeof(char));

    printf("Digite o CEP: ");
    fgets(string, 10, stdin);

    printf("CEP: %s\nState: %d\n", string, match(string, 0, 10, a));
    return 0;
}

char match(char *string, int start, int final, struct afd a) {
    int state = 0;

    for(int i = 0; string[i] != '\0'; i++) {
        state = a.transitions[state][string[i]];
    }
    return state;
}


struct afd create_afd(unsigned int n) {
    struct afd a;
    a.n = n;
    a.transitions = malloc((a.n - 1) * sizeof(unsigned int*));
    for(int i = 0; i < a.n-1; i++) {
        a.transitions[i] = malloc(128 * sizeof(unsigned int));
        for(int j = 0; j < 128; j++) {
            a.transitions[i][j] = i;
        }
    }
    return a;
}
