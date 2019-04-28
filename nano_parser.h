#ifndef __NANO_PARSER_H__
#define __NANO_PARSER_H__

/*
 * Register commands is a function that stores tokens and callbacks in
 * a command list to parse and execute commands received in strings,
 * usually from a serial terminal console.
 * 
 *
 * Input Parameters
 * token: a string with less or equal MAX_TOKEN_SIZE characters to be 
 * recognized
 *
 * function_address: a function with void return type and one integer argument
 * in the signature
 *
 * Output Parameters
 * if there is no space left for registering new commands, -1 is returned.
 */
int register_command(const char* token, int token_length, void (*function_address)(int) );

/*
 * Receives a string, extract the command token, the number parameter and
 * tries to match with already registered commands.
 *
 * Format of accetable input string:
 * command number DELIMITER_TOKEN
 *
 * The process_input function is case sensitive
 * (command: ascii letters)
 * (number:integers with MAX_NUMBER SIZE digits [optional])
 * (char:DELIMITER_TOKEN)
 *
 */
void process_input(char* buffer, int buffer_length);


#endif /* __NANO_PARSER_H__ */
