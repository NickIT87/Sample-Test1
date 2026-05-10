#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <queue>

// ================= Node =================
template <typename T>
class TreeNode {
public:
    T value;
    std::vector<std::unique_ptr<TreeNode>> children;

    explicit TreeNode(T val) : value(val) {}

    void AddChild(std::unique_ptr<TreeNode> child) {
        children.push_back(std::move(child));
    }
};

// ================= DFS Iterator =================
template <typename T>
class DFSIterator {
public:
    explicit DFSIterator(TreeNode<T>* root) {
        if (root) stack_.push(root);
    }

    bool IsDone() const {
        return stack_.empty();
    }

    void Next() {
        if (stack_.empty()) return;

        TreeNode<T>* node = stack_.top();
        stack_.pop();

        // добавляем детей в обратном порядке
        for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
            stack_.push(it->get());
        }
    }

    T& Current() {
        return stack_.top()->value;
    }

private:
    std::stack<TreeNode<T>*> stack_;
};

// ================= BFS Iterator =================
template <typename T>
class BFSIterator {
public:
    explicit BFSIterator(TreeNode<T>* root) {
        if (root) queue_.push(root);
    }

    bool IsDone() const {
        return queue_.empty();
    }

    void Next() {
        if (queue_.empty()) return;

        TreeNode<T>* node = queue_.front();
        queue_.pop();

        for (auto& child : node->children) {
            queue_.push(child.get());
        }
    }

    T& Current() {
        return queue_.front()->value;
    }

private:
    std::queue<TreeNode<T>*> queue_;
};

// ================= Tree =================
template <typename T>
class Tree {
public:
    std::unique_ptr<TreeNode<T>> root;

    std::unique_ptr<DFSIterator<T>> CreateDFSIterator() {
        return std::make_unique<DFSIterator<T>>(root.get());
    }

    std::unique_ptr<BFSIterator<T>> CreateBFSIterator() {
        return std::make_unique<BFSIterator<T>>(root.get());
    }
};

// ================= Client =================
int main() {
    Tree<int> tree;

    // строим дерево
    tree.root = std::make_unique<TreeNode<int>>(1);

    auto child1 = std::make_unique<TreeNode<int>>(2);
    child1->AddChild(std::make_unique<TreeNode<int>>(4));
    child1->AddChild(std::make_unique<TreeNode<int>>(5));

    auto child2 = std::make_unique<TreeNode<int>>(3);
    child2->AddChild(std::make_unique<TreeNode<int>>(6));
    child2->AddChild(std::make_unique<TreeNode<int>>(7));

    tree.root->AddChild(std::move(child1));
    tree.root->AddChild(std::move(child2));

    // ===== DFS =====
    std::cout << "DFS:\n";
    auto dfs = tree.CreateDFSIterator();

    while (!dfs->IsDone()) {
        std::cout << dfs->Current() << " ";
        dfs->Next();
    }

    std::cout << "\n";

    // ===== BFS =====
    std::cout << "BFS:\n";
    auto bfs = tree.CreateBFSIterator();

    while (!bfs->IsDone()) {
        std::cout << bfs->Current() << " ";
        bfs->Next();
    }

    std::cout << "\n";
}