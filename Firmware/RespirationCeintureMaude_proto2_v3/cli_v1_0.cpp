/**
 * @file    cli_v1_0.cpp
 * @author  Simon Latour (latoursimon@gmail.com)
 * @brief   The CLI is a simple Command line interpreter that is highly portable.
 * @version 1.0
 * @date 2020-09-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "cli_v1_0.h"

//#include <string.h>
//#include <iostream.h>
//using namespace std; 



//  cli::cli()
//  {

// //     m_verbose = 1;

// //     // Add internal commands to the command list.
// //     cli_commands_func[0] = &cli::cli_cmd_help;
// //     cli_commands_func[1] = &cli::cli_cmd_verbose;
// //     //cli_commands_func[2] = &cli_cmd_echo;
    
// //     strcpy(cli_commands_str[0],"help");
// //     strcpy(cli_commands_str[1],"verbose");

// }


/**
 * @brief Initialisation of the Command Line Interpreter
 * 
 * @param numcommands The number ofcommands
 * @param namesList List of command names
 * @param functionlist List offunction pointers to call when commands are received
 */
void cli::init(int numcommands, const char** namesList, CLI_FUNC_PTR* functionlist, CLI_SERIALOUT_CALLBACK outCallback, int verboseMode  )
{

    cli_commands_func[0] = &cli::cli_cmd_help;
    cli_commands_func[1] = &cli::cli_cmd_verbose;
    
    strcpy(cli_commands_str[0],"help");
    strcpy(cli_commands_str[1],"verbose");

    commandFuncList = functionlist;
    commandList = namesList;
    m_commandCount = numcommands;
    serialOut = outCallback;

    m_verbose = verboseMode;
}

/**
 * @brief This functions starts the CLI and send 
 * 
 * @return int 
 */
int cli::start(){

    if(m_verbose)
    {
        serialOut("\r\n\nSimple Command Line started");
        serialOut("\n\n\t Type \""); 
        serialOut(cli_commands_str[0]);
        serialOut("\" for a list of available command.\r\n");    

        displayCommandPrompt();
    }
    
    

    return ERROR_NO_ERROR;
}

void cli::displayCommandPrompt()
{
   
    serialOut(CLI_COMMAND_PROMPT_LABEL);
}


/**
 * @brief   This function is the principal function of the Command Line interpreter.
 *          It extract the commands and argument from a string and calls the associated function
 * 
 * @param buffer The string containing the command and aguments
 * @return int Status information.  
 *          Negative values are reserved for CLI errors.
 *          User functions can return positive values to communicate status or commands to execute
 */
int cli::run(const char* buffer){

    int status = extractCommand(buffer);

    if (0 < status ) // If no error
    {
        status = executeCommand();
    }
    
    if(0 > status) // If error from extract command or execute command.
    {
        sendErrorStatus(status);
    }

    if(m_verbose)
    {
        displayCommandPrompt();
    }

    return status;
    
}
/**
 * @brief This functions extrats the commands and arguments from a string
 * 
 * @param buffer the input string
 * @return int Status return. number of tokens found or Error code
 */
 int cli::extractCommand(const char* srcBuffer)
 {
    char *token;
    char buffer[LINE_BUF_SIZE];
    int counter = 0;

    strcpy(buffer,srcBuffer);


    token = strtok(buffer, " \n\r\t"); // separated by space, end of line, carriage retun, tab

    while((token != NULL)){
        if(counter < (TOTAL_TOKEN)){
            if(strlen(token) < ARG_BUF_SIZE){

                strcpy(args[counter],token);
                token = strtok(NULL," \n\r\t");
                counter++;
            }
            else{
                return ERROR_STRING_LENGHT;
            }
        }
        else{
            break;
        }
    }

    m_lineTokenCount = counter;
    if(m_lineTokenCount == 0)
    {
        counter = ERROR_COMMAND_UNKNOWN;
    }
    return counter;
    
 }


/**
 * @brief Call the function related to the extracted command
 * 
 * @return int error status (negative value) or called function status
 */
int cli::executeCommand()
{
    int i;

    // Try to match user command
    for(i=0; i<m_commandCount; i++){

        if(strcmp(args[0], commandList[i]) == 0){
            
            return(*commandFuncList[i])(args[1]);
        }
    }

    // Try to match cli command
     for(i=0; i<2; i++){

        if(strcmp(args[0], cli_commands_str[i]) == 0){
            
            int status = (this->*cli_commands_func[i])(args[1]);
            
             return (status);
        }
    }
 
    return ERROR_COMMAND_UNKNOWN;
}

void cli::sendErrorStatus(int status)
{
    char errorName[50];
    if(m_verbose){

         switch(status){

            case ERROR_COMMAND_UNKNOWN:
                //errorName = "\n\t Command Unknown";
               // strcpy(errorName,"\n\t Command Unknown");
            break;

            case ERROR_STRING_LENGHT:
                //errorName = "\n\t Command Lenght Error";
                //strcpy(errorName,"\n\t Command Lenght Error");
            break;

            case ERROR_ARGUMENT_ERROR:
                //errorName = "\n\t Command Argument Error";
                //strcpy(errorName,"\n\t Command Lenght Error");
            break;

            default:
                //errorName ="\n\t Undefined Error";
                //strcpy(errorName,"\n\t Command Lenght Error");
            break;
        }
        //serialOut(errorName.c_str());
        serialOut(errorName);

    }
   
}

//////////////////////////////////////
//  INTERNAL CLI COMMANDS
/////////////////////////////////////
/**
 * @brief 
 * 
 * @param argString 
 * @return int 
 */
int cli::cli_cmd_help(const char* argString){

    serialOut("\n\nUSER COMMANDS"); 
    for(int i=0 ; i < m_commandCount; i++)
    {
        
        serialOut("\n\t-"); 
        serialOut(commandList[i]);
    }

    serialOut("\n\nCLI COMMANDS"); 
    for(int i=0 ; i < INTERNAL_COMMAND_COUNT; i++)
    {
        serialOut("\n\t-"); 
        serialOut(cli_commands_str[i]);
    }

    serialOut("\n\n\r"); 
    return ERROR_NO_ERROR;

}

int cli::cli_cmd_verbose(const char* argString){
    
    if(!strcmp("on",argString))
    {
      serialOut("\n CLI VERBOSE ACTIVATED");
      m_verbose = 1;  
    }else if(!strcmp("off",argString)) 
    {
      m_verbose = 0; 
    }else
    {
        return ERROR_ARGUMENT_ERROR;
    }

    return ERROR_NO_ERROR;
}


/**
 * @brief Set the Verbose Mode object
 * 
 * @param verboseOffOn 
 *  
 */
void cli::setVerboseMode(int verboseOffOn)
{
    if(verboseOffOn == 0 ){
        m_verbose = 0;
    }
    else
    {
        m_verbose = 1;
    }
}