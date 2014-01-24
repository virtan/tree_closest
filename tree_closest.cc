#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct node {
    node(int value_in, node *parent_in) : value(value_in), left(0), right(0), parent(parent_in) {}
    int value;
    node *left;
    node *right;
    node *parent;
};

void tree_insert(node *root, int v) {
    while(true) {
        if(v < root->value)
            if(root->left) root = root->left;
            else {
                root->left = new node(v, root);
                return;
            }
        else
            if(root->right) root = root->right;
            else {
                root->right = new node(v, root);
                return;
            }
    }
}

string value_of(node *n) {
    string res(2, ' ');
    res[0] = n->value / 10 + '0';
    res[1] = n->value % 10 + '0';
    return res;
}

int tree_depth(node *n) {
    vector<node*> nodes;
    if(n) nodes.push_back(n);
    int i = 0;
    for(; nodes.size(); ++i) {
        vector<node*> new_nodes;
        for(node* n : nodes) {
            if(n->left) new_nodes.push_back(n->left);
            if(n->right) new_nodes.push_back(n->right);
        }
        nodes = new_nodes;
    }
    return i;
}

void tree_print(node *root, node *highlight) {
    int depth = tree_depth(root);
    vector<node*> nodes;
    nodes.push_back(root);
    while(--depth >= 0) {
        vector<node*> new_nodes;
        cout << string(pow(2, (depth + 1)) - 2, ' ');
        for(node* n : nodes) {
            if(n == 0) cout << string(2, ' ');
            else cout << (n == highlight ? "\033[32m": "") << value_of(n) << (n == highlight ? "\033[0m" : "");
            cout << string(pow(2, (depth + 2)) - 2, ' ');
            new_nodes.push_back(n ? n->left : 0);
            new_nodes.push_back(n ? n->right : 0);
        }
        cout << '\n';
        nodes = new_nodes;
    }
}

node *tree_random() {
    size_t x = rand() % 10;
    node *root = 0;
    while(x--) {
        int v = rand() % 100;
        if(!root) root = new node(v, 0);
        else tree_insert(root, v);
    }
    return root;
}

node *tree_lower_bound(int x, node *root) {
    node *fallback = 0;
    while(root) {
        if(root->value == x) return root;
        if(x < root->value)
            root = root->left;
        else {
            fallback = root;
            root = root->right;
        }
    }
    return fallback;
}

node *tree_first(node *root) {
    while(root && root->left)
        root = root->left;
    return root;
}

node *tree_next(node *p) {
    if(p->right)
        return tree_first(p->right);
    node *n = p;
    while(n->parent) {
        if(!n->parent->right || n->parent->right != n)
            return n->parent;
        else n = n->parent;
    }
    return 0;
}

node *tree_closest(int x, node *root) {
    node *l = tree_lower_bound(x, root);
    if(!l) return tree_first(root);
    if(l->value == x) return l;
    node *g = tree_next(l);
    cout << "prev: " << (l ? l->value : -1) << ", next: " << (g ? g->value : -1) << endl;
    if(l == 0) l = g;
    if(g == 0) g = l;
    if(l == 0) return 0;
    return abs(x - l->value) <= abs(g->value - x) ? l : g;
}

node *tree_from_array(int array[], int offset, int length) {
    node *root = 0;
    for(; offset < length; ++offset) {
        int v = array[offset];
        if(!root) root = new node(v, 0);
        else tree_insert(root, v);
    }
    return root;
}

int legacy[][5] = {
    { 40, 70, 45, 14, 32 },
    { 40, 31, 41, 31, 49 }
};

int main(int argc, char **argv) {
    if(argc < 2) { cerr << "Usage: " << argv[0] << " <target>\n"; return 1; }
    if(string(argv[1]) == "legacy") {
        for(int i = 0; i < sizeof(legacy) / sizeof(legacy[0]); ++i) {
            if(i > 0) cout << " -----\n";
            node *t = tree_from_array(legacy[i], 1, sizeof(legacy[0]) / sizeof(legacy[0][0]));
            node *closest = tree_closest(legacy[i][0], t);
            tree_print(t, closest);
        }
    } else {
        srand(time(NULL));
        node *t = tree_random();
        node *closest = tree_closest(atoi(argv[1]), t);
        tree_print(t, closest);
    }
    return 0;
}

