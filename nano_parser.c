#include "nano_parser.h"

/* Number of allowed tokens to be registered in the system*/
#define MAX_NUM_TOKENS 10

/* Number of characters each token can store */
#define MAX_TOKEN_SIZE 10

/* Number of digits the input number can receive */
#define MAX_NUMBER_SIZE 4

/* ASCII decimal of the delimiter token */
#define DELIMITER_TOKEN 13


/* Store the tokens and callback functions to process them.
   When these tokens are found, the callback functions are called */
typedef struct commands{
    //array of void function pointers
    void (*function[MAX_NUM_TOKENS]) (int argv);
    
    //array of tokens to compare
    char tokens[MAX_NUM_TOKENS][MAX_TOKEN_SIZE];
} Commands;


static Commands commands;


/* Index where to add new tokens and callbacks on the Commands structs */
int empty_index = 0;


/*
 * Receives a string, extracts command token and number parameter and
 * tries to match with already registered commands.
 *
 */
void process_input(const char* buffer, int buffer_length)
{                       
    //array of temp_token to compare to stored tokens
    char temp_token[MAX_TOKEN_SIZE];
    
    //array of digits to calc the actual number parameter 
    char number_array[MAX_NUMBER_SIZE];
    
    //index to store the next digit found in the buffer
    int number_index = 0;
    
    //actual number to be calculed, and passed to the command callback
    int number = -1;
    
    /*Stops if the buffer to be parsed is bigger than allowed
      * MAX_TOKEN_SIZE + MAX_NUMBER_SIZE + DELIMITER_TOKEN(CHAR)
      */
    if(buffer_length > (MAX_TOKEN_SIZE + MAX_NUMBER_SIZE + 1)){
        return;
    }
    
    
    //Clear temporary tokens
    int i,j = 0;
    for(i=0;i<MAX_TOKEN_SIZE;i++){
        temp_token[i] = 0;
    }
    
    for(j=0;j<MAX_NUMBER_SIZE;j++){
        number_array[j] = 0;
    }    
        
    //Start parsing buffer
    for(j=0;j<buffer_length;j++){  

        //Enqueue letter until MAX_TOKEN_SIZE characters processed
        if(j<MAX_TOKEN_SIZE){    
                    
            //65 - 90 ascii (A-Z)
            if( (buffer[j] >= 65) && (buffer[j] <= 90) ){
                temp_token[j] = buffer[j];
            }
            
            //97 - 122 ascii (a-z)
            if( (buffer[j] >= 97) && (buffer[j] <= 122) ){
                temp_token[j] = buffer[j];            
            }                
        }
                
        //Enqueueing numbers
        //48 - 57 ascii (0-9)        
        if( (buffer[j] >= 48) && (buffer[j] <= 57) ){
            if(number_index < MAX_NUMBER_SIZE){
                number_array[number_index]=buffer[j];
                number_index++;
            }                        
        }                
        
        //Delimiter_found
        if(buffer[j] == DELIMITER_TOKEN){
            int k,l = 0;
                        
            //If it have digits, we need to represent as
            //an integer to pass to the callback function
            if(number_index>0){
                number = 0;            
                //Transform enqueued digits list in an integer
                for(k=0;k<number_index;k++){
                    int multiplier = 1;
                    for(l=number_index;l>(k+1);l--){
                        multiplier *= 10;
                    }                
                    number += multiplier* (number_array[k]-48);
                }   
            }
            
            
            /*Try to match the registered tokens with the input*/
            for(k=0;k<empty_index;k++){                
                int matched = 1;                
                int end_index = -1;
                for(l=0;l<MAX_TOKEN_SIZE;l++){
                    if(commands.tokens[k][l]!= 0){
                        if(commands.tokens[k][l]!=temp_token[l]){
                            matched = 0;
                        }
                    }else{
                        /* Store the end of the matched stored token
                        */
                        if(end_index==-1){
                            end_index = l;
                        }
                    }
                }
                
                /* Discards if registered token is only a subset of the input
                */
                if(end_index<MAX_TOKEN_SIZE){
                    if(temp_token[end_index] != 0){
                        matched = 0;
                    }
                }
                                
                if(matched){
                   (*commands.function[k])(number);
                   return;
                }
            }
                                   
        }
       
    }
    
    
};

/*
 * Register commands is a function that stores tokens and function callbacks in
 * a command list to parse and execute commands received in strings,
 * usually from a serial terminal console.
 * 
 */
int register_command(const char* token, int token_length, void (*function_address)(int))
{
        /* If the list has space*/
        if(empty_index < MAX_NUM_TOKENS){
            commands.function[empty_index] = function_address;
            
            //clear token 
            int i,j = 0;
            for(i=0;i<MAX_TOKEN_SIZE;i++){
                commands.tokens[empty_index][i] = 0;                
            }

            for(j=0;j<token_length;j++){
                if(j<MAX_TOKEN_SIZE)
                    commands.tokens[empty_index][j] = token[j];                
            }                        
            
            empty_index++;
            return 0;
        }else{
            return -1;
        }
        

};
