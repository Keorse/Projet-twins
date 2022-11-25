#include <stdio.h>
#include <stdlib.h>

//jai modifier piste et avion pour que ce soit des liste chainée

typedef struct avion avion;
struct avion{
    int id;// à  verifier a chaque init pour que il soit unique
    int cat_av;//1 av de ligne    2 av affaire     3 av léger
    int is_parked;// 1 pour un avion en vol    0 pour un avion au sol
    int nb_passengers;
};

typedef struct liste{
    avion avion;
    struct liste * suiv;
}liste;


typedef struct piste piste;
struct piste{
    int num_piste;
    float longueur;
    int cat_piste;//1 petite  2 Moyenne   3 petite
    int max_await_takeoff;
    int is_busy;//1 occupé   2 libre
    //int ping-pong;
    liste* liste_av;//1er avion de la liste
};

//------------------------------structure parking
typedef struct parking parking;
struct parking{
    int maxParking;
    avion* liste_av;
};




int compteurPiste(piste* );
int compteurParking(parking* );
avion* rechercheID(avion *listeAvion, int ID);
void atterrissage(piste *piste_une, parking *parking1, avion *AVIONS);
avion init_avion(int id,int cat_av, int is_flying , int nb_passaers);
void print_avion(avion *avion);
void print_piste(piste*);
piste *init_piste(int num_piste, float longueur, int cat_piste, int max_await);
avion *init_avion(int id,int cat_av, int nb_passaers);


//--------------------------------main-------------------------------------------//



int main() {
   avion *avion1 = NULL;
    avion1 = init_avion(1, 3, 0);

    avion *avion2 = NULL;
    avion2 = malloc(sizeof(avion));
    init_avion(1, 3, 0);
    
    piste *piste1 = NULL;
    piste1 = init_piste(9, 90, 3, 1);
    piste1->liste_av = avion1;


    parking *parking1 = NULL;
    parking1 = malloc(sizeof(parking));
    parking1->maxParking =1;
    parking1->liste_av = avion2;
    
    avion *listeVOLENT = NULL;
    listeVOLENT = init_avion(2, 3, 10);

    atterrissage(piste1, parking1, listeVOLENT);
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
//JAI RAJOUTE RECHERCHE ID POUR POUVOIR SELECTIONNER LAVION A FAIRE ATTERIR DANS LA FONCTION ATTERRISSAGE

avion* rechercheID(avion *listeAvion, int ID){
    avion *retour;
    if(listeAvion != NULL){
        avion *parcour = listeAvion;
        while (parcour != NULL)
        {
            if (parcour->id == ID)
            {
                retour = parcour;
                return retour;
            }
            
            parcour = parcour->suiv;
        }
        printf("id non-trouvé\n");
    }
    else printf("erreur liste vide\n");
    return NULL;
}
//RECHERCHE ID, TROUVE LAVION,LE STOCK DANS UNE VARIABLE QUI SERA RETOURNER EN SORTIE. LAVION EST AUSSI SUPPRIME DE LA LISTE DES AVIONS EN VOLS


//-------------------------fonction atterrissage--------------------------------------//
void atterrissage(piste *piste_une, parking *parking1, avion *AVIONS){
    int cpt = compteurPiste(piste_une);
    int cpt2 = compteurParking(parking1);
    int ID;
    avion *avion_atterri; 
    printf("quel avion voulez-vous faire atterrir (entrez son id)");
    scanf("%d", &ID);
    avion_atterri = rechercheID(AVIONS, ID);

    if(avion_atterri != NULL){
        if(cpt < piste_une->max_await_takeoff){         //test si on peut passer sur piste pour aller garage
            if(cpt2 < parking1->maxParking){             //test si place dans le garage 
                avion *tmp = parking1->liste_av;
                while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                    tmp = tmp->suiv;
                }
                tmp->suiv = avion_atterri;                     //Place l'avion dans le garage
                printf("\nl'avion a atterri\n\n");
            }

            else{
                avion *stocktmp_parking = parking1->liste_av;
                stocktmp_parking->suiv = NULL;
                avion *parcour_piste = piste_une->liste_av;
                while (parcour_piste->suiv != NULL)
                {
                    parcour_piste = parcour_piste->suiv;
                }
                parcour_piste->suiv = stocktmp_parking;
                parking1->liste_av = parking1->liste_av->suiv;

                avion *tmp = parking1->liste_av;
                while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                    tmp = tmp->suiv;
                }
                tmp->suiv = avion_atterri;   
                printf("\nl'avion a atterri\n\n");             
            }
        }
        else if(cpt = piste_une->max_await_takeoff){


            avion *stock_temporaire = piste_une->liste_av;
            stock_temporaire->suiv = NULL;
            avion *parcour = AVIONS;

            while (parcour->suiv != NULL)
            {
                parcour = parcour->suiv;
            }
            parcour->suiv = stock_temporaire;

            piste_une->liste_av = piste_une->liste_av->suiv;            //on fait decoller le premiere avion qui est arrivé dans la liste,  ca revient a le supprimé de la liste mais pas le free


                    //une fois qu'on a fait decoller un avion, on peut utiliser la piste pour aller au parking si il y a de la place
            if(cpt2 < parking1->maxParking){             //test si place dans le garage 
                avion *tmp = parking1->liste_av;
                while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                    tmp = tmp->suiv;
                }
                tmp->suiv = avion_atterri;                     //Place l'avion dans le garage
                printf("\nl'avion a atterri\n\n");
            }
            else{
                avion *stocktmp_parking = parking1->liste_av;
                stocktmp_parking->suiv = NULL;
                avion *parcour_piste = piste_une->liste_av;
                while (parcour_piste->suiv != NULL)
                {
                    parcour_piste = parcour_piste->suiv;
                }
                parcour_piste->suiv = stocktmp_parking;
                parking1->liste_av = parking1->liste_av->suiv;

                avion *tmp = parking1->liste_av;
                while(tmp->suiv != NULL){               //parcour liste chainée du garage 
                    tmp = tmp->suiv;
                }
                tmp->suiv = avion_atterri;
                printf("\nl'avion a atterri\n\n");
            }
        }
    }
    else printf("\n il n'y a pas d'avions dans le ciel\n\n");
}
//---------------------------------------------------------------------------------------------------------------------//


piste *init_piste(int num_piste, float longueur, int cat_piste, int max_await){
        piste *piste1 = NULL;
        piste1 = malloc(sizeof(piste));
        
        piste1->num_piste=num_piste;
        piste1->longueur=longueur;
        piste1->cat_piste=cat_piste;
        piste1->max_await_takeoff=max_await;
        piste1->liste_av=NULL;
        return piste1;
};

avion *init_avion(int id,int cat_av, int nb_passaers){
    avion *avion1 = NULL;
    avion1 = malloc(sizeof(avion));
    avion1->nb_passengers=nb_passaers;
    avion1->cat_av=cat_av;
    avion1->id=id;
    avion1->suiv=NULL;
    return avion1;
};

void print_piste(piste *piste){
    printf("numero de la piste:%d\n",piste->num_piste);
    printf("longueur de la piste:%f\n",piste->longueur);
    printf("categorie de la piste:%d\n",piste->cat_piste);
    printf("numero de la piste:%d\n",piste->max_await_takeoff);
    avion* tmp= piste->liste_av;
    if(tmp!=NULL){
        while(tmp!=NULL){
            print_avion(tmp);
            tmp=tmp->suiv;
        }
    }
};



void print_avion(avion *avion){
    printf("id de l'avion:%d\n",avion->id);
    printf("catégorie de l'avion:%d\n",avion->cat_av);
    printf("is_flying:%d\n",avion->is_parked);
    printf("passagers de l'avion:%d\n",avion->nb_passengers);
}
