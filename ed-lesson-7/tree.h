#ifndef TREE_H
#define TREE_H

struct children_list {
    struct tree_node **values;
    unsigned capacity;
    unsigned used;
};

struct tree_node {
    void *value;
    struct children_list children;
};

typedef void (*node_action_t)(struct tree_node *);

struct tree_node *create_node(void *value);

void add_child(struct tree_node *parent, struct tree_node *child);

void dfs(struct tree_node *root, node_action_t action);

typedef void (*destroy_value_t)(void *);

void destroy_tree(struct tree_node *root, destroy_value_t destroy);

void remove_child(struct tree_node *parent, struct tree_node *child_rm,
                  destroy_value_t destroy);

#endif
