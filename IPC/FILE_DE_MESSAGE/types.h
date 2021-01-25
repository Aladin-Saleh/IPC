typedef struct 
{
	long type;
	char op;
	pid_t expediteur;
	int g,d;
} 
requete_t;

typedef struct 
{
	long type;
	int resu;
} 
reponse_t;

#define FICHIER_CLE "cle.serv"


/* Couleurs dans term */
#define couleur(param) fprintf(stdout,"\033[%sm",param)

#define NOIR  "30"
#define ROUGE "31"
#define VERT  "32"
#define JAUNE "33"
#define BLEU  "34"
#define CYAN  "36"
#define BLANC "37"
#define REINIT "0"


/* 
   Utilisation (pour ecrire en rouge) :

   couleur(ROUGE); fprintf(stdout,"Hello\n"); couleur(REINIT);

*/


