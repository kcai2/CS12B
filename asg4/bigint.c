// $Id: bigint.c,v 1.15 2015-02-03 18:11:58-08 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}


bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *do_add (bigint *this, bigint *that) {
  char carry = 0, input = 0;
  bigint *sum = new_bigint(this->capacity+1);
  sum->size = 0;
  //that->size = this->size;
  for(size_t inc =0; inc < this->size ; inc++){ 
      sum->size++;
      size_t new = inc < that->size ? that->digits[inc] : 0;
      char newSum = carry + this->digits[inc] +  new;
      input = newSum %10;
      carry = newSum/10;
      sum->digits[inc] = input;
      sum->digits[inc+1] += carry;
   }
   if(this->negative) sum->negative = true;
   trim_zeros(sum);
   return sum;
}

bigint *do_sub (bigint *left, bigint *right) {
      bigint *dif = new_bigint(left->capacity); 
      dif->size =0;
      char input;
      for(size_t inc = 0; inc < left->size; ++inc){
         dif->size++;
         int new = inc < right->size ? right->digits[inc] :0;
         if (left->digits[inc] >= new)
               input = left->digits[inc] - new; 

         else {input = left->digits[inc] + 10 - right->digits[inc];
               left->digits[inc+1] = left->digits[inc+1]- 1;
         }
         dif->digits[inc]= input;
      }
   if(left->negative)dif->negative = true;
   trim_zeros(dif);
   return dif;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this) {
   DEBUGS ('b', show_bigint (this));
   show_bigint(this);
}

bigint *do_compare(bigint *left, bigint *right){
     if (left->size == right->size) {
          for(int index = left->size-1; index >= 0;){
             if (left->digits[index] == right->digits[index]){
                index--;
             } else if (left->digits[index] >
                               right->digits[index]){
                 return left;
             } else return right;
          }
     } else if (left->size > right->size) return left;
                  else return right;
     return left;
}

bigint *add_bigint (bigint *this, bigint *that) {
   if (this->negative == that->negative) {
      if (this->size > that->size) return do_add(this, that);
                   else return do_add(that,this);

   } else {
      if (this->size == that->size){
         return this->digits[this->size-1] >= 
                   that->digits[this->size-1]?
                   do_sub(this, that) : do_sub(that, this);
      }
      else if (this->size > that->size) return do_sub(this, that);
                   else return do_sub(that,this);
   }
}

bigint *sub_bigint (bigint *left, bigint *right) {
   bigint *result = new_bigint(left->capacity > right->capacity ?
                                 left->capacity : right->capacity);
   bigint *compare;
   if (left->negative != right->negative) {
       if (left->size >= right->size){ 
                 result = do_add(left, right);
       }
                     else result  = do_add(right, left);
   } else {

        compare = do_compare(left, right);
        if (left == compare)
            result = do_sub(left, right);
        else {
            result =  do_sub(right, left);
            result->negative = true;
        }
   }
   return result;
}

bigint *mul_bigint (bigint *this, bigint *that) {
   bigint *prod = new_bigint(this->size + that->size);
   prod->size = this->size + that->size;
   prod->capacity = this->capacity +that->capacity;
   for(size_t i = 0; i < that->size; i++){ 
       char carry =0;
      for(size_t j = 0; j< this->size; j++){
          char input = prod->digits[i+j] + that->digits[i] * 
                               this->digits[j] + carry;
          prod->digits[i+j] = input%10;
          carry = input/10;
      }  
   prod->digits[i+this->size]= carry;
   }
   trim_zeros(prod);
   if(this->negative != that->negative) prod->negative = true;
   return prod;
}


void show_bigint (bigint *this) {
   int count =0;
   if(this->size == 0)printf("0");
   else{
      if (this->negative) printf("-");
      for (char *byte = &this->digits[this->size - 1];
           byte >= this->digits; --byte) {
               printf ( "%d", *byte);
               count++;
               if(count == 69){printf("\\ \n"); count = 0;}
           }
   }
      printf ("\n");
}
