#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// works fine

#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define BMAG "\033[0;35m"

void version() {
  puts("TP : Programmation Linux Windows, CentraleSupélec, 2022 \u00A9\ntee "
       "program : basic functionnality works fine with some few features");
}

void usage(char *pn) {
  printf("Usage : %s [OPTION] ... [FILE] ...\n", pn);
  printf("Copy standard input to each FILE, and also to standard output.\n\n");
  printf("   -v  show version\n");
  printf("   -a  append to the file\n");
  printf("   -h  display this help and exit\n\n");
  printf("Examples\n");
  printf("  %s file     Copy input to output and the file\n", pn);
}

void tee(int argc, char **argv, int a) {
  char buffer[1024] = {0};
  size_t nread = 0;
  while ((nread = read(STDIN_FILENO, buffer, 1024)) != 0) {
    if (write(STDOUT_FILENO, buffer, nread) != nread) {
      fprintf(stderr, "%s\n", "Failed to write to stdout");
    }
    for (int i = 1; i < argc; i++) {
      if (a) {
        int outfd = open(argv[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (outfd == -1) {
          fprintf(stderr, "%s\n", "Failed to create output file");
        }
        if (write(outfd, buffer, nread) != nread) {
          fprintf(stderr, "%s\n", "Failed to write to file");
        }
      } else {
        int outfd = open(argv[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (outfd == -1) {
          fprintf(stderr, "%s\n", "Failed to create output file");
        }
        if (write(outfd, buffer, nread) != nread) {
          fprintf(stderr, "%s\n", "Failed to write to file");
        }
      }
    }
  };
}
int main(int argc, char *argv[]) {
  int a;
  if (argc == 1){
      tee(argc,argv,0);
  }
  else if (argv[1][0] == '-') {
    char *p = ++argv[1];
    while (*p) {
      switch (*p) {
      case 'h':
        usage(argv[0]);
        break;
      case 'v':
        version();
        break;
      case 'a':
        a = 1; tee(argc,argv,a); 
        break;
      default :
        printf("%sNo such option %c%s\n",COLOR_RED,*p,COLOR_RESET);
        usage(argv[0]);
        break;
      }
      p++;
    }
  } else {
      tee(argc,argv,0);
  }
  return 0;
}
