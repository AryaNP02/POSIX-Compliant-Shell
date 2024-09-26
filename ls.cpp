#include "shell.h"

#include <sys/stat.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>


using namespace std;

#define max_path_length 1024



struct dirInfo   // handle directory
{
    DIR *dirP;
    struct dirent *entry;
    struct stat fileStat;
};







void getPermission(mode_t mode, char * permission) {
    int index = 0;

    permission[index++] = (S_ISDIR(mode)) ? 'd' : (S_ISLNK(mode)) ? 'l' : '-';
    string perms= "rwx";

    mode_t owner_perm[3] = {S_IRUSR, S_IWUSR, S_IXUSR};
     for (int i = 0; i < 3; ++i) 
    {
        permission[index] = (mode & owner_perm[i]) ? perms[i] : '-';
        index++;
    }

    mode_t group_perm[3] = {S_IRGRP, S_IWGRP, S_IXGRP};
      for (int i = 0; i < 3; ++i) 
    {
        permission[index] = (mode & group_perm[i]) ? perms[i] : '-';
        index++;
    }

    mode_t others_perm[3] = {S_IROTH, S_IWOTH, S_IXOTH};
     for (int i = 0; i < 3; ++i) 
     {
        permission[index] = (mode & others_perm[i]) ? perms[i] : '-';
        index++;
    }

    permission[index] = '\0';
}
//////////////////////////////////  handle display   ////////////////////////////////////////

void displayFileInfo(const struct stat &fileStat, const char *filename) {
    char permissions[11];

    getPermission(fileStat.st_mode, permissions);

    int linkCount = fileStat.st_nlink;
    long long  fileSize = fileStat.st_size;

    struct group *grInfo = getgrgid(fileStat.st_gid);
    struct passwd *pwInfo = getpwuid(fileStat.st_uid);
   
  
    

    char *t = ctime(&fileStat.st_mtime);
    char Time[20];

    for (int i = 0; i < 12; ++i) {
        Time[i] = t[i + 4];
    }
    Time[12] = '\0';

    printf("%s %d %s %s %8lld %s %s\n", permissions, linkCount, pwInfo->pw_name, grInfo->gr_name, fileSize, Time, filename);
}



void getDirectory(const char *path, int for_a, int for_l) {
   
    dirInfo info,info_total;

    if ((info.dirP = opendir(path)) == NULL) {
        perror("opendir");
        exit(1);
    }
   
    //to display total 
    if ((info_total.dirP = opendir(path)) == NULL) {
        perror("opendir");
        exit(1);
    }
   
    long long total_blocks =0 ; 
//=================================This will get total_block ======================/
      while ((info_total.entry = readdir(info_total.dirP)) != NULL) {
        char* Fname = info_total.entry->d_name;

        if (Fname[0] == '.' && for_a == 0) {
            continue;
        }

        string fullpath = string(path) +"/" + string(Fname); 
         
        if (stat(fullpath.c_str(), &info_total.fileStat) < 0) {
            perror("stat");
            continue;
        }
         
         total_blocks += info_total.fileStat.st_blocks; //return Number of 512-size block. 
        
    }

    if (for_l)
      printf("Total %lld \n" , total_blocks/2);  //had to divide by  2  ..  my block size 1MB 

   //================================= geting File property ============================/

    while ((info.entry = readdir(info.dirP)) != NULL) {
        char* Fname = info.entry->d_name;

        if (Fname[0] == '.' && for_a == 0) {
            continue;
        }

        string fullpath = string(path) +"/" + string(Fname); 
         
        if (stat(fullpath.c_str(), &info.fileStat) < 0) {
            perror("stat");
            continue;
        }

        if (for_l) {
            displayFileInfo(info.fileStat, Fname);
        } else {
            printf("%s\n", Fname);
        }
    }

    closedir(info.dirP);
}

/////////////////////// start form here ////////////////////////

void ls_implementation(char **cmd_args, int arg_len) {
    int for_a = 0;
    int for_l = 0;
    vector<string> listDir;

   
    for (int i = 1; i < arg_len; ++i) {
        if (strcmp(cmd_args[i], "-la") == 0 || strcmp(cmd_args[i], "-al") == 0) {
            for_a = 1;
            for_l = 1;
        } else if (strcmp(cmd_args[i], "-a") == 0) {
            for_a = 1;
        } else if (strcmp(cmd_args[i], "-l") == 0) {
            for_l = 1;
        } else {
            listDir.push_back(cmd_args[i]);
        }
    }

    if (listDir.empty()) {
        listDir.push_back(".");
    }

    size_t dirC = listDir.size();

    for (size_t i = 0; i < dirC; ++i) {
        string path = listDir[i];
        if (path == "~") {
            path = myHome; 
        }

        getDirectory(path.c_str(), for_a, for_l);
        if (i < dirC - 1) {
            printf("\n");
        }
    }
}