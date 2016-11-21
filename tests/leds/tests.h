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
#ifndef TESTS_H_
#define TESTS_H_

void Test_columns(unsigned short display[PFAN_MAX_W]);

void Test_rows(unsigned short display[PFAN_MAX_W]);

void Test_square(unsigned short display[PFAN_MAX_W]);

void Test_26chars(unsigned short display[PFAN_MAX_W]);

void Test_oneChar(unsigned short display[PFAN_MAX_W]);

void Test_textOneChar(unsigned short display[PFAN_MAX_W]);

void Test_textNoChar(unsigned short display[PFAN_MAX_W]);

void Test_textLimit(unsigned short display[PFAN_MAX_W]);

void Test_tinyText(unsigned short display[PFAN_MAX_W]);

void Test_aragraph(unsigned short displays[PFAN_MAX_DISPLAY][PFAN_MAX_W]);

#endif /* !TESTS_H_ */
