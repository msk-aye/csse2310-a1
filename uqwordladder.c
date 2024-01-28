/* UQ Word Ladder
 * CSSE2310 assignment 1
 * Author: Muhammad Sulaman Khan
 * Student Number: 47511921
*/


// Imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <csse2310a1.h>


// Definitions
#define MAX_STEP_LIMIT 55
#define FROM "--from"
#define END "--end"
#define MAX "--max"
#define DICTIONARY "--dictionary"
#define LENGTH "--len"
#define HELP "?\n"
#define DEFAULT_DICTIONARY "/usr/share/dict/words"
#define MAX_WORD_SIZE 50
#define NEW_LINE "\n"
#define WELCOME_MESSAGE "Welcome to UQWordLadder!\nYour goal is to turn \
\'%s\' into \'%s\' in at most %d steps\n"
#define PROMPT_WORD "Enter word %d (or ? for help):\n"
#define NO_REMAINING_ATTEMPTS "Game over - no more attempts remaining.\n"
#define GAVE_UP "Game over - you gave up.\n"
#define INVALID_WORD_LENGTH "Word must be %d characters long - try again.\n"
#define NON_LETTER "Word contains non-letter characters - try again.\n"
#define ONE_LETTER_DIFFERENT "Word must have only one letter different \
- try again.\n"
#define REPEATED_WORD "You cannot repeat a previous word - try again.\n"
#define WORD_NOT_FOUND "Word can't be found in dictionary - try again.\n"
#define CONGRATULATIONS "Congratulations - you solved the ladder in %d \
steps.\n"
#define SUGGESTIONS "Suggestions:-----------\n"
#define END_SUGGESTIONS "-----End of Suggestions\n"
#define NO_SUGGESTIONS "No suggestions available.\n"
#define USAGE_ERROR "Usage: uqwordladder [--from fromWord] [--end endWord] \
[--max maxSteps] [--len wordLen] [--dictionary dictfilename]\n"
#define INCONSITENT_LENGTHS "uqwordladder: Word length conflict - lengths \
should be consistent\n"
#define INVALID_LENGTH "uqwordladder: Word length should be between 2 and 9 \
(inclusive)\n"
#define INVALID_WORD "uqwordladder: Words must contain only letters\n"
#define SAME_WORDS "uqwordladder: Start and end words must not be the same\n"
#define INVALID_MAX "uqwordladder: Step limit must be word length to 55 \
(inclusive)\n"
#define INVALID_DICT "uqwordladder: Dictionary file named \"%s\" cannot be \
opened\n"


/* Struct for storing all necessary variables of the game. */
typedef struct {
    unsigned int length;
    unsigned int max;

    char* fromWord;
    char* endWord;
    char* currentWord;
    char* previousWord;
    char* dictionaryFileName;

    FILE* dictionary;

    char** previousWords;
    char** validWords;

    bool fromWordGiven;
    bool endWordGiven;
    bool dictionaryFileNameGiven;
    bool help;

    int attempt;
    int previousWordsSize;
    int validWordsSize;
} Data;

/* init_data()
 * ---------------------
 * Initialises the Data struct with values to be later overwritten.
 * 
 * Returns: Initialised Data struct, with all values set to 0, false or NULL.
*/
Data init_data(void) {
    Data data;

    data.length = 0;
    data.max = 0;

    data.fromWord = NULL;
    data.endWord = NULL;
    data.currentWord = NULL;
    data.previousWord = NULL;
    data.dictionaryFileName = NULL;

    data.dictionary = NULL;

    data.previousWords = NULL;
    data.validWords = NULL;

    data.fromWordGiven = false;
    data.endWordGiven = false;
    data.dictionaryFileNameGiven = false;
    data.help = false;

    data.attempt = 1;
    data.previousWordsSize = 0;
    data.validWordsSize = 0;

    return data;
}

/* throw_usage_error()
 * ---------------------
 * Prints the usage error message and then exits the game. No return value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_usage_error(void) {
    fprintf(stderr, USAGE_ERROR);
    exit(7); 
}

/* throw_inconsistent_length_error()
 * ---------------------
 * Prints the inconsistent length error message and then exits the game.
 * No return value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_inconsistent_length_error(void) {
    fprintf(stderr, INCONSITENT_LENGTHS);
    exit(6);
}

/* throw_invalid_length_error()
 * ---------------------
 * Prints the invalid length error message and then exits the game. No return
 * value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_invalid_length_error(void) {
    fprintf(stderr, INVALID_LENGTH);
    exit(18);
}

/* throw_invalid_word_error()
 * ---------------------
 * Prints the invalid given words error message and then exits the game.
 * No return value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_invalid_word_error(void) {
    fprintf(stderr, INVALID_WORD);
    exit(19);
}

/* throw_same_words_error()
 * ---------------------
 * Prints the same words error message and then exits the game. No return
 * value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_same_words_error(void) {
    fprintf(stderr, SAME_WORDS);
    exit(17);
}

/* throw_invalid_max_error()
 * ---------------------
 * Prints the invalid max error message and then exits the game. No return
 * value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_invalid_max_error(void) {
    fprintf(stderr, INVALID_MAX);
    exit(14);
}

/* throw_invalid_dictionary_error()
 * ---------------------
 * Prints the invalid dictionary file error message and then exits the game.
 * No return value.
 * 
 * arg1: Data struct containing all variables needed for the game.
*/
void throw_invalid_dictionary_error(Data data) {    
    fprintf(stderr, INVALID_DICT, data.dictionaryFileName);
    exit(16);
}

/* to_upper()
 * ---------------------
 * Takes an input char* and returns the same char* with every char
 * in upper case.
 * 
 * arg1: Char pointer for word to capitalise.
 * 
 * Returns: Capitalised version of inpur word.
 * Errors: If input word is not terminated by '\0'.
*/
char* to_upper(char* word) {
    int i = 0;
    while (word[i]) {
        word[i] = toupper(word[i]); 
        i++;
    }
    return word;
}

/* is_alpha()
 * ---------------------
 * Takes an input char* and returns true or false depending on whether or not
 * every character in the char* is an alaphabetical character.
 * 
 * arg1: Char pointer for word to check for letters.
 * 
 * Returns: True if the word contains only letters, false otherwise.
 * Errors: If input word is not terminated by '\0'.
*/
bool is_alpha(char* word) {
    int i = 0;
    while (word[i]) { 
        if (!isalpha(word[i])) {
           return false; 
        }
        i++;
    }
    return true;
}

/* one_letter_diff()
 * ---------------------
 * Takes in two char* words and returns true/false depending on 
 * if they are different by only one character.
 * 
 * arg1: Char pointer for first word.
 * arg2: Cahr pointer for second word.
 * 
 * Returns: True if the words are only different by one char, false otherwise.
 * Errors: If eitehr input word is not terminated by '\0'.
*/
bool one_letter_diff(char* word1, char* word2) {
    int charDiff = strlen(word1);

    for (int i = 0; i < strlen(word1); i++) {
        if (word1[i] == word2[i]) {
            charDiff--;
        }
    }

    if (charDiff == 1) {
        return true;
    }

    return false;
}

/* help()
 * ---------------------
 * Prints out the suggested words in the game. No return value.
 * 
 * arg1: Data struct for the variables of the game state.
*/
void help(Data data) {
    char** suggestedWords = calloc(sizeof(char*), 1);
    int suggestedWordsSize = 0;

    if (one_letter_diff(data.endWord, data.previousWord)) {
        // If the word is one away from goal
        suggestedWords[0] = strdup(data.endWord);
        suggestedWordsSize++;

    } else {
        for (int i = 0; i < data.validWordsSize; i++) {
            // Iterating through validwords
            if (one_letter_diff(data.validWords[i], data.previousWord)) {
                // If a valid word is 1 char different to word
                bool add = true;
                for (int j = 0; j < data.previousWordsSize; j++) {
                    // Check if in previous words
                    if (!strcmp(data.validWords[i], data.previousWords[j])) {
                        add = false;
                        break;
                    }
                }

                if (!strcmp(data.validWords[i], data.fromWord)) {
                    continue;
                }

                if (add) {
                    // Add valid word to suggested 
                    suggestedWordsSize++;
                    suggestedWords = realloc(suggestedWords, sizeof(char*) *
                            suggestedWordsSize);
                    suggestedWords[suggestedWordsSize - 1] = strdup(
                            data.validWords[i]);
                }
            }
        }
    }

    if (suggestedWordsSize) {
        printf(SUGGESTIONS);
        for (int i = 0; i < suggestedWordsSize; i++) {
            printf(" %s\n", suggestedWords[i]);
        }
        printf(END_SUGGESTIONS);
    } else {
        printf(NO_SUGGESTIONS);
    }
    free(suggestedWords);
}

/* get_valid_words()
 * ---------------------
 * Gets all valid words from the specified dictionary file and saves them in
 * validWords variable.
 * 
 * arg1: Data struct for the variables of the game state.
 * 
 * Returns: Updated data struct with the validWords.
 * Errors: Errors if data.validWords is not initialised with a malloc function.
*/
Data get_valid_words(Data data) {
    char* tempWord = calloc(sizeof(char), MAX_WORD_SIZE + 1);
    char* word = calloc(sizeof(char), data.length + 1);

    while (fgets(tempWord, sizeof(char*) * MAX_WORD_SIZE, data.dictionary)) {
        // Check each word in dict
        if ((strlen(tempWord) != data.length + 1)) {
            continue;
        }

        memcpy(word, tempWord, sizeof(char) * (data.length + 1));
        word[data.length] = '\0';  // End word off with '\0'
        word = strdup(to_upper(word));

        if (!is_alpha(word)) {
            continue;  // Don't add if word contains non-letters
        }

        // Add word
        data.validWordsSize++;
        data.validWords = realloc(data.validWords,
                sizeof(char*) * (data.validWordsSize));
        data.validWords[data.validWordsSize - 1] =
                calloc(sizeof(char), data.length + 1);
        data.validWords[data.validWordsSize - 1] = strdup(word);
    }

    // Free char*
    free(word);
    free(tempWord);
    return data;
}

/* process_guess()
 * ---------------------
 * Checks the most recent gues made by user and returns true/false depending
 * on the validiity of the word in the current game state. Also handles the
 * printing of messages to inform user whats wrong with the guess.
 * 
 * arg1: Data struct for the variables of the game state.
 * 
 * Returns: True if the guess is valid, false otherwise.
*/
bool process_guess(Data data) {
    if (data.help) {
        // Check if user needs help
        help(data); 
        return false;
    }

    if (strlen(data.currentWord) != data.length ||
            (data.currentWord[data.length] != '\0')) {
        // Check guess length
        printf(INVALID_WORD_LENGTH, data.length);
        return false;
    }

    if (!is_alpha(data.currentWord)) {
        // Check guess contains only letters
        printf(NON_LETTER);
        return false;
    }

    for (int i = 0; i < data.validWordsSize; i++) {
        if (!strcmp(data.currentWord, data.validWords[i])) {
            // Check guess in validWords

            if (!one_letter_diff(data.currentWord, data.previousWord)) {
                // Check guess is 1 letter different prom previousword
                printf(ONE_LETTER_DIFFERENT);
                return false;
            }

            for (int i = 0; i < data.previousWordsSize; i++) {
                // Check if guess has been made before
                if (!strcmp(data.currentWord, data.previousWords[i])) {
                    printf(REPEATED_WORD);
                    return false;
                }
            }

            // Returns true if all above checks are valid
            return true;
        }
    }

    // Reaches here if loop ends and word never matched
    printf(WORD_NOT_FOUND);
    return false;
}

/* play_game()
 * ---------------------
 * Fnction to handle main loop of the game. Prints welcome screen then 
 * continuously prompts use for inputs until game is over. No return.
 * 
 * arg1: Data struct for the variables of the game state.
*/
void play_game(Data data) {
    // Initialise the currentWord and previousWords
    data.currentWord = calloc(sizeof(char), data.length + 1);
    data.previousWord = calloc(sizeof(char), data.length + 1); 
    data.previousWords = calloc(sizeof(char*), data.max + 1);

    // Initialise all words in previousWords
    for (int i = 0; i < data.max + 1; i++) {
        data.previousWords[i] = calloc(sizeof(char), data.length + 1);
    }

    // Set previosWord to fromWord
    data.previousWord = strdup(data.fromWord);
    data.previousWords[0] = strdup(data.fromWord);
    data.previousWordsSize = 1;

    // Initialise variables for getline
    char* input = NULL;
    size_t inputLength = 0;

    // Parse dictionary to obtain all valid words
    data.validWords = calloc(sizeof(char*), 1); 
    data = get_valid_words(data);

    // Print welcome screen
    printf(WELCOME_MESSAGE, data.fromWord, data.endWord, data.max);

    // While game not over
    while(true) {
        if (data.attempt > data.max) {
            // Game over
            printf(NO_REMAINING_ATTEMPTS);
            exit(10);
        }

        data.help = false;

        // Print prompt message
        printf(PROMPT_WORD, data.attempt);

        // Get user input
        if (getline(&input, &inputLength, stdin) == -1) {
            // Exit if user input is Ctrl + D
            free(input);
            printf(GAVE_UP);
            exit(9);
        }

        // If user input is "?"
        if (!strcmp(input, HELP)) {
            data.help = true;
        }

        // Copy user input to currentWord
        data.currentWord = strdup(input);
        data.currentWord[data.length] = '\0';
        data.currentWord = to_upper(data.currentWord);

        // Check if currentWord is valid
        if (!process_guess(data)) {
            continue;
        }

        // Add currentWord to previousWord if valid
        data.previousWordsSize++;
        data.previousWords = realloc(data.previousWords,
            sizeof(char*) * data.previousWordsSize);
        data.previousWords[data.previousWordsSize - 1] =
                strdup(data.currentWord);
        data.previousWord = data.currentWord;
        data.attempt++;

        // Check if currentWord is the endWord
        if (!strcmp(data.currentWord, data.endWord)) {
            // Exit game if user has won
            printf(CONGRATULATIONS, data.attempt - 1);
            exit(0);
        }
    }
}

int main(int argc, char** argv) {
    // Initialise data struct
    Data data = init_data();

    // Check if there is correct number of arguments
    if (argc % 2 != 1) {
        throw_usage_error();
    }

    // Usage error checking and storing max and length
    for (int i = 1; i < argc; i += 2) {
        if (!strcmp(argv[i], LENGTH)) {
            if (!data.length) {
                // Checking if length is already defined
                data.length = atol(argv[i + 1]);
            } else {
                // Length is specified multiple times
                throw_usage_error();
            }
            if ((int)data.length <= 0) {
                // Length argument invalid
                throw_usage_error();
            }
        } else if (!strcmp(argv[i], MAX)) {
            if (!data.max) {
                // Checking if max is already defined
                data.max = atol(argv[i + 1]);
            } else {
                // Max is specified multiple times
                throw_usage_error();
            }
            if (data.max <= 0) {
                // Max argument invalid
                throw_usage_error();
            }
        } else if (!strcmp(argv[i], FROM)) {
            if (data.fromWordGiven) {
                throw_usage_error();
            }
            data.fromWordGiven = true;
        } else if (!strcmp(argv[i], END)) {
            if (data.endWordGiven) {
                throw_usage_error();
            }
            data.endWordGiven = true;
        } else if (!strcmp(argv[i], DICTIONARY)) {
            if (data.dictionaryFileNameGiven) {
                throw_usage_error();
            }
            data.dictionaryFileNameGiven = true;
        } else {
            // Command is unknown (i.e not any of the above)
            throw_usage_error();
        }
    }



    for (int i = 1; i < argc; i += 2) {
        if (!strcmp(argv[i], FROM)) {
            if (data.length) {
                // If length already defined then check consistency
                if (strlen(argv[i + 1]) != data.length) {
                    throw_inconsistent_length_error();  
                }
                data.fromWord = calloc(sizeof(char), data.length + 1);
                data.fromWord = argv[i + 1];
                data.fromWord = to_upper(data.fromWord);

            } else {
                // If length not defined then store length of word
                data.length = strlen(argv[i + 1]);
                data.fromWord = calloc(sizeof(char), data.length + 1);
                data.fromWord = argv[i + 1];
                data.fromWord = to_upper(data.fromWord);
            }

        } else if (!strcmp(argv[i], END)) {
            // If length already defined then check consistency
            if (data.length) {
                if (strlen(argv[i + 1]) != data.length) {
                    throw_inconsistent_length_error();  
                }
                data.endWord = argv[i + 1];
                data.endWord = to_upper(data.endWord);

            } else {
                // If length not defined then store length of word
                data.length = strlen(argv[i + 1]);
                data.endWord = calloc(sizeof(char), data.length + 1);
                data.endWord = argv[i + 1];
                data.endWord = to_upper(data.endWord);
            }

        } else if (!strcmp(argv[i], DICTIONARY)) {
            data.dictionaryFileName = calloc(sizeof(char),
                    (strlen(argv[i]) + 1));
            data.dictionaryFileName = argv[i + 1];
            data.dictionaryFileNameGiven = true;
        }
    }

    // Validity checking
    // Length check
    if ((data.length > 0 && data.length < 2) || data.length > 9) {
        throw_invalid_length_error();
    }

    // Check words are valid
    if ((data.fromWord && !is_alpha(data.fromWord)) ||
            (data.endWord && !is_alpha(data.endWord))) {
        throw_invalid_word_error();
    }

    // Check if start and end words are the same
    if (data.fromWord && data.endWord &&
            !(strcmp(data.fromWord, data.endWord))) {
        throw_same_words_error();
    }

    // Default length
    // set defualts as function
    if (!data.length) {
        data.length = 4;
    }

    // Max check
    if (data.max && data.length && (data.max < data.length ||
            data.max > MAX_STEP_LIMIT)) {
        throw_invalid_max_error();
    }

    // Check dict
    if (data.dictionaryFileNameGiven) {
        data.dictionary = fopen(data.dictionaryFileName, "r");
        if (!data.dictionary) {
            throw_invalid_dictionary_error(data);
        }
    } else { 
        data.dictionary = fopen(DEFAULT_DICTIONARY, "r");
    }

    // Checking max
    if (!data.max) {
        data.max = 20;
    }

    // Get from and end words if unspecified
    if (!data.fromWord) {
        const char* tempFromWord = get_uqwordladder_word(data.length);
        data.fromWord = calloc(sizeof(char), data.length + 1);
        strcpy(data.fromWord, tempFromWord);
    }

    if (!data.endWord) {
        const char* tempEndWord = get_uqwordladder_word(data.length);
        data.endWord = calloc(sizeof(char), data.length + 1);
        strcpy(data.endWord, tempEndWord);
    }

    // Call play_game
    play_game(data);

    return 0;
}

