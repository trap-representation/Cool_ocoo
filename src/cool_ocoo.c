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
#include <string.h>
#include <stdint.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "evaluate.h"
#include "clean_up_routines.h"

int main(int argc, char *argv[]){
  unsigned int ret;
  char *infile="a.oc";
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"--input")==0 || strcmp(argv[i],"-i")==0){
      i++;
      if(i==argc){
        fprintf(stderr,"error: expected a file name after %s\n",argv[i-1]);
        return 1;
      }
      infile=argv[i];
    }
    else if(strcmp(argv[i],"--about")==0 || strcmp(argv[i],"-a")==0){
      printf("\
    Cool_ocoo is an implementation of the OCOO esoteric programming language.\n\
    This program is licensed under GPLv3 Copyright (C) 2023 Somdipto Chakraborty.\n");
    }
    else if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-h")==0){
      printf("available arguments:\n");
      printf("    --help (-h): display this help message and quit\n");
      printf("    --about (-a): display information about Cool_ocoo and quit\n");
      printf("    --input (-i) file-path-sequence: specify the input file path\n\n");

      printf("grammar:\n");
      printf("    file-path-sequence:\n");
      printf("      file-path file-path-sequence (opt)\n\n");

      printf("    file-path (one of):\n");
      printf("      a b c d e f g h i\n");
      printf("      j k l m n o p q r\n");
      printf("      s t u v w x y z A\n");
      printf("      B C D E F G H I J\n");
      printf("      K L M N O P Q R S\n");
      printf("      T U V W X Y Z ! \"\n");
      printf("      # $ %% & ' ( ) * +\n");
      printf("      , - . / 0 1 2 3 4\n");
      printf("      5 6 7 8 9 : ; < =\n");
      printf("      > ? @ [ \\ ] ^ _ `\n");
      printf("      ~\n");
      return 0;
    }
    else{
      fprintf(stderr,"error: invalid argument %s; use --help (-h) to display a list of available arguments\n",argv[i]);
      return 1;
    }
  }
  token *tokens=lex(infile,&ret);
  if(ret==1){
    clean_up_tokens(tokens);
    return 1;
  }
  if(tokens==NULL){
    fprintf(stderr,"error: empty translation unit\n");
    return 0;
  }
  translation_unit *tu=parse(tokens,&ret,infile);
  if(ret==1){
    clean_up_tokens(tokens);
    clean_up_translation_unit(tu);
    return 1;
  }
  ret=evaluate_translation_unit(tu);
  clean_up_tokens(tokens);
  clean_up_translation_unit(tu);
  return ret;
}
