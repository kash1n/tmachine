#ifndef TMACHINE_H
#define TMACHINE_H

#include <string>
#include <vector>
#include <stdio.h>

#define HALT_STATE "halt"

enum class direction
{
  left,
  right,
  stay
};

struct command_t
{
  std::string state_before;
  char symbol_before;
  char symbol_after;
  direction dir;
  std::string state_after;
};

class tmachine
{
public:
  tmachine (std::string initial_state = "0") { m_current_state = initial_state; }
  ~tmachine () {}

  void run (bool step_by_step);
  void print_commands ();

  void add_command (const command_t command);
  bool read_commands_from_file (FILE *fp);
  void set_input (const std::string input_data);

private:
  bool find_command (command_t &found_comm);
  void move (const direction dir);

  std::vector<command_t> m_commands;
  std::string m_input;

  std::string m_strip = "_";

  std::string m_current_state = "0";
  int m_current_pos = 0;

  int m_total_steps = 0;
};

std::vector<std::string> split_string_by_spaces (const std::string &str);

#endif
