#include <stdio.h>
#include <stdlib.h>


struct node {
    char symbol;
    unsigned int state;
    struct node *next;
};

struct afd {
    unsigned int n;
    struct node **transitions;
};

char match(char *string, struct afd *a, unsigned int final);

void create_afd(unsigned int n, struct afd *a);

struct node* create_node(char symbol, unsigned int state, struct node *next);

void make_transitions(struct afd *a);

void free_afd(struct afd *a);

void free_node(struct node *node);


int main(int argc, const char * argv[]) {
    struct afd *a = malloc(sizeof(struct afd));
    create_afd(3, a);
    
    make_transitions(a);
    
    printf("%c\n", match("-35555", a, 1));
    
    free_afd(a);
    
    return 0;
}

char match(char* string, struct afd *a, unsigned int final) {
    unsigned int s = 0;
    for(int i = 0; string[i] != '\0'; i++) {
        for(struct node *node = a->transitions[s]; node != NULL; node = node->next) {
            if(string[i] == node->symbol) {
                s = node->state;
                break;
            } else if(node->next == NULL) {
                return 'f';
            }
        }
    }
    
    return s == final ? 's' : 'f';
}

void create_afd(unsigned int n, struct afd *a) {
    a->n = n;
    a->transitions = malloc(n * sizeof(struct node));
    for(int i = 0; i < n; i++) {
        a->transitions[i] = NULL;
    }
}

struct node* create_node(char symbol, unsigned int state, struct node *next) {
    struct node *n = malloc(sizeof(struct node));
    n->symbol = symbol;
    n->state = state;
    n->next = next;
    return n;
}

void make_transitions(struct afd *a) {
    for(int i = 0; i < a->n; i++)
        for(char c = '0'; c <= '9'; c++)
            a->transitions[i] = create_node(c, 1, a->transitions[i]);
    a->transitions[0] = create_node('-', 2, a->transitions[0]);
    
    a->transitions[1] = create_node('*', 2, a->transitions[1]->next);
    for(char c = '+'; c <= '/'; c+=2)
        a->transitions[1] = create_node(c, 2, a->transitions[1]);
}

void free_node(struct node *node) {
    if(node->next != NULL)
        free_node(node->next);
    free(node);
}

void free_afd(struct afd *a) {
    for(int i = 0; i < a->n; i++) {
        free_node(a->transitions[i]);
    }
    free(a->transitions);
    free(a);
}
