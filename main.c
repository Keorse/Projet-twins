#include <stdio.h>
typedef struct avion{
    int id;
    int cat_av;//1 av de ligne    2 av affaire     3 av léger
    int is_flying;// 1 pour un avion en vol    0 pour un avion au sol
    int nb_passengers;
}avion;

typedef struct piste{
    int num_piste;
    float longueur;
    int cat_piste;//1 petite  2 Moyenne   3 petite
    int max_await;
    avion* liste_av;
}piste;
int main() {
    printf("Hello, World!\n");
    return 0;
}


piste init_piste(int num_piste, float longueur, int cat_piste, int max_await){
    piste piste;
    
    piste.num_piste=num_piste;
    
    
    return piste;
};
