# TicTacToe

## Getting Started

* Clone the repo using `git clone https://www.github.com/itsArtem/TicTacToe.git`.
* If you would like to use an IDE, run the Premake5 script to construct the necessary project files. If you're on Windows, navigate to `Tools/Premake/` and run the appropriate batch script. If you're using an older version of a supported IDE, just run the appropriate command using CMD. If your IDE isn't supported, you'll have to create the project files your IDE requires yourself. If you're not on Windows, download Premake and run the build script. Use `premake5 --help` for a list of supported IDEs and more.
* The included SDL2 dependencies are only for MSVC builds. If you're using a different compiler, you'll have to download the correct versions or compile them yourself.
* Compile the project.
* If you're using MSVC, navigate to `Dependencies/MsvcDlls/` and copy the DLL files to the output directory. If you're using a different compiler or you're not on Windows, use the dynamic link library files included in your download or the ones you compiled yourself.
* Run the .exe file.

## About

This was a simple project made for a coding challenge. It was written in C++17. 

The AI isn't the best as I didn't have much time to work on this project, but there is a neat player vs. player mode to make up for that.
