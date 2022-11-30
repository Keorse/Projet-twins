
#include <stdio.h>
#include <stdlib.h>


typedef struct avion{
    int id;// à  verifier a chaque init pour que il soit unique
    int cat_av;//1 av de ligne    2 av affaire     3 av léger
    int is_parked;// 1 pour un avion en vol    0 pour un avion au sol
    int nb_passengers;
}avion;

typedef struct liste liste;
struct liste{
    avion avion;
    struct liste *suiv;
};


typedef struct piste{
    int num_piste;
    float longueur;
    int cat_piste;//1 petite  2 Moyenne   3 petite
    int max_await_takeoff;
    int is_busy;//1 occupé   2 libre
    liste *liste_av;
}piste;

//------------------------------structure parking
typedef struct parking{
    int maxParking;
    liste *liste_av;
}parking;






int compteurPiste(piste piste_une);
int compteurParking(parking p);
avion rechercheID(liste *listeAvion, int ID);
liste* ajouteAvionFin(liste *Liste, avion Avion);
void atterrissage(piste piste_une, parking parking1, liste *AVIONS);
piste init_piste(int num_piste, float longueur, int cat_piste, int max_await);
avion init_avion(int id,int cat_av, int nb_passaers);
liste* add_waitlist_piste(piste piste_une, parking parking1);//retourne la liste d'attente de la piste donée en parametre
liste* RetireAvionDebut(liste *Liste);
void decollage(piste piste, liste* air);
void menu();
void affiche_Avion(avion avion);





int main(){
    avion avion1; 
    avion1 = init_avion(1, 1, 10);


    avion avion2 = init_avion(2,1,20);
    avion avion3 = init_avion(3,1,10); 
    piste piste1 = init_piste(3, 90, 3, 2);
    
    piste1.liste_av->avion = avion2; 

    parking parking1; 
    parking1.maxParking =1;
    parking1.liste_av = NULL;
    parking1.liste_av = malloc(sizeof(liste));
    parking1.liste_av->avion = avion3; 


    liste *tetedeliste = NULL;
    liste *avionVolent; 
    avionVolent = malloc(sizeof(liste));
    avionVolent->avion = avion1;
    avionVolent->suiv = NULL;
    
    tetedeliste = avionVolent;
    atterrissage(piste1, parking1, tetedeliste);

    return 0;
}


int compteurPiste(piste piste_une){
    int cpt = 0;
    liste *tmp = NULL;
    tmp = piste_une.liste_av; 
    while(tmp != NULL){
        cpt++;
        tmp = tmp->suiv;            //->suiv car on est dans la liste avion 
    }
    return cpt;
}

int compteurParking(parking p){
    int cpt = 0;
    liste *tmp;
    tmp = p.liste_av; 
    while(tmp != NULL){
        cpt++;
        tmp = tmp->suiv;
    }
    return cpt;
}


avion rechercheID(liste *listeAvion, int ID){
    avion retour;
    if(listeAvion != NULL){
        liste *parcour = listeAvion;
        while (parcour != NULL)
        {
            if (parcour->avion.id == ID)
            {
                retour = parcour->avion;
                return retour;
            }
            
            parcour = parcour->suiv;
        }
        printf("id non-trouvé\n");
    }
    else printf("erreur liste vide\n");
    exit(0);
}






liste* ajouteAvionFin(liste *Liste, avion Avion){
    if(Liste != NULL){
	    /*créer un maillon*/ 
	    liste *variable = NULL;
	    variable = malloc(sizeof(liste));
	    variable->avion = Avion;
	    variable->suiv = NULL;
	    
	    liste *tmp = Liste;
	    while (tmp->suiv != NULL)
	    {
		tmp = tmp->suiv;
	    }
	    tmp->suiv = variable;  //ajoute le maillon a la fin de la liste
	    return Liste;
    }
    else{					//en gros la jdisais que si la liste elle etais vide yavait un problème
	liste *variable = NULL;			//sauf que non, si elle est vide, on la comble
	variable = malloc(sizeof(liste));       // c'est le but de la fonction 
	variable->avion = Avion;
	variable->suiv = NULL;
	    
	Liste = variable; 
	return Liste;
    }
}





void atterrissage(piste piste_une, parking parking1, liste *AVIONS){
    int cpt = compteurPiste(piste_une);
    int cpt2 = compteurParking(parking1);
    int ID;
    avion avion_atterri; 
    printf("quel avion voulez-vous faire atterrir (entrez son id)");
    scanf("%d", &ID);
    avion_atterri = rechercheID(AVIONS, ID);

    if(&avion_atterri != NULL){                         //&avion != NULL parce que s'il y a une adresse c quil existe, sans le & ca marche pas parce que les NULL c bizarre
        if(cpt < piste_une.max_await_takeoff){         //test si on peut passer sur piste pour aller garage

            if(cpt2 < parking1.maxParking){             //test si place dans le garage 
                parking1.liste_av = ajouteAvionFin(parking1.liste_av, avion_atterri);                   //Place l'avion dans le garage
                printf("\nl'avion a atterri\n\n");
            }
            else{
                /*faire partir un avion du garage sur la piste*/
                avion premier_parking = parking1.liste_av->avion; //on stock le 1er avion du parking pour apres lamener sur piste
                parking1.liste_av = parking1.liste_av->suiv; //lavion est enlever du parking

                piste_une.liste_av = ajouteAvionFin(piste_une.liste_av, premier_parking);

                /*faire entrer l'avion dans le garage*/
                parking1.liste_av = ajouteAvionFin(parking1.liste_av, avion_atterri);
                printf("\nl'avion a atterri\n\n");             
            }

        }

        else if(cpt = piste_une.max_await_takeoff){
            /*sort un avion de la piste et on le met en lair*/
            avion degage_piste = piste_une.liste_av->avion;
            piste_une.liste_av = piste_une.liste_av->suiv;
            AVIONS = ajouteAvionFin(AVIONS, degage_piste); 

            /*une fois qu'on a fait decoller un avion, on peut utiliser la piste pour aller au parking si il y a de la place*/
            if(cpt2 < parking1.maxParking){             //test si place dans le garage 
                parking1.liste_av = ajouteAvionFin(parking1.liste_av, avion_atterri);                   //Place l'avion dans le garage
                printf("\nl'avion a atterri\n\n");
            }
            else{
                /*faire partir un avion du garage sur la piste*/
                avion premier_parking = parking1.liste_av->avion; //on stock le 1er avion du parking pour apres lamener sur piste
                parking1.liste_av = parking1.liste_av->suiv; //lavion est enlever du parking

                piste_une.liste_av = ajouteAvionFin(piste_une.liste_av, premier_parking);

                /*faire entrer l'avion dans le garage*/
                parking1.liste_av = ajouteAvionFin(parking1.liste_av, avion_atterri);
                printf("\nl'avion a atterri\n\n");
            }
        }
    }
    else printf("\n il n'y a pas d'avions dans le ciel\n\n");
}




piste init_piste(int num_piste, float longueur, int cat_piste, int max_await){
        piste piste1;
        piste1.num_piste=num_piste;
        piste1.longueur=longueur;
        piste1.cat_piste=cat_piste;
        piste1.max_await_takeoff=max_await;
        piste1.liste_av=NULL;
        piste1.liste_av = malloc(sizeof(liste));
        return piste1;
};




avion init_avion(int id,int cat_av, int nb_passaers){
    avion avion1;
    avion1.nb_passengers=nb_passaers;
    avion1.cat_av=cat_av;
    avion1.id=id;
    return avion1;
};

liste* add_waitlist_piste(piste piste_une, parking parking1){
    int cpt_piste = compteurPiste(piste_une);
    int cpt_park = compteurParking(parking1);
    if(cpt_park==0){
        printf("il n'y as pas d'avions dans le parking\n");
        return piste_une.liste_av;
    }
    int ID;
    avion avion_decollage;
    printf("quel avion voulez-vous faire atterrir (entrez son id)");
    scanf("%d", &ID);
    avion_decollage = rechercheID(parking1.liste_av, ID);
    if(cpt_piste<piste_une.max_await_takeoff){
        piste_une.liste_av= ajouteAvionFin(piste_une.liste_av,avion_decollage);
        printf("\nl'avion attends de décoller\n\n");
        return piste_une.liste_av;
    }
    if(cpt_piste==piste_une.max_await_takeoff){

        // METTRE FONCTION DECOLLAGE


        piste_une.liste_av= ajouteAvionFin(piste_une.liste_av,avion_decollage);
        printf("\nl'avion attends de décoller\n\n");
        return piste_une.liste_av;

    }
    return piste_une.liste_av;
}


void decollage(piste piste, liste* air){
    avion avion_decollage=piste.liste_av->avion;
    RetireAvionDebut(piste.liste_av);
    ajouteAvionFin(air,avion_decollage);
}
void menu()
{
	int a=0,j=0;
	while(j==0)
	{
		printf(" \t=================================================\n");
		printf("\t\tBIENVENUE DANS L'AEROPORT ONE PISTE\n");
		printf("\t=================================================\n");
		
		
			printf("\t----------------------------------------------------\n");
			printf("\tque voulez vous faire:\n\n");
			printf("\t1: lancer une simulation\n\t2: ajouter un avion a la base de donnés\n\t3: quitter\n");
			printf("\t----------------------------------------------------\n");
			scanf("%d",&a);
		switch(a)
		{
			case 1:
				//lancer la simulation
				system("clear");

				break;
			case 2:
			//	attribue_valeur_avion();// dedans on crée un avion lui donne des valeur et on rentre dans un fichier
				system("clear");

				break;
			case 3:
				j++;
				break;
			default:
				printf("ce choix n'est pas disponible");
				break;	
		}
	}
}
void affiche_Avion(avion avion)
{
	char vol[30],cat[30];
	
	if(avion.cat_av==1)
	{
		strcpy(cat,"avion de ligne");
	}
	else if(avion.cat_av==2)
	{
		strcpy(cat,"avion d'affaires");
	}
	else
	{
		strcpy(cat,"avion léger");
	}
	if(avion.is_parked==0)
	{
		strcpy(vol,"au sol");
	}
	else
	{
		strcpy(vol,"en vol");
	}
	printf("\t----------------------------------------------------\n");
	printf("\t description de l'avion:\n\n");
	printf("\t -id: %d\n", avion.id);
	printf("\t -catégorie de l'avion: %s\n",cat);
	printf("\t -état de l'avion : %s\n",vol );
	printf("\t -nombre de passager: %d\n",avion.nb_passengers);	
	printf("\t----------------------------------------------------\n");		
}
liste* RetireAvionDebut(liste *Liste){
    liste* deuxiemme_element=liste->suiv;
    return deuxiemme_element;

}
