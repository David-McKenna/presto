/*****
  command line parser -- generated by clig (Version: 1.1.3)

  The command line parser `clig':
  (C) 1995 Harald Kirsch (kir@iitb.fhg.de)
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* mainly a hack to make things work on SUNOS with gcc */
extern double strtod(const char *, char **);
extern long int strtol(const char *, char **, int);

#include "search_bin_cmd.h"

char *Program;

/*@-null*/

static Cmdline cmd = {
  /***** -ncand: Number of candidates to try to return */
  /* ncandP = */ TRUE,
  /* ncand = */ 100,
  /* ncandC = */ 1,
  /***** -minfft: Power-of-2 length of the shortest miniFFT */
  /* minfftP = */ TRUE,
  /* minfft = */ 32,
  /* minfftC = */ 1,
  /***** -maxfft: Power-of-2 length of the longest miniFFT */
  /* maxfftP = */ TRUE,
  /* maxfft = */ 262144,
  /* maxfftC = */ 1,
  /***** -rlo: The low Fourier frequency to check */
  /* rloP = */ FALSE,
  /* rlo = */ (int)0,
  /* rloC = */ 0,
  /***** -rhi: The high Fourier frequency to check */
  /* rhiP = */ FALSE,
  /* rhi = */ (int)0,
  /* rhiC = */ 0,
  /***** -lobin: The first Fourier frequency in the data file */
  /* lobinP = */ TRUE,
  /* lobin = */ 0,
  /* lobinC = */ 1,
  /***** -overlap: Fraction of a short FFT length to shift before performing another */
  /* overlapP = */ TRUE,
  /* overlap = */ 0.25,
  /* overlapC = */ 1,
  /***** -harmsum: Number of harmonics to sum in the miniFFTs */
  /* harmsumP = */ TRUE,
  /* harmsum = */ 3,
  /* harmsumC = */ 1,
  /***** -numbetween: Number of points to interpolate per Fourier bin (2 gives the usual bin value and an interbin) */
  /* numbetweenP = */ TRUE,
  /* numbetween = */ 2,
  /* numbetweenC = */ 1,
  /***** -stack: Number of stacked power spectra making up the data.  (The default means the data are complex amplitudes) */
  /* stackP = */ TRUE,
  /* stack = */ 0,
  /* stackC = */ 1,
  /***** -interbin: Use interbinning instead of full-blown Fourier interpolation.  (Faster but less accurate and sensitive) */
  /* interbinP = */ FALSE,
  /***** -noalias: Do not add aliased powers to the harmonic sum.  (Faster but less accurate and sensitive) */
  /* noaliasP = */ FALSE,
  /***** uninterpreted rest of command line */
  /* argc = */ 0,
  /* argv = */ (char**)0,
  /***** the original command line concatenated */
  /* full_cmd_line = */ NULL
};

/*@=null*/

/***** let LCLint run more smoothly */
/*@-predboolothers*/
/*@-boolops*/


/**********************************************************************/
int
getIntOpt(int argc, char **argv, int i, int *value, int force)
{
  char *end;

  if( ++i>=argc ) goto nothingFound;

  *value = (int)strtol(argv[i], &end, 10);

  /***** check for conversion error */
  if( end==argv[i] ) goto nothingFound;

  /***** check for surplus non-whitespace */
  while( isspace(*end) ) end+=1;
  if( *end ) goto nothingFound;

  return i;

nothingFound:
  if( !force ) return i-1;

  fprintf(stderr, 
	  "%s: missing or malformed integer value after option `%s'\n",
	  Program, argv[i-1]);
    exit(EXIT_FAILURE);
}
/**********************************************************************/
int
getIntOpts(int argc, char **argv, int i, 
	   int **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;
  char *end;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  /***** 
    alloc a bit more than cmin values. It does not hurt to have room
    for a bit more values than cmax.
  *****/
  alloced = cmin + 4;
  *values = (int*)calloc((size_t)alloced, sizeof(int));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory while parsing option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = (int *) realloc(*values, alloced*sizeof(int));
      if( !*values ) goto outMem;
    }

    (*values)[used] = (int)strtol(argv[used+i+1], &end, 10);

    /***** check for conversion error */
    if( end==argv[used+i+1] ) break;

    /***** check for surplus non-whitespace */
    while( isspace(*end) ) end+=1;
    if( *end ) break;
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
	    "%s: parameter `%s' of `%s' should be an "
	    "integer value\n",
	    Program, argv[i+used+1], argv[i]);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
int
getFloatOpt(int argc, char **argv, int i, double *value, int force)
{
  char *end;

  if( ++i>=argc ) goto nothingFound;

  *value = (double)strtod(argv[i], &end);

  /***** check for conversion error */
  if( end==argv[i] ) goto nothingFound;

  /***** check for surplus non-whitespace */
  while( isspace(*end) ) end+=1;
  if( *end ) goto nothingFound;

  return i;

nothingFound:
  if( !force ) return i-1;

  fprintf(stderr,
	  "%s: missing or malformed double value after option `%s'\n",
	  Program, argv[i-1]);
  exit(EXIT_FAILURE);
 
}
/**********************************************************************/
int
getFloatOpts(int argc, char **argv, int i, 
	   double **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;
  char *end;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  /***** 
    alloc a bit more than cmin values.
  *****/
  alloced = cmin + 4;
  *values = (double*)calloc((size_t)alloced, sizeof(double));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory while parsing option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = (double *) realloc(*values, alloced*sizeof(double));
      if( !*values ) goto outMem;
    }

    (*values)[used] = (double)strtod(argv[used+i+1], &end);

    /***** check for conversion error */
    if( end==argv[used+i+1] ) break;

    /***** check for surplus non-whitespace */
    while( isspace(*end) ) end+=1;
    if( *end ) break;
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
	    "%s: parameter `%s' of `%s' should be a "
	    "double precision value\n",
	    Program, argv[i+used+1], argv[i]);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
int
getStringOpt(int argc, char **argv, int i, char **value, int force)
{
  if( ++i>=argc ) {
    fprintf(stderr, "%s: missing string after option `%s'\n",
            Program, argv[i-1]);
    exit(EXIT_FAILURE);
  }
  
  if( !force && argv[i+1][0] == '-' ) return i-1;
  *value = argv[i];
  return i;
}
/**********************************************************************/
int
getStringOpts(int argc, char **argv, int i, 
	   char*  **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  alloced = cmin + 4;
    
  *values = (char**)calloc((size_t)alloced, sizeof(char*));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory during parsing of option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = (char **)realloc(*values, alloced*sizeof(char*));
      if( !*values ) goto outMem;
    }

    if( used>=cmin && argv[used+i+1][0]=='-' ) break;
    (*values)[used] = argv[used+i+1];
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
    "%s: less than %d parameters for option `%s', only %d found\n",
	    Program, cmin, argv[i], used);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
void
checkIntLower(char *opt, int *values, int count, int max)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]<=max ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' greater than max=%d\n",
	    Program, i+1, opt, max);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkIntHigher(char *opt, int *values, int count, int min)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]>=min ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' smaller than min=%d\n",
	    Program, i+1, opt, min);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkFloatLower(char *opt, double *values, int count, double max)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]<=max ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' greater than max=%f\n",
	    Program, i+1, opt, max);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkFloatHigher(char *opt, double *values, int count, double min)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]>=min ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' smaller than min=%f\n",
	    Program, i+1, opt, min);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
static char *
catArgv(int argc, char **argv)
{
  int i;
  size_t l;
  char *s, *t;

  for(i=0, l=0; i<argc; i++) l += (1+strlen(argv[i]));
  s = (char *)malloc(l);
  if( !s ) {
    fprintf(stderr, "%s: out of memory\n", Program);
    exit(EXIT_FAILURE);
  }
  strcpy(s, argv[0]);
  t = s;
  for(i=1; i<argc; i++) {
    t = t+strlen(t);
    *t++ = ' ';
    strcpy(t, argv[i]);
  }
  return s;
}
/**********************************************************************/
void
showOptionValues(void)
{
  int i;

  printf("Full command line is:\n`%s'\n", cmd.full_cmd_line);

  /***** -ncand: Number of candidates to try to return */
  if( !cmd.ncandP ) {
    printf("-ncand not found.\n");
  } else {
    printf("-ncand found:\n");
    if( !cmd.ncandC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.ncand);
    }
  }

  /***** -minfft: Power-of-2 length of the shortest miniFFT */
  if( !cmd.minfftP ) {
    printf("-minfft not found.\n");
  } else {
    printf("-minfft found:\n");
    if( !cmd.minfftC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.minfft);
    }
  }

  /***** -maxfft: Power-of-2 length of the longest miniFFT */
  if( !cmd.maxfftP ) {
    printf("-maxfft not found.\n");
  } else {
    printf("-maxfft found:\n");
    if( !cmd.maxfftC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.maxfft);
    }
  }

  /***** -rlo: The low Fourier frequency to check */
  if( !cmd.rloP ) {
    printf("-rlo not found.\n");
  } else {
    printf("-rlo found:\n");
    if( !cmd.rloC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.rlo);
    }
  }

  /***** -rhi: The high Fourier frequency to check */
  if( !cmd.rhiP ) {
    printf("-rhi not found.\n");
  } else {
    printf("-rhi found:\n");
    if( !cmd.rhiC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.rhi);
    }
  }

  /***** -lobin: The first Fourier frequency in the data file */
  if( !cmd.lobinP ) {
    printf("-lobin not found.\n");
  } else {
    printf("-lobin found:\n");
    if( !cmd.lobinC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.lobin);
    }
  }

  /***** -overlap: Fraction of a short FFT length to shift before performing another */
  if( !cmd.overlapP ) {
    printf("-overlap not found.\n");
  } else {
    printf("-overlap found:\n");
    if( !cmd.overlapC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.overlap);
    }
  }

  /***** -harmsum: Number of harmonics to sum in the miniFFTs */
  if( !cmd.harmsumP ) {
    printf("-harmsum not found.\n");
  } else {
    printf("-harmsum found:\n");
    if( !cmd.harmsumC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.harmsum);
    }
  }

  /***** -numbetween: Number of points to interpolate per Fourier bin (2 gives the usual bin value and an interbin) */
  if( !cmd.numbetweenP ) {
    printf("-numbetween not found.\n");
  } else {
    printf("-numbetween found:\n");
    if( !cmd.numbetweenC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.numbetween);
    }
  }

  /***** -stack: Number of stacked power spectra making up the data.  (The default means the data are complex amplitudes) */
  if( !cmd.stackP ) {
    printf("-stack not found.\n");
  } else {
    printf("-stack found:\n");
    if( !cmd.stackC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.stack);
    }
  }

  /***** -interbin: Use interbinning instead of full-blown Fourier interpolation.  (Faster but less accurate and sensitive) */
  if( !cmd.interbinP ) {
    printf("-interbin not found.\n");
  } else {
    printf("-interbin found:\n");
  }

  /***** -noalias: Do not add aliased powers to the harmonic sum.  (Faster but less accurate and sensitive) */
  if( !cmd.noaliasP ) {
    printf("-noalias not found.\n");
  } else {
    printf("-noalias found:\n");
  }
  if( !cmd.argc ) {
    printf("no remaining parameters in argv\n");
  } else {
    printf("argv =");
    for(i=0; i<cmd.argc; i++) {
      printf(" `%s'", cmd.argv[i]);
    }
    printf("\n");
  }
}
/**********************************************************************/

void
usage(void)
{
  fprintf(stderr, "usage: %s%s", Program, "\
 [-ncand ncand] [-minfft minfft] [-maxfft maxfft] [-rlo rlo] [-rhi rhi] [-lobin lobin] [-overlap overlap] [-harmsum harmsum] [-numbetween numbetween] [-stack stack] [-interbin] [-noalias] infile\n\
    Searches a long FFT for binary pulsar candidates using a phase modulation search.\n\
       -ncand: Number of candidates to try to return\n\
               1 integer value between 1 and 10000\n\
               default: `100'\n\
      -minfft: Power-of-2 length of the shortest miniFFT\n\
               1 integer value between 8 and 1048576\n\
               default: `32'\n\
      -maxfft: Power-of-2 length of the longest miniFFT\n\
               1 integer value between 8 and 1048576\n\
               default: `262144'\n\
         -rlo: The low Fourier frequency to check\n\
               1 integer value between 0 and oo\n\
         -rhi: The high Fourier frequency to check\n\
               1 integer value between 0 and oo\n\
       -lobin: The first Fourier frequency in the data file\n\
               1 integer value between 0 and oo\n\
               default: `0'\n\
     -overlap: Fraction of a short FFT length to shift before performing another\n\
               1 double precision value between 0.05 and 1.0\n\
               default: `0.25'\n\
     -harmsum: Number of harmonics to sum in the miniFFTs\n\
               1 integer value between 1 and 20\n\
               default: `3'\n\
  -numbetween: Number of points to interpolate per Fourier bin (2 gives the usual bin value and an interbin)\n\
               1 integer value between 1 and 16\n\
               default: `2'\n\
       -stack: Number of stacked power spectra making up the data.  (The default means the data are complex amplitudes)\n\
               1 integer value between 0 and oo\n\
               default: `0'\n\
    -interbin: Use interbinning instead of full-blown Fourier interpolation.  (Faster but less accurate and sensitive)\n\
     -noalias: Do not add aliased powers to the harmonic sum.  (Faster but less accurate and sensitive)\n\
       infile: Input file name (no suffix) of floating point fft data.  A '.inf' file of the same name must also exist\n\
               1 string value\n\
version: 04Dec99\n\
");
  exit(EXIT_FAILURE);
}
/**********************************************************************/
Cmdline *
parseCmdline(int argc, char **argv)
{
  int i, keep;

  Program = argv[0];
  cmd.full_cmd_line = catArgv(argc, argv);
  for(i=1, cmd.argc=1; i<argc; i++) {
    if( 0==strcmp("--", argv[i]) ) {
      while( ++i<argc ) argv[cmd.argc++] = argv[i];
      continue;
    }

    if( 0==strcmp("-ncand", argv[i]) ) {
      cmd.ncandP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.ncand, 1);
      cmd.ncandC = i-keep;
      checkIntLower("-ncand", &cmd.ncand, cmd.ncandC, 10000);
      checkIntHigher("-ncand", &cmd.ncand, cmd.ncandC, 1);
      continue;
    }

    if( 0==strcmp("-minfft", argv[i]) ) {
      cmd.minfftP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.minfft, 1);
      cmd.minfftC = i-keep;
      checkIntLower("-minfft", &cmd.minfft, cmd.minfftC, 1048576);
      checkIntHigher("-minfft", &cmd.minfft, cmd.minfftC, 8);
      continue;
    }

    if( 0==strcmp("-maxfft", argv[i]) ) {
      cmd.maxfftP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.maxfft, 1);
      cmd.maxfftC = i-keep;
      checkIntLower("-maxfft", &cmd.maxfft, cmd.maxfftC, 1048576);
      checkIntHigher("-maxfft", &cmd.maxfft, cmd.maxfftC, 8);
      continue;
    }

    if( 0==strcmp("-rlo", argv[i]) ) {
      cmd.rloP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.rlo, 1);
      cmd.rloC = i-keep;
      checkIntHigher("-rlo", &cmd.rlo, cmd.rloC, 0);
      continue;
    }

    if( 0==strcmp("-rhi", argv[i]) ) {
      cmd.rhiP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.rhi, 1);
      cmd.rhiC = i-keep;
      checkIntHigher("-rhi", &cmd.rhi, cmd.rhiC, 0);
      continue;
    }

    if( 0==strcmp("-lobin", argv[i]) ) {
      cmd.lobinP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.lobin, 1);
      cmd.lobinC = i-keep;
      checkIntHigher("-lobin", &cmd.lobin, cmd.lobinC, 0);
      continue;
    }

    if( 0==strcmp("-overlap", argv[i]) ) {
      cmd.overlapP = TRUE;
      keep = i;
      i = getFloatOpt(argc, argv, i, &cmd.overlap, 1);
      cmd.overlapC = i-keep;
      checkFloatLower("-overlap", &cmd.overlap, cmd.overlapC, 1.0);
      checkFloatHigher("-overlap", &cmd.overlap, cmd.overlapC, 0.05);
      continue;
    }

    if( 0==strcmp("-harmsum", argv[i]) ) {
      cmd.harmsumP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.harmsum, 1);
      cmd.harmsumC = i-keep;
      checkIntLower("-harmsum", &cmd.harmsum, cmd.harmsumC, 20);
      checkIntHigher("-harmsum", &cmd.harmsum, cmd.harmsumC, 1);
      continue;
    }

    if( 0==strcmp("-numbetween", argv[i]) ) {
      cmd.numbetweenP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.numbetween, 1);
      cmd.numbetweenC = i-keep;
      checkIntLower("-numbetween", &cmd.numbetween, cmd.numbetweenC, 16);
      checkIntHigher("-numbetween", &cmd.numbetween, cmd.numbetweenC, 1);
      continue;
    }

    if( 0==strcmp("-stack", argv[i]) ) {
      cmd.stackP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.stack, 1);
      cmd.stackC = i-keep;
      checkIntHigher("-stack", &cmd.stack, cmd.stackC, 0);
      continue;
    }

    if( 0==strcmp("-interbin", argv[i]) ) {
      cmd.interbinP = TRUE;
      continue;
    }

    if( 0==strcmp("-noalias", argv[i]) ) {
      cmd.noaliasP = TRUE;
      continue;
    }

    if( argv[i][0]=='-' ) {
      fprintf(stderr, "\n%s: unknown option `%s'\n\n",
              Program, argv[i]);
      usage();
    }
    argv[cmd.argc++] = argv[i];
  }/* for i */


  /*@-mustfree*/
  cmd.argv = argv+1;
  /*@=mustfree*/
  cmd.argc -= 1;

  if( 1>cmd.argc ) {
    fprintf(stderr, "%s: there should be at least 1 non-option argument(s)\n",
            Program);
    exit(EXIT_FAILURE);
  }
  if( 1<cmd.argc ) {
    fprintf(stderr, "%s: there should be at most 1 non-option argument(s)\n",
            Program);
    exit(EXIT_FAILURE);
  }
  /*@-compmempass*/  return &cmd;
}

