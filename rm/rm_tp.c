#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define COLOR_RED "\x1b[31m"
#define BLU "\e[0;34m"
#define COLOR_RESET "\x1b[0m"

//works fine

void version(){
  puts("TP : Programmation Linux Windows, CentraleSupélec, 2022 \u00A9\nrm program : basic functionnality works fine with some few features");
}

void usage(char* pn){
  printf("Usage : %s [OPTION] ... [FILE] ...\n",pn);
  printf("Can be used to concatenate FILES to standard output.\n\n");
  printf("   -v  show version\n");
  printf("   -d  remove empty directories\n");
  printf("   -r  remove directories and their content\n");
  printf("   -i  prompt before every removal\n");
  printf("   -h  display this help and exit\n\n");
  printf("Examples\n");
  printf("  %s -r ffolder    removes the file or directory folder and its content\n",pn);
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int DirectoryEmpty(char *dirname) {
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(dirname);
  if (dir == NULL)
    return 4;
  while ((d = readdir(dir)) != NULL) {
    if(++n > 2)
      break;
  }
  closedir(dir);
  if (n <= 2) 
    return 1;
  else
    return 0;
}

int
rm_recursive(const char * path)
{
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    stat(path, &stat_path);

    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "Is not directory\n", path);
        return 0;
    }

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Can`t open directory\n", path);
        return 0;
    }

    path_len = strlen(path);

    while ((entry = readdir(dir)) != NULL) {

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        stat(full_path, &stat_entry);

        if (S_ISDIR(stat_entry.st_mode) != 0) {
            rm_recursive(full_path);
            continue;
        }

        if (unlink(full_path) == 0)
            continue;
        else
            printf("Can`t remove the file: %s\n", full_path);
        free(full_path);
    }

    if (rmdir(path) == 0)
        printf("Removed %s successfully\n", path);
    else
        printf("Can`t remove the directory: %s\n", path);

    closedir(dir);
    return 0;
}

void rm(int argc, char**argv, int i, int d, int r){
  if ((d | i | r) && argc < 3){
    fprintf(stderr, "%sPlease Provide all necessary arguemnts%s\n",COLOR_RED,COLOR_RESET);
    usage(argv[0]);
  }
  for(int j = 1 + (d | i | r); j < argc; j++){
    if (d) {
      if(!is_regular_file(argv[j])){
        if(DirectoryEmpty(argv[j])==1){
          rmdir(argv[j]);
        } else {
          printf("%s is not an empty directory\n",argv[j]); break;
        }
      } else {
        printf("%s not a directory\n",argv[j]); break;
      }
    }
    if (i) {
      char interactive;
      printf("Do u really wanna remove %s%s%s (y/n) ",BLU,argv[j],COLOR_RESET);
      scanf("%c",&interactive);
      while(getchar() != '\n');
      if (interactive == 'n'){
        printf("the file %s%s%s won't be deleted, moving on to the next one\n",BLU,argv[j],COLOR_RESET);
      } else {
        if(unlink(argv[j]) < 0){
          fprintf(stderr, "rm1: %s failed to delete\n", argv[j]);
        }
      }
      continue;
    }
    if(r){
      rm_recursive(argv[j]); break;
    }
    if(unlink(argv[j]) < 0 && !d && !r && !i){
      fprintf(stderr, "rm: %s failed to delete\n", argv[j]);
      break;
    }
  }
}



int
main(int argc, char *argv[])
{
  int i=0,r=0,d=0;

  if(argc < 2){
    fprintf(stderr, "%sPlease Provide all necessary arguemnts%s\n",COLOR_RED,COLOR_RESET);
    usage(argv[0]);
    return 0;
  }

  if (argv[1][0]=='-'){
    char * p = ++argv[1];
    while (*p){
      switch (*p){
        case 'r' : r=1; rm(argc,argv,i,d,r); break;
        case 'v' : version(); break;
        case 'h' : usage(argv[0]); break;
        case 'i' : i=1; rm(argc,argv,i,d,r); break;
        case 'd' : d=1; rm(argc,argv,i,d,r); break;
        default  : printf("Incorrect Option %c, use -h to see the help menu",*p);
      }
      p++;
    }
  }
  else {
    rm(argc,argv,i,d,r);
  }
  

  return 0;
}
