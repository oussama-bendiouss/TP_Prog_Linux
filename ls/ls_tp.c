/*================================================================
   *   Copyright (C) 2018 . All rights reserved.
   *   
   *   File name: myls2.c
   *   Creator: ghostwu (Wu Hua)
   *   Date of establishment: January 9, 2018
   *   Description: ls command (version 1.2)
   *
   ================================================================*/
  
 #include <stdio.h>
 #include <sys/types.h>
 #include <dirent.h>
 #include <stdlib.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
 #include <string.h>
 #include <sys/types.h>
 #include <pwd.h>
 #include <grp.h>
 #include <time.h>

void do_ls( char [] );
void do_stat( char* filename );
void show_list( char* filename, struct stat* statinfo );
void mode_to_letters( mode_t filemode, char str[] );
void show_time( time_t filetime );
char* format_time( char* dsttime, const char* srctime );
//user id Transfer name
char* uid_to_name( uid_t uid );
//group id Transfer name
char* gid_to_name( gid_t gid );

int main(int argc, char *argv[])
{
    if( argc == 1 ) {
         do_ls( "." );
   }else {
        while( --argc ) {
            printf( "arg=%s\n", * ++argv );
           do_ls( *argv );
        }
     }
     return 0;
  }

 void do_ls( char dir_entry[] ) {
     DIR* pDir;
     struct dirent* pCurDir;
     if( ( pDir = opendir( dir_entry ) ) == NULL ){
        perror( "read dir" );
         exit( -1 );
     }else {
         while( ( pCurDir = readdir( pDir ) ) != NULL ) {
             do_stat( pCurDir->d_name );
        }
         closedir( pDir );
     }
 }

//Get file information
void do_stat( char* filename ){
     struct stat statinfo;
     if ( stat( filename, &statinfo ) == -1 ) {
         printf( "open%s fail\n", filename );
        exit( -1 );
     }else {
         show_list( filename, &statinfo );
     }
 }
 
 //Show file list
 void show_list( char* filename, struct stat* statinfo ) {
     mode_t st_mode = statinfo->st_mode;
 
     char str[10];
      mode_to_letters( st_mode, str );
     printf( "%s\t", str );
 
     printf( "%ld\t", statinfo->st_nlink ); //Symbolic link
     printf( "%s\t\t", uid_to_name( statinfo->st_uid ) ); //user name
     printf( "%s\t", gid_to_name( statinfo->st_gid ) ); //Group name
     printf( "%10ld", statinfo->st_size ); //file size
     show_time( statinfo->st_mtime ); //Last modification time
     printf( "\t%s", filename );
 
     printf( "\n" );
 }
 
 char* uid_to_name( uid_t uid ){
     return getpwuid( uid )->pw_name;
 }
 
 char* gid_to_name( gid_t gid ){
     return getgrgid( gid )->gr_name;
}
 
 void mode_to_letters( mode_t filemode, char str[] ) {
 
     strcpy( str, "----------" );
    if( S_ISREG( filemode ) ) str[0] = '-';
     if( S_ISDIR( filemode ) ) str[0] = 'd';
    if( S_ISLNK( filemode ) ) str[0] = 'l';

     //User permission bit
     if( filemode & S_IWUSR ) str[2] = 'w';
     if( filemode & S_IXUSR ) str[3] = 'x';
 
     //Group permission bit
    if( filemode & S_IRGRP ) str[4] = 'r';
     if( filemode & S_IWGRP ) str[5] = 'w';
    if( filemode & S_IXGRP ) str[6] = 'x';

    //Other group permission bits
    if( filemode & S_IROTH ) str[7] = 'r';
     if( filemode & S_IWOTH ) str[8] = 'w';
    if( filemode & S_IXOTH ) str[9] = 'x';
 }
 
 void show_time( time_t filetime ) {
     struct tm* ptm;
    ptm = localtime( &filetime );
 
     int month = ptm->tm_mon + 1;
     int day = ptm->tm_mday;
     int hour = ptm->tm_hour;
     int min = ptm->tm_min;
 
    char srchour[3] = "0";
     char srcmin[3] = "0";
     char dsthour[3] = "0";
     char dstmin[3] = "0";
     sprintf( srchour, "%d", hour );
     sprintf( srcmin, "%d", min );
     format_time( dsthour, srchour );
     format_time( dstmin, srcmin );
 
     printf( "%4d month%4d%4s:%2s", month, day, dsthour, dstmin );
 }

char* format_time( char* dsttime, const char* srctime ) {
     if( strlen( srctime ) < 2 ) {
         return strcat( dsttime, srctime );
     }
     return strcpy( dsttime, srctime );
 }
