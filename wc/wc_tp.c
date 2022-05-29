#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BMAG "\033[0;35m"
#define COLOR_RESET "\e[0m"
//works fine
void version() {
  puts("TP : Programmation Linux Windows, CentraleSupélec, 2022 \u00A9\nwc "
       "program : basic functionnality works fine with some few features");
}

void usage(char *pn) {
  printf("Usage : %s [OPTION] [FILE]\n", pn);
  printf("Print newline, word, and byte counts for a file.\n\n");
  printf("   -v  show version\n");
  printf("   -l  number of lines in a file \n");
  printf("   -c  number of characters in a file \n");
  printf("   -w  number of words in a file \n");
  printf("   -h  display this help and exit\n\n");
  printf("Examples\n");
  printf("  %s -l file    number of lines in the file file\n", pn);
}

void wc(int argc, char **argv, int l, int m, int c, int stdin_) {

  int character_count = 0;
  int word_count = 0;
  int line_count = 0;
  char buffer[1] = {0};
  size_t nread = 0;
  char ch;
  if (stdin_) {
    while ((nread = read(STDIN_FILENO, buffer, 1)) != 0) {
      ch = buffer[0];
      if (ch != EOF) {
        character_count++;
        if (ch == ' ') {
          word_count++;
        }
        if (ch == '\n') {
          line_count++;
          word_count++;
        }
      }
    }
  } else {
    FILE *fp;
    
    fp = fopen(argv[1+(l | m | c)], "r"); 
      while ((ch = getc(fp)) != EOF) {
        character_count++;
        if (ch == ' ') {
          word_count++;
        }
        if (ch == '\n') {
          line_count++;
          word_count++;
        }
      }
  }
  

  if (c) {
    printf("The number of characters : %d\n", character_count);
  }
  if (l) {
    printf("The number of lines : %d\n", line_count);
  }
  if (m) {
    printf("The number of words : %d\n", word_count);
  }
  if (!c && !m && !l && !stdin_) {
    printf("%s%s%s :: lines : %d / words : %d / characters : %d\n", BMAG,argv[1],COLOR_RESET,line_count, word_count, character_count);
  }
  if (!c && !m && !l && stdin_ ){
    printf("lines : %d / characters : %d / words : %d\n",line_count,character_count,word_count);
  }
}

int main(int argc, char *argv[]) {

  int l = 0;
  int m = 0;
  int c = 0;
  int stdin_ = 0;

  if (argv[1]) {
    if (argv[1][0] == '-') {
      char *p = ++argv[1];
      while (*p) {
        switch (*p) {
        case 'l':
          l = 1; if (argc<3) stdin_=1;
          wc(argc, argv, l, m, c, stdin_);
          l=0;
          break;
        case 'v':
          version();
          break;
        case 'h':
          usage(argv[0]);
          break;
        case 'w':
          m = 1; if (argc<3) stdin_=1;
          wc(argc, argv, l, m, c, stdin_);
          m=0;
          break;
        case 'c':
          c = 1; if (argc<3) stdin_=1;
          wc(argc, argv, l, m, c, stdin_);
          c=0;
          break;
        default:
          printf("No Such option %c\n", *p);
          usage(argv[0]);
          break;
        }
        p++;
      }
    } else {
      if (argc<=1) stdin_ = 1;
      wc(argc, argv, l, m, c, stdin_);
    }
  } else {
    if (argc<=1) stdin_ = 1;
    wc(argc, argv, l, m, c, stdin_);
  }

  return 0;
}
