// Copyright 2022 NNTU-CS
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <vector>
#include  "tree.h"

PMTree::PMTree(const std::vector<char>& elements) {
  if (elements.empty()) {
    root = nullptr;
    total_permutations = 0;
    return;
  }
  std::vector<char> sorted = elements;
  std::sort(sorted.begin(), sorted.end());
  root = std::make_shared<Node>('\0');
  buildTree(root, sorted);
  total_permutations = 1;
  for (int i = 1; i <= static_cast<int>(sorted.size()); ++i) {
    total_permutations *= i;
  }
}

void PMTree::buildTree(std::shared_ptr<Node> parent,
  const std::vector<char>& remaining) {
  if (remaining.empty()) return;
  for (size_t i = 0; i < remaining.size(); ++i) {
    auto child = std::make_shared<Node>(remaining[i]);
    parent->children.push_back(child);
    std::vector<char> new_remaining;
    for (size_t j = 0; j < remaining.size(); ++j) {
      if (j != i) {
        new_remaining.push_back(remaining[j]);
      }
    }
    buildTree(child, new_remaining);
  }
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
  std::vector<std::vector<char>> result;
  if (!root) return result;
  std::vector<char> current;
  getAllPermsHelper(root, current, result);
  return result;
}

void PMTree::getAllPermsHelper(std::shared_ptr<Node> node,
  std::vector<char>& current,
  std::vector<std::vector<char>>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }
  if (node->children.empty()) {
    result.push_back(current);
  } else {
    for (const auto& child : node->children) {
      getAllPermsHelper(child, current, result);
    }
  }
  if (node->value != '\0') {
    current.pop_back();
  }
}

std::vector<char> PMTree::getPerm1(int num) const {
  std::vector<char> result;
  if (num < 1 || num > total_permutations) return result;
  std::vector<char> current;
  int counter = 0;
  getPermHelper1(root, current, counter, num, result);
  return result;
}

void PMTree::getPermHelper1(std::shared_ptr<Node> node,
  std::vector<char>& current,
  int& counter,
  int target,
  std::vector<char>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }
  if (node->children.empty()) {
    counter++;
    if (counter == target) {
      result = current;
    }
  } else {
    for (const auto& child : node->children) {
      if (result.empty()) {
        getPermHelper1(child, current, counter, target, result);
      }
    }
  }
  if (node->value != '\0') {
    current.pop_back();
  }
}

std::vector<char> PMTree::getPerm2(int num) const {
  std::vector<char> result;
  if (num < 1 || num > total_permutations) return result;

  std::vector<char> elements;
  if (root) {
    for (const auto& child : root->children) {
      elements.push_back(child->value);
    }
  }

  int n = elements.size();
  if (n == 0) return result;

  std::vector<int> factorials(n);
  factorials[0] = 1;
  for (int i = 1; i < n; ++i) {
    factorials[i] = factorials[i - 1] * i;
  }

  int index = num - 1;
  std::vector<char> available = elements;
  std::vector<char> permutation;

  for (int i = n - 1; i >= 0; --i) {
    int factorial = factorials[i];
    int pos = index / factorial;
    index %= factorial;

    permutation.push_back(available[pos]);
    available.erase(available.begin() + pos);
  }

  return permutation;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  return tree.getAllPerms();
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  return tree.getPerm1(num);
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  return tree.getPerm2(num);
}
