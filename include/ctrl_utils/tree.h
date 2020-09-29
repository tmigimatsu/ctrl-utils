/**
 * tree.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: November 12, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_TREE_H_
#define CTRL_UTILS_TREE_H_

#include <algorithm>      // std::find
#include <exception>      // std::invalid_argument
#include <functional>     // std::function
#include <limits>         // std::numeric_limits
#include <ostream>        // std::ostream
#include <queue>          // std::queue
#include <type_traits>    // std::conditional_t
#include <unordered_map>  // std::unordered_map
#include <utility>        // std::pair
#include <vector>         // std::vector

#if __cplusplus >= 201703L
#include <optional>  // std::optional
#else                // __cplusplus
// #include <ctrl_utils/optional.h>
#include "optional.h"
#endif  // __cplusplus

namespace ctrl_utils {

/**
 * Tree represented as a flat map of keys to nodes and a map from keys to parent
 * keys.
 *
 * Inserting and accessing nodes is O(1).
 *
 * Removing nodes is O(M), where M is the number of the removed node's siblings.
 *
 * Iterating over ancestors is O(D), where D is the number of ancestors of the
 * given node.
 *
 * Iterating over descendants is O(N), where N is the number of descendants of
 * the given node.
 */
template <typename Key, typename T>
class Tree {
 protected:
  template <bool Const>
  class ChainView;

  template <bool Const>
  class DescendantView;

  template <bool Const>
  class ChildView;

  template <bool Const>
  class RootView;

  template <bool Const>
  class Iterator;

 public:
  using const_chain_view = ChainView<true>;
  using chain_view = ChainView<false>;
  using const_descendant_view = DescendantView<true>;
  using descendant_view = DescendantView<false>;
  using const_child_view = ChildView<true>;
  using child_view = ChildView<false>;
  using const_root_view = RootView<true>;
  using root_view = RootView<false>;

  enum class SearchType { kDepthFirstSearch, kBreadthFirstSearch };

  /**
   * Returns whether the tree is empty.
   */
  bool empty() const { return nodes().empty(); }

  /**
   * Returns the number of nodes in the tree.
   */
  size_t size() const { return nodes().size(); }

  /**
   * Returns a flattened map of all the nodes in the tree.
   */
  const std::unordered_map<Key, T>& nodes() const { return nodes_; }
  const std::unordered_map<Key, T>& values() const { return nodes_; }

  /**
   * Returns the roots (nodes without parents).
   *
   * Search is O(N), where N is the number of nodes in the tree.
   */
  const_root_view roots() const { return const_root_view(this); }
  root_view roots() { return root_view(this); }

  /**
   * Returns the node at the given key.
   *
   * Access is O(1).
   */
  const T& at(const Key& id) const { return nodes_.at(id); }
  T& at(const Key& id) { return nodes_.at(id); }

  /**
   * Returns an iterator over all the ancestors of the given node, from the
   * given node (inclusive) up to the root.
   *
   * Dereferencing the iterator returns a (key, value) pair.
   *
   * @code
   * Tree<int, char> tree;
   * tree.insert(0, 'a');
   * tree.insert_child(0, 1, 'b');
   * tree.insert_child(0, 2, 'c');
   * for (const std::pair<int, char>& key_val : tree.ancestors(2)) {
   *   std::cout << key_val.first << ": " << key_val.second << std::endl;
   * }
   *
   * // Expected output:
   * // >>> 2: c
   * // >>> 0: a
   * @endcode
   */
  chain_view ancestors(const Key& id) { return chain_view(this, id); }

  /**
   * Returns a const iterator over all the ancestors of the given node, from the
   * given node (inclusive) up to the root.
   *
   * @see ancestors(const Key&)
   */
  const_chain_view ancestors(const Key& id) const {
    return const_chain_view(this, id);
  }

  /**
   * Returns an iterator over all the descendants of the given node, from the
   * given node (inclusive) to all the leaves found with depth first search.
   *
   * Dereferencing the iterator returns a (key, value) pair.
   *
   * @code
   * Tree<int, char> tree;
   * tree.insert(0, 'a');
   * tree.insert_child(0, 1, 'b');
   * tree.insert_child(0, 2, 'c');
   * for (const std::pair<int, char>& key_val : tree.descendants(0)) {
   *   std::cout << key_val.first << ": " << key_val.second << std::endl;
   * }
   *
   * // Expected output:
   * // >>> 0: a
   * // >>> 1: b
   * // >>> 2: c
   * @endcode
   *
   * @param id Key to start descendants search.
   * @param search_type Whether to use breadth first or depth first search.
   * @param max_depth Maximum depth to search.
   */
  descendant_view descendants(
      const Key& id, SearchType search_type = SearchType::kBreadthFirstSearch,
      size_t max_depth = std::numeric_limits<size_t>::max()) {
    return descendant_view(this, id, search_type, max_depth);
  }

  /**
   * Returns a const iterator over all the ancestors of the given node, from the
   * given node (inclusive) up to the root.
   *
   * @see descendants(const Key&)
   */
  const_descendant_view descendants(
      const Key& id, SearchType search_type = SearchType::kBreadthFirstSearch,
      size_t max_depth = std::numeric_limits<size_t>::max()) const {
    return const_descendant_view(this, id, search_type, max_depth);
  }

  /**
   * Returns an iterator over all the nodes of the tree, in order of
   * depth/breadth first search.
   *
   * @param search_type Whether to use breadth first or depth first search.
   * @param max_depth Maximum depth to search.
   * @see descendants(const Key&)
   */
  descendant_view nodes(SearchType search_type,
                        size_t max_depth = std::numeric_limits<size_t>::max()) {
    return descendant_view(this, search_type, max_depth);
  }

  /**
   * Returns a const iterator over all the nodes of the tree, in order of
   * depth/breadth first search.
   *
   * @param id Key to start descendants search.
   * @param bfs Whether to use breadth first or depth first search.
   * @param max_depth Maximum depth to search.
   * @see descendants(const Key&)
   */
  const_descendant_view nodes(
      SearchType search_type,
      size_t max_depth = std::numeric_limits<size_t>::max()) const {
    return const_descendant_view(this, search_type, max_depth);
  }

  /**
   * Returns whether the tree contains a node at the given key.
   *
   * Returns in O(1) time.
   */
  bool contains(const Key& id) const {
    return nodes().find(id) != nodes().end();
  }

  /**
   * Returns the depth of the node in the tree, where 0 is the depth of a root
   * node.
   *
   * Returns in O(d) time, where d is the depth of the tree.
   */
  size_t depth(const Key& id) const {
    const std::optional<Key>& id_parent = parent(id);
    return id_parent ? depth(*id_parent) + 1 : 0;
  }

  /**
   * Inserts a new node without a parent.
   *
   * Insertion is O(1).
   */
  void insert(const Key& id, const T& value) {
    if (contains(id)) {
      throw std::invalid_argument("Tree::insert_child(): Key already exists.");
    }
    nodes_[id] = value;
    parents_[id].reset();
    children_[id].clear();
  }

  /**
   * Inserts a new node without a parent.
   *
   * @see insert(const Key&, const T&)
   */
  void insert(const Key& id, T&& value) {
    if (contains(id)) {
      throw std::invalid_argument("Tree::insert_child(): Key already exists.");
    }
    nodes_[id] = std::move(value);
    parents_[id].reset();
    children_[id].clear();
  }

  /**
   * Inserts a new node with the given parent.
   *
   * Throws an error if a node with the same id already exists.
   *
   * Insertion is O(1).
   */
  void insert_child(const Key& id_parent, const Key& id, const T& value) {
    if (contains(id)) {
      throw std::invalid_argument("Tree::insert_child(): Key already exists.");
    }
    nodes_[id] = value;
    parents_[id] = id_parent;
    children_[id].clear();
    children_.at(id_parent).push_back(id);
  }

  /**
   * Inserts a new node with the given parent.
   *
   * @see insert_child(const Key&, const Key&, const T&)
   */
  void insert_child(const Key& id_parent, const Key& id, T&& value) {
    if (contains(id)) {
      throw std::invalid_argument("Tree::insert_child(): Key already exists.");
    }
    nodes_[id] = std::move(value);
    parents_[id] = id_parent;
    children_[id].clear();
    children_.at(id_parent).push_back(id);
  }

  /**
   * Returns the parent of the given node (the root node will have an empty
   * optional).
   *
   * Lookup is O(1).
   */
  const std::optional<Key>& parent(const Key& id) const {
    return parents_.at(id);
  }

  /**
   * Returns an iterator over the children of the given node.
   *
   * Dereferencing the iterator returns a (key, value) pair.
   *
   * @code
   * Tree<int, char> tree;
   * tree.insert(0, 'a');
   * tree.insert_child(0, 1, 'b');
   * tree.insert_child(0, 2, 'c');
   * for (const std::pair<int, char>& key_val : tree.children(0)) {
   *   std::cout << key_val.first << ": " << key_val.second << std::endl;
   * }
   *
   * // Expected output:
   * // >>> 1: b
   * // >>> 2: c
   * @endcode
   *
   * Lookup is O(1).
   */
  child_view children(const Key& id) { return child_view(this, id); }

  const_child_view children(const Key& id) const {
    return const_child_view(this, id);
  }

  /**
   * Clears the parent of the given node.
   *
   * This function is O(M), where M is the number of siblings of the given node.
   */
  void clear_parent(const Key& id) {
    // Remove current node from parent's children.
    const std::optional<Key>& id_parent_old = parents_.at(id);
    if (id_parent_old) {
      std::vector<Key>& siblings = children_.at(*id_parent_old);
      auto it = std::find(siblings.begin(), siblings.end(), id);
      if (it != siblings.end()) {
        siblings.erase(it);
      }
    }

    // Reset parent of current node.
    parents_.at(id).reset();
  }

  /**
   * Clears the children of the given node.
   *
   * This function is O(M), where M is the number of children of the given node.
   */
  void clear_children(const Key& id) {
    // Reset parents of children
    for (const Key& id_child : children_.at(id)) {
      parents_.at(id_child).reset();
    }

    // Reset children of current node
    children_.at(id).clear();
  }

  /**
   * Sets the parent of the given node and clears any previous parent.
   *
   * This function is O(M), where M is the number of siblings of the given node.
   */
  void set_parent(const Key& id, const Key& id_parent) {
    // Clear previous parent
    clear_parent(id);

    // Add new parent
    parents_.at(id) = id_parent;
    children_.at(id_parent).push_back(id);
  }

  /**
   * Adds a child to the given node and clears the child's previous parent.
   *
   * This function is O(M), where M is the number of children of the given node.
   */
  void add_child(const Key& id, const Key& id_child) {
    return set_parent(id_child, id);
  }

  /**
   * Checks whether id_ancestor is an ancestor of id.
   *
   * Nodes are ancestors of themselves.
   *
   * This function is O(D), where D is the depth of the given node.
   */
  bool is_ancestor(const Key& id_ancestor, const Key& id) const {
    for (const auto& key_val : ancestors(id)) {
      if (key_val.first == id_ancestor) return true;
    }
    return false;
  }

  /**
   * Checks whether id_descendant is a descendant of id.
   *
   * Nodes are descendants of themselves.
   *
   * This function is O(D), where D is the depth of the descendant node.
   */
  bool is_descendant(const Key& id_descendant, const Key& id) const {
    return is_ancestor(id, id_descendant);
  }

  /**
   * Prints the tree with tab-indented nodes using depth first search.
   *
   * @param os Stream to print to.
   * @param StringifyValue Functions that converts the node value to a string.
   */
  void printf(std::ostream& os,
              const std::function<std::string(const Key& key, const T& val)>&
                  StringifyValue) const {
    for (const auto& key_val : nodes(SearchType::kDepthFirstSearch)) {
      const Key& key = key_val.first;
      for (size_t i = 0; i < depth(key); i++) {
        os << "  ";
      }
      os << "- " << key << ": " << StringifyValue(key, key_val.second) << std::endl;
    }
    os << std::endl;
  }

  /**
   * Prints the tree with tab-indented nodes using depth first search.
   */
  friend std::ostream& operator<<(std::ostream& os, const Tree<Key, T>& tree) {
    for (const auto& key_val : tree.nodes(SearchType::kDepthFirstSearch)) {
      const Key& key = key_val.first;
      for (size_t i = 0; i < tree.depth(key); i++) {
        os << "  ";
      }
      os << key << ": " << key_val.second << std::endl;
    }
    os << std::endl;
    return os;
  }

 protected:
  /**
   * Flat map of all the nodes in the tree.
   */
  std::unordered_map<Key, T> nodes_;

  /**
   * Mapping from each node to its (optional) parent.
   */
  std::unordered_map<Key, std::optional<Key>> parents_;

  /**
   * Mapping from each node to its children.
   */
  std::unordered_map<Key, std::vector<Key>> children_;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::ChainView {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  class iterator;

  ChainView(TreeT* tree, const Key& id) : tree_(tree), id_(id) {}

  iterator begin() const { return iterator(tree_, id_); }
  iterator end() const { return iterator(tree_, std::optional<Key>()); }

 protected:
  TreeT* tree_;
  std::optional<Key> id_;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::ChainView<Const>::iterator {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;
  using ValueT = std::conditional_t<Const, const std::pair<const Key, T>,
                                    std::pair<const Key, T>>;

  // Iterator traits
  using iterator_category = std::forward_iterator_tag;
  using value_type = ValueT;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  // Constructor
  iterator(TreeT* tree, const std::optional<Key>& id) : tree_(tree), id_(id) {}

  // Forward iterator
  iterator& operator++() {
    id_ = tree_->parents_.at(*id_);
    return *this;
  }

  iterator operator++(int) {
    iterator it(*this);
    operator++();
    return it;
  }

  reference operator*() const { return *tree_->nodes_.find(*id_); }

  pointer operator->() const { return tree_->nodes_.find(*id_).operator->(); }

  bool operator==(const iterator& other) const {
    return tree_ == other.tree_ && id_ == other.id_;
  }

  bool operator!=(const iterator& other) const { return !(*this == other); }

 protected:
  TreeT* tree_;
  std::optional<Key> id_;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::Iterator {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;
  using ValueT = std::conditional_t<Const, const std::pair<const Key, T>,
                                    std::pair<const Key, T>>;

  // Iterator traits
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueT;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  // Constructor
  Iterator(TreeT* tree, const std::vector<Key>* ids, size_t idx_id)
      : tree_(tree), ids_(ids), idx_id_(idx_id) {}

  // Forward iterator
  Iterator& operator++() { return operator+=(1); }

  Iterator operator++(int) {
    Iterator it(*this);
    operator++();
    return it;
  }

  reference operator*() const { return *tree_->nodes_.find((*ids_)[idx_id_]); }

  pointer operator->() const {
    return tree_->nodes_.find((*ids_)[idx_id_]).operator->();
  }

  bool operator==(const Iterator& rhs) const {
    return tree_ == rhs.tree_ && ids_ == rhs.ids_ && idx_id_ == rhs.idx_id_;
  }

  bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

  // Bidirectional iterator
  Iterator& operator--() { return operator+=(-1); }

  Iterator operator--(int) {
    Iterator it(*this);
    operator--();
    return it;
  }

  // Random access iterator
  Iterator& operator+=(difference_type n) {
    idx_id_ += n;
    return *this;
  }

  Iterator operator+(difference_type n) const {
    Iterator it(*this);
    return it += n;
  }

  Iterator& operator-=(difference_type n) { return operator+=(-n); }

  Iterator operator-(difference_type n) const { return operator+(-n); }

  difference_type operator-(const Iterator& rhs) const {
    return idx_id_ - rhs.idx_id_;
  }

  value_type operator[](difference_type n) const { return *(operator+(n)); }

  bool operator<(const Iterator& rhs) const {
    return tree_ == rhs.tree_ && ids_ == rhs.ids_ && idx_id_ < rhs.idx_id_;
  }

  bool operator<=(const Iterator& rhs) const {
    return tree_ == rhs.tree_ && ids_ == rhs.ids_ && idx_id_ <= rhs.idx_id_;
  }

  bool operator>(const Iterator& rhs) const { return rhs < *this; }

  bool operator>=(const Iterator& rhs) const { return rhs <= *this; }

 protected:
  TreeT* tree_ = nullptr;
  const std::vector<Key>* ids_ = nullptr;
  size_t idx_id_ = 0;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::DescendantView {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  using iterator = Tree<Key, T>::Iterator<Const>;

  DescendantView(TreeT* tree, const Key& id, SearchType search_type,
                 size_t max_depth)
      : tree_(tree) {
    void (DescendantView::*AddDescendants)(const Key&, size_t) =
        search_type == SearchType::kBreadthFirstSearch
            ? &DescendantView::AddDescendantsBfs
            : &DescendantView::AddDescendantsDfs;

    ids_.reserve(tree_->size());
    (this->*AddDescendants)(id, max_depth);
  }

  DescendantView(TreeT* tree, SearchType search_type, size_t max_depth)
      : tree_(tree) {
    void (DescendantView::*AddDescendants)(const Key&, size_t) =
        search_type == SearchType::kBreadthFirstSearch
            ? &DescendantView::AddDescendantsBfs
            : &DescendantView::AddDescendantsDfs;

    ids_.reserve(tree_->size());
    for (const auto& key_val : tree->roots()) {
      (this->*AddDescendants)(key_val.first, max_depth);
    }
  }

  iterator begin() const { return iterator(tree_, &ids_, 0); }
  iterator end() const { return iterator(tree_, &ids_, ids_.size()); }

 protected:
  void AddDescendantsDfs(const Key& id, size_t max_depth) {
    AddDescendantsDfsDepth(id, max_depth, 0);
  }

  void AddDescendantsDfsDepth(const Key& id, size_t max_depth, size_t depth) {
    ids_.push_back(id);
    if (depth >= max_depth) return;
    for (const Key& id_child : tree_->children_.at(id)) {
      AddDescendantsDfsDepth(id_child, max_depth, depth + 1);
    }
  }

  void AddDescendantsBfs(const Key& id, size_t max_depth) {
    std::queue<std::pair<size_t, Key>> bfs;
    bfs.emplace(0, id);
    while (!bfs.empty()) {
      const size_t depth = bfs.front().first;
      const Key& id_current = bfs.front().second;
      ids_.push_back(id_current);
      bfs.pop();
      if (depth >= max_depth) continue;
      for (const Key& id_child : tree_->children_.at(id_current)) {
        bfs.emplace(depth + 1, id_child);
      }
    }
  }

  TreeT* tree_;
  std::vector<Key> ids_;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::ChildView {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  using iterator = Tree<Key, T>::Iterator<Const>;

  ChildView(TreeT* tree, const Key& id)
      : tree_(tree), ids_(&tree->children_.at(id)) {}

  iterator begin() const { return iterator(tree_, ids_, 0); }
  iterator end() const { return iterator(tree_, ids_, ids_->size()); }

 protected:
  TreeT* tree_;
  const std::vector<Key>* ids_;
};

template <typename Key, typename T>
template <bool Const>
class Tree<Key, T>::RootView {
 public:
  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  using iterator = Tree<Key, T>::Iterator<Const>;

  explicit RootView(TreeT* tree) : tree_(tree), ids_(FindRoots(tree)) {}

  iterator begin() const { return iterator(tree_, &ids_, 0); }
  iterator end() const { return iterator(tree_, &ids_, ids_.size()); }

 protected:
  std::vector<Key> FindRoots(TreeT* tree) {
    std::vector<Key> roots;
    for (const auto& key_val : tree->nodes()) {
      const Key& key = key_val.first;
      if (!tree->parent(key)) roots.push_back(key);
    }
    return roots;
  }

  TreeT* tree_;
  std::vector<Key> ids_;
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_TREE_H_
