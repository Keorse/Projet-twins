#include <stdio.h>
typedef struct avion{
    int id;
    int cat_av;//1 av de ligne    2 av affaire     3 av léger
    int is_parked;// 1 pour un avion en vol    0 pour un avion au sol
    int nb_passengers;
    struct avion* suiv;
}avion;






typedef struct piste{
    int num_piste;
    float longueur;
    int cat_piste;//1 petite  2 Moyenne   3 petite
    int max_await_takeoff;
    int is_busy;//1 occupé   2 libre
    //int ping-pong;
    avion* liste_av;//1er avion de la liste
}piste;



avion init_avion(int id,int cat_av, int is_flying , int nb_passaers);
void print_avion(avion avion);
piste init_piste(int num_piste, float longueur, int cat_piste, int max_await);
avion init_avion(int id,int cat_av, int is_flying , int nb_passaers);

int main() {
    piste piste_une=init_piste(1, 3000, 1, 10);

    return 0;
}


piste init_piste(int num_piste, float longueur, int cat_piste, int max_await){
        piste piste;
        piste.num_piste=num_piste;
        piste.longueur=longueur;
        piste.cat_piste=cat_piste;
        piste.max_await=max_await;
        piste.liste_av=NULL;
        return piste;
};

avion init_avion(int id,int cat_av, int is_flying , int nb_passaers){
    avion avion1;
    avion1.nb_passengers=nb_passaers;
    avion1.is_flying=is_flying;
    avion1.cat_av=cat_av;
    avion1.id=id;
    avion1.suiv=NULL;
    return avion1;
};

void print_piste(piste piste){
    printf("numero de la piste:%d\n",piste.num_piste);
    printf("longueur de la piste:%f\n",piste.longueur);
    printf("categorie de la piste:%d\n",piste.cat_piste);
    printf("numero de la piste:%d\n",piste.max_await_takeoff);
    avion* tmp= piste.liste_av;
    if(tmp!=NULL){
        while(tmp!=NULL){
            print_avion(*tmp);
            tmp=tmp->suiv;
        }
    }
};



void print_avion(avion avion){
    printf("id de l'avion:%d\n",avion.id);
    printf("catégorie de l'avion:%d\n",avion.cat_av);
    printf("is_flying:%d\n",avion.is_parked);
    printf("passagers de l'avion:%d\n",avion.nb_passengers);
}
