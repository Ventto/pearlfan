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
#include "draw.h"
#include "defutils.h"
#include "tests.h"

void Test_columns(unsigned short display[PFAN_MAX_W])
{
	int x = 0;

	for (int i = 10; i >= 0; --i) {
		for (int j = i; j <= 10; ++j)
				pfan_draw_point(x, j, display);
		x += 2;
	}

	x += 22;

	for (int i = 0; i <= 10; ++i) {
		for (int j = 0; j <= i; ++j)
				pfan_draw_point(x, j, display);
		x += 2;
	}

	x = 48;
	for (int i = 0; i <= 10; ++i) {
		pfan_draw_point(47, i, display);    // column
		pfan_draw_point(x, i, display);     // diagonale
		pfan_draw_point(69, i, display);    // column
		x += 2;
	}

	x = 78;
	for (int i = 0; i <= 10; ++i) {
		pfan_draw_point(x++, i, display);   // each column led
		for (int j = 0; j <= 10; ++j)
			pfan_draw_point(x, j, display); // column
		++x;
	}
}

void Test_rows(unsigned short display[PFAN_MAX_W])
{
	int j = 0;
	for (int i = 0; i < 40; i+=3) {
		j = (j > 10) ? 0 : j;
		pfan_draw_point(i, j, display);
		pfan_draw_point(i+1, j++, display);
	}
}

void Test_square(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_point(0, 0, display);
	pfan_draw_point(1, 0, display);
	pfan_draw_point(2, 0, display);
	pfan_draw_point(3, 0, display);
	pfan_draw_point(4, 0, display);

	pfan_draw_point(0, 4, display);
	pfan_draw_point(1, 4, display);
	pfan_draw_point(2, 4, display);
	pfan_draw_point(3, 4, display);
	pfan_draw_point(4, 4, display);

	pfan_draw_point(0, 0, display);
	pfan_draw_point(0, 1, display);
	pfan_draw_point(0, 2, display);
	pfan_draw_point(0, 3, display);
	pfan_draw_point(0, 4, display);

	pfan_draw_point(4, 0, display);
	pfan_draw_point(4, 1, display);
	pfan_draw_point(4, 2, display);
	pfan_draw_point(4, 3, display);
	pfan_draw_point(4, 4, display);
}

void Test_26chars(unsigned short display[PFAN_MAX_W])
{
	for (int i = 0; i < 26; ++i)
		pfan_draw_char(i * 6, 'A', display);
}

void Test_oneChar(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_char(0, 'A', display);
}

void Test_textOneChar(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_text("A", 1, 1, display);
}

void Test_textNoChar(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_text("", 0, 1, display);
}

void Test_textLimit(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_text("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26, 1, display);
}

void Test_tinyText(unsigned short display[PFAN_MAX_W])
{
	pfan_draw_text("Hello World !", 13, 1, display);
}

void Test_paragraph(unsigned short displays[PFAN_MAX_DISPLAY][PFAN_MAX_W])
{
	pfan_draw_paragraph("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum porta libero nec blandit laoreet. Fusce bibendum tincidunt nunc, et pretium mi tincidunt ut. Nunc sit amet neque finibus, sagittis nibh sed.", displays);
}
