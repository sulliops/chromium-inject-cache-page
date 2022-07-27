// TITLE: Chromium Cache Clearer
// CREATED BY: Owen Sullivan
// FOR: Benefitfocus.com, Inc.
// PURPOSE: This program opens Chromium-based browsers (Chrome or Edge, for the purposes of deployment) and injects the URL "chrome://settings/clearBrowserData" by simulating key presses,
//          in order to provide a one-click shortcut for browser cache clearing for less tech-savvy operators. Chromium-based browsers do not allow hyperlinks or shortcuts to URLs using
//          the "chrome://" or "edge://" protocols, so this is the next-best thing.

// TODO: Add error handling for failures to launch program executables. This involves the use of the WIN32API GetLastError function, which is too confusing for my novice brain to work with.
//       SOURCE: https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror

// USAGE: Run the program as a standalone executable without command-line arguments to be shown a selection menu. Choose the browser to launch, then wait for the magic to happen.
//        Alternatively, run the program with a single command-line argument (either "chrome" or "edge") to silently open the selected browser. This can be accomplished through a program shortcut.

// COMPILE: Use the following command on any system with the G++ compiler installed to compile this program:
//          g++ -Wall chromium-cache-clearer.cpp -o chromium-cache-clearer.exe

// NOTE: This program is only designed to work on Windows computers with Google Chrome and Microsoft Edge installed.

#define WINVER 0x0500

// Import libraries
#include <iostream> // Console output
#include <string> // Use of strings in program
#include <windows.h> // Windows API

// Use std namespace for simplicity
using namespace std;

// Define macros for hexadecimal virtual-keys
#define KEY_C 0x43
#define KEY_H 0x48
#define KEY_R 0x52
#define KEY_O 0x4F
#define KEY_M 0x4D
#define KEY_E 0x45
#define KEY_COLON 0xBA
#define KEY_SLASH 0xBF
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_I 0x49
#define KEY_N 0x4E
#define KEY_G 0x47
#define KEY_L 0x4C
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_W 0x57
#define KEY_D 0x44

// Define macros for Chrome and Edge corresponding to values in openBrowser() and other areas of program
#define CHROME 1
#define EDGE 2

// Function prototypes
void clearScreen();
void presentOptions();
int promptSelection(bool isFirstTime);
void openBrowser(int browserSelection);
void insertKeySequence();
void keyDown(INPUT &key);
void keyUp(INPUT &key);

// Begin main function
int main(int argc, char **argv) {

    if (!(argc > 1)) { // If no command-line arguments are supplied

        // Print browser choices to user's terminal
        presentOptions();

        // Store selected browser choice
        int browserSelection = promptSelection(true); // Passes "true", indicating that this is the first prompt

        // Error-checking for browser choice (makes sure that a valid choice is selected)
        while (browserSelection != CHROME && browserSelection != EDGE) { // Uses defined macros

            browserSelection = promptSelection(false); // Passes "false", indicating that this is not the first prompt (shows extra error message)

        }

        clearScreen();

        // Tell user what's going on
        cout << "Please wait while the program launches your browser's cache clearing page." << endl;
        
        if (browserSelection == CHROME) { // Different button text for each browser

            cout << "When the browser window appears, click the \"Clear data\" button." << endl;

        } else {

            cout << "When the browser window appears, click the \"Clear now\" button." << endl;

        }

        cout << endl;

        // Wait 5 seconds to give user a chance to read above messages
        Sleep(5000);

        openBrowser(browserSelection); // Open selected browser

        if (browserSelection == CHROME) { // Different message for each browser

            cout << "Opened Google Chrome successfully.";

        } else {

            cout << "Opened Microsoft Edge successfully.";

        }

        cout << endl;

    } else { // If command-line arguments are supplied

        if (argc > 2) { // Check for more than one command-line argument

            // Display error message and usage message
            cout << "Error: Too many command-line arguments." << endl;
            cout << "Usage: \"clear-cache.exe [BROWSER_NAME]\" where BROWSER_NAME is either \"chrome\" or \"edge\"." << endl;
            cout << endl;

            cout << "Aborting program." << endl;

            // End program
            return -1;

        }

        string commandLineOption = argv[1]; // Store command-line argument as string for comparison

        // Open browser according to supplied argument
        if (commandLineOption == "chrome") { // If argument matches "chrome"

            openBrowser(CHROME); // Open Chrome

        } else if (commandLineOption == "edge") { // If argument matches "edge"

            openBrowser(EDGE); // Open Edge

        } else { // If argument is not "chrome" or "edge"

            // Display error message and usage message
            cout << "Error: Invalid command-line argument." << endl;
            cout << "Usage: \"clear-cache.exe [BROWSER_NAME]\" where BROWSER_NAME is either \"chrome\" or \"edge\"." << endl;
            cout << endl;

            cout << "Aborting program." << endl;

            // End program
            return -1;

        }

    }

    // Simulate key presses to navigate to browser cache clearing page
    insertKeySequence();

}

void clearScreen() { // Clear terminal screen to simplify output and improve readability

    // SOURCES: https://stackoverflow.com/questions/6486289/how-can-i-clear-console
    
    system("cls"); // Microsoft recommends this method for clearing CMD output, despite potential safety issues with system()

}

void presentOptions() { // Print options for user to choose from

    cout << "What browser would you like to clear cache for?" << endl;
    cout << endl;

    cout << "\t1. Google Chrome" << endl;
    cout << "\t2. Mircosoft Edge" << endl;
    cout << endl;

}

int promptSelection(bool isFirstTime) { // Prompt user to input an integer for browser selection

    int browserSelection;

    if (isFirstTime) { // If "true" is passed in function call, assume this is the first time the user will see the prompt and act casual

        cout << "Input selection (number 1 or 2): ";
        cin >> browserSelection;

    } else { // If "false" is passed in function call, assume user keeps entering invalid integers and show them an extra error

        clearScreen();
        
        cout << "Error: You must enter a valid browser selection." << endl;
        cout << "Please try again." << endl;
        cout << endl;

        presentOptions(); // Present choices again
        
        cout << "Input selection (number 1 or 2): ";
        cin >> browserSelection;

    }

    return browserSelection;

}

void openBrowser(int browserSelection) { // Opens a new window of the specified browser

    // SOURCES: https://stackoverflow.com/questions/42531/how-do-i-call-createprocess-in-c-to-launch-a-windows-executable
    //          https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
    
    // Create shell process that will be used to call browser executable
    STARTUPINFO si = {sizeof(STARTUPINFO)};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;

    // NOTE: The below is a bit clunky due to an issue with typecasting strings in CreateProcess that I was too lazy to spend time fixing.

    if (browserSelection == CHROME) { // If selected browser is Chrome

        // Use WIN32API CreateProcess function to call program executable
        if (CreateProcess("C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe", NULL , NULL, NULL, FALSE, CREATE_NO_WINDOW , NULL, NULL, &si, &pi)) {

            // Close thread when program has been called successfully
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

        }

    } else {

        // Use WIN32API CreateProcess function to call program executable
        if (CreateProcess("C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", NULL , NULL, NULL, FALSE, CREATE_NO_WINDOW , NULL, NULL, &si, &pi)) {

            // Close thread when program has been called successfully
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

        }

    }

}

void insertKeySequence() { // Simulate key presses for "chrome://settings/clearBrowserData"

    // SOURCES: https://batchloaf.wordpress.com/2012/04/17/simulating-a-keystroke-in-win32-c-or-c-using-sendinput/
    //          https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    //          https://batchloaf.wordpress.com/2012/10/18/simulating-a-ctrl-v-keystroke-in-win32-c-or-c-using-sendinput/
    
    INPUT key; // WIN32API input method

    // Set input method to act as virtual keyboard
    key.type = INPUT_KEYBOARD;
    key.ki.wScan = 0;
    key.ki.time = 0;
    key.ki.dwExtraInfo = 0;

    Sleep(500); // Wait half a second to allow browser extra time to launch

    key.ki.wVk = KEY_C;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_H;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_R;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_O;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_M;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_E;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = VK_SHIFT; // Uppercase
    keyDown(key);

    key.ki.wVk = KEY_COLON;
    keyDown(key);
    
    key.ki.wVk = VK_SHIFT;
    keyUp(key);
    
    key.ki.wVk = KEY_COLON;
    keyUp(key);
    
    key.ki.wVk = KEY_SLASH;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_SLASH;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_S;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_E;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_T;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_T;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_I;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_N;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_G;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_S;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_SLASH;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_C;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_L;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_E;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_A;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_R;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = VK_SHIFT; // Uppercase
    keyDown(key);
    
    key.ki.wVk = KEY_B;
    keyDown(key);
    
    key.ki.wVk = VK_SHIFT;
    keyUp(key);
    
    key.ki.wVk = KEY_B;
    keyUp(key);
    
    key.ki.wVk = KEY_R;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_O;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_W;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_S;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_E;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_R;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = VK_SHIFT; // Uppercase
    keyDown(key);

    key.ki.wVk = KEY_D;
    keyDown(key);
    
    key.ki.wVk = VK_SHIFT;
    keyUp(key);

    key.ki.wVk = KEY_D;
    keyUp(key);
    
    key.ki.wVk = KEY_A;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_T;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = KEY_A;
    keyDown(key);
    
    keyUp(key);
    
    key.ki.wVk = VK_RETURN; // "Enter" key
    keyDown(key);

    keyUp(key);

}

void keyDown(INPUT &key) { // Simulate key being pressed down, works as its own function for code simplicity

    key.ki.dwFlags = 0;
    SendInput(1, &key, sizeof(INPUT));

}

void keyUp(INPUT &key) { // Simulate key being released, works as its own function for code simplicity

    key.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &key, sizeof(INPUT));

}