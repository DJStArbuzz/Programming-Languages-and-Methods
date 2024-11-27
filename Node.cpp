#include "Node.h" 
#include <iostream> 

Node::Node(const string& name, const Token& token)
    : name(name), token(token) {}

void Node::addChild(shared_ptr<Node> child) {
    children.push_back(child);
}

const string& Node::getName() const {
    return name;
}

const Token& Node::getToken() const {
    return token;
}

const vector<shared_ptr<Node>>& Node::getChildren() const {
    return children;
}

// Печать дерева в консоль с отступами. 
void printTree(const shared_ptr<Node>& node, int indent) {
    for (int i = 0; i < indent; i++)
        cout << "    ";
    cout << node->getName();
    if (node->getToken().type != END_OF_FILE) {
        string tmp = node->getToken().value;
        if (tmp == "(" || tmp == ")") {
            tmp = "Scob";
        }
        if (tmp == "{" || tmp == "}") {
            tmp = "Brace";
        }
        if (tmp == "," || tmp == ";") {
            tmp = "Separ";
        }
        if (tmp.size() != 0) cout << "    [" << tmp << "] ";
    }
    cout << '\n';
    for (const auto& child : node->getChildren()) {
        printTree(child, indent + 1);
    }

}
