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
  deleteCommandList();
  s_Instance = NULL;
}

void CommandInterpreter::addCommand(std::string commandWord, commandFunc commandFunction)
{
  commandList_t *ptr;
  commandList_t *entry = new commandList_t(commandWord, commandFunction);

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

void CommandInterpreter::deleteCommand(std::string commandWord, commandFunc commandFunction)
{
  commandList_t *ptr;

  if (m_CommandList == NULL)
  {
    return;
  }
  else
  {
    ptr = m_CommandList;
    // First handle the beginning elements of the list
    while (ptr->command.compare(commandWord) == 0 && ptr->func == commandFunction)
    {
      m_CommandList = ptr->next;
      delete(ptr);
      ptr = m_CommandList;
      if (m_CommandList == NULL)
        return;
    }
    // Then all others
    while(ptr->next != NULL)
    {
      if (ptr->next->command.compare(commandWord) == 0 && ptr->next->func == commandFunction)
      {
        commandList_t *tmp = ptr->next->next;
        delete(ptr->next);
        ptr->next = tmp;
        // Will happen when removing end of list
        if (ptr->next == NULL)
          return;
      }
      ptr = ptr->next;
    }
  }
}

void CommandInterpreter::deleteCommandList()
{
  commandList_t *ptr = m_CommandList;
  commandList_t *next;

  while(ptr != NULL)
  {
    next = ptr->next;
    delete(ptr);
    ptr = next;
  }
}

void CommandInterpreter::interprete(std::string &s)
{

  std::string commandWord = getCommandWord(s);
  commandList_t *ptr = m_CommandList;

  while(ptr != NULL)
  {
    if (commandWord.compare(ptr->command) == 0)
    {
      ptr->func(s);
    }
    ptr = ptr->next;
  }
}

std::string CommandInterpreter::getCommandWord(std::string &s)
{
  String arduinoS(s.c_str());
  const char token = ' ';

  // unfortunatly string::find does not compile/link under arduino
  // workaround: use arduino String and indexOf
  int32_t pos = arduinoS.indexOf(token);
  if (pos >= 0)
  {
    return s.substr(0, pos);
  }
  // if no token is present. take wohle input
  return s;
}

bool CommandInterpreter::getArgument(std::string &s, std::string &arg, uint8_t argNum)
{
  String arduinoS(s.c_str());
  const char token = ' ';

  int32_t posBefore = 0, posBehind = 0;

  for (uint8_t i = 0; i <= argNum; i++)
  {
    posBefore = arduinoS.indexOf(token, posBehind);
    if (posBefore == -1)
      return false;
    posBehind = arduinoS.indexOf(token, posBefore + 1);
    if (posBehind == -1)
      posBehind = arduinoS.length();
  }

  arg = s.substr(posBefore + 1, posBehind - posBefore - 1);

  return true;
}

uint8_t CommandInterpreter::getNumArgument(std::string &s)
{
  String arduinoS(s.c_str());
  const char token = ' ';
  int8_t pos = 0, oldPos = 0;
  uint8_t args = 0;

  while(1)
  {
    pos = arduinoS.indexOf(token, oldPos);
    if (pos == -1)
      return args;

    args++;
    oldPos = pos + 1;
  }
}
