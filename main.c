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

typedef struct liste{
    avion* avion;
}liste;

//------------------structure parking---------------------//
typedef struct parking parking;
struct parking{
    int maxParking;
    avion* liste_av;
};




int compteurPiste(piste* );
int compteurParking(parking* );
void atterrissage(piste *piste_une, parking *parking1, avion *avion1);
avion init_avion(int id,int cat_av, int is_flying , int nb_passaers);
void print_avion(avion avion);
piste init_piste(int num_piste, float longueur, int cat_piste, int max_await);
avion init_avion(int id,int cat_av, int is_flying , int nb_passaers);

int main() {
    piste piste_une=init_piste(1, 3000, 1, 10);

    return 0;
}



//-------------------------------------fonction pour compter le nb davions--------------//
int compteurPiste(piste* piste_une){
    int cpt = 0;
    avion *tmp;
    tmp = piste_une->liste_av; 
    while(tmp != NULL){
        cpt++;
        tmp = tmp->suiv;            //->suiv car on est dans la liste avion 
    }
    return cpt;
}

int compteurParking(parking* p){
    int cpt = 0;
    avion *tmp;
    tmp = p->liste_av; 
    while(tmp != NULL){
        cpt++;
        tmp = tmp->suiv;
    }
    return cpt;
}
//-------------------------------------------------------------------------------------//
//-------------------------fonction atterrissage--------------------------------------//
void atterrissage(piste *piste_une, parking *parking1, avion *avion1){
    int cpt = compteurPiste(piste_une);
    int cpt2 = compteurParking(parking1);
    if((cpt+1) < piste_une->max_await_takeoff){         //test si on peut passer sur piste pour aller garage    !!!!!! c'est cpt+1 enft
        if(cpt2 < parking1->maxParking){             //test si place dans le garage 
            avion *tmp = parking1->liste_av;
            while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                tmp = tmp->suiv;
            }
            tmp->suiv = avion1;                     //Place l'avion dans le garage
        }

        else if(cpt2 >= parking1->maxParking){
            //mettre la fonction affectation a la liste d'attente de la piste ici(adri)
            
            //BA LA JE SAIS PAS, par ce que les avions dans le garage n'ont pas forcement besoin de decoller....
            //Si si il faut en degager un si il n'y a plus de place dans le garage a moins que on fasse un Allahouabar mais bof perso(adri)
        }
    }
    else if(cpt = piste_une->max_await_takeoff){


        //!!doit trouver un truc pour stockée l'avion qui part!!//
        //fonction takeoff dcp(adri)
        piste_une->liste_av = piste_une->liste_av->suiv;            //on fait decoller le premiere avion qui est arrivé dans la liste,  ca revient a le supprimé de la liste mais pas le free



                //une fois qu'on a fait decoller un avion, on peut utiliser la piste pour aller au parking si il y a de la place
        if(cpt2 < parking1->maxParking){             //test si place dans le garage 
            avion *tmp = parking1->liste_av;
            while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                tmp = tmp->suiv;
            }
            tmp->suiv = avion1;                     //Place l'avion dans le garage
        }
        else{
            //BA LA JE SAIS PAS, par ce que les avions dans le garage n'ont pas forcement besoin de decoller....
            //la meme que au dessus mon gars
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------//


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
