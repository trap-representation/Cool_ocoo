/*
    This file is a part of Cool_ocoo
    Copyright (C) 2023  Somdipto Chakraborty

    Cool_ocoo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cool_ocoo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cool_ocoo.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdint.h>

#include "write_error.h"
#include "evaluate.h"

#define BLOCK_OPERAND1 0
#define BLOCK_OPERAND2 1
#define BLOCK_SWAP 2
#define BLOCK_SIGN 3
#define BLOCK_ZERO 4
#define BLOCK_JUMP 5
#define BLOCK_STORE 6
#define BLOCK_LOAD 7
#define BLOCK_NULL 8
#define BLOCK_IMPL1 9
#define BLOCK_IMPL2 10

unsigned int bp;
unsigned int tape[30000];
unsigned int block_operand1=0;
unsigned int block_operand2=0;
unsigned int block_sign=0;
unsigned int block_impl1=0;
unsigned int block_impl2=0;

/* `evaluate_phrase_operator' returns a pointer to the next
   `operator_sequence'. Since a `NULL' returned from it
   isn't enough to determine whether it succeeded or failed,
   it sets `ret' based on whether the evaluation was a success.
   1 indicates a failure, while 0 indicates a success */

operator_sequence *evaluate_phrase_operator(operator *phrase_operator, unsigned int *ret){
  *ret=0;
  unsigned int ut;
  int st;
  if(phrase_operator->operator_type=='+'){
    switch(bp){
    case BLOCK_OPERAND1:
      if(block_sign==0){
        block_operand1++;
      }
      else{
        block_operand1--;
      }
      break;
    case BLOCK_OPERAND2:
      if(block_sign==0){
        block_operand2++;
      }
      else{
        block_operand2--;
      }
      break;
    case BLOCK_SWAP:
      ut=block_operand1;
      block_operand1=block_operand2;
      block_operand2=ut;
      break;
    case BLOCK_SIGN:
      if(block_sign==0){
        block_sign=1;
      }
      else{
        block_sign=0;
      }
      break;
    case BLOCK_ZERO:
      block_operand1=0;
      break;
    case BLOCK_JUMP:
      if(block_operand2!=0){
        if(block_sign==0){
          operator_sequence *phrase_operator_sequence=phrase_operator->parent;
          while(block_operand1!=0 && phrase_operator_sequence!=NULL){
            phrase_operator_sequence=phrase_operator_sequence->phrase_operator_sequence;
            block_operand1--;
          }
          if(block_operand1!=0){
            write_error("error: could not jump to location\n",phrase_operator->loc_x,phrase_operator->loc_y,phrase_operator->file);
            *ret=1;
            return NULL;
          }
          return phrase_operator_sequence;
        }
        else{
          if(block_operand1!=0 && phrase_operator->parent->parent_type==phrase_type_translation_unit){
            write_error("error: could not jump to location\n",phrase_operator->loc_x,phrase_operator->loc_y,phrase_operator->file);
            *ret=1;
            return NULL;
          }
          else{
            operator_sequence *phrase_operator_sequence=phrase_operator->parent;
            while(block_operand1!=0 && phrase_operator_sequence->parent_type!=phrase_type_translation_unit){
              phrase_operator_sequence=phrase_operator_sequence->parent.phrase_operator_sequence;
              block_operand1--;
            }
            if(block_operand1!=0){
              write_error("error: could not jump to location\n",phrase_operator->loc_x,phrase_operator->loc_y,phrase_operator->file);
              *ret=1;
              return NULL;
            }
            return phrase_operator_sequence;
          }
        }
      }
      break;
    case BLOCK_STORE:
      tape[block_operand2]=block_operand1;
      break;
    case BLOCK_LOAD:
      block_operand1=tape[block_operand2];
      break;
    case BLOCK_NULL:
      break;
    case BLOCK_IMPL1:
      block_impl1++;
      break;
    case BLOCK_IMPL2:
      block_impl2++;
      break;
    }
  }
  else{
    if(bp==BLOCK_IMPL2){
      bp=BLOCK_OPERAND1;
    }
    else{
      bp++;
    }
  }
  switch(block_impl2){
  case 1:
    switch(block_impl1){
    case 1:
      if(fputc(block_operand1,stdout)==EOF){
        write_error("implementation error: failed to write character to stdout\n",phrase_operator->loc_x,phrase_operator->loc_y,phrase_operator->file);
        *ret=1;
        return NULL;
      }
      block_impl1=0;
      block_impl2=0;
      break;
    case 2:
      if((st=fgetc(stdin))==EOF && ferror(stdin)){
        write_error("implementation error: failed to read character from stdin\n",phrase_operator->loc_x,phrase_operator->loc_y,phrase_operator->file);
        *ret=1;
        return NULL;
      }
      if(st==EOF){
        st=1;
        block_sign=1;
      }
      block_operand1=st;
      block_impl1=0;
      block_impl2=0;
      break;
    case 3:
      return NULL;
    }
  }
  return phrase_operator->parent->phrase_operator_sequence;
}

unsigned int evaluate_phrase_operator_sequence(operator_sequence *phrase_operator_sequence){
  unsigned int ret;
  while(phrase_operator_sequence!=NULL){
    phrase_operator_sequence=evaluate_phrase_operator(phrase_operator_sequence->phrase_operator,&ret);
  }
  return ret;
}

unsigned int evaluate_translation_unit(translation_unit *phrase_translation_unit){
  if(evaluate_phrase_operator_sequence(phrase_translation_unit->phrase_operator_sequence)==1){
    return 1;
  }
  return 0;
}
