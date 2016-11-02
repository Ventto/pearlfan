#ifndef OPTUTILS_H_
#define OPTUTILS_H_

#define PFAN_NO_OPT         0
#define PFAN_VALID_OPT      1
#define PFAN_INVALID_OPT    2

typedef struct {
	int cflag;
	int dflag;
	int fflag;
	char *carg;
	char *darg;
} pfan_opts;

int pfan_getopt(int argc, char **argv, pfan_opts *opts);

#endif /* !OPTUTILS_H_ */
