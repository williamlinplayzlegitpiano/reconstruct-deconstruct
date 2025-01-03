#include "HCTree.hpp"
#include <stack>

//helper to delete root
void deleteRoot(HCNode* node) {
    if (!node) return;
    deleteRoot(node->c0);
    deleteRoot(node->c1);
    delete node;
}

//Destructor
HCTree::~HCTree() {
    if (root) { //helper to recursively delete root
        deleteRoot(root);
    }
}

/**
 * build tree
 * @param const vector<int> &
 */
void HCTree::build(const vector<int> & freqs) {
    //intialize priorQ 
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> priorityQ; //use HCNodePtrComp to order node

    //create leaf node
    for (size_t i = 0; i < freqs.size(); i++) {
        //create new node if non-zero freq
        if (freqs[i] > 0) {
            HCNode* newNode = new HCNode(freqs[i], static_cast<unsigned char>(i));
            leaves[i] = newNode; //store node in leaves and push into queue
            priorityQ.push(newNode);
        }
    }

    //one symbol case
    if (priorityQ.size() == 1) {
        HCNode* node = priorityQ.top();
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
 * @param unsigned char symbol, FancyOutputStream & out
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    HCNode* currNode = leaves[symbol];

    if (!currNode) { //if nullptr, symbol DNE
        error("symbol not found during encoding");
    }

    vector<int> bts; //store path

    //traverse from root
    while(currNode->p) {
        if (currNode->p->c0 == currNode) {
            bts.push_back(0); //append 0 if currNode is left child
        } else {
            bts.push_back(1); //append 1 if currNode is right child
        }
        currNode = currNode->p; //move to parent
    }

    //reverse stored path
    for (int i = (int)bts.size() - 1; i >= 0; i--) {
        out.write_bit(bts[i]); //write into FancyOutputStream
    }
}

/**
 * decode
 * obj: decode the next symbol from input stream
 * @param FancyInputStream & in
 */

unsigned char HCTree::decode(FancyInputStream & in) const {
    HCNode* currNode = root;

    //check if nullptr
    if (!currNode) {
        error("empty tree during decoding");
    }

    int bts;
    
    while(currNode->c0 || currNode->c1) {
        bts = in.read_bit();
        if (bts == -1) {
            error("end of input during decoding");
        } else if (bts == 0) {
            currNode = currNode->c0; //if bts is 0, move to left child
        } else if (bts == 1) {
            currNode = currNode->c1; //right child
        } else {
            error("invalid bit value"); 
        }

        //check null
        if (!currNode) {
            error("null node");
        }

    }

    return currNode->symbol; //return symbol stored in node

}


/**
 * fill codeLengths vector with code length of each symbol
 */
/*void HCTree::getCodeLengths(vector<unsigned char> & codeLengths) const {
    for (int i = 0; i < 256; i++) {
        HCNode* leaf = leaves[i];
        if (leaf) {
            int length = 0;
            HCNode* currNode = leaf;
            while(curr->p) {
                length++;
                currNode = currNode->p;
            }
            codeLengths[i] = static_cast<unsigned char>(length);
        }
    }
}

/**
 * build using canonical huffman code
 */
/*void HCTree::buildFromCodeLengths(const vector<unsigned char> & symbols, const vector<unsigned char> & codeLengths) {
    map<int, vector<unsigned char>> symbolLengths;
    for (size_t i = 0; i < symbols)
} 