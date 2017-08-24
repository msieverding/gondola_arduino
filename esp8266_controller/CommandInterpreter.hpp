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
   * Structure for a single ended list of all registered commands
   */
  typedef struct commandList_s {
    /**
     * Constructor for struct element
     * @param s  command to use
     * @param cf function which belongs to this command
     */
    commandList_s(std::string &s, commandFunc cf) : command(s), func(cf), next(NULL) {}
    std::string command;    //!< Command to react on
    commandFunc func;       //!< function to call when command appears
    commandList_s *next;    //!< next element in list
  } commandList_t;

  /**
   * Get instance of CommandInterpreter
   * @return pointer to the instance
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
   * delete a command from the command interpreter
   * @param commandWord     command to react on
   * @param commandFunction function to call when commandWord was found
   */
  void deleteCommand(std::string commandWord, commandFunc commandFunction);

  /**
   * interprete a string s
   * When a registered command word appears, the given callback is called
   * by this interpreter
   * @param s input string to interprete
   */
  void interprete(std::string &s);

  /**
   * Get the commandWord of a string s
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
  static CommandInterpreter  *s_Instance;         //!< Instance of singleton

  // membervariables
  commandList_t              *m_CommandList;      //!< List of all registered Commands
};

#endif /* _COMMAND_INTERPRETER_HPP_ */
