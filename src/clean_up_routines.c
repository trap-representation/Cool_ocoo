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
#include <stdint.h>

#include "clean_up_routines.h"

void clean_up_tokens(token *tokens){
  while(tokens!=NULL){
    token *prev=tokens;
    tokens=tokens->next_token;
    free(prev);
  }
}

void clean_up_operator(operator *phrase_operator){
  free(phrase_operator);
}

void clean_up_operator_sequence(operator_sequence *phrase_operator_sequence){
  while(phrase_operator_sequence!=NULL){
    clean_up_operator(phrase_operator_sequence->phrase_operator);
    operator_sequence *prev=phrase_operator_sequence;
    phrase_operator_sequence=phrase_operator_sequence->phrase_operator_sequence;
    free(prev);
  }
}

void clean_up_translation_unit(translation_unit *phrase_translation_unit){
  clean_up_operator_sequence(phrase_translation_unit->phrase_operator_sequence);
  free(phrase_translation_unit);
}
