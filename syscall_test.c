#include <stdio.h>

#include <unistd.h>
/*
    ssize_t write(int fd, const void *buf, size_t count);
*/

#include <sys/types.h> // Included in dirent.h, but used for the type in the dirent struct?
#include <dirent.h> // for DIR format
/*
    DIR *opendir(const char *name); ???
    struct dirent *readdir(DIR *dirp); read the DIR content
    int closedir(DIR *dirp); Used to free allocated DIR struct by opendir?

    struct dirent
    {
    #ifndef __USE_FILE_OFFSET64
        __ino_t d_ino;
        __off_t d_off;
    #else
        __ino64_t d_ino;
        __off64_t d_off;
    #endif
        unsigned short int d_reclen;
        unsigned char d_type;
        char d_name[256];		We must not include limits.h!
  };
*/

void test_dir(void)
{
    DIR *result = opendir(".");
    struct dirent *dp;
    printf("List of all files in current directory\n");
    while ((dp=readdir(result)) != NULL)
        printf("%s              File type: %u\n", dp->d_name, dp->d_type);
    printf("Result of closing dir %d\n-----------------------------------------------------------\n", closedir(result));
}

    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdlib.h>
/*
    int stat(const char *pathname, struct stat *statbuf);
    int lstat(const char *pathname, struct stat *statbuf);
    http://codewiki.wikidot.com/c:struct-stat
*/
char *wtf(uid_t id);
char *get_time(__time_t *timer);
#include <time.h>

void stat_baby(char *file_path)
{
    struct stat *statbuf = malloc(sizeof(struct stat));
    int result = stat(file_path, statbuf);
    printf("Information about %s\n", file_path);
    printf("The mode for the file are %d\n", statbuf->st_mode);
    printf("The file size is %ld\n", statbuf->st_size);
    printf("User id of the file owner %s\n", wtf(statbuf->st_uid));
    printf("Numer of hard link: %ld\n", statbuf->st_nlink);
    printf("Last modified %s", ctime(&statbuf->st_mtime));
}

    #include <pwd.h>
/*
    struct passwd *getpwuid(uid_t uid); user name?
    struct group *getgrgid(gid_t gid); group name?
*/

char *wtf(uid_t id)
{
    struct group *test;
    struct passwd *result = getpwuid(id);
    return result->pw_name;
}


#include <time.h>
/*
    char *ctime(const time_t *timer)
*/


char *get_time(__time_t *timer)
{
    return ctime(timer);
}

     #include <sys/xattr.h>
/*
    ssize_t listxattr(const char *path, char *list, size_t size);
    ssize_t getxattr(const char *path, const char *name, void *value, size_t size);
    https://stackoverflow.com/questions/36229265/how-to-get-extended-attributes-of-a-fileunix-c
*/



int main(int argc, char **argv)
{
    if (argc == 1){
        printf("Please provid a file\n"); 
        return 0;
    }
    test_dir();
    stat_baby(argv[1]);
    return 0;
}