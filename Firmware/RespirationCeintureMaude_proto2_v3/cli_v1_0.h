/**
 * @file    cli_v1_0.h
 * @author  Simon Latour (latoursimon@gmail.com)
 * @brief   The CLI is a simple Command line interpreter that is highly portable.
 * @version 1.0
 * @date 2020-09-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CLI_H
#define CLI_H

#include <string.h>
// For internal commands only
#define INTERNAL_COMMAND_COUNT      2
#define  INTERNAL_COMMAND_MAX_SIZE  10

#define LINE_BUF_SIZE 128   //Maximum input string length
#define ARG_BUF_SIZE  50    //Maximum argument string length
#define MAX_NUM_ARGS  1
#define TOTAL_TOKEN  1 + MAX_NUM_ARGS      

// CLI ERRORSAND STATUS
#define ERROR_NO_ERROR         0
#define ERROR_STRING_LENGHT   -1
#define ERROR_COMMAND_UNKNOWN -2
#define ERROR_ARGUMENT_ERROR  -3


typedef int (*CLI_FUNC_PTR)(const char*);
typedef void (*CLI_SERIALOUT_CALLBACK)(const char*);

#define CLI_COMMAND_PROMPT_LABEL "\r\nEnterCommand:>"

class cli 
{
  private:
    
    typedef int (cli::*CLI_INTERNAL_FUNC_PTR)(const char*);

    char args[TOTAL_TOKEN][ARG_BUF_SIZE];
    const char** commandList;

    CLI_FUNC_PTR* commandFuncList;
    CLI_SERIALOUT_CALLBACK serialOut;


    int m_commandCount;
    int m_lineTokenCount;
    int m_verbose;

    int extractCommand(const char* buffer);
    int executeCommand();
    void sendErrorStatus(int status);

    // Internal CLI commands
    int cli_cmd_help(const char* argString);
    int cli_cmd_verbose(const char* argString);
    int cli_cmd_echo(const char* argString);

    CLI_INTERNAL_FUNC_PTR cli_commands_func[2];


    char cli_commands_str[INTERNAL_COMMAND_COUNT][INTERNAL_COMMAND_MAX_SIZE];


  public:
        
    
    //cli();

    void init(int numcommands, const char** namesList, CLI_FUNC_PTR* functionlist, CLI_SERIALOUT_CALLBACK outCallback, int verboseMode );   

    int start();
    int run(const char* buffer);

  #define VERBOSE_OFF 0
  #define VERBOSE_ON 1
  /**
   * @brief Set the Verbose Mode object
   * 
   * @param verboseOffOn  
   */
    void setVerboseMode(int verboseOffOn);

    void displayCommandPrompt();
};



#endif

