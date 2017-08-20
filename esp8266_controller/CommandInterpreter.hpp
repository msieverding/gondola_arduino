#ifndef _COMMAND_INTERPRETER_HPP_
#define _COMMAND_INTERPRETER_HPP_

#include <string>

/**
 * An interpreter for commands from the serial input
 */
class CommandInterpreter
{
public:
  /**
   * Type of a command function
   */
  typedef void (*commandFunc)(std::string&);
  /**
   * Structure for all registered commands
   * @param s  commandWord to react on
   * @param cf function to call when commandWord is found
   */
  typedef struct commandList_s {
    commandList_s(std::string &s, commandFunc cf) : command(s), func(cf), next(NULL) {}
    std::string command;
    commandFunc func;
    commandList_s *next;
  } commandList_t;

  /**
   * Get instance of CommandInterpreter
   * @return pointer to instance
   */
  static CommandInterpreter *get();
  /**
   * virtual destructor
   */
  virtual ~CommandInterpreter();

  /**
   * add a command to the interpreter
   * @param commandWord     command to react on
   * @param commandFunction function to call when commandWord was found
   */
  void addCommand(std::string commandWord, commandFunc commandFunction);
  /**
   * delete a command from the interpreter
   * @param commandWord     command to react on
   * @param commandFunction function to call when commandWord was found
   */
  void deleteCommand(std::string commandWord, commandFunc commandFunction);

  /**
   * interprete the string s and search for a valid command
   * @param s input string to interprete
   */
  void interprete(std::string &s);
  /**
   * get the commandword of a string
   * @param  s string to interprete
   * @return   command of string s
   */
  static std::string getCommandWord(std::string &s);
  /**
   * Get an argument of an input s
   * @param  s      input to use
   * @param  arg    (output) argument content
   * @param  argNum argument number
   * @return        success
   */
  static bool getArgument(std::string &s, std::string &arg, uint8_t argNum);
  /**
   * Get the number of arguments
   * @param  s input to use
   * @return   number of arguments of s. (CommandWord is not counted as argument)
   */
  static uint8_t getNumArgument(std::string &s);

  /**
   * Print all registered commands to the serial
   */
  void printAllCommands(void);

private:
  /**
   * private constructor
   */
  CommandInterpreter();
  /**
  * delete the whole command list
  */
  void deleteCommandList();

  // instance
  static CommandInterpreter  *s_Instance;

  // membervariables
  commandList_t              *m_CommandList;
};

#endif /* _COMMAND_INTERPRETER_HPP_ */
