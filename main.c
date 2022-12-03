#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<math.h>

#define NOIR 1
#define ROUGE 2
#define VERT 3
#define ORANGE 4
#define BLEU 5
#define MAGENTA 6
#define CYAN 7
#define GRIS 8
#define BLANC 9

typedef struct avion
{
    int id;        // à  verifier a chaque init pour que il soit unique
    int cat_av;    // 1 av de ligne    2 av affaire     3 av léger
    int is_parked; // 1 pour un avion en vol    0 pour un avion au sol
    int nb_passengers;
} avion;

typedef struct liste liste;
struct liste
{
    avion avion;
    struct liste *suiv;
};

typedef struct piste
{
    int num_piste;
    float longueur;
    int cat_piste; // 1 petite  2 Moyenne   3 petite
    int max_await_takeoff;
    int is_busy; // 1 occupé   2 libre
    liste *liste_av;
} piste;

//------------------------------structure parking
typedef struct parking
{
    int maxParking;
    liste *liste_av;
} parking;

avion  creerAvion(avion avion1);
void saveListeFichier(avion cel, char *nomFichier);
void loading(char *blabla);
int compteurPiste(piste piste_une);
int compteurParking(parking p);
avion rechercheID(liste **listeAvion, int ID);
liste *ajouteAvionFin(liste *Liste, avion Avion);
void atterrissage(piste *piste_une, parking *parking1, liste **AVIONS);
piste init_piste(int num_piste, float longueur, int cat_piste, int max_await);
avion init_avion(int id, int cat_av, int nb_passaers);
parking init_parking(int maxparking);
liste *add_waitlist_piste(piste piste_une, parking parking1); // retourne la liste d'attente de la piste donée en parametre
liste *RetireAvionDebut(liste *Liste);
void decollage(piste *Piste, liste **air);
void menu();
void affiche_liste(liste *l1);
void affiche_Avion(avion avion);
void printParking(parking);
void printPiste(piste p1);
void print_piste(int avionpiste, int numpiste);
void print_air(int avionair);
void print_garage(int aviongarage);

void couleur_char(char c);

int main(void)
{
	menu();
    couleur_char(BLANC);

    avion avion1;
    avion1 = init_avion(1, 1, 10);

    avion avion2 = init_avion(2, 1, 20);
    avion avion3 = init_avion(3, 1, 10);
    piste piste1 = init_piste(3, 90, 3, 1);

    piste1.liste_av->avion = avion2;

    parking parking1;
    parking1.maxParking = 1;
    parking1.liste_av = NULL;
    parking1.liste_av = malloc(sizeof(liste));
    parking1.liste_av->avion = avion3;

    liste *tetedeliste = NULL;
    liste *avionVolent;
    avionVolent = malloc(sizeof(liste));
    avionVolent->avion = avion1;
    avionVolent->suiv = NULL;

    tetedeliste = avionVolent;

    piste *pointeurPiste = &piste1;
    parking *pointeurParking = &parking1;

    atterrissage(pointeurPiste, pointeurParking, &tetedeliste);
    printParking(parking1);
    printf("\n");
    printPiste(piste1);

    /*printf("Affichage de la liste\n");
    affiche_liste(tetedeliste);*/
    couleur_char(BLANC);
    return 0;
}

int compteurPiste(piste piste_une)
{
    int cpt = 0;
    liste *tmp = NULL;
    tmp = piste_une.liste_av;
    while (tmp != NULL)
    {
        cpt++;
        tmp = tmp->suiv; //->suiv car on est dans la liste avion
    }
    return cpt;
}

int compteurParking(parking p)
{
    int cpt = 0;
    liste *tmp;
    tmp = p.liste_av;
    while (tmp != NULL)
    {
        cpt++;
        tmp = tmp->suiv;
    }
    return cpt;
}

avion rechercheID(liste **listeAvion, int ID)
{
    avion retour;
    if (listeAvion != NULL)
    {
        liste *parcour = *listeAvion;

        if (parcour->avion.id == ID)
        {
            printf("Test\n");
            retour = parcour->avion;
            parcour = RetireAvionDebut(parcour);
            *listeAvion = parcour;
            return retour;
        }

        else
        {
            while (parcour->suiv != NULL)
            {
                if (parcour->avion.id == ID)
                {
                    liste *reste = parcour->suiv;
                    retour = parcour->avion;
                    free(parcour);
                    parcour->suiv = reste;
                    *listeAvion = parcour;
                    return retour;
                }

                parcour = parcour->suiv;
            }
            couleur_char(ROUGE);
            printf("ID non trouvé\n");
        }
    }
    couleur_char(ROUGE);
    printf("Liste vide\n");
    exit(0);
}

liste *ajouteAvionFin(liste *Liste, avion Avion)
{
    if (Liste != NULL)
    {
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
        tmp->suiv = variable; // ajoute le maillon a la fin de la liste
        return Liste;
    }
    else
    {                                     // en gros la jdisais que si la liste elle etais vide yavait un problème
        liste *variable = NULL;           // sauf que non, si elle est vide, on la comble
        variable = malloc(sizeof(liste)); // c'est le but de la fonction
        variable->avion = Avion;
        variable->suiv = NULL;
        Liste = variable;
        return Liste;
    }
}

void atterrissage(piste *piste_une, parking *parking1, liste **AVIONS)
{
    int cpt = compteurPiste(*piste_une);
    int cpt2 = compteurParking(*parking1);
    int ID;
    avion avion_atterri;
    printf("Quel avion voulez-vous faire atterrir (entrez son ID) : ");
    scanf("%d", &ID);
    printf("\n");

    avion_atterri = rechercheID(&*AVIONS, ID);

    if (&avion_atterri != NULL)
    { //&avion != NULL parce que s'il y a une adresse c quil existe, sans le & ca marche pas parce que les NULL c bizarre
        if (cpt < piste_une->max_await_takeoff)
        { // test si on peut passer sur piste pour aller garage

            if (cpt2 < parking1->maxParking)
            {                                                                           // test si place dans le garage
                parking1->liste_av = ajouteAvionFin(parking1->liste_av, avion_atterri); // Place l'avion dans le garage

                loading("L'avion est en train d'atterrir");
                couleur_char(BLANC);
                printf("\n\nL'avion a atterri\n\n");
                loading("L'avion est en train d'aller au parking");
                couleur_char(BLANC);
                printf("\n\nL'avion est dans le parking\n\n");
            }
            else
            {
                /*faire partir un avion du garage sur la piste*/
                avion premier_parking = parking1->liste_av->avion; // on stock le 1er avion du parking pour apres lamener sur piste
                parking1->liste_av = parking1->liste_av->suiv;     // lavion est enlever du parking

                piste_une->liste_av = ajouteAvionFin(piste_une->liste_av, premier_parking);

                /*faire entrer l'avion dans le garage*/
                parking1->liste_av = ajouteAvionFin(parking1->liste_av, avion_atterri);
                loading("L'avion est en train d'atterrir");
                printf("\n\nL'avion a atterri\n\n");
                couleur_char(BLANC);
                loading("L'avion est en train d'aller au parking");
                couleur_char(BLANC);
                printf("\n\nL'avion est dans le parking\n\n");
            }
        }

        else if (cpt == piste_une->max_await_takeoff)
        {
            /*sort un avion de la piste et on le met en lair*/
            decollage(piste_une, &*AVIONS);

            /*une fois qu'on a fait decoller un avion, on peut utiliser la piste pour aller au parking si il y a de la place*/
            if (cpt2 < parking1->maxParking)
            {                                                                           // test si place dans le garage
                parking1->liste_av = ajouteAvionFin(parking1->liste_av, avion_atterri); // Place l'avion dans le garage
                loading("L'avion est en train d'atterrir");
                couleur_char(BLANC);
                printf("\n\nL'avion a atterri\n\n");
                loading("L'avion est en train d'aller au parking");
                couleur_char(BLANC);
                printf("\n\nL'avion est dans le parking\n\n");
            }
            else
            {

                /*faire partir un avion du garage sur la piste*/
                avion premier_parking = parking1->liste_av->avion; // on stock le 1er avion du parking pour apres lamener sur piste
                parking1->liste_av = parking1->liste_av->suiv;     // lavion est enlever du parking

                piste_une->liste_av = ajouteAvionFin(piste_une->liste_av, premier_parking);

                /*faire entrer l'avion dans le garage*/
                parking1->liste_av = ajouteAvionFin(parking1->liste_av, avion_atterri);
                loading("L'avion est en train d'atterrir");
                couleur_char(BLANC);
                printf("\n\nL'avion a atterri\n\n");
                loading("L'avion est en train d'aller au parking");
                couleur_char(BLANC);
                printf("\n\nL'avion est dans le parking\n\n");
            }
        }
    }
    else
        printf("\nIl n'y a pas d'avions dans le ciel\n\n");
}

piste init_piste(int num_piste, float longueur, int cat_piste, int max_await)
{
    piste piste1;
    piste1.num_piste = num_piste;
    piste1.longueur = longueur;
    piste1.cat_piste = cat_piste;
    piste1.max_await_takeoff = max_await;
    piste1.liste_av = NULL;
    piste1.liste_av = malloc(sizeof(liste));
    return piste1;
};

avion init_avion(int id, int cat_av, int nb_passaers)
{
    avion avion1;
    avion1.nb_passengers = nb_passaers;
    avion1.cat_av = cat_av;
    avion1.id = id;
    return avion1;
};

parking init_parking(int maxparking)
{
    parking parking1;
    parking1.maxParking = maxparking;
    parking1.liste_av = NULL;
    return parking1;
}

liste *add_waitlist_piste(piste piste_une, parking parking1)
{
    int cpt_piste = compteurPiste(piste_une);
    int cpt_park = compteurParking(parking1);
    if (cpt_park == 0)
    {
        printf("Il n'y a pas d'avions dans le parking\n");
        return piste_une.liste_av;
    }
    int ID;
    avion avion_decollage;
    printf("Quel avion voulez-vous faire décoller (entrez son ID) : ");
    scanf("%d", &ID);
    avion_decollage = rechercheID(&parking1.liste_av, ID);
    if (cpt_piste < piste_une.max_await_takeoff)
    {
        piste_une.liste_av = ajouteAvionFin(piste_une.liste_av, avion_decollage);
        printf("\nL'avion attend de décoller\n\n");
        return piste_une.liste_av;
    }
    if (cpt_piste == piste_une.max_await_takeoff)
    {

        // METTRE FONCTION DECOLLAGE

        piste_une.liste_av = ajouteAvionFin(piste_une.liste_av, avion_decollage);
        printf("\nL'avion attend de décoller\n\n");
        return piste_une.liste_av;
    }
    return piste_une.liste_av;
}

void decollage(piste *Piste, liste **air)
{
    avion avion_decollage = Piste->liste_av->avion;
    Piste->liste_av = RetireAvionDebut(Piste->liste_av);
    *air = ajouteAvionFin(*air, avion_decollage);
}
void menu(void)
{

    int a = 0, j = 0;
    avion avion1;
    while (j == 0)
    {
    	  a=0;
        printf(" \t=================================================\n");
        printf("\t\tBIENVENUE DANS L'AEROPORT ONE PISTE\n");
        printf("\t=================================================\n");

        printf("\t----------------------------------------------------\n");
        printf("\tQue voulez-vous faire:\n\n");
        printf("\t(1) - Lancer une simulation\n\t(2) - Ajouter un avion dans la base de données\n\t(3) - Quitter\n");
        printf("\t----------------------------------------------------\n");
        scanf("%d", &a);
        printf("%d", a);
        switch (a)
        {
        case 1:
            // lancer la simulation
            system("clear");
		
		loading(" ...");
            break;
        case 2:
            //	attribue_valeur_avion();// dedans on crée un avion lui donne des valeur et on rentre dans un fichier
           
		do 
		{
			 avion1=creerAvion(avion1);
           
			if(avion1.cat_av==1 || avion1.cat_av==2 || avion1.cat_av==3)
			{
				affiche_Avion(avion1);
				loading(" ...");
				system("clear");
			}
			else{
				printf("catégorie non disponible");
			}
		}while(avion1.cat_av!=1 && avion1.cat_av!=2 && avion1.cat_av!=3);
	
	break;	      
        case 3:
            j +=1;
            break;
        default:
            printf("ce choix n'est pas disponible");
            break;
        }
    }
}
avion  creerAvion(avion avion1)
{
	int id=500;
	srand(time(NULL));
	FILE *fp=NULL;
	
	printf("\t----------------------------------------------------\n");
	couleur_char(BLANC);
      couleur_char(BLEU);
      printf("\t\tCREATION DE L'AVION:\n");
      couleur_char(BLANC);
       do 
       {
       	 printf("\tquelle est la catégorie de l'avion\n");
       	 printf("\t1: avion de ligne\n\t2:avion d'affaires\n\t3:avion léger\n");
       	 scanf("%d", &avion1.cat_av);
       	/* if(avion1.cat_av>3)
       	 {
       	 	printf("cette catégorie n'est pas disponible");
       	 }*/
      }while(avion1.cat_av<1 && avion1.cat_av>3);
      printf("pr");
       switch(avion1.cat_av)
       {
       	case 1:
       	// parcourir id de fichier catégorie 1 et voir si id différent 
       		do
       		{
       		
       			fp=fopen("categorie1.txt", "r");
       			if(fp!=NULL)
       			{
       				avion1.id=(rand() % (100 - 0 + 1)) + 0;
	       			for(int i=0;i<100;i++)
	       			{
	       				fseek(fp, i, SEEK_SET);
	       				id = fscanf(fp,"%d", &id);
	       				if(id==avion1.id)
	       				{
	       					i=101;
	       				}
	       				i++;
	       			}
       			}	
       		}while(avion1.id==id);
       		avion1.is_parked=1;
    			avion1.nb_passengers=rand()%100;
       		saveListeFichier(avion1, "categorie1.txt");
       		
       		break;
       		
       	case 2:
       	do
       		{
       			fp=fopen("categorie2.txt", "r");
       			
       			if(fp!=NULL)
       			{
       				avion1.id=(rand() % (100 - 0 + 1)) + 0;
	       			for(int i=0;i<100;i++)
	       			{
	       				fseek(fp, i, SEEK_SET);
	       				id = fscanf(fp,"%d", &id);
	       				if(id==avion1.id)
	       				{
	       					i=101;
	       				}
	       				i++;
	       			}
       			}	
       			
       		}while(avion1.id==id);
       		avion1.is_parked=1;
     			avion1.nb_passengers=rand()%100;
       		saveListeFichier(avion1, "categorie2.txt");
       		
       		break;
       		
       	case 3:
       	do
       		{
       			fp=fopen("categorie3.txt", "r");
       			if(fp!=NULL)
       			{
       				avion1.id=(rand() % (100 - 0 + 1)) + 0;
	       			for(int i=0;i<100;i++)
	       			{
	       				fseek(fp, i, SEEK_SET);
	       				id = fscanf(fp,"%d", &id);
	       				if(id==avion1.id)
	       				{
	       					i=101;
	       				}
	       				i++;
	       			}
       			}	
       		}while(avion1.id==id);
       		avion1.is_parked=1;
     			avion1.nb_passengers=rand()%100;
       		saveListeFichier(avion1, "categorie3.txt");
       		break;
       	default: 
       		break;
       		
       }
     
     return avion1;
   //  printf("\tquelle est le nom de l'avion");
    // scanf("%s", 
}
void affiche_Avion(avion avion)
{
    char vol[30], cat[30];

    if (avion.cat_av == 1)
    {
        strcpy(cat, "avion de ligne");
    }
    else if (avion.cat_av == 2)
    {
        strcpy(cat, "avion d'affaires");
    }
    else
    {
        strcpy(cat, "avion léger");
    }
    if (avion.is_parked == 0)
    {
        strcpy(vol, "au sol");
    }
    else
    {
        strcpy(vol, "en vol");
    }
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
    couleur_char(BLEU);
    printf("\t\tDESCRIPTION DE L'AVION:\n");
    couleur_char(BLANC);
    printf("\t - ID: %d\n", avion.id);
    printf("\t - Catégorie de l'avion: %s\n", cat);
    printf("\t - Etat de l'avion : %s\n", vol);
    printf("\t - Nombre de passager: %d\n", avion.nb_passengers);
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
}
liste *RetireAvionDebut(liste *Liste)
{
    liste *deuxiemme_element = Liste->suiv;
    return deuxiemme_element;
}

void affiche_liste(liste *l1)
{
    liste *tmp = l1;
    // affiche_Avion(tmp->avion);
    while (tmp != NULL)
    {
        affiche_Avion(tmp->avion);
        tmp = tmp->suiv;
    }
    printf("\n");
}

void printParking(parking p1)
{
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
    couleur_char(MAGENTA);
    printf("\t\tINFORMATION SUR LE PARKING :\n\n");
    couleur_char(BLANC);
    printf("\t- Capacité maximale du parking : %d\n", p1.maxParking);
    int cpt;
    cpt = compteurParking(p1);
    printf("\t- Le parking possède %d avion(s)\n", cpt);
    printf("\t- Avion dans le parking :\n\n");
    affiche_liste(p1.liste_av);
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
}

void printPiste(piste p1)
{
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
    couleur_char(MAGENTA);
    printf("\t\tINFORMATION SUR LA PISTE :\n\n");
    couleur_char(BLANC);
    printf("\t- Numéro de piste %d\n", p1.num_piste);
    printf("\t- Longueur de piste %f\n", p1.longueur);

    printf("\t- Catégorie de piste %d\n", p1.cat_piste);
    printf("\t- Capacité maximale de la piste %d\n", p1.max_await_takeoff);

    if (p1.is_busy == 1)
        printf("La piste est occupée\n");
    else if (p1.is_busy == 2)
        printf("La piste est libre\n");

    printf("\t- Avion sur la piste :\n\n");
    affiche_liste(p1.liste_av);
    couleur_char(CYAN);
    printf("\t----------------------------------------------------\n");
    couleur_char(BLANC);
}

void loading(char *blabla)
{
    int msec = 0;
    const int trigger = 50; // ms
    const int printWidth = 4;
    int counter = 0;
    clock_t before = clock();
    couleur_char(VERT);
    while (1)
    {
        fputs(blabla, stdout);
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        if (msec >= trigger)
        {
            counter++;
            msec = 0;
            before = clock();
        }
        for (int i = 0; i < counter; ++i)
        {
            fputc('.', stdout);
        }
        for (int i = 0; i < printWidth - counter; ++i)
        {
            fputc(' ', stdout);
        }
        fputc('\r', stdout);
        fflush(stdout);

        if (counter == printWidth)
        {
            counter = 0;
            return;
        }
    }
}

void saveListeFichier(avion cel, char *nomFichier)
{

    FILE *fp = NULL;
    fp = fopen(nomFichier, "a");
  //  printf(" rentre dans save%d %d %d %d", cel.id, cel.cat_av, cel.is_parked, cel.nb_passengers);
    if (fp != NULL)
    {
        fprintf(fp, "%d %d %d %d\n", cel.id, cel.cat_av, cel.is_parked, cel.nb_passengers);
    }
    fclose(fp);
}
void print_garage(int aviongarage)
{
    char *tmp;
    tmp = NULL;
    tmp = calloc(1, sizeof(char) * 30);
    char *tmp_escpace;
    tmp_escpace = NULL;
    tmp_escpace = calloc(1, sizeof(char) * 30);
    char *tmp_toit;
    tmp_toit = NULL;
    tmp_toit = calloc(1, sizeof(char) * 30);
    for (int i = 0; i < aviongarage; ++i)
    {
        tmp = strcat(tmp, " (ooo} ");
        tmp_escpace = strcat(tmp_escpace, "       ");
        tmp_toit = strcat(tmp_toit, "_________");
    }
    printf("\t      Garage\n\t_%s_\n\t |%s|\n\t |%s|\n", tmp_toit, tmp_escpace, tmp);
}
void print_air(int avionair)
{
    char *air;
    air = NULL;
    air = calloc(1, sizeof(char) * 30);
    for (int i = 0; i < avionair; ++i)
    {
        air = strcat(air, "(ooo} ");
    }
    printf("%s\n\n", air);
}
void print_piste(int avionpiste, int numpiste)
{
    char *tmp;
    tmp = NULL;
    tmp = calloc(1, sizeof(char) * 30);
    for (int i = 0; i < avionpiste; ++i)
    {
        tmp = strcat(tmp, "(ooo}  ");
    }
    printf("\n\n\t \t piste %d \n\t  ----------------------------------------------\n\t/   ==   ==   == %s\n\t----------------------------------------------------\n", numpiste, tmp);
}

void couleur_char(char c)
{
    switch (c)
    {
    case NOIR:
        printf("\033[30m");
        break;
    case ROUGE:
        printf("\033[31m");
        break;
    case VERT:
        printf("\033[32m");
        break;
    case ORANGE:
        printf("\033[33m");
        break;
    case BLEU:
        printf("\033[34m");
        break;
    case MAGENTA:
        printf("\033[35m");
        break;
    case CYAN:
        printf("\033[36m");
        break;
    case GRIS:
        printf("\033[37m");
        break;
    case BLANC:
        printf("\033[00m");
        break;
    default:
        break;
    }
}
