#ifndef RBTree_HPP
# define RBTree_HPP

# include <cstddef>

namespace RBTree
{
    enum Color
    {
        BLACK = 0,
        RED = 1
    };

    class FastTree
    {
    public:
        struct Node
        {
        public:
            RBTree::Color color;
            RBTree::FastTree::Node *parent;
            RBTree::FastTree::Node *left;
            RBTree::FastTree::Node *right;
        };

    private:
        RBTree::FastTree::Node sentinel;
    public:
        constexpr inline FastTree() noexcept: sentinel()
        {
            this->sentinel.color = RBTree::Color::BLACK;
            this->sentinel.parent = &(this->sentinel);
            this->sentinel.left = &(this->sentinel);
            this->sentinel.right = &(this->sentinel);
        }

        RBTree::FastTree::Node *min() const noexcept;

        RBTree::FastTree::Node *max() const noexcept;

        RBTree::FastTree::Node *min(RBTree::FastTree::Node *start) const noexcept;

        RBTree::FastTree::Node *max(RBTree::FastTree::Node *start) const noexcept;

    protected:

        void link(
                RBTree::FastTree::Node *parent,
                RBTree::FastTree::Node **parent_child_p,
                RBTree::FastTree::Node *node
        ) noexcept;

        void unlink(
                RBTree::FastTree::Node *node
        ) noexcept;
    };

#if 0
    class FastOrderedTree : public FastTree
    {
    public:
        struct Node : public FastTree::Node
        {
        public:
            RBTree::FastTree::Node *prev;
            RBTree::FastTree::Node *next;
        };

    private:
    public:
        constexpr inline FastOrderedTree() noexcept: FastTree()
        {}

    protected:
    };
#endif
}

#endif /* RBTree_HPP */
