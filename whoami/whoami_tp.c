#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

void version(){
  puts("TP : Programmation Linux Windows, CentraleSupélec, 2022 \u00A9\nwhoami program : basic functionnality works fine with some few features");
  exit(EXIT_FAILURE);

}

void usage(char* pn){
  printf("Utilisation : %s ",pn);
  printf("Tu peux utiliser cette fonction pour afficher l'utilisateur actuel.\n\n");
 
       printf("   -V  Afficher des informations sur la version du %s\n",pn);
  printf("   -h  Afficher de l'aide \n\n");
  printf("Examples\n");
  printf("  %s \nLe résultat : root",pn);
}

//works fine
int main(int argc, char **argv) {
  if (argc > 2) {
    fprintf(stderr, "Moins d'argument s'il te plait. \n\nusage: whoami\n");
    return 1;
  }
  if(argc==2){
  switch(argv[1][1]){
       case 'h': usage(argv[0]);break;
        case 'V' : version();break;

  }}

  struct passwd *pw;
  char *user = NULL;

  pw = getpwuid(geteuid());
  if (pw)
    user = pw->pw_name;
  else if ((user = getenv("USER")) == NULL) {
    fprintf(stderr, "No user in the environemnt !\n");
    return 1;
  }
  printf("%s\n", user);
  return 0;
}
