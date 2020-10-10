#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "tmachine.h"

int main (int argc, char *argv[])
{
  if (argc != 3)
    {
      fprintf (stderr, "Using: %s <command file> <input>\n", argv[0]);
      return 1;
    }

  FILE *fp;
  if (!(fp = fopen (argv[1], "r")))
    {
      fprintf (stderr, "Cannot open %s!\n", argv[1]);
      return 1;
    }

  tmachine TM;

  if (!TM.read_commands_from_file (fp))
    {
      fprintf (stderr, "Cannot read %s!\n", argv[1]);
      fclose (fp);
      return 1;
    }
  fclose (fp);

  std::string input_data = argv[2];
  TM.set_input (input_data);

  //TM.print_commands ();

  bool run_step_by_step = true;
  TM.run (run_step_by_step);

  return 0;
}
