Name: Sichen Pan
Netid: span9

I have only tested my program on my own computer but not the cycle machine.
So I could not provide instructions on how to build and run this java program on the cycle machine
After running the java program, there will be an output file named "myfile.txt"
The program will directly write lines concatentately into this file, so after each run, to test again, will need to manually delete it.

Sample output for test:
pandosirachen@DESKTOP-V4JBM67 ~/csc254p5
$ ./check_output.py myfile.txt
Correctness check passed!
Fairness breakdown:
                Think   Wait    Eat
Philosopher 1:  71.7%   7.8%    20.5%
Philosopher 2:  70.4%   7.6%    22.0%
Philosopher 3:  75.7%   8.2%    16.1%
Philosopher 4:  69.3%   8.0%    22.6%
Philosopher 5:  72.7%   7.6%    19.7%

I require the philosophers with odd indexes can only access to their left fork first and the philosopher with even indexes can only access to their right fork first to prevent the deadlock situation that every philosopher hold one fork at once
I use the synchronized keywork to prevent concurrency in this program between multiple threads.
The final result shows that each philosopher get to eat estimately 20% and there are no two neighbor philosopher eating at the same time.
Also, type "-v" in the argument will let the program print the process on the console.