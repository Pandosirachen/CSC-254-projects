import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


/*The C# program and the java program are pretty similar,
 * so I implemented the same DP function for them
 * I will generate a list of list of trees and
 * generate from base case of list of all trees with 0 nodes and list of all trees with 1 nodes
 * For java and C#, it's very useful since both of them are object-oriented.
 */

public class Node {
	
	public static List<Treenode> createtree(int n, List<List> trees) //The core function for generating trees
	{
		if (trees.size()>n)// return the list of trees if the list has already been generated.
		{
			return trees.get(n);
		}
		else  // else generate the list of tree
		{
			List<Treenode> current=new ArrayList();
			
			for(int i=n-1;i>=0;i--)
			{
				Treenode node;
				int left=i;
				int right=n-1-i;
				
				List<Treenode> lefttrees = (List<Treenode>) createtree(left, trees);
				List<Treenode> righttrees = (List<Treenode>) createtree(right, trees);
				
				if(right>0 && left>0)  //If there are nodes exist on both the right and the left side of the current node
				{
					for(Treenode leftnode:lefttrees)
					{
						node=new Treenode();
						node.left=leftnode;
						for(Treenode rightnode:righttrees)
						{
							node.right=rightnode;
							current.add(node);
						}
					}
				}
				
				else if(right>0 && left<=0)   //If there are nodes exist on only the right side of the current node
				{
					for(Treenode rightnode:(ArrayList <Treenode>) righttrees)
					{
						node=new Treenode();
						node.right=rightnode;
						current.add(node);
					}
				}
				
				else if(left>0 && right<=0)   //If there are nodes exist on only the left side of the current node
				{
					for(Treenode leftnode: (ArrayList <Treenode>)lefttrees)
					{
						node=new Treenode();
						node.left=leftnode;
						current.add(node);	
					}
				}
			}	
			trees.add(n,current);
			return (List<Treenode>) current;
		}		
	}
	
	public static void printtree(Treenode n)
	{
		System.out.print("(");
		if(n.left!=null)
			printtree(n.left);
		System.out.print(".");
		if(n.right!=null)
			printtree(n.right);
		System.out.print(")");
	}

	public static void main(String[] args) {
		
		Treenode n1=new Treenode();
		List<Treenode> l1=new ArrayList();
		l1.add(n1);
		
		List<List> l=new ArrayList<List>();
		l.add(null);// add the base case of 0 node
		l.add(l1);// add the base case of 1 node
		
		//Let user input the number of input numbers of nodes
		System.out.println("Please type the number of nodes:");
		Scanner scanner = new Scanner(System.in);
		int number=scanner.nextInt();
		
		List<Treenode> listtree=new ArrayList();
		listtree=createtree(number,l);
		
		for(int i=0;i<=listtree.size()-1;i++)// print all the trees in the list 
		{
			printtree(listtree.get(i));
			System.out.println();
		}
		
		
		
		

	}
}
