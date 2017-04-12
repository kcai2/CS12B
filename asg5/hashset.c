// $Id: hashset.c,v 1.9 2014-05-15 20:01:08-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %zd, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void sizedup(hashset *this) {
   size_t old = this->size;
   size_t new = (this->size * 2) + 1;
   hashnode **array = malloc(new * sizeof(hashnode *));
   for (size_t i = 0; i < new; i++) {
      array[i] = NULL;
   }

   for (size_t oldi = 0; oldi < old; oldi++) {
      if (this->chains[oldi] != NULL) {
         hashnode *tmp = malloc(sizeof(hashnode));
         tmp = this->chains[oldi];
         while (tmp != NULL) {
            size_t hashi = strhash(tmp->word) % new;
            if(array[hashi] != NULL) {
               tmp = array[hashi];
               while (tmp->link != NULL) {
                  tmp = tmp->link;
               }
               array[hashi] = tmp;
            } else {
               array[hashi] = tmp;
            }
            tmp = tmp->link;
         }
      }
   }
   this->chains = array;
}


void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
}

void xbug(hashset *this, bool twox) {
   int counter[10];
   size_t chaincount = 1;
   for (int p = 0; p < 10; p++) {
      counter[p] = 0;
   }
   for (size_t i = 0; i < this->size; i++) {
      chaincount = 1;
      hashnode *curr = this->chains[i];
      if (curr != NULL) {
         if (this->chains[i]->link == NULL) {
            counter[chaincount]++;
         } else {
            if(twox) {
               printf("array[%10zd] = %20lu \"%s\"\n",
                     i, strhash(curr->word), curr->word); 
            }
            while (curr->link != NULL) {
               curr = curr->link;
               chaincount++;
               if(twox) {
                  printf("%17s = %20lu \"%s\"\n", " ", 
                        strhash(curr->word), curr->word);
               }
               
               counter[chaincount]++;
            }
         }
      }
      if (!twox) {
         printf("%zd words in the hash set\n", this->load);
         printf("%zd size of the hash array\n", this->size);
         for (int inc = 0; inc < 10; inc++) {
            if (counter[inc] != 0) {
               printf("%d chains of size %d\n", counter[inc], inc);
            }
         }
      }
   }
}
void put_hashset (hashset *this, const char *obj) {
   if (has_hashset(this, obj)) return;
   char *item = strdup(obj);
   size_t index = strhash(item)%this->size;
   //printf("%zd\n", index);
   hashnode *tmp = malloc(sizeof(hashnode));
   this->load++;
   tmp->word = (char*)item;
   tmp->link = NULL;
   hashnode *curr = this->chains[index];
   if(this->chains[index] == NULL) this->chains[index] = tmp;
   else {
      for(; curr == NULL;) {
         curr = curr->link;
      }
      curr->link = tmp;
   }
   if(this->load * 2 > this->size) sizedup(this);
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
}

bool has_hashset (hashset *this, const char *item) {
   size_t hash_index = strhash(item) % this->size;
   hashnode *curr = this->chains[hash_index];
   while (curr != NULL) {
      int cmp = strcmp(curr->word, item);
      if (cmp == 0) return true;
      curr = curr->link;
   }
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
   return false;;
}

