// Reference implementation of combinatory algebra

#include <stdio.h>
#include <stdlib.h>

struct term {
    enum {
        ATOM_S,
        ATOM_K,
        ATOM_I,
        NOT_ATOM
    } type;
    struct term* f; // The combinator being applied. Traditionally "left," here "right"
    struct term* x; // The combinator being operated on. Traditionally "right," here "left"
};

typedef struct term* term_t;

term_t stack[640]; //ought to be enough for anybody (TODO)
term_t* sp = stack;

struct term atoms[3][1] = {{{ATOM_S, NULL, NULL}}, {{ATOM_K, NULL, NULL}}, {{ATOM_I, NULL, NULL}}};

void indent(int level) {
    while(level--) { putchar(' '); }
}

void show(term_t t, int level) {
    indent(level);
    switch(t->type) {
        case ATOM_S:
            putchar('S'); putchar('\n');
            break;
        case ATOM_K:
            putchar('K'); putchar('\n');
            break;
        case ATOM_I:
            putchar('I'); putchar('\n');
            break;
        case NOT_ATOM:
            puts("=>");
            show(t->x,level+2);
            show(t->f,level+2);
            break;
    }
}

int parse(char c) {
    switch(c) {
        case 'S':
            *(sp++) = atoms[ATOM_S];
            break;
        case 'K':
            *(sp++) = atoms[ATOM_K];
            break;
        case 'I':
            *(sp++) = atoms[ATOM_I];
            break;
        case '.':
            if(sp - stack >= 1) {
                //fprintf(stderr,"foo\n");
                //show(*(sp-1),0);
                //show(*(sp-2),0);
                term_t node = malloc(sizeof(struct term));
                node->type = NOT_ATOM;
                node->f = *(--sp);
                node->x = *(--sp);
                *sp = node;
                sp++;
                //show(*sp,0);
                //fprintf(stderr,"oof\n");
            }
            break;
        default:
            return c;
    }
    return 0;
}

int main(void) {
    char c;
    do {
        c = getchar();
    } while(!parse(c));

    while(sp > stack) {
        sp--;
        show(*sp,0);
        printf("===\n");
    }

    return 0;
}

