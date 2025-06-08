// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include "tree.h"

void printPermutations(const std::vector<char>& elements) {
  PMTree tree(elements);
  auto perms = getAllPerms(tree);

  std::cout << "All permutations for [";
  for (char c : elements) std::cout << c << " ";
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
  for (char c : elements) std::cout << c << " ";
  std::cout << "]:\n";

  for (int i = 1; i <= 6; ++i) {
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
  std::vector<long> all_perms_times;
  std::vector<long> perm1_times;
  std::vector<long> perm2_times;

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
    auto all_perms_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    start = std::chrono::high_resolution_clock::now();

    for (int num : random_nums) {
      getPerm1(tree, num);
    }
    end = std::chrono::high_resolution_clock::now();
    auto perm1_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 3;

    start = std::chrono::high_resolution_clock::now();
    for (int num : random_nums) {
      getPerm2(tree, num);
    }
    end = std::chrono::high_resolution_clock::now();
    auto perm2_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 3;

    std::cout << n << "\t" << all_perms_time << "\t\t\t" << perm1_time << "\t\t\t" << perm2_time << "\n";

int main() {
  printPermutations({ '1', '2', '3' });
  testSpecificPermutations({ '1', '2', '3' });
  performanceExperiment();
  return 0;
}
