// C99

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // fabs()

// ----------------------------------------------------------------------
typedef struct {
    double x;
    double y;
} Complexe;

// ----------------------------------------------------------------------
void affiche(const Complexe* z);
Complexe* bad_addition(const Complexe* z1, const Complexe* z2);
Complexe* addition(const Complexe* z1, const Complexe* z2);
int small_c(const Complexe* z);

// ----------------------------------------------------------------------
#define PRECISION 1e-10
#define small_lf(x) (fabs(x) < PRECISION)
#define small_c(pz) (small_lf((pz)->x) && small_lf((pz)->y))

// ----------------------------------------------------------------------
void myfree(const Complexe** ptr)
{
    free((void*) *ptr);
    *ptr = NULL;
}

// ======================================================================
int main(void)
{
    const Complexe a = {  3.0, -2.0 };
    const Complexe* p_b;

    affiche(&a);
    affiche(p_b); // ERR: use non init ptr

    const Complexe b = { -5.0, 1.0 };
    p_b = &b;
    affiche(p_b);

    affiche(bad_addition(&a, p_b));

    affiche(addition(&a, p_b)); // ERR: memory leak (result non free)

    const Complexe* p_c = addition(&a, p_b);
    affiche(p_c);
    const Complexe* p_d = p_c;
    myfree(&p_c);

    const Complexe* p_e = addition(&a, p_d); // ERR: use free ptr
    affiche(p_e);
    myfree(&p_e);

    Complexe* tab = NULL;
    const size_t size = 2;
    tab = calloc(size, sizeof(Complexe));
    for (size_t i = 0; i <= size; ++i) { // ERR: buffer overflow
        p_c = addition(&a, p_b); // temporary value
        tab[i] = *p_c; // make a copy of temporary (for whatever reason...)
        myfree(&p_c); // delete temporary
    }

    myfree(&p_b); // ERR: free ptr on stack

    // ERR: memory leak (tab non free)

    return 0;
}

// ======================================================================
void affiche(const Complexe* p_z)
{
    if (small_c(p_z)) {
        puts("0");
        return;
    }

    double y_affiche = p_z->y;

    if (! small_lf(p_z->x)) {
        printf("%g", p_z->x);
        if (p_z->y > 0.0)
            putchar('+');
        else if (p_z->y < 0.0) {
            putchar('-');
            y_affiche = -p_z->y;
        }
    }
    if (! small_lf(y_affiche)) {
        if (small_lf(p_z->x) && small_lf(y_affiche + 1.0)) {
            putchar('-');
        } else if (! small_lf(y_affiche - 1.0)) {
            printf("%g", y_affiche);
        }
        putchar('i');
    }
    putchar('\n');
}

// ======================================================================
Complexe* bad_addition(const Complexe* p_z1, const Complexe* p_z2)
{
    Complexe z3 = { p_z1->x + p_z2->x, p_z1->y + p_z2->y };
    return &z3; // ERR: local address returned
}

// ======================================================================
Complexe* addition(const Complexe* p_z1, const Complexe* p_z2)
{
    Complexe* p_z3 = malloc(sizeof(Complexe));
    *p_z3 = (Complexe) {
        p_z1->x + p_z2->x, p_z1->y + p_z2->y
    };
    return p_z3;
}
