#pragma once

#include <cassert>
#include <string>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        
        explicit BasicIterator(Node* node) {
            node_ = node;
        }
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
        
        BasicIterator() = default;
        
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
        
        BasicIterator& operator=(const BasicIterator& rhs) = default;
        
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
        
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
        
        BasicIterator& operator++() noexcept {
            node_ = node_ == nullptr ? nullptr : node_->next_node;
            return *this;
        }
        
        BasicIterator operator++(int) noexcept {
            auto it_value = *this;
            ++(*this);
            return it_value;
        }
        
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }
        
        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }
        
    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
    
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }
    
    [[nodiscard]] Iterator end() noexcept {
        return {};
    }
    
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }
    
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    
    [[nodiscard]] ConstIterator cend() const noexcept {
        return {};
    }
    
    SingleLinkedList(): size_(0) {}
    
    SingleLinkedList(std::initializer_list<Type> values) {
        for(auto it = std::rbegin(values); it != std::rend(values); it++) {
            PushFront(*it);
        }
        size_ = values.size();
    }
    
    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList tmp;

        for(auto it = other.end - 1; it != other.before_begin; it--) {
            tmp.PushFront(*it);
        }

        swap(tmp);
    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this == &rhs) {
            return *this;
        }
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }
    
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        size_++;
    }
    
    void Clear() noexcept {
        while(head_.next_node != nullptr) {
            Node* d = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete d;
        }
        size_ = 0;
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return size_ == 0;
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    void PopFront() noexcept {
        Node* d = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        size_--;
        delete d;
    }
    
    Iterator InsertAfter(ConstIterator it, const Type& value) {
        Node* new_node = new Node(value, it.node_->next_node);
        it.node_->next_node = new_node;
        size_++;
        return Iterator(new_node);
    }
    
    Iterator EraseAfter(ConstIterator it) noexcept {
        Node* d = it.node_->next_node;
        it.node_->next_node = it.node_->next_node->next_node;
        delete d;
        return Iterator(it.node_->next_node);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }
    
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (&lhs == &rhs) || (std::equal(lhs.begin(), lhs.end(), rhs.begin())); 
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
