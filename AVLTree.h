
template <typename T>
struct Node {
    T       value;
    int     height;
    Node *  left;
    Node *  right;
};

template <typename T>
struct Set {
    Node<T> *  root;
};

template <typename T>
void initialize(Set<T> &set) {
    set.root = nullptr;
}

template <typename T>
void destroyFunction(Node<T> *node) { //Post-order traversal, removing the left and right subtrees prior to deleting the node
    if(node != nullptr) { 
        destroyFunction(node->left); //Recursively destroying left node
        destroyFunction(node->right); //Recursively destroying right node
        delete node; //Deletes the node that you are currently in while traversing
    }
}

template <typename T>
void destroy(Set<T> &set) {
    destroyFunction(set.root);   
    set.root = nullptr; //Tree is destroyed
}

template <typename T>   //Function that finds the maximum of two values (cannot use <algorithm> header)
T maximum(T x, T y) {
    return (x > y) ? x : y;
}

template <typename T>
int fixHeight(Node<T> *node) {
    if (node == nullptr) {
        return -1; // Change the initial height of nullptr nodes to -1
    }

    int leftHeight = (node->left != nullptr) ? node->left->height : -1; // Updated leftHeight
    int rightHeight = (node->right != nullptr) ? node->right->height : -1; // Updated rightHeight

    node->height = 1 + maximum(leftHeight, rightHeight);
    return node->height;
}


template <typename T>
Node<T> *rRotation(Node<T> *q) {
    Node<T> *p = q->left;
    Node<T> *pivot = p->right;

    // Perform a right rotation:
    // 1. Make 'p' the new root of the subtree
    // 2. Update the left child of 'p' to be 'q'
    // 3. Update the right child of 'q' to be the left child of 'p'

    p->right = q;
    q->left = pivot;

    // Update the heights of 'q' and 'p' after rotation
    fixHeight(q);
    fixHeight(p);

    return p;   // Return the new root of the subtree
}

template <typename T>
Node<T> *lRotation(Node<T> *p) {
    Node<T> *q = p->right;
    Node <T> *pivot = q->left;

    // Perform a left rotation:
    // 1. Make 'q' the new root of the subtree
    // 2. Update the right child of 'q' to be 'p'
    // 3. Update the left child of 'p' to be the right child of 'q'

    q->left = p;
    p->right = pivot;

    // Update the heights of 'p' and 'q' after rotation

    // Update the heights of 'p' and 'q' after rotation
    fixHeight(p);
    fixHeight(q);

    return q;   // Return the new root of the subtree
}

template <typename T>
int balanceFactor (Node<T> *node) {
    if (node == nullptr) {
        return 0;   // Return 0 for an empty subtree (no balance factor)
    }

    int leftNodeHeight = -1;    // Initialize the height of the left subtree to -1
    int rightNodeHeight = -1;   // Initialize the height of the right subtree to -1


    // Check if the left child exists, and if so, get its height
    if(node->left != nullptr) {
        leftNodeHeight = node->left->height;
    }


    // Check if the right child exists, and if so, get its height
    if(node->right != nullptr) {
        rightNodeHeight = node->right->height;
    }


    // Calculate and return the balance factor of the current node
    // by subtracting the height of the right subtree from the height of the left subtree
    return leftNodeHeight - rightNodeHeight;
}



template <typename T>
Node<T> *fixBalance (Node<T> *avlNode) {
    
    int balance = balanceFactor(avlNode);   // Calculate the balance factor of the current node
    
    if(balance > 1) {   // Left-heavy subtree
        if (balanceFactor(avlNode->left) >= 0) {
            // Left-left case: Left subtree of left child is taller or equally tall
            return rRotation(avlNode);  // Perform a right rotation
        } else {
            // Left-right case: Right subtree of left child is taller
            avlNode->left = lRotation(avlNode->left);   // Perform a left rotation on left child
            return rRotation(avlNode);  // Perform a right rotation on current node
        }
    }

    if(balance < -1) {  // Right-heavy subtree
        if(balanceFactor(avlNode->right) <= 0) {
            // Right-right case: Right subtree of right child is taller or equally tall
            return lRotation(avlNode);  // Perform a left rotation
        } else {
            // Right-left case: Left subtree of right child is taller
            avlNode->right = rRotation(avlNode->right); // Perform a right rotation on right child
            return lRotation(avlNode);  // Perform a left rotation on current node
        }
    }
    return avlNode; // If no balancing is needed, return the original node
}

template <typename T>
Node<T> *createNewNode(T item) {
    //Newly created node
    Node<T> *newAVLnode = new Node<T>; 
    newAVLnode->value = item; //Creates a new node with the required value
    newAVLnode->left = nullptr; //No left child
    newAVLnode->right = nullptr; //No right child

    return newAVLnode;
}

template <typename T>
Node<T> *insertRepairBalance(Node<T> *avlNode, T item) {
    int balance = balanceFactor(avlNode); // Calculate the balance factor of the current node

    if (avlNode == nullptr) { // If the current node is nullptr (i.e., a leaf node), we insert the new node
        Node<T> *newNode = createNewNode(item); // Create a new node with the given item
        newNode->height = 0; // Initialize the height for the new leaf node to 0
        return newNode; // Return the new node as it becomes the new leaf
    }

    if (item < avlNode->value) { // If the item is less than the value in the current node
        avlNode->left = insertRepairBalance(avlNode->left, item); // Recursively insert the item in the left subtree
    } else if (item > avlNode->value) { // If the item is greater than the value in the current node
        avlNode->right = insertRepairBalance(avlNode->right, item); // Recursively insert the item in the right subtree
    }

    fixHeight(avlNode); // After insertion, update the height of the current node

    return fixBalance(avlNode); // Perform AVL tree balancing and return the balanced subtree
}


template <typename T>
void insert(Set<T> &set, T item) {
    set.root = insertRepairBalance(set.root, item);
}


template <typename T>
Node<T> *removeRepairBalance(Node<T> *node, T item) {
    int balance = balanceFactor(node);

    if (node == nullptr) {
        // If the node is not in the tree, return nullptr
        return nullptr;
    }

    if (item < node->value) {
        // If the item is smaller than the current node's value,
        // recursively search in the left subtree
        node->left = removeRepairBalance(node->left, item);
    } else if (item > node->value) {
        // If the item is larger than the current node's value,
        // recursively search in the right subtree
        node->right = removeRepairBalance(node->right, item);
    } else {
        // Item matches the value in the current node

        if (node->left == nullptr && node->right == nullptr) {
            // Node has no children, so we can safely delete it
            delete node;
            return nullptr; // Return nullptr to indicate the node is removed
        } else if (node->left != nullptr && node->right == nullptr) {
            // Node has a left child but no right child
            Node<T>* temporary = node->left;
            delete node;
            return temporary; // The left child replaces the current node
        } else if (node->left == nullptr && node->right != nullptr) {
            // Node has a right child but no left child
            Node<T>* temporary = node->right;
            delete node;
            return temporary; // The right child replaces the current node
        } else {
            // Node has both left and right children

            //Found in 6.09 of Data Structures interactive that in deletion of node
            //the left-most, smallest number in the right subtree becomes the new current node  
            
            Node<T>* smallestNode = node->right; //Initializes pointer of smallest valued node which points to right child of current node
            while (smallestNode->left) { //Continues traversing until left-most node is found
                smallestNode = smallestNode->left;
            }

            //The current node's value is overwritten with the smallest node's value
            node->value = smallestNode->value;

            //Removes the smallest value node from the right subtree
            node->right = removeRepairBalance(node->right, smallestNode->value);
        }
    }

    fixHeight(node);

    return fixBalance(node);
}


template <typename T>
void remove(Set<T> &set, T item) {
    set.root = removeRepairBalance(set.root, item);
}


template <typename T>
bool containsChecklist(Node<T> *bstBox, T item) {

    if (bstBox == nullptr) {
        return false;
    }

    if (item == bstBox->value) { //Item is the same as the value in the box, thus true
        return true;
    }

    if (item < bstBox->value) { //If item is less than the value in the box, recursive call to go to the left of the box and begin the checklist again 
        return containsChecklist(bstBox->left, item);
    }

    //If item is greater than the value in the box, recursive call to go to the right of the box and begin the checklist again
    return containsChecklist(bstBox->right, item);
}

template <typename T>
bool contains(Set<T> &set, T item) {
    return containsChecklist(set.root, item);
}

template <typename T>
int sizeTraversal(Node<T> *node) {
    if(node == nullptr) { //If there is no node, then there is no count
        return 0;
    }

    int leftBSTCount = sizeTraversal(node->left);
    int rightBSTCount = sizeTraversal(node->right);

    //Case where there is a root, in which 1 is the root node. Checks left and right of node recursively and counts +1 each time it is found
    return 1 + leftBSTCount + rightBSTCount;
}

template <typename T>
int size(Set<T> &set) {
    return sizeTraversal(set.root);
}





//Height of alpha changes, height of beta doesnt change
//Height is the distance of root node to the most-bottom node of a subtree
//Height of a tree that doesn't exit is undefined (or return -1)
//Wiki tree rotation 
//Only heights that need to be updated when rotating is P and Q (root and pivot)
//AVL tree is a height balanced within 1 tree {-1, 0, 1}