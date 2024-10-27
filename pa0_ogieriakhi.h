

#include <stdio.h>            
#include <stdlib.h>    
#include <string.h>        

typedef enum type {
    D, F
} NodeType;

typedef struct node {
	char  name[64];       // node's name string
	NodeType  type;
	struct node *child, *sibling, *parent;
	// other fields if needed
} NODE;

/* initialize()
 *  Functionality: This function is designed to initilaize the UNIX system
 *  Arguments: void
 *  Return: void
 */
void initialize();

/* handleInput()
 *  Functionality: Handle the input of the user for the program
 *  Arguments: The recieved input from the user
 *  Return: void 
 */
void handleInput(char* input);

int find_commad(char *user_command);

/* parsePathname()
 *  Functionality: Parse the received pathname
 *  Arguments: pathname, the directory and basename for the desired action
 *  Return: void
 */
void parsePathname(const char* pathname, char* dirname, char* basename);

/* searchDirectory()
 *  Functionality: Be able to search directory for a desired NODE
 *  Arguments: the root and pathname of the desired NODE.
 *  Return: a pointer to the target NODE
 */
NODE* searchDirectory(NODE* root, const char* path);

/* addDirectory()
 *  Functionality: Helper function for the mkdir functionality which adds a directory at a specified path
 *  Arguments: the parent node which the Node will be put under and name of the new directory
 *  Return: void
 */
void addDirectory(NODE *parent, NODE *child);

/* createNode()
 *  Functionality: Creates memory for a NodeType
 *  Arguments: the desired nodeType and name
 *  Return: a pointer to the new Node
 */
NODE* createNode(const char* name, NodeType type);

/* cdCommand()
 *  Functionality: change the current directory of the system
 *  Arguments: pathname of the directory
 *  Return: void
 */
void cdCommand(const char* pathname);

/* mkdirCommand()
 *  Functionality: Creating new directory in the system
 *  Arguments: the root pointer and a pathname for the directory
 *  Return: void
 */
void mkdirCommand(NODE* root, const char* pathname);

/* getNodeTypeString()
 *  Functionality: Designed to find the type a Node
 *  Arguments: NodeType that needs to be examined
 *  Return: a string of the type the argument is
 */
const char* getNodeTypeString(NodeType type);

/* lsCommand()
 *  Functionality: Print out all the files in a directory
 *  Arguments: a pathname to the desired path
 *  Return: void
 */
void lsCommand(const char* pathname);

/* isDirectoryEmpty()
 *  Functionality: Find whether a directory is empty or not
 *  Arguments: a directory NodeType
 *  Return: a int signifying empty or not
 */
int isDirectoryEmpty(NODE* dir);

/* removeNodeFromParent()
 *  Functionality: Helper function designed to remove a node from its Parent safely 
 *  Arguments: target node
 *  Return: void
 */
void removeNodeFromParent(NODE* node);

/* rmdir()
 *  Functionality: Remove a directory in the system, given it is not empty
 *  Arguments: pathname for the desired directory
 *  Return: void
 */
void rmdir(const char* pathname);

/* buildPath()
 *  Functionality: Helper function for building the absolute path of a node recursively
 *  Arguments: A pointer to the node and its current path
 *  Return: void
 */
void buildPath(NODE* node, char* path);

/* pwd()
 *  Functionality: Printing the current working directory in the system
 *  Arguments: NONE
 *  Return: void
 */
void pwd();

/* findChildNode()
 *  Functionality: Helper function designed to find a Child Node from a given parent 
 *  Arguments: parent node and name of desired node
 *  Return: A pointer to the target Node
 */
NODE* findChildNode(NODE* parent, const char* name);

/* creatFile()
 *  Functionality: Create a file in the Unix system
 *  Arguments: pathname of where the file should be located and where the path should begin (CWD or root)
 *  Return: void
 */
void creatFile(char* pathname, NODE* startNode);

/* removeFile()
 *  Functionality: Deleting a File Node from the system
 *  Arguments: Pathname of the file location and the beginning of its location
 *  Return: possibleReturnValues
 *  References: document/website...
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
void removeFile(char *pathname, NODE *startNode);

/* saveTreeToFile()
 *  Functionality: Helper function to recursivley save the Unix Tree to a specified file
 *  Arguments: Desired output file, current node, and current depth
 *  Return: possibleReturnValues
 */
void saveTreeToFile(FILE *fp, NODE *node, int depth);

/* reloadTreeFromFile()
 *  Functionality: Helper function to reload tree from a saved file, it reads and initializes a new node
 *  Arguments: Input file, the parent Node and the starting depth of reload
 *  Return: A pointer to newly created node
 */
NODE* reloadTreeFromFile(FILE *fp, NODE *parent, int expectedDepth);

/* freeTree()
 *  Functionality: Helper function to safely delete a Tree in the Unix system
 *  Arguments: The root of the tree to be deleted
 *  Return: void
 */
void freeTree(NODE* node);

/* saveCommand()
 *  Functionality: Save the Unix system into an output txt file
 *  Arguments: the desired output file
 *  Return: void
 */
void saveCommand(const char* filename);

/* reloadCommand()
 *  Functionality: Reload a system from a txt file
 *  Arguments: The input file
 *  Return: void
 */
void reloadCommand(const char* filename);

/* quitCommand()
 *  Functionality: Quit the entire program and save the current version to a txt file
 *  Arguments: NONE
 *  Return: void
 */
void quitCommand();

 
NODE* root;
NODE* CWD;
