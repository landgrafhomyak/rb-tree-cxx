#include <rb_tree.hpp>
#include <map>

namespace RBTree
{
    static constexpr inline void rotate_left(
            RBTree::FastTree::Node *nil,
            RBTree::FastTree::Node **parent_child_p,
            RBTree::FastTree::Node *top
    ) noexcept
    {
        RBTree::FastTree::Node *node = top->right;
        top->right = node->left;

        if (node->left != nil)
        { node->left->parent = top; }

        node->parent = top->parent;

        *parent_child_p = node;

        node->left = top;
        top->parent = node;
    }

    static constexpr inline void rotate_right(
            RBTree::FastTree::Node *nil,
            RBTree::FastTree::Node **parent_child_p,
            RBTree::FastTree::Node *top
    ) noexcept
    {
        RBTree::FastTree::Node *node = top->left;
        top->left = node->right;

        if (node->right != nil)
        { node->right->parent = top; }

        node->parent = top->parent;

        *parent_child_p = node;

        node->right = top;
        top->parent = node;
    }
}

#define rb_tree_self_pp(NODE_P)                                                                         \
    (                                                                                                   \
        (NODE_P) == (NODE_P)->parent->right ? &((NODE_P)->parent->right) :                              \
        (NODE_P) == (NODE_P)->parent->left ? &((NODE_P)->parent->left) : &(this->sentinel.parent)       \
    )


RBTree::FastTree::Node *RBTree::FastTree::min() const noexcept
{
    RBTree::FastTree::Node *node = this->sentinel.parent;
    RBTree::FastTree::Node const *const end = &(this->sentinel);
    while (node->left != end)
    { node = node->left; }
    return node;
}

RBTree::FastTree::Node *RBTree::FastTree::max() const noexcept
{
    RBTree::FastTree::Node *node = this->sentinel.parent;
    RBTree::FastTree::Node const *const end = &(this->sentinel);
    while (node->right != end)
    { node = node->right; }
    return node;
}

RBTree::FastTree::Node *RBTree::FastTree::min(RBTree::FastTree::Node *node) const noexcept
{
    RBTree::FastTree::Node const *const end = &(this->sentinel);
    while (node->left != end)
    { node = node->left; }
    return node;
}

RBTree::FastTree::Node *RBTree::FastTree::max(RBTree::FastTree::Node *node) const noexcept
{
    RBTree::FastTree::Node const *const end = &(this->sentinel);
    while (node->right != end)
    { node = node->right; }
    return node;
}

void RBTree::FastTree::link(
        RBTree::FastTree::Node *parent,
        RBTree::FastTree::Node **parent_child_p,
        RBTree::FastTree::Node *node
) noexcept
{
    RBTree::FastTree::Node *pointer;
    RBTree::FastTree::Node *parent_sibling;
    RBTree::FastTree::Node *rotate_temp;

    node->parent = parent;
    *parent_child_p = node;
    node->left = &(this->sentinel);
    node->right = &(this->sentinel);
    if (parent == &(this->sentinel))
    {
        node->color = RBTree::Color::BLACK;
        return;
    }
    node->color = RBTree::Color::RED;

    pointer = node;
    while (pointer->parent->color == RBTree::Color::RED)
    {
        if (pointer->parent == pointer->parent->parent->left)
        {
            parent_sibling = pointer->parent->parent->right;
            if (parent_sibling->color == RBTree::Color::RED)
            {
                pointer->parent->color = RBTree::Color::BLACK;
                parent_sibling->color = RBTree::Color::BLACK;
                pointer->parent->parent->color = RBTree::Color::RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->right)
                {
                    pointer = pointer->parent;
                    RBTree::rotate_left(&(this->sentinel), rb_tree_self_pp(pointer), pointer);
                }

                pointer->parent->color = RBTree::Color::BLACK;
                pointer->parent->parent->color = RBTree::Color::RED;
                RBTree::rotate_right(&(this->sentinel), rb_tree_self_pp(pointer->parent->parent), pointer->parent->parent);
            }
        }
        else
        {
            parent_sibling = pointer->parent->parent->left;
            if (parent_sibling->color == RBTree::Color::RED)
            {
                pointer->parent->color = RBTree::Color::BLACK;
                parent_sibling->color = RBTree::Color::BLACK;
                pointer->parent->parent->color = RBTree::Color::RED;
                pointer = pointer->parent->parent;
            }
            else
            {
                if (pointer == pointer->parent->left)
                {
                    pointer = pointer->parent;
                    RBTree::rotate_right(&(this->sentinel), rb_tree_self_pp(pointer), pointer);
                }

                pointer->parent->color = RBTree::Color::BLACK;
                pointer->parent->parent->color = RBTree::Color::RED;
                RBTree::rotate_left(&(this->sentinel), rb_tree_self_pp(pointer->parent->parent), pointer->parent->parent);
            }
        }
    }

    this->sentinel.parent->color = RBTree::Color::BLACK;
}

void RBTree::FastTree::unlink(RBTree::FastTree::Node *node) noexcept
{
    RBTree::FastTree::Node *fix_node;
    RBTree::FastTree::Node *fix_node_parent;
    RBTree::FastTree::Node *pointer = node;

    if (pointer->left == &(this->sentinel))
    { fix_node = pointer->right; }
    else if (pointer->right == &(this->sentinel))
    { fix_node = pointer->left; }
    else
    {
        pointer = this->min(node->right);
        fix_node = pointer->right;
        goto TWO_CHILDREN;
    }

    fix_node_parent = node->parent;
    if (fix_node != &(this->sentinel))
    { fix_node->parent = fix_node_parent; }

    if (node->parent == &(this->sentinel))
    { this->sentinel.parent = fix_node; }
    else if (fix_node_parent->left == node)
    { fix_node_parent->left = fix_node; }
    else
    { fix_node_parent->right = fix_node; }

#   if 0
    if (this->lowest == node)
            { this->lowest = (fix_node ==  &(this->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_PrevSubKey(fix_node); }

            if (this->greatest == node)
            { this->greatest = (fix_node ==  &(this->sentinel)) ? fix_node_parent : _COMiC_RedBlackTree_NextSubKey(fix_node); }
#   endif

    if (node->color == RBTree::Color::RED)
    { goto NOT_BLACK; }

    goto BALANCE;

    TWO_CHILDREN:
    node->left->parent = pointer;
    pointer->left = node->left;

    if (pointer == node->right)
    { fix_node_parent = pointer; }
    else
    {
        fix_node_parent = pointer->parent;
        if (fix_node != &(this->sentinel))
        { fix_node->parent = fix_node_parent; }

        fix_node_parent->left = fix_node;
        pointer->right = node->right;
        node->right->right = pointer;
    }

    if (node->parent == &(this->sentinel))
    { this->sentinel.parent = pointer; }
    else if (node->parent->left == node)
    { node->parent->left = pointer; }
    else
    { node->parent->right = pointer; }

    pointer->parent = node->parent;
    if (node->color == RBTree::Color::RED)
    { goto NOT_BLACK; }
    node->color = pointer->color;
    pointer->color = RBTree::Color::BLACK;

    BALANCE:
    while ((fix_node_parent = fix_node->parent) != &(this->sentinel) && fix_node->color == RBTree::Color::BLACK)
    {
        if (fix_node == fix_node_parent->left)
        {
            pointer = fix_node_parent->right;
            if (pointer->color == RBTree::Color::RED)
            {
                pointer->color = RBTree::Color::BLACK;
                fix_node_parent->color = RBTree::Color::RED;
                RBTree::rotate_left(&(this->sentinel), rb_tree_self_pp(fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->right;
            }

            if (pointer == &(this->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->left->color == RBTree::Color::BLACK && pointer->right->color == RBTree::Color::BLACK)
            {
                pointer->color = RBTree::Color::RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->right->color == RBTree::Color::BLACK)
                {
                    pointer->left->color = RBTree::Color::BLACK;
                    pointer->color = RBTree::Color::RED;
                    RBTree::rotate_right(&(this->sentinel), rb_tree_self_pp(pointer), pointer);
                    pointer = fix_node_parent->right;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = RBTree::Color::BLACK;
                pointer->right->color = RBTree::Color::BLACK;
                RBTree::rotate_left(&(this->sentinel), rb_tree_self_pp(fix_node_parent), fix_node_parent);
                break;
            }
        }
        else
        {
            pointer = fix_node_parent->left;
            if (pointer->color == RBTree::Color::RED)
            {
                pointer->color = RBTree::Color::BLACK;
                fix_node_parent->color = RBTree::Color::RED;
                RBTree::rotate_right(&(this->sentinel), rb_tree_self_pp(fix_node_parent), fix_node_parent);
                pointer = fix_node_parent->left;
            }

            if (pointer == &(this->sentinel))
            { fix_node = fix_node_parent; }
            else if (pointer->right->color == RBTree::Color::BLACK && pointer->left->color == RBTree::Color::BLACK)
            {
                pointer->color = RBTree::Color::RED;
                fix_node = fix_node_parent;
            }
            else
            {
                if (pointer->left->color == RBTree::Color::BLACK)
                {
                    pointer->right->color = RBTree::Color::BLACK;
                    pointer->color = RBTree::Color::RED;
                    RBTree::rotate_left(&(this->sentinel), rb_tree_self_pp(pointer), pointer);
                    pointer = fix_node_parent->left;
                }

                pointer->color = fix_node_parent->color;
                fix_node_parent->color = RBTree::Color::BLACK;
                pointer->left->color = RBTree::Color::BLACK;
                RBTree::rotate_right(&(this->sentinel), rb_tree_self_pp(fix_node_parent), fix_node_parent);
                break;
            }
        }
    }

    NOT_BLACK:
    fix_node->color = RBTree::Color::BLACK;
}
