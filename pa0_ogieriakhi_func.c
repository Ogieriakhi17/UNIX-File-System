#include "pa0_ogieriakhi.h"
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "quit", 0};

void initialize() {
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = D;
	CWD = root;
	// other steps as needed
	
	printf("Filesystem initialized!\n");
}

void handleInput(char* input) {
    // Tokenize the input by splitting the string by space
    char* command = strtok(input, " ");
    char* pathname = strtok(NULL, " ");


    // Check the command and call the corresponding function
    if (strcmp(command, "cd") == 0) {
        if(pathname == NULL )
        {
            CWD = root;
            printf("CWD set to root\n");
        }
        
        else cdCommand(pathname);  // Call the 'cd' function
    } else if (strcmp(command, "ls") == 0) {
        lsCommand(pathname);  // Call the 'ls' function (pathname can be NULL for CWD)
    }
    else if(strcmp(command, "mkdir")== 0)
    {
        if (pathname[0] == '/') {
        // Absolute path, start from the root
        mkdirCommand(root, pathname);
    } else {
        // Relative path, start from the CWD
        mkdirCommand(CWD, pathname);

    }
    } 
    else if(strcmp(command, "rmdir")== 0)
    {
        rmdir(pathname);
    } 
    else if(strcmp(command, "rm") == 0)
    {
        if (pathname[0] == '/') {
        // Absolute path, start from root
        removeFile(pathname, root);
    } else {
        // Relative path, start from CWD
        removeFile(pathname, CWD);
    }
    }
    else if(strcmp(command, "pwd")== 0)
    {
        pwd();
    } 
    else if (strcmp(command, "quit")== 0)
    {
        quitCommand();
    } 
    else if (strcmp(command, "save")== 0)
    {
        saveCommand(pathname);
    } 
    else if (strcmp(command, "reload")== 0)
    {
        reloadCommand(pathname);
    } 
    else if (strcmp(command, "creat")== 0)
    {
        creatFile(pathname, CWD);
    } 
    else {
        printf("Error: Unknown command '%s'\n", command);
    }
}

NODE* createNode(const char* name, NodeType type) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (!newNode) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    strncpy(newNode->name, name, 63);  
    newNode->name[63] = '\0';          
    newNode->type = type;              
    newNode->child = NULL;
    newNode->sibling = NULL;
    newNode->parent = NULL;
    
    return newNode;
}

int find_commad(char *user_command)
{
int i = 0;
while(cmd[i]){
if (strcmp(user_command, cmd[i])==0)
return i;
i++;
}
return -1;
}

void parsePathname(const char* pathname, char* dirname, char* basename) {

    const char *lastSlash = strrchr(pathname, '/');
    
    if (lastSlash) {
        // Extract dirname and basename
        strncpy(basename, lastSlash + 1, 64);
        strncpy(dirname, pathname, lastSlash - pathname);
        dirname[lastSlash - pathname] = '\0';  // Null-terminate dirname
    } else {
        // No '/' found: the entire pathname is the basename
        strcpy(basename, pathname);
        strcpy(dirname, pathname);
    }
}

NODE* searchDirectory(NODE* root, const char* path) {
    if (!path || strlen(path) == 0) {
        printf("Error: Invalid path\n");
        return NULL;
    }

    NODE* currentNode = root; // Start from root or CWD
    char tempPath[256];
    strcpy(tempPath, path); // Copy the path to a temporary variable
  

    char* token = strtok(tempPath, "/");  // Split the path by '/'
    
    if (token == NULL) {
        printf("Error: Tokenization failed, path might be invalid\n");
        return NULL;
    }

    while (token) {
      
        NODE* found = NULL;
        
        // Search for the token (directory or file) among currentNode's children
        NODE* child = currentNode->child;  // Start with the first child
        while (child) {
            if (strcmp(child->name, token) == 0) {
                found = child;  // Found the correct directory or file
                break;
            }
            child = child->sibling;  // Move to the next sibling
        }
        
        if (!found) {
           // printf("Error: Path '%s' does not exist\n", token);
            return NULL;
        }
        
        currentNode = found; // Move to the found node (directory or file)
        token = strtok(NULL, "/"); // Get the next component of the path
    }
    
    return currentNode; // Return the final node (can be a file or directory)
}


NODE* createNODE(const char* name, NodeType type)
{
    NODE* newNode = (NODE*)malloc(sizeof(NODE*));
    if (!newNode) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    // Initialize the node's fields
    strncpy(newNode->name, name, 63);  // Copy the name (max 63 chars)
    newNode->name[63] = '\0';          // Ensure the name is null-terminated
    newNode->type = type;              // Set the node type (D or F)
    
    // Initialize the pointers to NULL
    newNode->child = NULL;
    newNode->sibling = NULL;
    newNode->parent = NULL;
    
    return newNode;  // Return the newly created node
}

void cdCommand(const char* pathname) {
    NODE* newDir;

    // Handle "cd .." to move up one directory level
    if (strcmp(pathname, "..") == 0) {
        // Check if we're not already at the root
        if (CWD->parent != NULL) {
            CWD = CWD->parent;  // Move up to the parent directory
            printf("Moved up to directory '%s'\n", CWD->name);
        } else {
            printf("Error: Already at the root directory, cannot move up\n");
        }
        return;
    }

    // Handle absolute path
    if (pathname[0] == '/') {
        newDir = searchDirectory(root, pathname);
    } else {
        // Handle relative path
        newDir = searchDirectory(CWD, pathname);
    }

    // Check if the new directory is valid and is of type DIR
    if (newDir && newDir->type == D) {
        CWD = newDir;  // Update current working directory
        printf("Changed directory to '%s'\n", newDir->name);
    } else {
        printf("Error: '%s' is not a valid directory\n", pathname);
    }
}


void mkdirCommand(NODE* root, const char* pathname) {
    char dirname[256], basename[256];
    
    // Step 1: Parse the pathname into dirname and basename
    parsePathname(pathname, dirname, basename);


     NODE *parentDir = CWD;
    

    // If parent directory is found and it's a directory
    if (parentDir && parentDir->type == D) {
        // Check if a node with the same name already exists
        if (searchDirectory(parentDir, basename)) {
            printf("Error: Directory '%s' already exists!\n", basename);
            return;
        }

        
        // Create new directory node under the parent
        NODE *newDir = createNode(basename, D);
        addDirectory(parentDir, newDir);
        printf("Directory '%s' created under '%s'\n", basename, parentDir->name);
    } else {
        printf("Error: Directory '%s' not found\n", dirname);
    }
}

void addDirectory(NODE *parent, NODE *child) {
    if (parent->child == NULL) {
        // If the parent has no children, make this the first child
        parent->child = child;
    } else {
        // If the parent already has children, add the new child to the sibling list
        NODE *current = parent->child;
        while (current->sibling != NULL) {
            current = current->sibling;
        }
        current->sibling = child;
    }

    // Set the parent of the child node
    child->parent = parent;
}

// Function to print node type as a string
const char* getNodeTypeString(NodeType type) {
    return type == D ? "D " : "F";
}

void lsCommand(const char* pathname) {
    NODE* targetNode;

    // If pathname is empty, list the current directory (CWD)
    if (pathname == NULL || strcmp(pathname, "") == 0) {
        targetNode = CWD;  // Use the current working directory (CWD)
    } else if (pathname[0] == '/') {
        // Absolute path: search from root
        targetNode = searchDirectory(root, pathname);
    } else {
        // Relative path: search from CWD
        targetNode = searchDirectory(CWD, pathname);
    }

    // Step 1: Check if targetNode is valid
    if (targetNode == NULL) {
        printf("Error: Invalid path '%s'\n", pathname);
        return;
    }

    // Step 2: Check if targetNode is a directory
    if (targetNode->type != D) {
        printf("Error: '%s' is not a directory\n", pathname);
        return;
    }

    // Step 3: List all child nodes (if any)
    NODE* child = targetNode->child;
    if (!child) {
        printf("Directory is empty\n");
        return;
    }

    while (child) {
        // Print the type and name of each child
        printf("%s %s\n", getNodeTypeString(child->type), child->name);
        child = child->sibling;
    }
}

// Function to check if a directory is empty
int isDirectoryEmpty(NODE* dir) {
    return dir->child == NULL;
}

// Function to remove a node from its parent's child list
void removeNodeFromParent(NODE* node) {
    NODE* parent = node->parent;
    if (!parent) return;  // Node has no parent, can't remove

    NODE* prev = NULL;
    NODE* current = parent->child;

    // Traverse the child list to find the node to remove
    while (current) {
        if (current == node) {
            if (prev) {
                // If node is not the first child, update sibling pointers
                prev->sibling = current->sibling;
            } else {
                // If node is the first child, update the parent's child pointer
                parent->child = current->sibling;
            }
            free(current);  // Free the memory of the node
            return;
        }
        prev = current;
        current = current->sibling;
    }
}


void rmdir(const char* pathname) {

    if (!pathname || strcmp(pathname, "") == 0) {
        printf("Error: No pathname provided\n");
        return;
    }

    // Determine the starting point (root or CWD)
    NODE* start = (pathname[0] == '/') ? root : CWD;


    // Search for the directory node using the pathname
    NODE* targetNode = searchDirectory(start, pathname);


    // Check if the node exists
    if (!targetNode) {
        printf("Error: Directory '%s' does not exist\n", pathname);
        return;
    }

    // Check if the node is of D type
    if (targetNode->type != D) {
        printf("Error: '%s' is not a directory\n", pathname);
        return;
    }

    // Check if the directory is empty
    if (!isDirectoryEmpty(targetNode)) {
        printf("Error: Directory '%s' is not empty\n", pathname);
        return;
    }

    // Remove the directory from the parent's child list
    removeNodeFromParent(targetNode);
    printf("Directory '%s' removed successfully\n", pathname);
}

void buildPath(NODE* node, char* path) {
    if (node == NULL) {
        return;
    }
    
    if (node->parent != NULL) {
        // Recursively go up to the parent before appending the current node name
        buildPath(node->parent, path);
    }

    // If the current node is not the root, append "/name"
    if (strcmp(node->name, "/") != 0) {
        strcat(path, "/");
        strcat(path, node->name);
    } else {
        strcat(path, "/");
    }
}

// pwd function
void pwd() {
    char path[1024] = "";  // Buffer to store the path

    if (CWD == root) {
        // If the current working directory is root, just print "/"
        strcpy(path, "/");
    } else {
        // Build the absolute path by recursively going up the tree
        buildPath(CWD, path);
    }

    printf("%s\n", path);  // Print the full path
}

NODE* findChildNode(NODE* parent, const char* name) {
    NODE* temp = parent->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->sibling;
    }
    return NULL;
}


void creatFile(char* pathname, NODE* startNode) {
    // Split pathname into dirname and basename
    char *dirname = strdup(pathname);  // Copy to avoid modifying original string
    char *basename = strrchr(dirname, '/');  // Find the last '/' for basename
    NODE *parentDir;

    if (basename != NULL) {
        // Separate basename and dirname
        *basename = '\0';  // Terminate dirname at the last '/'
        basename++;  // Move pointer to the basename
    } else {
        // If no '/' in pathname, it's just a file in the current directory
        basename = dirname;
        dirname = NULL;
    }

    // 1. Find the directory (startNode is either root or CWD)
    if (dirname != NULL) {
        parentDir = searchDirectory(startNode, dirname);
        if (parentDir == NULL || parentDir->type != D) {
            printf("Error: Directory '%s' does not exist or is not a directory!\n", dirname);
            free(dirname);
            return;
        }
    } else {
        parentDir = startNode;  // If no dirname, parentDir is the startNode
    }

    // 2. Check if a node with the basename already exists in the directory
    if (findChildNode(parentDir, basename) != NULL) {
        printf("Error: File '%s' already exists!\n", basename);
        free(dirname);
        return;
    }

    // 3. Create the new F node
    NODE* newFile = (NODE*)malloc(sizeof(NODE));
    strcpy(newFile->name, basename);
    newFile->type = F;  
    newFile->child = NULL;
    newFile->sibling = parentDir->child;  // Insert the new file at the beginning of the child list
    newFile->parent = parentDir;

    parentDir->child = newFile;  // Set the new file as the first child

    printf("File '%s' created successfully in directory '%s'.\n", basename, parentDir->name);
    
    free(dirname);  // Free the allocated memory
}

// Function to remove a file (rm pathname)
void removeFile(char *pathname, NODE *startNode) {
    // Split pathname into dirname and basename
    char *dirname = strdup(pathname);  // Copy to avoid modifying original string
    char *basename = strrchr(dirname, '/');  // Find the last '/' for basename
    NODE *parentDir;

    if (basename != NULL) {
        // Separate basename and dirname
        *basename = '\0';  // Terminate dirname at the last '/'
        basename++;  // Move pointer to the basename
    } else {
        // If no '/' in pathname, it's just a file in the current directory
        basename = dirname;
        dirname = NULL;
    }

    // 1. Find the directory (startNode is either root or CWD)
    if (dirname != NULL) {
        parentDir = searchDirectory(startNode, dirname);
        if (parentDir == NULL || parentDir->type != D) {
            printf("Error: Directory '%s' does not exist or is not a directory!\n", dirname);
            free(dirname);
            return;
        }
    } else {
        parentDir = startNode;  // If no dirname, parentDir is the startNode
    }

    // 2. Check if a node with the basename exists in the directory
    NODE *fileNode = findChildNode(parentDir, basename);
    if (fileNode == NULL) {
        printf("Error: File '%s' does not exist!\n", basename);
        free(dirname);
        return;
    }

    // 3. Check if the node is a FILE type
    if (fileNode->type != F) {  // Assuming you renamed FILE to MY_FILE
        printf("Error: Cannot remove '%s' (not a FILE)!\n", basename);
        free(dirname);
        return;
    }

    // 4. Remove the file node
   // removeNode(parentDir, fileNode);
   // printf("File '%s' removed successfully.\n", basename);

    free(dirname);  // Free the allocated memory
}


void saveTreeToFile(FILE *fp, NODE *node, int depth) {
    if (node == NULL) {
        return;
    }

    // Write current node details: type, name, and depth (for hierarchy)
    fprintf(fp, "%d %s %d\n", node->type, node->name, depth);

    // Recursively save the children
    saveTreeToFile(fp, node->child, depth + 1);

    // Recursively save siblings at the same level
    saveTreeToFile(fp, node->sibling, depth);
}

NODE* reloadTreeFromFile(FILE *fp, NODE *parent, int expectedDepth) {
    int type, depth;
    char name[64];

    // Read the next line
    if (fscanf(fp, "%d %s %d", &type, name, &depth) != 3) {
        return NULL;  // End of file or error
    }

    // If the depth doesn't match the expected depth, go back one step in file
    if (depth != expectedDepth) {
        fseek(fp, -strlen(name) - 5, SEEK_CUR);  // Roll back the file pointer
        return NULL;
    }

    // Create a new node
    NODE *newNode = createNode(name, (NodeType)type);

    // Set the parent-child relationship
    newNode->parent = parent;
    if (parent != NULL) {
        if (parent->child == NULL) {
            parent->child = newNode;
        } else {
            NODE *sibling = parent->child;
            while (sibling->sibling != NULL) {
                sibling = sibling->sibling;
            }
            sibling->sibling = newNode;
        }
    }

    // Recursively add children and siblings
    newNode->child = reloadTreeFromFile(fp, newNode, expectedDepth + 1);
    newNode->sibling = reloadTreeFromFile(fp, parent, expectedDepth);

    return newNode;
}

void freeTree(NODE* node) {
    if (node == NULL) {
        return;
    }

    // Recursively free the child nodes first
    freeTree(node->child);

    // Recursively free the sibling nodes
    freeTree(node->sibling);

    // Finally, free the current node itself
    free(node);
}

void saveCommand(const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Unable to open file '%s' for saving.\n", filename);
        return;
    }

    // Save the filesystem tree starting from root
    saveTreeToFile(fp, root, 0);

    fclose(fp);
    printf("Filesystem saved to '%s'.\n", filename);
}

void reloadCommand(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Unable to open file '%s' for reloading.\n", filename);
        return;
    }

    // Free the existing tree
    freeTree(root);  // Implement a function to free the current tree
    root = createNode("/", D);  // Reinitialize the root node

    // Reload the filesystem tree starting from root
    root->child = reloadTreeFromFile(fp, root, 1);

    fclose(fp);
    printf("Filesystem reloaded from '%s'.\n", filename);
}

void quitCommand() {
    char filename[100];
    sprintf(filename, "fssim_ogieriakhi.txt");  

    // Save the filesystem before exiting
    saveCommand(filename);

    printf("Filesystem saved to '%s'. Exiting program.\n", filename);
    exit(0);  // Terminate the program
}
