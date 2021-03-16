#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"


struct node
{
	char *name, *color;
	
	
};



void sorted(struct node arr[], int size)
{
	for(int i=0;i<size;i++)
	{
		for(int j=i+1;j<size; j++)
		{
			if(strcmp(arr[i].name, arr[j].name)>0)
			{
				struct node tmp = arr[i];
				arr[i]=arr[j];
				arr[j] = tmp;
			}
		}
	}
	
	///printf("%d \n", (int)(strcmp(arr[0].name, arr[1].name)));
	
	 for(int i=0;i<size;i++)
    {
		printf("%s%s\n", arr[i].color, arr[i].name);
	}
}



void show_dir_content(char * path)
{
    DIR * d = opendir("."); /// open the path
    if(d==NULL) return; /// if was not able return
    struct dirent * dir; /// for the directory entries

    
    struct node arr[100];

    int idx = 0;

    while ((dir = readdir(d)) != NULL) /// if we were able to read somehting from the directory
    {
        if(dir-> d_type != DT_DIR) /// if the type is not directory just print it with blue
        {  
			arr[idx].name = dir->d_name;
			arr[idx].color = BLUE;
			idx++;
			
			 
			 ///printf("%s%s\n",BLUE, dir->d_name);
			 
        }
        else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
        {
			arr[idx].name = dir->d_name;
			arr[idx].color = GREEN;
			idx++;
			
           /// printf("%s%s\n",GREEN, dir->d_name); // print its name in green
           /// char d_path[255]; // here I am using sprintf which is safer than strcat
           /// sprintf(d_path, "%s/%s ", path, dir->d_name);
           /// show_dir_content(d_path); // recall with the new path
        }
        
    }
    
   ///printf("%d \n \n", idx);
   ///printf("\n");
    
    
    sorted(arr, idx);
    
    
   

    closedir(d); /// finally close the directory
}

int main(int argc, char **argv)
{
    ///show_dir_content("..");


    printf("%s\n", NORMAL_COLOR);

    printf("%s argv\n", argv[1]);

    if(strcmp(argv[1], "-a")==0)
        show_dir_content(".");
    else

        printf("%s\n", NORMAL_COLOR);

    return(0);
}
