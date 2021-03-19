
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define  NORMAL_COLOR  "\x1B[0m"
#define  GREEN  "\x1B[32m"
#define  BLUE  "\x1B[34m"
#define  BOLDGREEN  "\033[1m\033[32m"
#define  BOLDBLUE    "\x1b[1m\x1b[34m"
#define  BOLDRED     "\x1b[1m\x1b[31m"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
//#include <ctype.h>

int cnt_chk = 0;

struct node
{
    char *name,
         *color,
         last_access_time[50],
         *user_name,
         *group_name,
         *mode
         ;
    bool name_with_space;
    long int link;
    long int blocksize;
    long int block_count;

    long long int main_un_time;

};

int max(int i, int j)
{
    if(i>j) return i;
    return j;

}

int space_ase = 0;

char* func(char *c, int max_size, int main_size)
{
    char *tm = malloc(max_size);
    //strcpy(tm, c);

    int f = 0;
    int r = 0;

    for(int i=0; i<max_size; i++)
    {
        if(i<main_size && !((c[i]>='a' && c[i]<='z') || (c[i]>='A' && c[i]<='Z') || (c[i]=='.')))
        {
            space_ase  = 1;
        }

        if(c[i]=='.' && r==0 ) {
            r=1;
            //continue;
        }

        if(i>=max_size)
        {
            tm[i-r]='#';
            continue;
        }
        else if(((c[i]>='a' && c[i]<='z') || (c[i]>='A' && c[i]<='Z')))
        {
            tm[i-r] = tolower(c[i]);
        }
        
    }

    if(r)
    {
        tm[max_size-1]='#';
    }

    //if(cnt_chk<=5 && r) {
        //printf("%s %d %d\n", tm, main_size, max_size);
        //cnt_chk++;
    //}


    return tm;

}

struct node *allinfo ;
int size = 0;
long long int total_count = 0;
long long int tmpdot = 0;
bool a=0, l=0, t=0;


void show()
{
    if(l==0) {
        for(int i=0; i<size; i++)
        {
            if( allinfo[i].name[0]=='.')
            {
                if(a==0) continue;
            }
           
            printf("%s%s \n",allinfo[i].color, allinfo[i].name);
            printf("%s", NORMAL_COLOR);
            //printf("%s %-10s %-10ld\n", allinfo[i].color, allinfo[i].name, allinfo[i].blocksize);
        }

        //printf("\v");
        printf("%s", NORMAL_COLOR);
       // getch();
    }
    else {


        if(a!=0)
        {
            total_count+=tmpdot;
        }



        printf("total %lld\n",total_count);
        for(int i=0; i<size; i++)
        {
            if(strcmp(allinfo[i].name, ".")==0 || strcmp(allinfo[i].name, "..")==0 || allinfo[i].name[0]=='.')
            {
                if(a==0) continue;
            }

            printf("%s", NORMAL_COLOR);

            printf("%-10s %-2lld %s %s %-8lld %-20s",allinfo[i].mode, allinfo[i].link, allinfo[i].user_name,
                   allinfo[i].group_name, allinfo[i].blocksize,
                   allinfo[i].last_access_time );
            printf(" %s%s\n", allinfo[i].color, allinfo[i].name );
            printf("%s", NORMAL_COLOR);
            //printf("\n");
        }

    }

    // printf("\n");
}


void alphabetically_sort()
{

    for(int i=0; i<size ; i++)
    {
        for(int j=0; j<size - i -1; j++)
        {
            int s_size_max= max((int)strlen(allinfo[j].name), (int)strlen(allinfo[j+1].name));

            char *p =malloc(s_size_max);
            strcpy(p, allinfo[j].name);
            char *g = malloc(s_size_max);
            strcpy(g,allinfo[j+1].name);
            //if(i==0 )
            space_ase = 0;
            p = func(p, s_size_max, (int)strlen(allinfo[j].name));


            if(space_ase == 1)
            {
                allinfo[j].name_with_space = 1;
            }
            space_ase = 0;
            g= func(g, s_size_max, (int)strlen(allinfo[j+1].name));
            if(space_ase == 1)
            {
                allinfo[j+1].name_with_space = 1;
            }

            //printf("%s %s\n", p, g);


            if(strcmp(p,g) >0)
            {
                struct node tmp = allinfo[j];
                allinfo[j] = allinfo[j+1];
                allinfo[j+1] = tmp;
            }
        }
    }
}

void withtime_sort()
{

    for(int i=0; i<size - 1; i++)
    {
        for(int j=0; j<size - i -1; j++)
        {
            if(allinfo[j].main_un_time<=allinfo[j+1].main_un_time)
            {
                struct node tmp = allinfo[j];
                allinfo[j] = allinfo[j+1];
                allinfo[j+1] = tmp;
            }
        }
    }
}



void strmode(mode_t mode, char * buf) {

    /*
     d             directory
      rwx          owner
       r-x         group
    	r-x        other
    */

    //printf("%s checking \n", buf);

    const char chars[] = "rwxrwxrwx";
    for (size_t i = 0; i < 9; i++) {
        buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
    }
    buf[9] = '\0';
}

void init()
{
    DIR * d = opendir(".");
    if(d==NULL) return;

    struct dirent * dir; //

    while ((dir = readdir(d)) != NULL)
    {
        struct stat buf;
        char *ne = dir->d_name;



        if(stat(ne, &buf)==0)
        {
            if(size!=0)
                allinfo = (struct node*)realloc(allinfo, sizeof(struct node)*(size+1));
            else
            {
                allinfo  = (struct node*)malloc(sizeof(struct node));
            }
            char time[50];
            strftime(time, 50, "%Y-%m-%d %H:%M:%S", localtime(&buf.st_mtime));

            char *mode = malloc(10);
            strmode(buf.st_mode, mode);

            struct passwd *pw = getpwuid(buf.st_uid);
            struct group  *gr = getgrgid(buf.st_gid);

            //printf("%s  %ld  ", mode, buf.st_nlink);
            //printf("%s  %s  %ld ", pw->pw_name, gr->gr_name, buf.st_size );
            //// printf("  %s  ", dirEntry->d_name);
            //printf ("%s  %s  \n",time, dir->d_name);

            allinfo[size].name = dir->d_name;
            allinfo[size].blocksize = buf.st_size;
            allinfo[size].link = buf.st_nlink;
            strcpy(allinfo[size].last_access_time,time);
            allinfo[size].user_name = pw->pw_name;
            allinfo[size].group_name =  gr->gr_name;
            allinfo[size].mode =mode;
            allinfo[size].block_count = buf.st_blocks;
            allinfo[size].main_un_time = buf.st_mtime;

            if( dir-> d_type == DT_DIR)
            {
                allinfo[size].color = BOLDBLUE;
            }
            else if(buf.st_nlink==1 && mode[8]=='x')
            {
                allinfo[size].color = BOLDGREEN;
            }
            else if(buf.st_nlink==1 && allinfo[size].blocksize>= 1000000)
            {
                allinfo[size].color = BOLDRED;
            }
            else
            {
                allinfo[size].color = NORMAL_COLOR;

            }

            //printf("%s\n",allinfo[size].last_access_time);

            //printf("%s -- %s\n", dir->d_name, dir->d_type);
            size++;

            if( allinfo[size-1].name[0]=='.')
            {
                tmpdot+=(long long int)(buf.st_blocks/2);

            }
            else {
                total_count+=(long long int)(buf.st_blocks/2);
            }

        }

    }
}


void wrong()
{
    printf(" %sWrong Command.. \n%s   -l   : use a long listing format\n   -a   : do not ignore entries starting with .\n   -t   : sort by modification time, newest first\n ", BOLDRED, NORMAL_COLOR);
}



int main(int argc, char **argv)
{
    init();

    printf("%s", NORMAL_COLOR);

    //printf("%d\n", size);

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-a")==0) a=1;
        else if(strcmp(argv[i], "-l")==0) l=1;
        else if(strcmp(argv[i], "-t")==0) t=1;
        else
        {
            // printf("Wrong Command. \n Again. Thanks\n");
            wrong();
            return 0;
        }

    }

    //int v1 = a==1?1:0 ;
    //int v2 = l==1?1:0 ;
    //int v3 = t==1?1:0 ;

    ////printf("%d %d %d\n", v1, v2, v3);

    //if(l==1)
    alphabetically_sort();

    if(t==1)
        withtime_sort();

    show();

    printf("%s", NORMAL_COLOR);

    free(allinfo);

    return 0;
}
