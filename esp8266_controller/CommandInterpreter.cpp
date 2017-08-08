#include "CommandInterpreter.hpp"
#include <Arduino.h>

CommandInterpreter *CommandInterpreter::s_Instance = NULL;

CommandInterpreter *CommandInterpreter::get()
{
  if (!s_Instance)
  {
    s_Instance = new CommandInterpreter();
  }
  return s_Instance;
}

CommandInterpreter::CommandInterpreter()
 : m_CommandList(NULL)
{

}

CommandInterpreter::~CommandInterpreter()
{

}

void CommandInterpreter::addCommand(std::string s, commandFunc cf)
{
  commandList_t *ptr;
  commandList_t *entry = new commandList_t(s, cf);

  if (m_CommandList == NULL)
  {
    m_CommandList = entry;
  }
  else
  {
    while(ptr->next != NULL)
      ptr = ptr->next;

    ptr->next = entry;
  }
}

void CommandInterpreter::interprete(std::string &s)
{
  commandList_t *ptr = m_CommandList;
  std::string commandWord(s.substr(0, s.find(" ")));

  while(ptr != NULL)
  {
    if (commandWord.compare(ptr->command) == 0)
    {
      ptr->func(s);
    }
    ptr = ptr->next;
  }
}
