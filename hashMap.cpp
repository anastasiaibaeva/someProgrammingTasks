#include <iostream>
#include <list>
#include <utility>
#include <stdexcept>
#include <exception>
#include <vector>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using KVPair = std::pair<KeyType, ValueType>;
    using const_iterator = typename std::list<KVPair>::const_iterator;
    using iterator = typename std::list<KVPair>::iterator;

    explicit HashMap(Hash hasher = Hash()) : hasher_(std::move(hasher)) {
        size_ = 0;
        capacity_ = 32;
        table_.resize(capacity_);
    }

    template <typename Iterator>
    HashMap(Iterator begin, Iterator end, Hash hasher = Hash()) : HashMap(hasher) {
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
    }

    HashMap(const std::initializer_list<KVPair>& list, Hash hasher = Hash()) : HashMap(hasher) {
        for (auto const& elements : list) {
            insert(elements);
        }
    }

    iterator begin() {
        return elements_.begin();
    }

    iterator end() {
        return elements_.end();
    }

    const_iterator begin() const {
        return elements_.cbegin();
    }

    const_iterator end() const {
        return elements_.cend();
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    Hash hash_function() const {
        return hasher_;
    }

    void insert(const KVPair& element) {
        size_t num = HashFunction(element.first);
        for (auto& elements : table_[num]) {
            if (elements->first == element.first) {
                return;
            }
        }
        elements_.push_back(element);
        auto it = elements_.end();
        table_[num].push_back(std::prev(it));
        ++size_;
        ReHash();
    }

    void erase(const KeyType& key) {
        size_t num = HashFunction(key);
        for (auto it = table_[num].begin(); it != table_[num].end(); ++it) {
            if ((*it)->first == key) {
                elements_.erase(*it);
                table_[num].erase(it);
                --size_;
                return;
            }
        }
    }

    const_iterator find(const KeyType& key) const {
        size_t num = HashFunction(key);
        for (auto& elements : table_[num]) {
            if (elements->first == key) {
                return elements;
            }
        }
        return elements_.end();
    }

    iterator find(const KeyType& key) {
        size_t num = HashFunction(key);
        for (auto& elements : table_[num]) {
            if (elements->first == key) {
                return elements;
            }
        }
        return elements_.end();
    }

    ValueType& operator[] (const KeyType& key) {
        if (find(key) != elements_.end()) {
            return find(key)->second;
        } else {
            insert({key, ValueType()});
            return find(key)->second;
        }
    }

    const ValueType& at(const KeyType& key) const {
        if (find(key) == elements_.end()) {
            throw std::out_of_range("");
        } else {
            return find(key)->second;
        }
    }

    void clear() {
        table_.clear();
        elements_.clear();
        capacity_ = 32;
        size_ = 0;
        table_.resize(capacity_);
    }

private:
    std::list<KVPair> elements_;
    std::vector<std::list<iterator>> table_;
    Hash hasher_;
    size_t size_{};
    size_t capacity_{};

    size_t HashFunction(KeyType key) const {
        return hasher_(key) % capacity_;
    }

    void ReHash() {
        if (size_ == capacity_) {
            table_.clear();
            capacity_ *= 2;
            table_.resize(capacity_);
            for (auto it = elements_.begin(); it != elements_.end(); ++it) {
                size_t num = HashFunction(it->first);
                table_[num].push_back(it);
            }
        } else {
            return;
        }
    }
};
