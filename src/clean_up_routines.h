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

#ifndef CLEAN_UP_ROUTINES_H
#define CLEAN_UP_ROUTINES_H

#include "tree.h"
#include "token.h"

extern void clean_up_tokens(token *tokens);

extern void clean_up_operator(operator *phrase_operator);

extern void clean_up_operator_sequence(operator_sequence *phrase_operator_sequence);

extern void clean_up_translation_unit(translation_unit *phrase_translation_unit);

#endif
