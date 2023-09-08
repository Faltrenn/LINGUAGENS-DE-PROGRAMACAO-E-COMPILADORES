#include <stdio.h>
#include <stdlib.h>


struct line {
    char symbol;
    unsigned int next_state;
    struct line *next;
};

struct afd {
    unsigned int n;
    struct line **transitions;
};

struct afd create_afd(unsigned int n);

struct line* create_line(char symbol, unsigned int next_state);

char match(char *string, int start, int final, struct afd a);


int main(void) {
    struct afd a = create_afd(10);
    for(int i = 0; i < a.n; i++) {
        if(i != 5) {
            struct line *aux;
            for(int j = '0'; j <= '9'; j++) {
                aux = create_line(j, i+1);
                aux->next = a.transitions[i];
                a.transitions[i] = aux;
            }
        } else {
            a.transitions[i] = create_line('-', 6);
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
        for(struct line *aux = a.transitions[state]; aux != NULL; aux = aux->next) {
            if(aux->symbol == string[i]) {
                state = aux->next_state;
                break;
            } else if (aux->next == NULL) {
                return state;
            }
        }
    }
    return state;
}


struct afd create_afd(unsigned int n) {
    struct afd a;
    a.n = n;
    a.transitions = malloc(a.n * sizeof(struct line*));
    for(int i = 0; i < a.n; i++) {
        a.transitions[i] = NULL;
    }
    return a;
}

struct line* create_line(char symbol, unsigned int next_state) {
    struct line *l = malloc(sizeof(struct line));
    l->symbol = symbol;
    l->next_state = next_state;
    l->next = NULL;
    return l;
}
