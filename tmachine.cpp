#include "tmachine.h"

void tmachine::add_command (const command_t command)
{
  m_commands.push_back (command);
}

bool tmachine::read_commands_from_file (FILE *fp)
{
  char buf[1024];
  while (fgets (buf, 1024, fp))
    {
      command_t new_command;

      std::string buf_str (buf);
      if (buf_str.empty ())
        continue;
      if (buf_str[0] == '\n')
        continue;
      if (buf_str[0] == ';')
        continue;

      std::vector<std::string> tokens = split_string_by_spaces (buf_str);
      if (tokens.size () < 5)
        {
          printf ("Cannot parse command \'%s\'!\n", buf);
          return false;
        }

      new_command.state_before = tokens[0];
      if (tokens[1].size () > 1)
        {
          printf ("Wrong input symbol in command \'%s\'!\n", buf);
          return false;
        }
      new_command.symbol_before = tokens[1][0];
      if (tokens[2].size () > 1)
        {
          printf ("Wrong output symbol in command \'%s\'!\n", buf);
          return false;
        }
      new_command.symbol_after = tokens[2][0];
      if (tokens[3].size () > 1)
        {
          printf ("Wrong direction in command \'%s\'!\n", buf);
          return false;
        }
      char dirch = tokens[3][0];
      if (dirch == '*')
        new_command.dir = direction::stay;
      else if (dirch == 'l')
        new_command.dir = direction::left;
      else if (dirch == 'r')
        new_command.dir = direction::right;
      else
        {
          printf ("Wrong direction in command %s!\n", buf);
          return false;
        }
      auto space_pos = tokens[4].find_first_of (" ;\n");
      new_command.state_after = tokens[4].substr (0, space_pos);;

      m_commands.push_back (new_command);
    }
  return true;
}

void tmachine::set_input (const std::string input_data)
{
  m_input = input_data;
  m_strip = input_data;
  m_current_pos = 0;
}

void tmachine::run (bool step_by_step)
{
  while (true)
    {
      printf ("\nCurrent state: %s\n", m_current_state.c_str ());
      printf ("Step: %d\n", m_total_steps);
      for (int i = 0; i < (int) m_strip.size (); i++)
        printf ("_");
      printf ("\n");
      printf ("%s\n", m_strip.c_str ());
      for (int i = 0; i < m_current_pos; i++)
        printf (" ");
      printf ("^\n");

      if (m_current_state == HALT_STATE)
        break;
      command_t curr_command;
      if (!find_command (curr_command))
        {
          printf ("TM : cannot find command for state \'%s\' and symbol \'%c\'!\n", m_current_state.c_str (), m_strip[m_current_pos]);
          break;
        }
      if (curr_command.symbol_after != '*')
        m_strip[m_current_pos] = curr_command.symbol_after;
      m_current_state = curr_command.state_after;
      move (curr_command.dir);
      m_total_steps++;

      if (step_by_step)
        getc (stdin);
    }
}

bool tmachine::find_command (command_t &found_comm)
{
  for (const command_t &comm : m_commands)
    {
      if (comm.state_before == m_current_state
          && comm.symbol_before == m_strip[m_current_pos])
        {
          found_comm = comm;
          return true;
        }
    }
  for (const command_t &comm : m_commands)
    {
      if (comm.state_before == m_current_state
          && comm.symbol_before == '*')
        {
          found_comm = comm;
          return true;
        }
    }
  return false;
}

void tmachine::move (const direction dir)
{
  switch (dir)
    {
    case direction::stay:
      return;
    case direction::right:
      {
        if (m_current_pos == static_cast<int> (m_strip.size () - 1))
          m_strip.push_back ('_');
        m_current_pos++;
        return;
      }
    case direction::left:
      {
        if (m_current_pos == 0)
          m_strip.insert (0, "_");
        else
          m_current_pos--;
        return;
      }
    }
}

void tmachine::print_commands ()
{
  printf ("My commands:\n");
  for (const command_t &comm : m_commands)
    {
      char dirch = '-';
      if (comm.dir == direction::stay)
        dirch = '*';
      else if (comm.dir == direction::left)
        dirch = 'l';
      else if (comm.dir == direction::right)
        dirch = 'r';
      printf ("%s %c %c %c %s\n", comm.state_before.c_str (),
                                  comm.symbol_before,
                                  comm.symbol_after,
                                  dirch,
                                  comm.state_after.c_str ());
    }
}

std::vector<std::string> split_string_by_spaces (const std::string &str)
{
  std::vector<std::string> all_words;
  int str_size = str.size ();
  std::string word;
  for (int i = 0; i < str_size; i++)
    {
      char ch = str[i];
      if (ch == ' ')
        {
          if (!word.empty ())
            {
              all_words.push_back (word);
              word.clear ();
            }
        }
      else
        word.push_back (ch);
    }
  if (!word.empty ())
    all_words.push_back (word);
  return all_words;
}

