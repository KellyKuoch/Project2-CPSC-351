#include <iostream>
#include <string>
#include <pthread.h>
#include <ctype.h>

using namespace std;

// global variable to hold the input phrase
string inputPhrase;

// function to be executed by alpha thread
void* alphaThread(void* arg) {
    string::size_type start = 0;
    string::size_type end = inputPhrase.find_first_of(" \t\n\r", start);
    while (end != string::npos) {
        if (isalpha(inputPhrase[start])) {
            cout << "alpha: " << inputPhrase.substr(start, end - start) << endl;
        }
        start = inputPhrase.find_first_not_of(" \t\n\r", end);
        end = inputPhrase.find_first_of(" \t\n\r", start);
    }
    if (start != string::npos && isalpha(inputPhrase[start])) {
        cout << "alpha: " << inputPhrase.substr(start) << endl;
    }
    pthread_exit(NULL);
}

// function to be executed by numeric thread
void* numericThread(void* arg) {
    string::size_type start = 0;
    string::size_type end = inputPhrase.find_first_of(" \t\n\r", start);
    while (end != string::npos) {
        if (isdigit(inputPhrase[start])) {
            cout << "numeric: " << inputPhrase.substr(start, end - start) << endl;
        }
        start = inputPhrase.find_first_not_of(" \t\n\r", end);
        end = inputPhrase.find_first_of(" \t\n\r", start);
    }
    if (start != string::npos && isdigit(inputPhrase[start])) {
        cout << "numeric: " << inputPhrase.substr(start) << endl;
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {

    // read input phrase from command line argument or stdin
    if (argc == 2) {
        inputPhrase = string(argv[1]);
    } else {
        cout << "Enter string: ";
        getline(cin, inputPhrase);
    }

    // create alpha and numeric threads
    pthread_t alphaTid, numericTid;
    pthread_create(&alphaTid, NULL, alphaThread, NULL);
    pthread_create(&numericTid, NULL, numericThread, NULL);

    // wait for threads to finish
    pthread_join(alphaTid, NULL);
    pthread_join(numericTid, NULL);

    return 0;
}
