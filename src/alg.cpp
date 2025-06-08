// Copyright 2022 NNTU-CS
#include <algorithm>
#include <stdexcept>
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
  for (int i = 1; i <= sorted.size(); ++i) {
    total_permutations *= i;
  }
}
void PMTree::buildTree(std::shared_ptr<Node> parent, const std::vector<char>& remaining) {
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
void PMTree::getAllPermsHelper(std::shared_ptr<Node> node, std::vector<char>& current,
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
void PMTree::getPermHelper1(std::shared_ptr<Node> node, std::vector<char>& current,
  int& counter, int target, std::vector<char>& result) const {
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
  if (root && !root->children.empty()) {
    for (const auto& child : root->children) {
      elements.push_back(child->value);
    }
  }
  std::vector<char> permutation;
  std::vector<char> available = elements;
  int position = num - 1;

  for (int i = elements.size() - 1; i >= 0; --i) {
    int factorial = 1;
    for (int j = 1; j <= i; ++j) {
      factorial *= j;
    }

    int index = position / factorial;
    position %= factorial;

    permutation.push_back(available[index]);
    available.erase(available.begin() + index);
  }
  return permutation;
}
bool PMTree::getPermHelper2(std::shared_ptr<Node> node, std::vector<char>& current,
  int& remaining, std::vector<char>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }
  if (node->children.empty()) {
    if (remaining == 0) {
      result = current;
      return true;
    }
    remaining--;
    return false;
  } else {
    for (const auto& child : node->children) {
      if (getPermHelper2(child, current, remaining, result)) {
        return true;
      }
    }
  }
  if (node->value != '\0') {
    current.pop_back();
  }
  return false;
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
