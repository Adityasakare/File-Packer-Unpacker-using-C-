
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

// myexe Directoryname Filename
// myexe Argv[1]
// Argv[1]  : Name of file which contains all merged data

struct FileInfo
{
    char Fname[256];        // File name
    int Fsize;                     // File size
};

int main(int argc, char *argv[])
{
    int ret = 0;
    int fddest = 0, fdsrc = 0;
    char name[300] = {'\0'};
    struct FileInfo fobj;
    char *Buffer;
    
    if(argc != 2)
    {
        printf("Insufficient arguments\n");
        printf("Usage :  File_name");
        printf("File_Name : Name of the file which contains the data of all files]n");
        return -1;
    }
    
    fdsrc = open(argv[1],O_RDONLY);
    if(fdsrc == -1)
    {
        printf("Unable to open the file : %s\n",argv[1]);
        return -1;
    }
    
    while((ret = read(fdsrc,&fobj,sizeof(fobj))) != 0)
    {
        printf("File Name : %s Size : %d\n",fobj.Fname, fobj.Fsize);
        fddest = creat(fobj.Fname,0777);
        if(fddest == -1)
        {
            printf("Unable to create file\n");
            return -1;
        }
        // Dynamic memory for data to read
        Buffer = (char*)malloc(fobj.Fsize);
        // Read the data from merged file
        read(fdsrc,Buffer,fobj.Fsize);
        // Write the data into the newly created file
        write(fddest,Buffer,fobj.Fsize);
        // Free the memory
        free(Buffer);
        close(fddest);
    }

    close(fdsrc);
    
    //unlink(argv[1]);
    
    return 0;
}
