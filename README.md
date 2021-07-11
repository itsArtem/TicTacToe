# TicTacToe

## Getting Started

* Clone the repo using `git clone https://www.github.com/itsArtem/TicTacToe.git`.
* If you would like to use an IDE, run the Premake5 script to construct the necessary project files. If you're on Windows, navigate to `Tools/Premake/` and run the appropriate batch script. If you're using an older version of a supported IDE, just run the appropriate command using CMD. If your IDE isn't supported, you'll have to create the project files your IDE requires yourself. If you're not on Windows, download Premake and run the build script. Use `premake5 --help` for a list of supported IDEs and more.
* Compile the project. Make sure you're compiling it as C++17 or newer.
* If you're using MSVC, navigate to `Dependencies/MsvcDlls/` and copy the .dll files to the output directory. If you're using a different compiler or you're not on Windows, you must download or compile the required dynamic link library files yourself. All required dynamic link library files are included when you download the development libraries from SDL, but you may have to compile them yourself if your compiler/platform isn't listed there.
* Run the .exe file.

## About

This was a simple project made for a coding challenge. It was written in C++17. 

The AI isn't the best as I didn't have much time to work on this project, but there is a neat player vs. player mode to make up for that.
