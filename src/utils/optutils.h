/*
 * The GPL License (GPLv3)
 *
 * Copyright © 2016 Thomas "Ventto" Venriès  <thomas.venries@gmail.com>
 *
 * Pearlfan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pearlfan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pearlfan.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPTUTILS_H_
#define OPTUTILS_H_

#define PFAN_NO_OPT         0
#define PFAN_VALID_OPT      1
#define PFAN_INVALID_OPT    2

typedef struct {
    int cflag;
    int dflag;
    int tflag;
    int fflag;
    char *carg;
    char *darg;
    char *targ;
} pfan_opts;

int pfan_getopt(int argc, char **argv, pfan_opts *opts);

#endif /* !OPTUTILS_H_ */
