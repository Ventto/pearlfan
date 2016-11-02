#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "ioutils.h"
#include "optutils.h"

static int no_conjunction(char opt_a, char opt_b)
{
	fprintf (stderr, "Option -%c cannot be used in conjuntion with %c.\n",
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
		fprintf (stderr, "Missing argument for option -%c.\n", opt);
	return missing;
}

static void usage()
{
	printf("Usage: pfan <file.config>\n");
	printf("<file.config> has to follow the pattern below:\n");
	printf("{relative_imagepath}+{open}/{close}/{before_close}\n");
	printf("An example of configuration file in /usr/share/pearlfan/.\n");
	printf("See more details:");
	printf("https://github.com/Ventto/pearlfan#configuration-file\n");
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

	while ((opt = getopt(argc, argv, "c:d:fh")) != -1)
	{
		switch (opt)
		{
			case 'h':
				usage();
				return 0;
				return PFAN_NO_OPT;
				break;
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
