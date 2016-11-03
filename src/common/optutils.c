#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "ioutils.h"
#include "optutils.h"

static int no_conjunction(char opt_a, char opt_b)
{
	fprintf(stderr, "Option -%c cannot be used in conjuntion with %c.\n",
			opt_a, opt_b);
	return PFAN_INVALID_OPT;
}

static int missing_optvar(char opt)
{
	int missing = PFAN_VALID_OPT;

	switch (opt) {
	case 'c':
		missing = PFAN_INVALID_OPT;
		break;
	case 'd':
		missing = PFAN_INVALID_OPT;
		break;
	}

	if (missing == PFAN_INVALID_OPT)
		fprintf(stderr, "Missing argument for option -%c.\n", opt);
	return missing;
}

static void usage(void)
{
	fprintf(stdout, "Usage:\tpearlfan [-f] [-c FILE]\n");
	fprintf(stdout, "or:\tpearlfan [-f] [-d DIRECTORY]\n\n");
	fprintf(stdout, "Options:\n\n");
	fprintf(stdout, "Setting:\n");
	fprintf(stdout, "  -c:\tDisplays at most eight images with transition ");
	fprintf(stdout, "effects described in FILE.\n");
	fprintf(stdout, "  -d:\tDisplays at most eight .PBM images ");
	fprintf(stdout, "(11x156) in DIRECTORY.\n\n");
	fprintf(stdout, "Mode:\n");
	fprintf(stdout, "  -f:\tEnables fast-mode. Disables all others ");
	fprintf(stdout, "effect transitions.\n\n");
	fprintf(stdout, "Miscellaneous:\n");
	fprintf(stdout, "  -h:\tPrints this help and exits.\n");
	fprintf(stdout, "  -v:\tPrints version info and exits.\n\n");
	fprintf(stdout, "Examples:\n\n");
	fprintf(stdout, "Example of configuration file: ");
	fprintf(stdout, "/usr/share/pearlfan/images/example.config\n\n");
	fprintf(stdout, "Example of images: ");
	fprintf(stdout, "/usr/share/pearlfan/images\n\n");
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

	while ((opt = getopt(argc, argv, "c:d:fhv")) != -1) {
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
			if (!pfan_readaccess_file(optarg))
				return PFAN_INVALID_OPT;
			opts->carg = optarg;
			opts->cflag = 1;
			break;
		case 'd':
			if (opts->cflag)
				return no_conjunction(opt, 'c');
			if (!pfan_isdir(optarg))
				return PFAN_INVALID_OPT;
			opts->darg = optarg;
			opts->dflag = 1;
			break;
		case 'f':
			opts->fflag = 1;
			break;
		case '?':
			if (!missing_optvar(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			return PFAN_INVALID_OPT;
		default:
			return PFAN_INVALID_OPT;
		}
	}
	return PFAN_VALID_OPT;
}
