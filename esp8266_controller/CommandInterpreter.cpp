#include "CommandInterpreter.hpp"
#include <Arduino.h>
#include <string.h>

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
    ptr = m_CommandList;
    while(ptr->next != NULL)
      ptr = ptr->next;

    ptr->next = entry;
  }
}

void CommandInterpreter::interprete(std::string &s)
{
  String arduinoS(s.c_str());
  commandList_t *ptr = m_CommandList;
  const char tokens[] = {' ', '\r', '\0', '\n'};
  bool found = false;
  std::string commandWord;
  int32_t pos;

  for (uint8_t i = 0; i < sizeof(tokens) / sizeof(tokens[0]) && std::string::npos; i++)
  {
    // unfortunatly string::find does not compile/link under arduino
    // workaround: use arduino String and indexOf
    pos = arduinoS.indexOf(tokens[i]);
    if (pos >= 0)
    {
      commandWord = s.substr(0, pos);
      found = true;
      break;
    }
  }
  if (!found)     // if no token is present. take wohle input
    commandWord = s;

  while(ptr != NULL)
  {
    if (commandWord.compare(ptr->command) == 0)
    {
      ptr->func(s);
    }
    ptr = ptr->next;
  }
}
