#include "bst.h"
#include <queue>
#include <format>

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {

}

BST::Node::Node(int value, Node* left, Node* right) : value(value), left(left), right(right) {

}

BST::Node::Node(const Node &node) : value(node.value), left(node.left), right(node.right) {

}

std::ostream& operator<<(std::ostream& os, const BST::Node& node) {
    os << std::format("{:<16} => ", static_cast<const void*>(&node))
       << std::format("value:{:<10}", node.value)
       << std::format("left:{:<16}",static_cast<const void*>(node.left))
       << std::format("right:{:<16}",static_cast<const void*>(node.right)) << std::endl;
    return os;
}

std::strong_ordering BST::Node::operator<=>(int value) const {
    return this->value <=> value;
}

std::strong_ordering operator<=>(int value, const BST::Node& node) {
    return value <=> node.value;
}

bool BST::Node::operator==(int value) const {
    return this->value == value;
}

bool operator==(int value, const BST::Node &node) {
    return value == node.value;
}

BST::BST() : _root(nullptr), _length(0) {

}

BST::BST(const BST& other) : BST() {
    auto add = [this](Node* node) {
        this->add_node(node->value);
    };

    other.bfs(add);
}

BST::BST(BST&& other) : _root(std::move(other._root)), _length(other._length) {
    other._root = nullptr;
    other._length = 0;
}

BST BST::operator=(const BST& other) {
    if (this != &other) {
        auto add = [this](Node* node) {
            this->add_node(node->value);
        };

        other.bfs(add);
    }

    return *this;
}

BST BST::operator=(BST&& other) {
    if (this != &other) {
        _root = std::move(other._root);
        _length = other._length;
        other._root = nullptr;
        other._length = 0;
    }

    return *this;
}

BST::~BST() {
    auto delete_node = [](Node* node) {
        delete node;
    };

    bfs(delete_node);
}

BST::BST(const std::initializer_list<int>& list) : BST() {
    for (const auto& value : list)
        add_node(value);
}

BST::Node*& BST::get_root() {
    return _root;
}

void BST::bfs(std::function<void(Node *&node)> func) const {
    if (_root == nullptr)
        return;

    std::queue<Node *> q;

    q.push(_root);
    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node->left != nullptr)
            q.push(node->left);
        if (node->right != nullptr)
            q.push(node->right);

        func(node);
    }
}

void BST::bfs(std::function<void(Node *&node)> func) {
    static_cast<const BST*>(this)->bfs(func);
}


size_t BST::length() const {
    return _length;
}

std::pair<BST::Node**, BST::Node**> BST::_find_node(int value) {
    Node **curr = &_root, **prev = nullptr;

    while (*curr != nullptr) {
        if (value < **curr) {
            prev = curr;
            curr = &((*curr)->left);
        } else if (value > *(*curr)) {
            prev = curr;
            curr = &((*curr)->right);
        } else {
            return {prev, curr};
        }
    }

    return {prev, nullptr};
}

bool BST::add_node(int value) {
    auto [prev, curr] = _find_node(value);

    if (curr != nullptr)
        return false;

    if (prev == nullptr)
        _root = new Node(value);
    else if (value < **prev)
        (*prev)->left = new Node(value);
    else
        (*prev)->right = new Node(value);

    ++_length;
    return true;
}

BST::Node** BST::find_node(int value) {
    return _find_node(value).second;
}

BST::Node** BST::find_parrent(int value) {
    return _find_node(value).first;
}

BST::Node** BST::find_successor(int value) {
    auto [prev, curr] = _find_node(value);

    if (curr == nullptr)
        return nullptr;

    Node** successor = nullptr;
    if ((*curr)->left != nullptr) {
        Node** t = &((*curr)->left);
        while (*t != nullptr) {
            successor = t;
            t = &((*t)->right);
        }
    } else {
        if (prev != nullptr && (*prev)->right == *curr) {
            successor = prev;
        }
    }

    return successor;
}

bool BST::delete_node(int value) {
    auto [prev, curr] = _find_node(value);

    if (curr == nullptr)
        return false;

    Node* cur = *curr;
    if (cur->left == nullptr && cur->right == nullptr) {
        if (prev != nullptr) {
            if (cur->value < **prev)
                (*prev)->left = nullptr;
            else
                (*prev)->right = nullptr;
        }
        else {
            _root = nullptr;
        }
    }
    else if (cur->left == nullptr || cur->right == nullptr) {
        if (prev != nullptr) {
            if (cur->value < **prev) {
                if (cur->left != nullptr)
                    (*prev)->left = cur->left;
                else
                    (*prev)->left = cur->right;
            }
            else {
                if (cur->left != nullptr)
                    (*prev)->right = cur->left;
                else
                    (*prev)->right = cur->right;
            }
        }
        else {
            if (cur->left != nullptr)
                _root = cur->left;
            else
                _root = cur->right;
        }
    }
    else {
        auto successor = find_successor(cur->value);
        auto successor_parent = find_parrent((*successor)->value);

        std::swap((*successor)->value, cur->value);

        if ((*successor_parent)->right != nullptr && (*successor_parent)->right == *successor)
            (*successor_parent)->right = nullptr;
        else
            (*successor_parent)->left = nullptr;

        cur = *successor;
    }

    delete cur;
    --_length;
    return true;
}

// bool BST::delete_node(int value) {
//     auto [prev, curr] = _find_node(value);
//
//     if (curr == nullptr)
//         return false;
//     Node* cur = *curr;
//     if (prev == nullptr) {
//         if (_root->left != nullptr) {
//             auto left_node = _root->left;
//
//             auto t = _root->right;
//             _root = left_node;
//             if (t != nullptr) {
//                 auto [prev_, curr_] = _find_node(t->value);
//
//                 (*prev_)->right = t;
//             }
//         }
//         else if ((*prev)->right != nullptr) {
//             _root = _root->right;
//         }
//         else {
//             _root = nullptr;
//         }
//     } else if (value < **prev) {
//         if ((*curr)->left != nullptr) {
//             auto left_node = (*curr)->left;
//             (*prev)->left = left_node;
//
//             auto t = cur->right;
//             if (t != nullptr) {
//                 auto [prev_, curr_] = _find_node(t->value);
//
//                 (*prev_)->right = t;
//             }
//         }
//         else if ((*curr)->right != nullptr) {
//             (*prev)->left = cur->right;
//         }
//         else {
//             (*prev)->left = nullptr;
//         }
//     } else {
//         if ((*curr)->right != nullptr) {
//             auto right_node = cur->right;
//             (*prev)->right = right_node;
//
//             auto t = cur->left;
//             if (t != nullptr) {
//                 auto [prev_, curr_] = _find_node(t->value);
//
//                 (*prev_)->left = t;
//             }
//         }
//         else if ((*curr)->left != nullptr) {
//             (*prev)->right = cur->right;
//         }
//         else {
//             (*prev)->right = nullptr;
//         }
//     }
//
//     delete cur;
//     --_length;
//
//     return true;
// }

BST BST::operator++() {
    auto add = [](Node*& node){ ++(node->value); };

    bfs(add);

    return *this;
}

BST BST::operator++(int) {
    auto t =  *this;
    ++(*this);
    return t;
}

std::ostream& operator<<(std::ostream& os, BST& bst) {
    os << "********************************************************************************" << std::endl;
    auto output = [&os](BST::Node*& node) {
        os << *node << std::endl;
    };
    bst.bfs(output);
    os << std::format("binary search tree size:{:<2} \n", bst._length);
    os << "********************************************************************************" << std::endl;

    return os;
}
