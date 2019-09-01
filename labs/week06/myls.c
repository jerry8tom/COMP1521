#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXDIRNAME 100
#define MAXFNAME   200
#define MAXNAME    20

char *rwxmode(mode_t, char *);
char *username(uid_t, char *);
char *groupname(gid_t, char *);
	
int main(int argc, char *argv[])
{
    // string buffers for various names
    char dirname[MAXDIRNAME];
    char uname[MAXNAME+1]; // UNCOMMENT this line
    char gname[MAXNAME+1]; // UNCOMMENT this line
    char mode[MAXNAME+1]; // UNCOMMENT this line

    // collect the directory name, with "." as default
    if (argc < 2)
        strlcpy(dirname, ".", MAXDIRNAME);
    else
        strlcpy(dirname, argv[1], MAXDIRNAME);

    // check that the name really is a directory
    struct stat info;
    if (lstat(dirname, &info) < 0)
        { perror(argv[0]); exit(EXIT_FAILURE); }
    if ((info.st_mode & S_IFMT) != S_IFDIR)
        { fprintf(stderr, "%s: Not a directory\n",argv[0]); exit(EXIT_FAILURE); }

    // open the directory to start reading       // BRIAN COMMENTS
    DIR *df;                                     // 1. Declare DIR object
    df = opendir(argv[1]);                       // 2. opendir() opens the dir by filename, associates a dir stream
                                                //    with it and returns a pointer to the directory stream.

    // read directory entries
    struct dirent *entry;                        // 1. Declare a Dirent struct, a special struct used to return info about dir entries
    while ((entry = readdir(df)) != NULL) {      // 2. readdir() returns a ptr to the next dir entry.
                                                //    returns NULL at the end of the directory
        // ignore if object is '.'
        if (strcmp(entry->d_name,".") == 0) continue;

        // fill meta-data from entry into stat info
        lstat(entry->d_name, &info);

        // grab read write permissions
        mode_t ModeInfo = info.st_mode;
        rwxmode(ModeInfo, mode);       

        printf("%s  %-8.8s %-8.8s %8lld  %s\n",
        rwxmode(info.st_mode, mode),     // rwx mode
        username(info.st_uid, uname),   // user
        groupname(info.st_gid, gname),  // group
        info.st_size,                 // file size
        entry->d_name);                 // name of file

        
    }

    // Close DIR * file pointer object
    closedir(df);
    return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode(mode_t mode, char *str)
{   

    //printf("OCTAL MODE = %o\n", mode);

    // Bitwise & operations to determine file type
    // S_IFMT = octal code for "Type of file"

    // mode & IFMT -> remove lower bits to determine file type
    if ((mode & S_IFMT) == S_IFDIR) {            
        str[0] = 'd';
        //printf("directory\n");
    } else if ((mode & S_IFMT) == S_IFREG) {
        str[0] = '-';
        //printf("regular file\n");
    } else if ((mode & S_IFMT) == S_IFLNK) {
        str[0] = '1';
        //printf("symbolic link\n");
    } else {
        str[0] = '?';
        //printf("OTHER\n");
    }

    // Mask mode code with 0x1FF (0b111111111) to cut out last 3 octal permission digits
    unsigned mask = 0x1FF;
    printf("MASK = %o\n", mask);
    printf("MODE = %o\n", mode);
    unsigned bits = mode & mask;
    printf("BITS = %o\n", bits);

    // determine owner premission 
    strcat(str,((mode&S_IRUSR)?"r":"-"));
    strcat(str,((mode&S_IWUSR)?"w":"-"));
    strcat(str,((mode&S_IXUSR)?"x":"-"));
    
    // determine group premission 
    strcat(str,((mode&S_IRGRP)?"r":"-"));
    strcat(str,((mode&S_IWGRP)?"w":"-"));
    strcat(str,((mode&S_IXGRP)?"x":"-"));
    
    // determine others premission 
    strcat(str,((mode&S_IROTH)?"r":"-"));
    strcat(str,((mode&S_IWOTH)?"w":"-"));
    strcat(str,((mode&S_IXOTH)?"x":"-"));

    return str;
}

    // convert user id to user name
char *username (uid_t uid, char *name)
{
	struct passwd *uinfo = getpwuid (uid);
	if (uinfo != NULL)
		snprintf (name, MAXNAME, "%s", uinfo->pw_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) uid);
	return name;
}

// convert group id to group name
char *groupname (gid_t gid, char *name)
{
	struct group *ginfo = getgrgid (gid);
	if (ginfo != NULL)
		snprintf (name, MAXNAME, "%s", ginfo->gr_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) gid);
	return name;
}