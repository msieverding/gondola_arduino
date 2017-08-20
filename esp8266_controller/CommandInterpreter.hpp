#ifndef _COMMAND_INTERPRETER_HPP_
#define _COMMAND_INTERPRETER_HPP_

#include <string>

class CommandInterpreter
{
public:
  typedef void (*commandFunc)(std::string&);
  typedef struct commandList_s {
    commandList_s(std::string &s, commandFunc cf) : command(s), func(cf), next(NULL) {}
    std::string command;
    commandFunc func;
    commandList_s *next;
  } commandList_t;

  static CommandInterpreter *get();
  virtual ~CommandInterpreter();

  void addCommand(std::string s, commandFunc cf);
  // void deleteCommand(std::string s);

  void interprete(std::string &s);
  std::string getCommandWord(std::string &s);
  bool getArgument(std::string &s, std::string &arg, uint8_t argNum);
  uint8_t getNumArgument(std::string &s);

private:
  CommandInterpreter();

  static CommandInterpreter  *s_Instance;
  commandList_t              *m_CommandList;
};

#endif /* _COMMAND_INTERPRETER_HPP_ */
