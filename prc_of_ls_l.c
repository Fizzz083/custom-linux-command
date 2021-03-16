#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define  NORMAL_COLOR  "\x1B[0m"
#define  GREEN  "\x1B[32m"
#define  BLUE  "\x1B[34m"
#define  BOLDGREEN  "\033[1m\033[32m"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>


void strmode(mode_t mode, char * buf) {

    //printf("%s checking \n", buf);

    const char chars[] = "rwxrwxrwx";
    for (size_t i = 0; i < 9; i++) {
        buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
    }
    buf[9] = '\0';
}


void show()
{
    DIR * d = opendir("."); /// open the path
    if(d==NULL) return; /// if was not able return

    struct dirent * dir; //


    while ((dir = readdir(d)) != NULL) /// if we were able to read somehting from the directory
    {

        struct stat buf;

        char *ne = dir->d_name;


        if(stat(ne, &buf)==0) {
            char time[50];
            strftime(time, 50, "%Y-%m-%d %H:%M:%S", localtime(&buf.st_mtime));



            char *mode = malloc(10);

            strmode(buf.st_mode, mode);



            struct passwd *pw = getpwuid(buf.st_uid);
            struct group  *gr = getgrgid(buf.st_gid);

            printf("%s  %ld  ", mode, buf.st_nlink);
            printf("%s  %s ", pw->pw_name, gr->gr_name );
            // printf("  %s  ", dirEntry->d_name);
            printf ("%s  %s  \n",time, dir->d_name);

        }
        else
        {
            printf ("dey na\n");
        }



    }



}


int main(int argc, char **argv)
{

    show();


    return(0);
}
