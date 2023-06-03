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

#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include "token.h"

extern translation_unit *parse(token *tokens, unsigned int *parse_ret, char *file_name);

extern unsigned int parse_translation_unit(translation_unit **phrase_translation_unit, token *curr, char *file_name);

extern unsigned int parse_operator_sequence(translation_unit *phrase_translation_unit, operator_sequence *parent, token **curr, char *file_name);

extern unsigned int parse_operator(operator_sequence *phrase_operator_sequence, token **curr, char *file_name);

#endif
