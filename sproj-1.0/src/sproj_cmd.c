/*
  File autogenerated by gengetopt version 2.22
  generated with the following command:
  gengetopt --input=sproj.ggo --file-name=sproj_cmd --unamed-opts 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

#include "sproj_cmd.h"

const char *gengetopt_args_info_purpose = "Compute forward or inverse cartographic projections";

const char *gengetopt_args_info_usage = "Usage: sproj [OPTIONS] [projection options]";

const char *gengetopt_args_info_description = "A program to compute forward or inverse cartographic projection.\n\n$Id: sproj.ggo,v 1.2 2009/05/16 19:37:40 gie Exp gie $";

const char *gengetopt_args_info_detailed_help[] = {
  "  -h, --help                 Print help and exit",
  "      --detailed-help        Print help, including all details and hidden \n                               options, and exit",
  "  -V, --version              Print version and exit",
  "  -I, --inverse              Inverse projection  (default=off)",
  "  -r, --revin                reverse order of input to phi-lam or y-x  \n                               (default=off)",
  "  -R, --revout               reverse order of output to y-x or phi-lam  \n                               (default=off)",
  "  -f, --format=C_format_str  output format",
  "  for forward projection use standard floating point opertation,\n  for inverse use DMS or colon delimited, two or three field format",
  "  -l, --list=ENUM            list options  (possible values=\"proj\", \n                               \"ellps\", \"pms\", \"units\" default=`proj')",
  "  \n  List option values for\n  \n  	\"proj\": projection list, short form\n  \n  	\"ellps\": list of elliptical figures and their names\n  \n  	\"pms\": list of prime meridian systems\n  \n  	\"units\": list of units conversion for Cartesian coordinated.",
  "  -P, --longproj[=proj_id]   detail projection list",
    0
};

static void
init_help_array(void)
{
  gengetopt_args_info_help[0] = gengetopt_args_info_detailed_help[0];
  gengetopt_args_info_help[1] = gengetopt_args_info_detailed_help[1];
  gengetopt_args_info_help[2] = gengetopt_args_info_detailed_help[2];
  gengetopt_args_info_help[3] = gengetopt_args_info_detailed_help[3];
  gengetopt_args_info_help[4] = gengetopt_args_info_detailed_help[4];
  gengetopt_args_info_help[5] = gengetopt_args_info_detailed_help[5];
  gengetopt_args_info_help[6] = gengetopt_args_info_detailed_help[6];
  gengetopt_args_info_help[7] = gengetopt_args_info_detailed_help[8];
  gengetopt_args_info_help[8] = gengetopt_args_info_detailed_help[10];
  gengetopt_args_info_help[9] = 0; 
  
}

const char *gengetopt_args_info_help[10];

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_ENUM
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char * const *argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);


char *cmdline_parser_list_values[] = {"proj", "ellps", "pms", "units", 0} ;	/* Possible values for list.  */

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->detailed_help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->inverse_given = 0 ;
  args_info->revin_given = 0 ;
  args_info->revout_given = 0 ;
  args_info->format_given = 0 ;
  args_info->list_given = 0 ;
  args_info->longproj_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  args_info->inverse_flag = 0;
  args_info->revin_flag = 0;
  args_info->revout_flag = 0;
  args_info->format_arg = NULL;
  args_info->format_orig = NULL;
  args_info->list_arg = list_arg_proj;
  args_info->list_orig = NULL;
  args_info->longproj_arg = NULL;
  args_info->longproj_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{

  init_help_array(); 
  args_info->help_help = gengetopt_args_info_detailed_help[0] ;
  args_info->detailed_help_help = gengetopt_args_info_detailed_help[1] ;
  args_info->version_help = gengetopt_args_info_detailed_help[2] ;
  args_info->inverse_help = gengetopt_args_info_detailed_help[3] ;
  args_info->revin_help = gengetopt_args_info_detailed_help[4] ;
  args_info->revout_help = gengetopt_args_info_detailed_help[5] ;
  args_info->format_help = gengetopt_args_info_detailed_help[6] ;
  args_info->list_help = gengetopt_args_info_detailed_help[8] ;
  args_info->longproj_help = gengetopt_args_info_detailed_help[10] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n", CMDLINE_PARSER_PACKAGE, CMDLINE_PARSER_VERSION);
}

static void print_help_common(void) {
  cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n", gengetopt_args_info_description);
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_print_detailed_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_detailed_help[i])
    printf("%s\n", gengetopt_args_info_detailed_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);

  args_info->inputs = NULL;
  args_info->inputs_num = 0;
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{
  unsigned int i;
  free_string_field (&(args_info->format_arg));
  free_string_field (&(args_info->format_orig));
  free_string_field (&(args_info->list_orig));
  free_string_field (&(args_info->longproj_arg));
  free_string_field (&(args_info->longproj_orig));
  
  
  for (i = 0; i < args_info->inputs_num; ++i)
    free (args_info->inputs [i]);

  if (args_info->inputs_num)
    free (args_info->inputs);

  clear_given (args_info);
}

/**
 * @param val the value to check
 * @param values the possible values
 * @return the index of the matched value:
 * -1 if no value matched,
 * -2 if more than one value has matched
 */
static int
check_possible_values(const char *val, char *values[])
{
  int i, found, last;
  size_t len;

  if (!val)   /* otherwise strlen() crashes below */
    return -1; /* -1 means no argument for the option */

  found = last = 0;

  for (i = 0, len = strlen(val); values[i]; ++i)
    {
      if (strncmp(val, values[i], len) == 0)
        {
          ++found;
          last = i;
          if (strlen(values[i]) == len)
            return i; /* exact macth no need to check more */
        }
    }

  if (found == 1) /* one match: OK */
    return last;

  return (found ? -2 : -1); /* return many values or none matched */
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, char *values[])
{
  int found = -1;
  if (arg) {
    if (values) {
      found = check_possible_values(arg, values);      
    }
    if (found >= 0)
      fprintf(outfile, "%s=\"%s\" # %s\n", opt, arg, values[found]);
    else
      fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->detailed_help_given)
    write_into_file(outfile, "detailed-help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->inverse_given)
    write_into_file(outfile, "inverse", 0, 0 );
  if (args_info->revin_given)
    write_into_file(outfile, "revin", 0, 0 );
  if (args_info->revout_given)
    write_into_file(outfile, "revout", 0, 0 );
  if (args_info->format_given)
    write_into_file(outfile, "format", args_info->format_orig, 0);
  if (args_info->list_given)
    write_into_file(outfile, "list", args_info->list_orig, cmdline_parser_list_values);
  if (args_info->longproj_given)
    write_into_file(outfile, "longproj", args_info->longproj_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = NULL;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char * const *argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, NULL);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, NULL);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  return EXIT_SUCCESS;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, char *possible_values[], const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  if (possible_values && (found = check_possible_values((value ? value : default_value), possible_values)) < 0)
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s' (`-%c')%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt, short_opt,
          (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s'%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt,
          (additional_error ? additional_error : ""));
      return 1; /* failure */
    }
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_ENUM:
    if (val) *((int *)field) = found;
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };


  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
cmdline_parser_internal (int argc, char * const *argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error = 0;
  struct gengetopt_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "detailed-help",	0, NULL, 0 },
        { "version",	0, NULL, 'V' },
        { "inverse",	0, NULL, 'I' },
        { "revin",	0, NULL, 'r' },
        { "revout",	0, NULL, 'R' },
        { "format",	1, NULL, 'f' },
        { "list",	1, NULL, 'l' },
        { "longproj",	2, NULL, 'P' },
        { NULL,	0, NULL, 0 }
      };

      c = getopt_long (argc, argv, "hVIrRf:l:P::", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'I':	/* Inverse projection.  */
        
        
          if (update_arg((void *)&(args_info->inverse_flag), 0, &(args_info->inverse_given),
              &(local_args_info.inverse_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "inverse", 'I',
              additional_error))
            goto failure;
        
          break;
        case 'r':	/* reverse order of input to phi-lam or y-x.  */
        
        
          if (update_arg((void *)&(args_info->revin_flag), 0, &(args_info->revin_given),
              &(local_args_info.revin_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "revin", 'r',
              additional_error))
            goto failure;
        
          break;
        case 'R':	/* reverse order of output to y-x or phi-lam.  */
        
        
          if (update_arg((void *)&(args_info->revout_flag), 0, &(args_info->revout_given),
              &(local_args_info.revout_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "revout", 'R',
              additional_error))
            goto failure;
        
          break;
        case 'f':	/* output format.  */
        
        
          if (update_arg( (void *)&(args_info->format_arg), 
               &(args_info->format_orig), &(args_info->format_given),
              &(local_args_info.format_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "format", 'f',
              additional_error))
            goto failure;
        
          break;
        case 'l':	/* list options.  */
        
        
          if (update_arg( (void *)&(args_info->list_arg), 
               &(args_info->list_orig), &(args_info->list_given),
              &(local_args_info.list_given), optarg, cmdline_parser_list_values, "proj", ARG_ENUM,
              check_ambiguity, override, 0, 0,
              "list", 'l',
              additional_error))
            goto failure;
        
          break;
        case 'P':	/* detail projection list.  */
        
        
          if (update_arg( (void *)&(args_info->longproj_arg), 
               &(args_info->longproj_orig), &(args_info->longproj_given),
              &(local_args_info.longproj_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "longproj", 'P',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          if (strcmp (long_options[option_index].name, "detailed-help") == 0) {
            cmdline_parser_print_detailed_help ();
            cmdline_parser_free (&local_args_info);
            exit (EXIT_SUCCESS);
          }

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */




  cmdline_parser_release (&local_args_info);

  if ( error )
    return (EXIT_FAILURE);

  if (optind < argc)
    {
      int i = 0 ;
      int found_prog_name = 0;
      /* whether program name, i.e., argv[0], is in the remaining args
         (this may happen with some implementations of getopt,
          but surely not with the one included by gengetopt) */

      i = optind;
      while (i < argc)
        if (argv[i++] == argv[0]) {
          found_prog_name = 1;
          break;
        }
      i = 0;

      args_info->inputs_num = argc - optind - found_prog_name;
      args_info->inputs =
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        if (argv[optind++] != argv[0])
          args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind-1]) ;
    }

  return 0;

failure:
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
