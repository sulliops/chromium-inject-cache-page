# chromium-cache-clearer

PURPOSE: This program opens Chromium-based browsers (Chrome or Edge, for the purposes of deployment) and injects the URL "chrome://settings/clearBrowserData" by simulating key presses, in order to provide a one-click shortcut for browser cache clearing for less tech-savvy operators. Chromium-based browsers do not allow hyperlinks or shortcuts to URLs using the "chrome://" or "edge://" protocols, so this is the next-best thing.

TODO: Add error handling for failures to launch program executables. This involves the use of the WIN32API GetLastError function, which is too confusing for my novice brain to work with. SOURCE: https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror

USAGE: Run the program as a standalone executable without command-line arguments to be shown a selection menu. Choose the browser to launch, then wait for the magic to happen. Alternatively, run the program with a single command-line argument (either "chrome" or "edge") to silently open the selected browser. This can be accomplished through a program shortcut.

COMPILE: Use the following command on any system with the G++ compiler installed to compile this program: 
```
g++ -Wall chromium-cache-clearer.cpp -o chromium-cache-clearer.exe
```

NOTE: This program is only designed to work on Windows computers with Google Chrome and Microsoft Edge installed.
