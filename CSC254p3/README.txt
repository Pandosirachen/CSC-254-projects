Name: Sichen Pan
Netid: span9

For this project, I implemented the build of syntax tree and a interpreter based on the source code provided by professor.
The program will catch the dynamic errors and interpret the syntax tree build in the first part of the project if there is no error.
The name of the functions should be clearly to understand the function of the function.
For instance: add_vals represents the function interpret the add symbol.
The sample output is listed below on the sample tests.

Build and run the program on sample output:
ocamlc str.cma interpreter.ml
./a.out

Sample Output:
10 5

2 3 5 7 11 13 17 19 23 29

3 divide by zero

foo: symbol not found

3 divide by zero

unexpected end of input

3 divide by zero
