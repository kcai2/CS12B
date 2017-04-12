// $Id: spellchk.c,v 1.9 2014-05-15 21:07:47-07 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

bool optx = false;
bool optxx = false;

void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", program_name, object, message);
   fflush (NULL);
   exit_status = EXIT_FAILURE;
}

FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (const char *filename, hashset *hashset) {
   (void) filename;
   yylineno = 1;
   //DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
   //             filename, hashset);
   bool lowfound = false;
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      if (has_hashset(hashset, yytext)) {
         continue;
      } else {
         yytext[0] = tolower(yytext[0]);
         if (!lowfound) {
            printf("%s not in dictionary\n", yytext);
            exit_status = 1;
         }
      }
      //STUBPRINTF ("%s: %d: %s\n", filename, yylineno, yytext);
      
   }
}

void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   //DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
   //        dictionary_name, hashset);
   //STUBPRINTF ("Open dictionary, load it, close it\n");
   FILE *dic = open_infile(dictionary_name);
   char buffer[1024];
   char *trimmed;
      for (int linenr = 1; ; ++linenr) {
         char *linepos = fgets (buffer, sizeof buffer, dic);
         if (linepos == NULL || linepos == EOF) break;
         if (buffer[strlen(buffer) -1] == '\n') {
            trimmed = strtok(buffer, "\n");
         }
         put_hashset(hashset, trimmed);
      }
   fclose(dic);
}


void scan_options (int argc, char** argv,
                   char **default_dictionary,
                   char **user_dictionary) {
   // Scan the arguments and set flags.
   opterr = false;
   int counter = 0;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': *user_dictionary = optarg;
                   break;
         case 'n': *default_dictionary = NULL;
                   break;
         case 'x': counter++;
                   if (counter <= 1) optx = true;
                   if (counter > 1) optx = true;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debug_flags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }
}


int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset();
   yy_flex_debug = false;
   scan_options (argc, argv, &default_dictionary, &user_dictionary);

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   //if multiple x arguments
   if (optx == true) xbug(hashset, optxx);
   
   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   
   yylex_destroy ();
   return exit_status;
}

