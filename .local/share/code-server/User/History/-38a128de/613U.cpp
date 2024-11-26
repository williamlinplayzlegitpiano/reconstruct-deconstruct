#include "HCTree.hpp"
#include <stack>

//Destructor

HCTree::~HCTree() {
    if (root) { //helper to recursively delete root
        deleteRoot(root);
    }
}

//helper to delete root
void HCTree::deleteRoot(HCNode* node) {
    if (!node) return;
    deleteRoot(node->c0);
    deleteRoot(node->c1);
    delete node;
}

/**
 * build tree
 * @param const vector<int> &
 */
void HCTree::build(const vector<int> & freqs) {
    //intialize priorQ 
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> priorityQ; //use HCNodePtrComp to order node

    //create leaf node
    for (int i = 0; i < freqs.size(); i++) {
        //create new node if non-zero freq
        if (freqs[i] > 0) {
            HCNode* newNode = new HCNode(freqs[i], static_cast<unsigned char>(i));
            leaves[i] = node; //store node in leaves and push into queue
            priorityQ.push(node);
        }
    }

    //one symbol case
    if (priorityQ.size() == 1) {
        HCNode* node == priorityQ.top();
        root = new HCNode(node->count, node->symbol);
        root->c0 = node; //set existing node to left child, and parent pointer to root
        node->p = root; 
        return;
    }

    //build
    while(priorityQ.size() > 1) {
        //pop two nodes at top of stack, create parent
        HCNode* firstNode = priorityQ.top(); priorityQ.pop();
        HCNode* secondNode = priorityQ.top(); priorityQ.pop();
        HCNode* parentNode = new HCNode(firstNode->count + secondNode->count, firstNode->symbol);

        //assign c0 and c1 to popped nodes, update parent pointers of child nodes
        parentNode->c0 = firstNode;
        parentNode->c1 = secondNode;
        firstNode->p = parentNode;
        secondNode->p = parentNode;

        priorityQ.push(parentNode);
    }

    //set root
    if (!priorityQ.empty()) {
        root = priorityQ.top();
        priorityQ.pop();
    }
}

/**
 * encode 
 * obj: writing huffman code for symbol to output stream
 * @param symbol, FancyOutputStream & out
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    vector<int> bits;
    HCNode* node = leaves[symbol];

    if (!node) {
        error("symbol not found");
    }

    while(node->)
}