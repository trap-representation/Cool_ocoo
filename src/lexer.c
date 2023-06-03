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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include "token.h"
#include "lexer.h"

#define FILE_READ_CHUNK_SIZE 1048576

#define create_new_token_node \
  if(*token_head==NULL){ \
    if((*token_head=malloc(sizeof(**token_head)))==NULL){ \
      fprintf(stderr,"implementation error: failed to allocate enough memory\n"); \
      return 1; \
    } \
    *tokens_curr=*token_head; \
    (*tokens_curr)->prev_token=NULL; \
  } \
  else{ \
    if(((*tokens_curr)->next_token=malloc(sizeof(**tokens_curr)))==NULL){ \
      fprintf(stderr,"implementation error: failed to allocate enough memory\n"); \
      return 1; \
    } \
    (*tokens_curr)->next_token->prev_token=*tokens_curr; \
    *tokens_curr=(*tokens_curr)->next_token; \
  } \
  (*tokens_curr)->next_token=NULL; \
  (*tokens_curr)->loc_x=*curr_x; \
  (*tokens_curr)->loc_y=*curr_y; \


/* `lex_buffer' returns 0 if it succeeds;
   otherwise, it returns 1 */


unsigned int lex_buffer(char *buf, size_t read_size, token **token_head, token **tokens_curr, uintmax_t *curr_x, uintmax_t *curr_y, char *file_name){
  size_t last_i=1;
  for(size_t i=0;i<read_size;i++){
    if(last_i!=i){
      if(buf[i]=='\n'){
        if(*curr_y==UINTMAX_MAX){
          fprintf(stderr,"%s:%ju:%ju: implementation error: input size too large\n",file_name,*curr_y,*curr_x);
          return 1;
        }
        *curr_y=*curr_y+1;
        *curr_x=0;
      }
      else{
        if(*curr_x==UINTMAX_MAX){
          fprintf(stderr,"%s:%ju:%ju: implementation error: input size too large\n",file_name,*curr_y,*curr_x);
          return 1;
        }
        *curr_x=*curr_x+1;
      }
      last_i=i;
    }
    if(buf[i]=='+' || buf[i]==';'){
      create_new_token_node;
      (*tokens_curr)->token_type=buf[i];
    }
  }
  return 0;
}


/* since for an empty source file the function `lex' can
   return `NULL', whether `lex' succeeded or not is
   returned in `lex_ret'.
   1 indicates a failure;
   0 indicates a success */


token* lex(char *file_name, unsigned int *lex_ret){
  FILE *lex_file=fopen(file_name,"r");
  if(lex_file==NULL){
    fprintf(stderr,"implementation error: failed to open file %s for reading\n",file_name);
    *lex_ret=1;
    return NULL;
  }
  char *read_buf;
  if((read_buf=malloc(FILE_READ_CHUNK_SIZE))==NULL){
    fprintf(stderr,"implementation error: failed to allocate enough memory\n");
    *lex_ret=1;
    return NULL;
  }
  size_t read_size;
  token *token_head=NULL;
  token *tokens_curr=NULL;
  uintmax_t curr_x=0;
  uintmax_t curr_y=1;
  while((read_size=fread(read_buf,1,FILE_READ_CHUNK_SIZE,lex_file))){
    if(ferror(lex_file)){
      fprintf(stderr,"implementation error: error while reading from file %s\n",file_name);
      *lex_ret=1;
      free(read_buf);
      fclose(lex_file);
      return token_head;
    }
    if(lex_buffer(read_buf,read_size,&token_head,&tokens_curr,&curr_x,&curr_y,file_name)){
      *lex_ret=1;
      free(read_buf);
      fclose(lex_file);
      return token_head;
    }
  }
  if(ferror(lex_file)){
    fprintf(stderr,"implementation error: error while reading from file %s\n",file_name);
    *lex_ret=1;
    free(read_buf);
    fclose(lex_file);
    return token_head;
  }
  free(read_buf);
  fclose(lex_file);
  *lex_ret=0;
  return token_head;
}
