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

#ifndef TREE_H
#define TREE_H

typedef struct s_operator {
  int operator_type;
  struct s_operator_sequence *parent;
  char *file;
  uintmax_t loc_x;
  uintmax_t loc_y;
} operator;

typedef struct s_operator_sequence {
  struct s_operator *phrase_operator;
  struct s_operator_sequence *phrase_operator_sequence;
  unsigned int parent_type;
  union operator_parent_type_union {
    #define phrase_type_translation_unit 0
    struct s_translation_unit *phrase_translation_unit;
    #define phrase_type_operator_sequence 1
    struct s_operator_sequence *phrase_operator_sequence;
  } parent;
  char *file;
} operator_sequence;

typedef struct s_translation_unit {
  struct s_operator_sequence *phrase_operator_sequence;
  char *file;
} translation_unit;

#endif
