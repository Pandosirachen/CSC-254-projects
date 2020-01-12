using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSC_254_pj1
{
    public class Treenode
    {
        public Treenode() { }
        public Treenode left;
        public Treenode right;
    }

    
    public class Program
    {
        public static List<Treenode> CreateTree(int n, List<List<Treenode>> trees)
        {
        if (trees.Count > n)
        {
             return trees[n];
        }
        else
        {
            List<Treenode> current = new List<Treenode>();

            for (int i = n - 1; i >= 0; i--)
            {
                Treenode node;
                int leftnodes = i;
                int rightnodes = n - 1 - i;

                List<Treenode> lefttrees = CreateTree(leftnodes, trees);
                List<Treenode> righttrees = CreateTree(rightnodes, trees);

                if (rightnodes > 0 && leftnodes > 0) //When the left tree and the right tree of current node are not null.
                    {
                    foreach (Treenode leftnode in lefttrees)
                    {
                        node = new Treenode();
                        node.left = leftnode;
                        foreach (Treenode rightnode in righttrees)
                        {
                            node.right = rightnode;
                            current.Add(node);
                        }
                    }
                }

                else if (rightnodes > 0 && leftnodes <= 0) //When the left tree of current node is null.
                    {
                    foreach (Treenode rightnode in righttrees)
                    {
                        node = new Treenode();
                        node.right = rightnode;
                        current.Add(node);
                    }
                }

                else if (leftnodes > 0 && rightnodes <= 0) //When the right tree of current node is null.
                {
                    foreach (Treenode leftnode in lefttrees)
                    {
                        node = new Treenode();
                        node.left = leftnode;
                        current.Add(node);
                    }
                }
            }
            trees.Insert(n,current);
            return (List<Treenode>)current;
        }
    }
        public static void printtree(Treenode n)
        {
            Console.Write("(");
            if (n.left != null)
                printtree(n.left);
            Console.Write(".");
            if (n.right != null)
                printtree(n.right);
            Console.Write(")");
        }
        static void Main(string[] args)
        {
            Treenode n1 = new Treenode();
            List<Treenode> l1 = new List<Treenode>();
            l1.Add(n1);  // create the base case of 1 node

            List<List<Treenode>> l = new List<List<Treenode>>();  //initialize the List of List of trees
            l.Add(null);  // add the base case of 0 node
            l.Add(l1);  // add the base case of 1 node

            //Let user input the number of input numbers of nodes
            Console.WriteLine("Please type the number of nodes:");
            String k = Console.ReadLine();
            int number = Convert.ToInt32(k);

            List<Treenode> listtree = new List<Treenode>();
            listtree = CreateTree(number, l);

            for (int i = 0; i <= listtree.Count- 1; i++) // print all the trees in the list 
            {
                printtree(listtree[i]);
                Console.Write("\n");
            }
            Console.WriteLine("Press Enter to Exit.");

            Console.ReadLine();
        }
    }
}
