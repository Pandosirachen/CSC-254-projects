import sys

class TreeNode(object):
    def __init__(self, val):
        self.val = val
	self.left = None
        self.right = None


def printtrees(self): #print the trees to the console
    sys.stdout.write("(")
    if self.left is not None:
        printtrees(self.left)
    sys.stdout.write(".")
    if self.right is not None:
        printtrees(self.right)
    sys.stdout.write(")")


def generatetrees(self, n):
    def node(root, left, right): #define a node with left node and right node
        node = TreeNode(root)
        node.left = left
        node.right = right
        return node

    def trees(min, max): #define the recursive function to generate list of trees with max nodes
        return [node(root, left, right)
                for root in range(min, max + 1)
                for left in trees(min, root - 1) #generate the left subtree, same for the right in the next line
                for right in trees(root + 1, max)] or [None] #return a tree, or null for the 0 node
    return trees(1, n) #return the list of trees

val = int(input("Enter your value: ") )
t = TreeNode(1)
trees = generatetrees(t, val)

for x in range(len(trees)):  # print the trees in the list
    printtrees(trees[x])
    sys.stdout.write("\n")
