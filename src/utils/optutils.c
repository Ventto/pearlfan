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
#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "optutils.h"
#include "ioutils.h"

static int no_conjunction(char opt_a, char opt_b)
{
	fprintf(stderr, "Option -%c cannot be used in conjuntion with %c.\n\n",
			opt_a, opt_b);
	return PFAN_INVALID_OPT;
}

static int missing_optvar(char opt)
{
	switch (opt) {
	case 'c':
		return 1;
	case 'd':
		return 1;
	case 't':
		return 1;
	}
	return 0;
}

static void usage(void)
{
	fprintf(stdout, "Usage:\tpearlfan [-f] [-c FILE | -d DIRECTORY | -t TEXT]\n\n");
	fprintf(stdout, "Options:\n\n");
	fprintf(stdout, "Setting:\n");
	fprintf(stdout, "  -c:\tDisplays at most eight images with transition ");
	fprintf(stdout, "effects described in FILE.\n");
	fprintf(stdout, "  -d:\tDisplays at most eight .PBM images ");
	fprintf(stdout, "(156x11) in DIRECTORY.\n");
	fprintf(stdout, "  -t:\tDraws TEXT on multiple displays.\n\n");
	fprintf(stdout, "Mode:\n");
	fprintf(stdout, "  -f:\tEnables fast-mode. Disables all others ");
	fprintf(stdout, "effect transitions.\n\n");
	fprintf(stdout, "Miscellaneous:\n");
	fprintf(stdout, "  -h:\tPrints this help and exits.\n");
	fprintf(stdout, "  -v:\tPrints version info and exits.\n\n");
	fprintf(stdout, "Examples:\n\n");
	fprintf(stdout, "Example of configuration file and images: ");
	fprintf(stdout, "/usr/share/pearlfan\n\n");
	fprintf(stdout, "Documentation: <https://github.com/Ventto/pearlfan>\n\n");
	fprintf(stdout, "Report bugs to <thomas.venries@gmail.com>\n\n");
}

static void version(void)
{
	fprintf(stdout, "Pearlfan 1.0\n\n");
	fprintf(stdout, "Copyright (C) 2016 Thomas \"Ventto\" Venries.\n\n");
	fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later "),
	fprintf(stdout, "<http://gnu.org/licenses/gpl.html>.\n");
	fprintf(stdout, "This is free software: you are free to change and ");
	fprintf(stdout, "redistribute it.\n");
	fprintf(stdout, "There is NO WARRANTY; not even for MERCHANTABILITY ");
	fprintf(stdout, "or FITNESS FOR A PARTICULAR PURPOSE\n");
}

int pfan_getopt(int argc, char **argv, pfan_opts *opts)
{
	int opt;

	opterr = 0;

	if (argc == 1) {
		usage();
		return PFAN_NO_OPT;
	}

	memset(opts, 0, sizeof(pfan_opts));

	while ((opt = getopt(argc, argv, "c:d:t:fhv")) != -1) {
		switch (opt) {
		case 'h':
			usage();
			return PFAN_NO_OPT;
		case 'v':
			version();
			return PFAN_NO_OPT;
		case 'c':
			if (opts->dflag)
				return no_conjunction(opt, 'd');
			if (opts->tflag)
				return no_conjunction(opt, 't');
			if (!pfan_readaccess_file(optarg))
				return PFAN_INVALID_OPT;
			opts->carg = optarg;
			opts->cflag = 1;
			break;
		case 'd':
			if (opts->cflag)
				return no_conjunction(opt, 'c');
			if (opts->tflag)
				return no_conjunction(opt, 't');
			if (!pfan_isdir(optarg)) {
				fprintf(stderr, "%s: directory does not exist or can't be opened.\n\n", optarg);
				return PFAN_INVALID_OPT;
			}
			opts->darg = optarg;
			opts->dflag = 1;
			break;
		case 't':
			if (opts->cflag)
				return no_conjunction(opt, 'c');
			if (opts->dflag)
				return no_conjunction(opt, 'd');
			opts->targ = optarg;
			opts->tflag = 1;
			break;
		case 'f':
			opts->fflag = 1;
			break;
		case '?':
			if (missing_optvar(optopt))
				fprintf(stderr, "Missing argument for option -%c.\n\n", optopt);
			else
				fprintf(stderr, "Unknown option `-%c'.\n\n", optopt);
			return PFAN_INVALID_OPT;
		default:
			return PFAN_INVALID_OPT;
		}
	}
	if (!opts->cflag && !opts->dflag && !opts->tflag) {
		if (opts->fflag)
			fprintf(stderr, "Missing setting option.\n\n");
		else
			fprintf(stderr, "Missing option.\n\n");
		return PFAN_INVALID_OPT;
	}
	return PFAN_VALID_OPT;
}
