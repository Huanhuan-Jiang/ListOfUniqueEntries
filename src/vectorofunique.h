#pragma once

#include <functional> // For std::hash
#include <initializer_list>
#include <optional> // For std::nullopt
#include <unordered_set>
#include <utility> // For std::swap
#include <vector>

namespace containerofunique {

template <class T, class Hash = std::hash<T>, class KeyEqual = std::equal_to<T>>
class vector_of_unique {
public:
  // *Member types
  using value_type = T;
  using key_type = T;
  using hasher = Hash;
  using key_equal = KeyEqual;
  using reference = value_type &;
  using const_reference = const value_type &;
  using VectorType = std::vector<T>;
  using UnorderedSetType = std::unordered_set<T, Hash, KeyEqual>;
  using size_type = typename VectorType::size_type;
  using iterator = typename VectorType::iterator;
  using const_iterator = typename VectorType::const_iterator;
  using reverse_iterator = typename VectorType::reverse_iterator;
  using const_reverse_iterator = typename VectorType::const_reverse_iterator;

  // Member functions
  // Constructor
  vector_of_unique() = default;

  template <class input_it> vector_of_unique(input_it first, input_it last) {
    _push_back(first, last);
  }

  vector_of_unique(const std::initializer_list<T> &init)
      : vector_of_unique(init.begin(), init.end()) {}

  vector_of_unique(const vector_of_unique &other) { _push_back(other); }

  vector_of_unique(vector_of_unique &&other) noexcept {
    std::swap(vector_, other.vector_);
    std::swap(set_, other.set_);
  }

  vector_of_unique &operator=(const vector_of_unique &other) = default;
  vector_of_unique &operator=(vector_of_unique &&other) noexcept = default;
  vector_of_unique &operator=(std::initializer_list<T> ilist) {
    vector_of_unique temp(ilist);
    std::swap(vector_, temp.vector_);
    std::swap(set_, temp.set_);
    return *this;
  }

  // Element access
  const_reference at(size_type pos) const { return vector_.at(pos); }
  const_reference front() const { return vector_.front(); }
  const_reference operator[](size_type pos) const { return vector_[pos]; }
  const_reference back() const { return vector_.back(); }

  // Iterators
  const_iterator cbegin() const noexcept { return vector_.cbegin(); }
  const_iterator cend() const noexcept { return vector_.cend(); }

  const_reverse_iterator crbegin() const noexcept { return vector_.crbegin(); }
  const_reverse_iterator crend() const noexcept { return vector_.crend(); }

  // Modifiers
  void clear() noexcept {
    vector_.clear();
    set_.clear();
  }

  const_iterator erase(const_iterator pos) {
    set_.erase(*pos);
    return vector_.erase(pos);
  }
  // Destructor
  ~vector_of_unique() = default;

  // Get member variables
  const VectorType &vector() const { return vector_; }
  const UnorderedSetType &set() const { return set_; }

private:
  VectorType vector_;
  UnorderedSetType set_;
}; // class vector_of_unique
}; // namespace containerofunique