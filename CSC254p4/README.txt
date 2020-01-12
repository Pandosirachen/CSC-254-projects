CSC 254 project 4
Name: Sichen Pan
Netid: span9

How to run
To run the program, first build the program from the source file using the following command
Here is an example of my simple testing program:
	gcc -g3 test.c -o test
Then, to test the xref.rb program, using the following command:
	ruby xref.rm myprogram
if the HTML is successfully build, the prompt should be like following:
	/home/hoover/u5/span9/Cross$ ruby xref.rb test
	Make HTML success
Otherwise(if the argument is not matched):
	/home/hoover/u5/span9/Cross$ ruby xref.rb 
	No program

Short explanation
For this assignment, I implemented a hashmap in ruby to store the addresses, assembly code and sources. I simply use two loops to go through
the output of the dwarf program and the objdump program and construct the final data table by using addresses as keys.
Because when I inspected objdump and llvm-dwarfdump, the addressesof the code are all in the output, 
So I decided to use the addresses as the key and the assembly and source code as values. 
To implement the hyperlink, I split the value and find the 6 digit number address after the "callq". 

For the html construction, I used a table to store the information and implement grids to format the webpage. 
The element in the table are stored with their id marked as their addresses. 
So by clicking the 6 digit address after the "callq" the webpage will jump directly to the place where the address is point to.
And by clicking back, the browser will return to the previos place. 

I include some test files I used. 

Files included:
HTML
 -index.html
func1.c
func2.c
test
test.c
xref.rb

	