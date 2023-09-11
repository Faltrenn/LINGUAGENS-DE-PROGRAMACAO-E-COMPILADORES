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

struct memory {
    char symbol;
    struct memory *next;
};

char match(const char *string, struct afd *a, unsigned int final);

void create_afd(unsigned int n, struct afd *a);

struct node* create_node(char symbol, unsigned int state, struct node *next);

void make_transitions(struct afd *a);

void free_afd(struct afd *a);

void free_node(struct node *node);

struct memory* add_memory(char symbol, struct memory *next);

int remove_memory(struct memory *m, char symbol);


int main(int argc, const char * argv[]) {
    struct afd *a = malloc(sizeof(struct afd));
    create_afd(4, a);
    
    make_transitions(a);
    
    printf("%c\n", match(argv[1], a, 1));
    
    free_afd(a);
    
    return 0;
}

char match(const char* string, struct afd *a, unsigned int final) {
    unsigned int s = 0;
    struct memory *m = malloc(sizeof(struct memory));
    m->symbol = '\0';
    m->next = NULL;

    for(int i = 0; string[i] != '\0'; i++) {
        for(struct node *node = a->transitions[s]; node != NULL; node = node->next) {
            if(string[i] == node->symbol) {
                char c = string[i];
                if(c == '(' || c == '[' || c == '{') {
                    add_memory(c, m);
                } else if(c == ')' || c == ']' || c == '}') {
                    if(!remove_memory(m, c)) {
                        return 'f';
                    }
                }
                s = node->state;
                break;
            } else if(node->next == NULL) {
                return 'f';
            }
        }
    }
    return (s == final && m->symbol == '\0') ? 's' : 'f';
}

int remove_memory(struct memory *m, char symbol) {
    if(m->symbol == symbol-1 || m->symbol == symbol-2) {
        if(m->next == NULL) {
            m->symbol = '\0';
            return 1;
        }
        struct memory *aux = m;
        m = m->next;
        free(aux);
        return 1;
    }
    return 0;
}

struct memory* add_memory(char symbol, struct memory *next) {
    if(next->symbol == '\0') {
        next->symbol = symbol;
        return next;
    }
    struct memory *m = malloc(sizeof(struct memory));
    m->symbol = symbol;
    m->next = next;
    return m;
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
    for(int i = 0; i < 4; i++)
        for(char c = '0'; c <= '9'; c++)
            a->transitions[i] = create_node(c, 1, a->transitions[i]);
    
    a->transitions[0] = create_node('-', 2, a->transitions[0]);
    a->transitions[0] = create_node('(', 3, a->transitions[0]);
    a->transitions[0] = create_node('[', 3, a->transitions[0]);
    a->transitions[0] = create_node('{', 3, a->transitions[0]);

    a->transitions[1] = create_node('*', 2, a->transitions[1]);
    for(char c = '+'; c <= '/'; c+=2)
        a->transitions[1] = create_node(c, 2, a->transitions[1]);
    
    a->transitions[1] = create_node(')', 1, a->transitions[1]);
    a->transitions[1] = create_node(']', 1, a->transitions[1]);
    a->transitions[1] = create_node('}', 1, a->transitions[1]);

    a->transitions[2] = create_node('(', 3, a->transitions[2]);
    a->transitions[2] = create_node('[', 3, a->transitions[2]);
    a->transitions[2] = create_node('{', 3, a->transitions[2]);

    a->transitions[3] = create_node('-', 2, a->transitions[3]);
    a->transitions[3] = create_node('(', 3, a->transitions[3]);
    a->transitions[3] = create_node('[', 3, a->transitions[3]);
    a->transitions[3] = create_node('{', 3, a->transitions[3]);
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
