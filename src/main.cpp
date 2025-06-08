// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <cstdint>
#include "tree.h"

void printPermutations(const std::vector<char>& elements) {
  PMTree tree(elements);
  auto perms = getAllPerms(tree);

  std::cout << "All permutations for [";
  for (size_t i = 0; i < elements.size(); ++i) {
    std::cout << elements[i];
    if (i != elements.size() - 1) std::cout << " ";
  }
  std::cout << "]:\n";

  for (const auto& perm : perms) {
    for (char c : perm) std::cout << c;
    std::cout << " ";
  }
  std::cout << "\n\n";
}

void testSpecificPermutations(const std::vector<char>& elements) {
  PMTree tree(elements);

  std::cout << "Testing specific permutations for [";
  for (size_t i = 0; i < elements.size(); ++i) {
    std::cout << elements[i];
    if (i != elements.size() - 1) std::cout << " ";
  }
  std::cout << "]:\n";

  int total_perm = tree.getTotalPermutations();
  for (int i = 1; i <= std::min(6, total_perm); ++i) {
    auto perm1 = getPerm1(tree, i);
    auto perm2 = getPerm2(tree, i);

    std::cout << "Permutation #" << i << ": ";
    std::cout << "Method1: ";
    for (char c : perm1) std::cout << c;
    std::cout << ", Method2: ";
    for (char c : perm2) std::cout << c;
    std::cout << "\n";
  }
  std::cout << "\n";
}

void performanceExperiment() {
  std::cout << "Performance experiment (n up to 10):\n";
  std::cout << "n\tgetAllPerms (µs)\tgetPerm1 (µs)\tgetPerm2 (µs)\n";

  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<int> n_values;
  std::vector<int64_t> all_perms_times;
  std::vector<int64_t> perm1_times;
  std::vector<int64_t> perm2_times;

  for (int n = 1; n <= 10; ++n) {
    std::vector<char> elements;
    for (int i = 1; i <= n; ++i) {
      elements.push_back('0' + i);
    }
    PMTree tree(elements);
    int total_perms = tree.getTotalPermutations();

    std::uniform_int_distribution<> distr(1, total_perms);
    std::vector<int> random_nums;
    for (int i = 0; i < 3 && total_perms > 0; ++i) {
      random_nums.push_back(distr(gen));
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto all_perms = getAllPerms(tree);
    auto end = std::chrono::high_resolution_clock::now();
    auto all_perms_time =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    int64_t perm1_time = 0;
    if (!random_nums.empty()) {
      start = std::chrono::high_resolution_clock::now();
      for (int num : random_nums) {
        getPerm1(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm1_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        / random_nums.size();
    }

    int64_t perm2_time = 0;
    if (!random_nums.empty()) {
      start = std::chrono::high_resolution_clock::now();
      for (int num : random_nums) {
        getPerm2(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm2_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        / random_nums.size();
    }

    std::cout << n << "\t" << all_perms_time
      << "\t\t\t" << perm1_time
      << "\t\t\t" << perm2_time << "\n";

    n_values.push_back(n);
    all_perms_times.push_back(all_perms_time);
    perm1_times.push_back(perm1_time);
    perm2_times.push_back(perm2_time);
  }

int main() {
  printPermutations({ '1', '2', '3' });
  testSpecificPermutations({ '1', '2', '3' });
  performanceExperiment();
  return 0;
}
