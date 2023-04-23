#include <iostream>
#include <string>
#include <cctype>
#include <bits/stdc++.h>
#include <pthread.h>

#define UNLOCKED false
#define LOCKED true

std::string input_phrase;

bool bit[2] = {UNLOCKED, LOCKED};

std::stringstream read;
std::string word;

void *alpha(void *);
void *numeric(void *);

/**
 * @brief This function is the numeric thread. It will print out all words that start with a digit.
 */
void *numeric(void *arg)
{
    // while there are still words to read
    while (read)
    {
        // if the word starts with a digit
        if (isdigit(word[0]))
        {
            // print the numeric
            std::cout << "numeric: " << word << std::endl;
            // lock the numeric thread
            bit[1] = LOCKED;
        }
        else
        {
            // otherwise, unlock the alpha thread
            bit[0] = UNLOCKED;
            // and unlock the numeric thread
            bit[1] = UNLOCKED;
            // if the alpha thread finishes reading the input_phrase
            if (!read)
            {
                // stop here
                break;
            }
            // while alpha thread is unlocked
            while (bit[0] == UNLOCKED)
            {
                // wait for it to finish
                continue;
            }
        }
        // if numeric thread is locked
        if (bit[1] == LOCKED)
        {
            // read the next word in the input_phrase
            read >> word;
        }
    }
    // then unlock the alpha thread
    bit[0] = UNLOCKED;
    // exit the thread
    pthread_exit(0);
}

/**
 * @brief This function is the alpha thread. It will print out all words that start with an alphabet.
 */
void *alpha(void *arg)
{
    // while there are still words to read
    while (read)
    {
        // if the word starts with an alphabet character
        if (isalpha(word[0]))
        {
            // print the word
            std::cout << "alpha: " << word << std::endl;
            // lock the alpha thread
            bit[1] = LOCKED;
        }
        else
        {
            // otherwise, lock the numeric thread
            bit[0] = LOCKED;
            // and unlock the alpha thread
            bit[1] = UNLOCKED;
            // if the numeric thread finishes reading the input_phrase
            if (!read)
            {
                // stop here
                break;
            }
            // while the numeric thread is locked
            while (bit[0] == LOCKED)
            {
                // wait for it to finish
                continue;
            }
        }
        // if the numeric thread is locked
        if (bit[1] == LOCKED)
        {
            // read the next word in the input_phrase
            read >> word;
        }
    }
    // lock the numeric thread
    bit[0] = true;
    // exit the thread
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    // if no arguments are given
    if (argc < 2)
    {
        // prompt user for input phrase
        printf("Enter string: ");
        // read input phrase
        getline(std::cin, input_phrase);
    }
    else
        // otherwise, read the input phrase from the command line
        input_phrase = argv[1];

    // initialize parser to read first word in input_phrase
    read << input_phrase;
    read >> word;
    // define the alpha and numeric concurrent threads
    pthread_t alpha_thread, num_thread;
    // create the threads
    pthread_create(&alpha_thread, NULL, alpha, NULL);
    pthread_create(&num_thread, NULL, numeric, NULL);
    // wait for the threads to finish
    pthread_join(alpha_thread, NULL);
    pthread_join(num_thread, NULL);
    // if we made it here, then the program has finished, return 0
    return 0;
}