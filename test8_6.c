//#include<dirent.h>
#include<stdio.h>
#include<string.h>
//#include"syscall.h"
#include<fcntl.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
//#include"dirent.h"

#define NAME_MAX 14
/*
#define S_IFMT  0160000
#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFBLK 0060000
#define S_IFREG 0100000
*/
typedef struct {    //可移植的目录项
    long ino; //i节点编号
    char name[NAME_MAX + 1];    //文件名加结束符'/0'
} Dirent;

typedef struct {    //最小的DIR： 无缓冲等特性
    int fd; //目录文件描述符
    Dirent d;   //目录项
} DIR;
DIR *
opendir(char *dirname);
Dirent *
readdir(DIR *dfd);
void closedir(DIR *dfd);
/*
char *name;


struct stat
{
    dev_t   st_dev;
    ino_t   st_ino;
    short   st_mode;
    short   st_nlink;
    short   st_uid;
    short   st_gid;
    dev_t   st_rdev;
    off_t   st_size;
    time_t  st_atime;
    time_t  st_mtime;
    time_t  st_ctime;
};
struct stat stbuf;
int stat(char *, struct stat *);
stat(name, &stbuf);
*/

void fsize(char *);
main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else 
        while (--argc > 0)
            fsize(*++argv);
    return 0;
}
void dirwalk(char *, void (*fcn)(char *));
void fsize(char *name)
{
    struct stat stbuf;

    if (stat(name, &stbuf) == -1) {
        fprintf(stderr, "fsize: can't access %s\n",name);
        return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name,fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}

#define MAX_PATH    1024
void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    Dirent  *dp;
    DIR     *dfd;

    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    while ((dp = readdir(dfd)) != NULL) {
        if ( strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
            continue;
        if (strlen(dir)+strlen(dp->name)+2  > sizeof(name))
            fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->name);
        else {
            sprintf(name, "%s/%s",dir, dp->name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}

//#include<sys/dir.h>
#ifndef DIRSIZ
#define DIRSIZ  14
#endif
struct direct
{
    ino_t d_ino;
    char  d_name[DIRSIZ];
};

DIR *
opendir(char *dirname)
{
    int fd;
    struct stat stbuf;
    DIR *dp;

    if ((fd = open(dirname, O_RDONLY, 0)) == -1 
        || fstat(fd, &stbuf) == -1
        || (stbuf.st_mode & S_IFMT) != S_IFDIR
        || (dp = (DIR *) malloc(sizeof(DIR))) == NULL)
            return NULL;
    dp->fd = fd;
    return dp;
}

void closedir(DIR *dp)
{
    if (dp) {
        close(dp->fd);
        free(dp);
    }
}

//#include<sys/dir.h>
Dirent *
readdir(DIR *dp)
{
    struct direct dirbuf;
    static Dirent d;

    while (read(dp->fd, (char *) &dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)) {
        if (dirbuf.d_ino == 0)
            continue;
        d.ino = dirbuf.d_ino;
        strncpy(d.name, dirbuf.d_name, DIRSIZ);
        d.name[DIRSIZ] = '\0';
        return &d;
    }
    return NULL;
}
