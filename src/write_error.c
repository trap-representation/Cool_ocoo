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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "write_error.h"

#define FILE_READ_CHUNK_SIZE 1048576

void write_error(char *err, uintmax_t loc_x, uintmax_t loc_y, char *file_name){
  FILE *err_file;
  if((err_file=fopen(file_name,"r"))==NULL){
    fprintf(stderr,"implementation error: failed to open file %s for reading\n",file_name);
    return;
  }
  char *read_buf;
  if((read_buf=malloc(FILE_READ_CHUNK_SIZE))==NULL){
    fprintf(stderr,"implementation error: failed to allocate enough memory\n");
  }
  size_t read_size;
  bool read_end=false;
  uintmax_t pos_x=1;
  uintmax_t pos_y=1;
  bool write=false;
  int pad;
  int file_pad;
  while(!read_end && (read_size=fread(read_buf,1,FILE_READ_CHUNK_SIZE,err_file))){
    if(ferror(err_file)){
      fprintf(stderr,"implementation error: error while reading from file %s\n",file_name);
      fclose(err_file);
      return;
    }
    for(size_t i=0;i<read_size;){
      if(write){
        fputc(read_buf[i],stderr);
        if(pos_x>=loc_x){
          if(read_buf[i]=='\n'){
            for(int i=0;i<pad+file_pad;i++){
              fputc(' ',stderr);
            }
            for(uintmax_t i=1;i<loc_x;i++){
              fputc(' ',stderr);
            }
            fputc('^',stderr);
            fputc('\n',stderr);
            read_end=true;
            break;
          }
        }
        pos_x++;
        i++;
      }
      else{
        if(pos_y==loc_y){
          fprintf(stderr,"%s",err);
          fprintf(stderr,"%s%n",file_name,&file_pad);
          fprintf(stderr,":%ju:%ju: %n",loc_y,loc_x,&pad);
          write=true;
        }
        else if(read_buf[i]=='\n'){
          pos_y++;
          pos_x=1;
          i++;
        }
        else{
          pos_x++;
          i++;
        }
      }
    }
  }
  if(ferror(err_file)){
    fprintf(stderr,"implementation error: error while reading from file %s\n",file_name);
  }
  fclose(err_file);
}
