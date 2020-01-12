Name: Sichen Pan
Netid: span9


For this project I implemented the error recoveryy mechanism of Niklaus Wirth. 
The program will output a syntax tree of the form ( a ( b e f ) ( c g ) d ) as required in the description.
Some sample inputs and outputs are given below. I use a syntax tree to represent the tree structure and by default each 
node of the tree has 4 children at most because S --> while C SL end will at most have 4 children. The output order will follow
the AST output, for instance, with input "write 10 - 3 * 4 + 2 $", the output will be
"( write ( -  10 ( *  3  4 ) ( +  2 ) ) )". The tree will print id or literals not as the child of id or num, instead, the program will take the string as directly the child of the parent.
The examples will show  how the output represents the structure. 


Files indluded:
README.txt
test1.txt
scan.h
node.h
parse.cpp


To build and run program:
first go to the directory of the files then using the command:

g++ scan.h
g++ node.h
g++ -o main.exe parse.cpp
./main.exe

Or, to run test on test1.txt:

./main.exe < test1.txt

Notice, for all input, $ marks the end of the text.

For the input, I use a single $ as an EOF. One example is provided as follow.
The sample input is also included in the test1.txt

Sample input for test1.txt(notice the input end with a $ as EOF)
read n
   cp := 2
   while n > 0
       found := 0
       cf1 := 2
       cf1s := cf1 * cf1
       while cf1s <= cp
           cf2 := 2
           pr := cf1 * cf2
           while pr <= cp
               if pr == cp
                   found := 1
               end
               cf2 := cf2 + 1
               pr := cf1 * cf2
           end
           cf1 := cf1 + 1
           cf1s := cf1 * cf1
       end
       if found == 0
           write cp
           n := n - 1
       end
       cp := cp + 1
   end $

Sample output for test.txt

( read  n ( :=  cp  2 ( while ( >  n  0 ) ( :=  found  0 ( :=  cf1  2 ( :=  cf1s ( *  cf1  cf1 ) ( while ( <=  cf1s  cp ) ( :=  cf2  2 ( :=  pr ( *  cf1  cf2 ) ( while ( <=  pr  cp ) ( if ( ==  pr  cp ) ( :=  found  1 ) ( :=  cf2 ( +  cf2  1 ) ( :=  pr ( *  cf1  cf2 ) ) ) ) ( :=  cf1 ( +  cf1  1 ) ( :=  cf1s ( *  cf1  cf1 ) ) ) ) ) ) ( if ( ==  found  0 ) ( write  cp ( :=  n ( -  n  1 ) ) ) ( :=  cp ( +  cp  1 ) ) ) ) ) ) ) ) ) )

Some other inputs and outputs:

write 10 - 3 * 4 + 2 $
( write ( -  10 ( *  3  4 ) ( +  2 ) ) )

write 10 - * 4 + 2 $
Syntax error: expected "id" at Token #: 4

write 10 - * 4 + - 2 $
Syntax error: expected "id" at Token #: 4
Syntax error: expected "id" at Token #: 7

while n == 2 $
Syntax error: expected "end" at Token #: 5


