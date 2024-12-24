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
  using const_reference = const value_type &;
  using VectorType = std::vector<T>;
  using UnorderedSetType = std::unordered_set<T, Hash, KeyEqual>;
  using size_type = typename VectorType::size_type;
  using const_iterator = typename VectorType::const_iterator;
  using reverse_iterator = typename VectorType::reverse_iterator;
  using const_reverse_iterator = typename VectorType::const_reverse_iterator;

  // Member functions
  // Constructor
  vector_of_unique() noexcept = default;

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
    if (vector_.empty()) {
      return vector_.cend();
    }
    set_.erase(*pos);
    return vector_.erase(pos);
  }

  const_iterator erase(const_iterator first, const_iterator last) {
    if (first == last) {
      return last;
    }

    for (auto it = first; it != last; ++it) {
      set_.erase(*it);
    }

    return vector_.erase(first, last);
  }

  std::pair<const_iterator, bool> insert(const_iterator pos, const T &value) {
    if (set_.insert(value).second) {
      return std::make_pair(vector_.insert(pos, value), true);
    }
    return std::make_pair(pos, false);
  }

  std::pair<const_iterator, bool> insert(const_iterator pos, T &&value) {
    if (set_.insert(value).second) {
      return std::make_pair(vector_.insert(pos, std::move(value)), true);
    }
    return std::make_pair(pos, false);
  }

  template <class input_it>
  const_iterator insert(const_iterator pos, input_it first, input_it last) {
    auto pos_index = pos - vector_.cbegin();
    auto first_inserted_index = pos_index;
    auto temp_pos = vector_.begin() + (pos - vector_.cbegin());
    auto any_inserted = false;

    for (auto it = first; it != last; ++it) {
      if (set_.insert(*it).second) {
        temp_pos = vector_.insert(temp_pos, *it);
        if (!any_inserted) {
          first_inserted_index = temp_pos - vector_.cbegin();
          any_inserted = true;
        }
        ++temp_pos;
      }
    }
    return any_inserted ? first_inserted_index + vector_.cbegin()
                        : pos_index + vector_.cbegin();
  }

  const_iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
  }

  template <class... Args>
  std::pair<const_iterator, bool> emplace(const_iterator pos, Args &&...args) {
    if (set_.emplace(args...).second) {
      return std::make_pair(vector_.emplace(pos, std::forward<Args>(args)...),
                            true);
    }
    return std::make_pair(pos, false);
  }

  template <class... Args>
  std::optional<std::reference_wrapper<T>> emplace_back(Args &&...args) {
    if (set_.emplace(args...).second) {
      return vector_.emplace_back(std::forward<Args>(args)...);
    }
    return std::nullopt;
  }

  bool push_back(const T &value) {
    if (set_.insert(value).second) {
      vector_.push_back(value);
      return true;
    }
    return false;
  }

  bool push_back(T &&value) {
    auto temp = std::move(value);
    if (set_.insert(temp).second) {
      vector_.push_back(std::move(temp));
      return true;
    }
    return false;
  }

  template <class input_it> void _push_back(input_it first, input_it last) {
    while (first != last) {
      push_back(*first++);
    }
  }

  bool _push_back(const vector_of_unique<T, Hash> &other) {
    return _push_back(other.vector_);
  }

  bool _push_back(const std::vector<T> &other) {
    bool any_added = false;
    for (const auto &entry : other) {
      auto added = push_back(entry);
      any_added = any_added || added;
    }
    return any_added;
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