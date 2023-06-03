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
#include <stdlib.h>
#include <stdint.h>

#include "parser.h"

unsigned int parse_operator(operator_sequence *phrase_operator_sequence, token **curr, char *file_name){
  if((phrase_operator_sequence->phrase_operator=malloc(sizeof(operator)))==NULL){
    fprintf(stderr,"implementation error: failed to allocate enough memory\n");
    return 1;
  }
  operator *phrase_operator=phrase_operator_sequence->phrase_operator;
  phrase_operator->file=file_name;
  phrase_operator->parent=phrase_operator_sequence;
  phrase_operator->operator_type=(*curr)->token_type;
  phrase_operator->loc_x=(*curr)->loc_x;
  phrase_operator->loc_y=(*curr)->loc_y;
  *curr=(*curr)->next_token;
  return 0;
}

unsigned int parse_operator_sequence(translation_unit *phrase_translation_unit, operator_sequence *parent, token **curr, char *file_name){
  operator_sequence *phrase_operator_sequence;
  while(1){
    if(parent==NULL){
      if((phrase_translation_unit->phrase_operator_sequence=malloc(sizeof(operator_sequence)))==NULL){
        fprintf(stderr,"implementation error: failed to allocate enough memory\n");
        return 1;
      }
      phrase_operator_sequence=phrase_translation_unit->phrase_operator_sequence;
      phrase_operator_sequence->parent_type=phrase_type_translation_unit;
      phrase_operator_sequence->parent.phrase_translation_unit=phrase_translation_unit;
    }
    else{
      if((parent->phrase_operator_sequence=malloc(sizeof(operator_sequence)))==NULL){
        fprintf(stderr,"implementation error: failed to allocate enough memory\n");
        return 1;
      }
      phrase_operator_sequence=parent->phrase_operator_sequence;
      phrase_operator_sequence->parent_type=phrase_type_operator_sequence;
      phrase_operator_sequence->parent.phrase_operator_sequence=parent;
    }
    phrase_operator_sequence->file=file_name;
    phrase_operator_sequence->phrase_operator=NULL;
    phrase_operator_sequence->phrase_operator_sequence=NULL;
    if(parse_operator(phrase_operator_sequence,curr,file_name)){
      return 1;
    }
    if(*curr!=NULL){
      parent=phrase_operator_sequence;
    }
    else{
      break;
    }
  }
  return 0;
}

unsigned int parse_translation_unit(translation_unit **phrase_translation_unit, token *curr, char *file_name){
  if((*phrase_translation_unit=malloc(sizeof(translation_unit)))==NULL){
    fprintf(stderr,"implementation error: failed to allocate enough memory\n");
    return 1;
  }
  (*phrase_translation_unit)->file=file_name;
  (*phrase_translation_unit)->phrase_operator_sequence=NULL;
  if(curr!=NULL){
    if(parse_operator_sequence(*phrase_translation_unit,NULL,&curr,file_name)){
      return 1;
    }
  }
  return 0;
}

translation_unit *parse(token *tokens, unsigned int *parse_ret, char *file_name){
  translation_unit *phrase_translation_unit;
  if(parse_translation_unit(&phrase_translation_unit,tokens,file_name)){
    *parse_ret=1;
    return phrase_translation_unit;
  }
  *parse_ret=0;
  return phrase_translation_unit;
}
