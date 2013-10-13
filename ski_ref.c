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
                term_t node = malloc(sizeof(struct term));
                node->type = NOT_ATOM;
                node->f = *(--sp);
                node->x = *(--sp);
                *sp = node;
                sp++;
            }
            break;
        default:
            return c;
    }
    return 0;
}

void freeall(term_t n) {
    if(n->type == NOT_ATOM) {
        freeall(n->f);
        freeall(n->x);
        free(n);
    }
}

int reduce(term_t* pp) {
    term_t p = *pp;

    // One level...
    if(p->type == NOT_ATOM) {

        // Check for I
        if(p->f->type == ATOM_I) {
            *pp = p->x;
            free(p);
            return 1;
        }

        // Two levels...
        if(p->f->type == NOT_ATOM) {

            // Check for K
            if(p->f->f->type == ATOM_K) {
                *pp = p->f->x;
                freeall(p->x);
                free(p);
                return 2;
            }

            // Three levels...

            if(p->f->f->type == NOT_ATOM) {

                // Check for S
                if(p->f->f->f->type == ATOM_S) {
                    term_t zy = malloc(sizeof(struct term));
                    term_t zx = malloc(sizeof(struct term));
                    term_t zyzx = malloc(sizeof(struct term));
                    zyzx->type = zy->type = zx->type = NOT_ATOM;

                    zy->x = zx->x = p->x;
                    zy->f = p->f->x;
                    zx->f = p->f->f->x;
                    zyzx->x = zy;
                    zyzx->f = zx;

                    free(p->f->f);
                    free(p->f);
                    free(p);
                    *pp = zyzx;
                    return 3;
                }
            }
        }

        int s = reduce(&(p->f));
        if(!s) {
            return reduce(&(p->x));
        } else {
            return s;
        }

    } else {
        return 0;
    }
}

int main(void) {
    char c;
    printf("> ");
    do {
        c = getchar();
    } while(!parse(c));

    do {
        term_t* p = sp;
        while(p > stack) {
            p--;
            show(*p,0);
            printf("===\n");
        }
    } while(reduce(sp-1));

    return 0;
}

