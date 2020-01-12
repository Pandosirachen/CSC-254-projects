Name:Sichen Pan
Netid:span9
Studentid:29849826



C#:
  mcs CSC254.cs
  mono CSC254.exe
Then following the prompt to input the number of nodes and press enter to exit

Python:
  python2.7 CSC254.py
Then following the prompt to input the number of nodes the program will exit automatically.

Prolog:
  swipl CSC254.pl
To test the program with n nodes given
input the query following the structure: treeenumeration(n)
For instance, input:
  treeenumeration(3).
will give all trees with 3 nodes.

Java:
  javac *.java
  java Node
Then following the prompt to input the number of nodes the program will exit automatically.
 
Are there noticeable differences in speed?  
For this project, I implement a dynamic programming process in Java and C#, which will generate all possible nodes from 1 to n.
Though it may seem very space consuming, the computing time will be dramatically decreased since I do not go over the same process to search all possible binary trees with 2 nodes when I encounter it the second time.
For python and prolog, I implement the recursive approach which would not be that efficient in time.

Hardest part?
I believe the hardest part of this project is the Prolog part since I need to shift my find from object oriencted to logic.
Another hard part that I haven't figure out by the deadline is how am I suppose to reach the same goal without lists in Ada.

Easy part?
The easiest part would be to implement the process in java since I use java the most and I'm very comfortable with object oriented programming

What do you like/dislike?  
I like the flexibility of python which leads to a concise code. However, I believe this will not be very efficient and stable. I really dislike about how ada needs us to specify every process and every variables. I believe this will improve the stability of the program but it brings tons of problems when coding.

Did you find iterators to be helpful? 
Though I do not use Iterators in my java program, I use for loops to recusively iterate through the lists acting the same way. It was very helpful for my project and it saves a lot of work.





