#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "version.h"

int main(int argc, char* argv[])
{
   if(argc==2 && strcmp(argv[1],"b")==0)
   {
      printf("%s\n", DBVERSION_B);
      return 0;
   }
   if(argc==2 && strcmp(argv[1],"v")==0)
   {
      printf("%s\n", SVNVERSION);
      return 0;
   }
   if(argc==2 && strcmp(argv[1],"h")==0)
   {
      printf("%s\n", "h    help");
      printf("%s\n", "v    svn version");
      printf("%s\n", "b    db version, such as m50_o10");
      printf("%s\n", "bo   db oracle version, such as oracle10");
      printf("%s\n", "bm   db mysql version, such as mysql51");
      printf("%s\n", "bs   db sqlite version, such as sqlite3");
      return 0;
   }

   char dbVersion[256];
   memset(dbVersion, 0, sizeof(dbVersion));
   strcpy(dbVersion, DBVERSION);

   char* p = dbVersion;
   char* mysqlVersion = NULL;
   char* oracleVersion = NULL;
   char* sqliteVersion = NULL;
   if(*p!=0)
      mysqlVersion = p;

   while(*p!=0 && *p!=',')
      p++;

   if(*p!=0)
   {
      *p=0;
      p+=2;
      oracleVersion = p;
   }
   
   while(*p!=0 && *p!=',')
      p++;
      
   if(*p!=0)
   {
      *p=0;
      p+=2;
      sqliteVersion = p;
   }
    
   if(argc==2 && strcmp(argv[1],"bo")==0)
   {
      printf("%s\n", oracleVersion);
      return 0;
   }
   if(argc==2 && strcmp(argv[1],"bm")==0)
   {
      printf("%s\n", mysqlVersion);
      return 0;
   }
   if(argc==2 && strcmp(argv[1],"bs")==0)
   {
      printf("%s\n", sqliteVersion);
      return 0;
   } 


   printf("\nXFrame version: %s\n\n", SVNVERSION);
   printf("For MySQL [%s]\n", mysqlVersion==NULL?"null":mysqlVersion);
   printf("For Oracle [%s]\n", oracleVersion==NULL?"null":oracleVersion);
   printf("For Sqlite [%s]\n", sqliteVersion==NULL?"null":sqliteVersion);
   printf("\n");

   char* uniDir = getenv("UNIDIR");
   if(uniDir == NULL)
   {
      printf("WARNING: Please set UNIDIR.\n");
      return 0;
   }
   char* ldLibPath = getenv("LD_LIBRARY_PATH");
   if(ldLibPath == NULL)
   {
      printf("WARNING: Please set LD_LIBRARY_PATH.\n");
      return 0;
   }
   if(mysqlVersion)
   {
      char mysqlLibPath[256];
      sprintf(mysqlLibPath, "%s/lib/%s", uniDir, mysqlVersion);
      if(strstr(ldLibPath, mysqlLibPath)==NULL)
         printf("WARNING: LD_LIBRARY_PATH must include %s\n", mysqlLibPath);
   }
   if(oracleVersion)
   {
      char oracleLibPath[256];
      sprintf(oracleLibPath, "%s/lib/%s", uniDir, oracleVersion);
      if(strstr(ldLibPath, oracleLibPath)==NULL)
         printf("WARNING: LD_LIBRARY_PATH must include %s\n", oracleLibPath);
   }
   if(sqliteVersion)
   {
      char sqliteLibPath[256];
      sprintf(sqliteLibPath, "%s/lib/%s", uniDir, sqliteVersion);
      if(strstr(ldLibPath, sqliteLibPath)==NULL)
         printf("WARNING: LD_LIBRARY_PATH must include %s\n", sqliteLibPath);
   }
   printf("\n");
   return 0;
}

