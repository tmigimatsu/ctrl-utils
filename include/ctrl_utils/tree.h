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

#include <map>          // std::map
#include <stack>        // std::stack
#include <type_traits>  // std::conditional_t
#include <utility>      // std::pair

#if __cplusplus >= 201703L
#include <optional>     // std::optional
#else   // __cplusplus
#include <ctrl_utils/optional.h>
#endif  // __cplusplus

namespace ctrl_utils {

template<typename Key, typename T>
class Tree {

 protected:

  template<bool Const>
  class ChainView;

  template<bool Const>
  class DescendantView;

 public:

  using const_chain_view = ChainView<true>;
  using chain_view = ChainView<false>;
  using const_descendant_view = DescendantView<true>;
  using descendant_view = DescendantView<false>;

  const std::map<Key, T>& values() const { return values_; }

  const T& at(const Key& id) const { return values_.at(id); }
  T& at(const Key& id) { return values_.at(id); }

  chain_view ancestors(const Key& id) { return chain_view(this, id); }

  const_chain_view ancestors(const Key& id) const { return const_chain_view(this, id); }

  descendant_view descendants(const Key& id) { return descendant_view(this, id); }

  const_descendant_view descendants(const Key& id) const { return const_descendant_view(this, id); }

  bool contains(const Key& id) const { return values().find(id) != values().end(); }

  void insert_child(const Key& id_parent, const Key& id, const T& value) {
    values_[id] = value;
    id_parents_[id] = id_parent;
  }
  void insert_child(const Key& id_parent, const Key& id, T&& value) {
    values_[id] = std::move(value);
    id_parents_[id] = id_parent;
  }
  void insert(const Key& id, const T& value) {
    values_[id] = value;
    id_parents_[id].reset();
  }
  void insert(const Key& id, T&& value) {
    values_[id] = std::move(value);
    id_parents_[id].reset();
  }

  const std::optional<Key>& parent(const Key& id) const { return id_parents_.at(id); }
  void set_parent(const Key& id, const Key& id_parent) {
    id_parents_[id] = id_parent;
  }
  void clear_parent(const Key& id) {
    id_parents_[id].reset();
  }

  // TODO: Make child_view version
  std::vector<Key> children(const Key& id) const {
    std::vector<Key> children_ids;
    for (const auto& key_val : id_parents_) {
      const Key& id_child = key_val.first;
      const std::optional<Key>& id_parent = key_val.second;
      if (id_parent && *id_parent == id) {
        children_ids.push_back(id_child);
      }
    }
    return children_ids;
  }

  /**
   * Check whether id_ancestor is an ancestor of id.
   *
   * Nodes are ancestors of themselves.
   */
  bool is_ancestor(const Key& id_ancestor, const Key& id) const {
    for (const auto& key_val : ancestors(id)) {
      if (key_val.first == id_ancestor) return true;
    }
    return false;
  }

  /**
   * Check whether id_descendant is a descendant of id.
   *
   * Nodes are descendants of themselves.
   */
  bool is_descendant(const Key& id_descendant, const Key& id) const {
    return is_ancestor(id, id_descendant);
  }

 protected:

  std::map<Key, T> values_;

  std::map<Key, std::optional<Key>> id_parents_;

};

template<typename Key, typename T>
template<bool Const>
class Tree<Key, T>::ChainView {

 public:

  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  class iterator;

  ChainView(TreeT* tree, const Key& id) : tree_(tree), id_(id) {}

  iterator begin() { return iterator(tree_, id_); }
  iterator end() { return iterator(tree_, std::optional<Key>()); }

 protected:

  TreeT* tree_;
  std::optional<Key> id_;

};

template<typename Key, typename T>
template<bool Const>
class Tree<Key, T>::ChainView<Const>::iterator {

 public:

  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;
  using ValueT = std::conditional_t<Const, const std::pair<const Key, T>, std::pair<const Key, T>>;

  using iterator_category = std::input_iterator_tag;
  using value_type = ValueT;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  iterator(TreeT* tree, const std::optional<Key>& id) : tree_(tree), id_(id) {}

  iterator& operator++() { id_ = tree_->id_parents_.at(*id_); return *this; }
  iterator& operator+=(size_t n) { for (size_t i = 0; i < n; i++) ++(*this); return *this; }
  iterator operator+(size_t n) { iterator it(*this); it += n; return it; }
  bool operator==(const iterator& other) const { return tree_ == other.tree_ && id_ == other.id_; }
  bool operator!=(const iterator& other) const { return !(*this == other); }
  reference operator*() const { return *tree_->values_.find(*id_); }
  pointer operator->() const { return tree_->values_.find(*id_).operator->(); }

 protected:

  TreeT* tree_;
  std::optional<Key> id_;

};

template<typename Key, typename T>
template<bool Const>
class Tree<Key, T>::DescendantView {

 public:

  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;

  class iterator;

  DescendantView(TreeT* tree, const Key& id) : tree_(tree) {
    // Collect all descendants, including self
    // ids_.push_back(id);
    // for (const auto& key_val : tree_->id_parents_) {
    //   const Key& id_descendant = key_val.first;
    //   const std::optional<Key>& id_ancestor = key_val.second;
    //   if (id_ancestor && *id_ancestor == id) {
    //     ids_.push_back(id_descendant);
    //   }
    // }
    // return;
    AddChildrenDfs(id);
  }

  iterator begin() { return iterator(tree_, ids_, 0); }
  iterator end() { return iterator(tree_, ids_, ids_.size()); }

 protected:

  std::vector<Key> GetChildren(const Key& id) {
    std::vector<Key> children_ids;
    for (const auto& key_val : tree_->id_parents_) {
      const Key& id_child = key_val.first;
      const std::optional<Key>& id_parent = key_val.second;
      if (id_parent && *id_parent == id) {
        children_ids.push_back(id_child);
      }
    }
    return children_ids;
  }

  void AddChildrenDfs(const Key& id) {
    ids_.push_back(id);
    for (const Key& id_child : GetChildren(id)) {
      AddChildrenDfs(id_child);
    }
  }

  TreeT* tree_;
  std::vector<Key> ids_;

};

template<typename Key, typename T>
template<bool Const>
class Tree<Key, T>::DescendantView<Const>::iterator {

 public:

  using TreeT = std::conditional_t<Const, const Tree<Key, T>, Tree<Key, T>>;
  using ValueT = std::conditional_t<Const, const std::pair<const Key, T>, std::pair<const Key, T>>;

  using iterator_category = std::input_iterator_tag;
  using value_type = ValueT;
  using difference_type = ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  iterator(TreeT* tree, const std::vector<Key>& ids, size_t idx_id) : tree_(tree), ids_(ids), idx_id_(idx_id) {}

  iterator& operator++() { ++idx_id_; return *this; }
  iterator& operator+=(size_t n) { idx_id_ += n; return *this; }
  iterator operator+(size_t n) { iterator it(*this); it += n; return it; }
  bool operator==(const iterator& other) const { return tree_ == other.tree_ && &ids_ == &other.ids_ && idx_id_ == other.idx_id_; }
  bool operator!=(const iterator& other) const { return !(*this == other); }
  reference operator*() const { return *tree_->values_.find(ids_[idx_id_]); }
  pointer operator->() const { return tree_->values_.find(ids_[idx_id_]).operator->(); }

 protected:

  TreeT* tree_;
  const std::vector<Key>& ids_;
  size_t idx_id_;

};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_TREE_H_
