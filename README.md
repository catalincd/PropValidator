# Logical Proposition Validator
Logical Proposition Validator written in C++.

## Compiling
Compile main.cpp and src/*.cpp

## Running on Windows
Run /Debug/PropValidator.exe

## Usage
Input text from Command Line:
```shell
PropValidator.exe "P & Q | R"
```

Input from keyboard: "P & Q | R"
```shell
PropValidator.exe
```

## Operators

'!' - NOT
'&' - AND
'|' - OR
'=>' - IMPLIES
'<=>' - IFF

These are declared in main.cpp and you can change them and recompile the program, as long as they are in the same order.



