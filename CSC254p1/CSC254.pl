:- use_module(library(clpfd)).
printtrees([L, R]):-
  write('('),printtrees(L),
  write('.'),printtrees(R),
  write(')'),!. %prevent the program from tracing back
printtrees(Node):- write(Node).
generatetrees(0,''). %base case for 
generatetrees(N, [L, R]) :- %generate trees by represent them as lists with left list and right list
    N #> 0, 
    N #= N1 + N2 + 1,
    N1 #>= 0, 
    N2 #>= 0,
    generatetrees(N1, L), 
    generatetrees(N2, R).
treeenumeration(Num_Nodes):-  
  generatetrees(Num_Nodes, Tree),
  printtrees(Tree), 
  writeln(''),  
  2=1.  
treeenumeration(_).