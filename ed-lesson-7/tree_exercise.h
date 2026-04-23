struct children_list{
    struct tree_node** values;
    unsigned capacity;
    unsigned used;
};

struct tree_node{
    int value;
    struct children_list children;
};


struct tree_node* create_node(int value);

void add_child(struct tree_node* parent, struct tree_node* child); 

void dfs(struct tree_node* root);

struct tree_node* find_node(struct tree_node* root, int val);

void destroy_tree(struct tree_node* root);
