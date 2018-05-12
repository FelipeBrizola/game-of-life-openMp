#include <stdio.h>
#include <omp.h>

#define CELL(I, J) (field[size * (I) + (J)])
#define ALIVE(I, J) t[size * (I) + (J)] = 1
#define DEAD(I, J) t[size * (I) + (J)] = 0

#define FIELD_SIZE 40
#define FIELD_GEN 10000

int count_alive(const char *field, int i, int j, int size) {
    int x, y, a = 0;

    #pragma omp parallel for private(x, y)
    for (x = i - 1; x <= (i + 1); x++) {
        for (y = j - 1; y <= (j + 1); y++) {
            if ((x == i) && (y == j))
                continue;
            if ((y < size) && (x < size) &&
                (x >= 0) && (y >= 0)) {
                a += CELL(x, y);
            }
        }
    }
    return a;
}

void evolve(const char *field, char *t, int size) {
    int i, j, alive, cs;


    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            alive = count_alive(field, i, j, size);
            cs = CELL(i, j);
            if (cs) {
                if ((alive > 3) || (alive < 2))
                    DEAD(i, j);
                else
                    ALIVE(i, j);
            }
            else {
                if (alive == 3)
                    ALIVE(i, j);
                else
                    DEAD(i, j);
            }
        }
    }
}
char field[FIELD_SIZE * FIELD_SIZE];
char temp_field[FIELD_SIZE * FIELD_SIZE];

/* set the cell i,j as alive */
#define SCELL(I, J) field[FIELD_SIZE * (I) + (J)] = 1

void dump_field(const char *f, int size) {
    int i;
    for (i = 0; i < (size * size); i++) {
        if ((i % size) == 0)
            printf("\n");
        printf("%c", f[i] ? 'X' : '.');
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int i;
    char *fa, *fb, *tt;

    for (i = 0; i < (FIELD_SIZE * FIELD_SIZE); i++)
        field[i] = 0;

    /* prepare the glider */
    SCELL(10, 1);
    SCELL(10, 2);
    SCELL(11, 2);

    SCELL(2, 0);
    SCELL(2, 1);
    SCELL(2, 2);

    SCELL(2, 10);
    SCELL(3, 11);
    SCELL(3, 9);
    SCELL(1, 12);
    
    SCELL(2, 15);
    SCELL(3, 14);
    SCELL(3, 13);
    SCELL(1, 16);

    SCELL(20, 21);
    SCELL(19, 20);
    SCELL(18, 22);

    /* evolve */
    fa = field;
    fb = temp_field;
    for (i = 0; i < FIELD_GEN; i++) {
        dump_field(fa, FIELD_SIZE);
        evolve(fa, fb, FIELD_SIZE);
        tt = fb;
        fb = fa;
        fa = tt;
    }
    return 0;
    
}