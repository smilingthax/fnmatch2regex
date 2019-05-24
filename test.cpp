#include "fnmatch2regex.h"
#include <regex>
#include <stdio.h>

// #define SYSTEM_FNMATCH

#ifdef SYSTEM_FNMATCH
#include <fnmatch.h>
#endif

static void test(const char *pat, const char *str, bool expected)
{
  std::string restr = fnmatch2regex(pat, FNF_PERIOD|FNF_PATHNAME);

  printf("/%s/  ->  /%s/\n", pat, restr.c_str());

  std::regex re(restr, std::regex_constants::nosubs);
  bool res = std::regex_search(str, re);
#ifdef SYSTEM_FNMATCH
  bool fnm = (fnmatch(pat, str, FNM_PERIOD|FNM_PATHNAME) == 0);
  if (res != fnm) {
    printf("  \"%s\": %d (%d %d)\n\n", str, res, expected, fnm);
  }
#else
  if (res != expected) {
    printf("  \"%s\": %d (%d)\n\n", str, res, expected);
  }
#endif
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s testfile\n", argv[0]);
    return 1;
  }

  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Could not open file\n");
    return 2;
  }

  char *line = NULL;
  size_t maxlen = 0;
  while (1) {
    ssize_t res = getline(&line, &maxlen, f);
    if (res<0) {
      break;
    }
    if (line[res-1]=='\n') {
      res--;
      line[res]=0;
    }
    if (!*line || *line=='#') {
//      printf("%s\n", line);
    } else {
      char *t1=strchr(line, '\t'), *t2=0;
      if (t1) {
        t2=strchr(t1+1, '\t');
      }
      if (!t2) {
        fprintf(stderr, "Bad file\n");
        break;
      }
      *t1=0;
      *t2=0;
      test(line, t1+1, (t2[1]=='1') ? true : false);
   }
  }
  free(line);
  fclose(f);

  return 0;
}
