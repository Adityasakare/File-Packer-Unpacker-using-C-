#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

// myexe Directoryname Filename
// myexe Argv[1]           Argv[2]
// Argv[1]  : Name of directory that we want to open
// Argv[2]  : Name of file that we want to create

struct FileInfo
{
    char Fname[256];        // File name
    int Fsize;                     // File size
};

int main(int argc, char *argv[])
{
    DIR *dp = NULL;
    struct dirent *dip = NULL;
    struct stat sobj;
    int ret = 0;
    int fddest = 0, fdsrc = 0;
    char name[300] = {'\0'};
    struct FileInfo fobj;
    char Buffer[1024];
    
    if(argc != 3)
    {
        printf("Insufficient arguments\n");
        printf("Usage : Directory_name  File_name");
        printf("Directory_Name : Name of directory which contains the file that we want to combine\n");
        printf("File_Name : Name of the file which contains the data of all filesn");
        return -1;
    }

    dp = opendir(argv[1]);
    if(dp == NULL)
    {
        printf("Unable to open the directory ie : %s\n",argv[1]);
        return -1;
    }
    
    fddest = creat(argv[2],0777);       // Create new file to merge the data
    if(fddest == -1)
    {
        printf("Unable to careate new file ie : %s\n",argv[2]);
        closedir(dp);
        return -1;
    }
    
    printf("Names the files are\n");
    while((dip = readdir(dp)) != NULL)
    {
        sprintf(name,"%s/%s",argv[1],dip->d_name);  // Create relative path
        ret = stat(name,&sobj);
        if(S_ISREG(sobj.st_mode))
        {
            // Fill the structure which contains file information
            strcpy(fobj.Fname,dip->d_name);
            fobj.Fsize = sobj.st_size;
            
            // Write the fileinfo structure into the file
            write(fddest,&fobj,sizeof(fobj));
        
            memset(&fobj,0,sizeof(fobj));       // zeroout the memory
            fdsrc = open(name,O_RDONLY);
            
            // copy the contents of all files from directory into the HEllo.txt
            while((ret = read(fdsrc,&Buffer,sizeof(Buffer)))!= 0)
            {
                write(fddest,Buffer,ret);
            }

            memset(Buffer,0,sizeof(Buffer));
            close(fdsrc);
            printf("%s : %lld\n",dip->d_name, sobj.st_size);
        }
    }
    
    close(fddest);
    closedir(dp);
    return 0;
}
