// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include "tree.h"

void savePerformanceDataToFile(
    const std::vector<int>& n_values,
    const std::vector<int64_t>& all_perms_times,
    const std::vector<int64_t>& perm1_times,
    const std::vector<int64_t>& perm2_times) {
  std::ofstream data_file("performance_data.dat");

  data_file << "# n\tgetAllPerms\tgetPerm1\tgetPerm2\n";
  for (size_t i = 0; i < n_values.size(); ++i) {
    data_file << n_values[i] << "\t"
              << all_perms_times[i] << "\t"
              << perm1_times[i] << "\t"
              << perm2_times[i] << "\n";
  }
  data_file.close();
}

void generatePlot() {
  std::ofstream plot_file("plot_script.gp");

  plot_file << "set terminal pngcairo enhanced font 'Verdana,10'\n";
  plot_file << "set output 'performance_plot.png'\n";
  plot_file << "set title 'Permutation Generation Performance'\n";
  plot_file << "set xlabel 'Number of elements (n)'\n";
  plot_file << "set ylabel 'Time (microseconds)'\n";
  plot_file << "set grid\n";
  plot_file << "set key left top\n";
  plot_file << "plot 'performance_data.dat' using 1:2 "
            << "with linespoints title 'getAllPerms', \\\n";
  plot_file << "     'performance_data.dat' using 1:3 "
            << "with linespoints title 'getPerm1', \\\n";
  plot_file << "     'performance_data.dat' using 1:4 "
            << "with linespoints title 'getPerm2'\n";

  plot_file.close();
  if (system("gnuplot plot_script.gp") != 0) {
    std::cerr << "Failed to generate plot\n";
  }
  std::cout << "Plot saved as 'performance_plot.png'\n";
}

void printPermutations(const std::vector<char>& elements) {
  PMTree tree(elements);
  auto perms = getAllPerms(tree);

  std::cout << "All permutations for [";
  for (size_t i = 0; i < elements.size(); ++i) {
    std::cout << elements[i];
    if (i != elements.size()-1) std::cout << " ";
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
    if (i != elements.size()-1) std::cout << " ";
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

    std::vector<int> random_nums;
    if (total_perms > 0) {
      std::uniform_int_distribution<> distr(1, total_perms);
      for (int i = 0; i < 3; ++i) {
        random_nums.push_back(distr(gen));
      }
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto all_perms = getAllPerms(tree);
    auto end = std::chrono::high_resolution_clock::now();
    auto all_perms_time = std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();

    int64_t perm1_time = 0;
    if (!random_nums.empty()) {
      start = std::chrono::high_resolution_clock::now();
      for (int num : random_nums) {
        getPerm1(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm1_time = std::chrono::duration_cast<
          std::chrono::microseconds>(end - start).count() /
          random_nums.size();
    }

    int64_t perm2_time = 0;
    if (!random_nums.empty()) {
      start = std::chrono::high_resolution_clock::now();
      for (int num : random_nums) {
        getPerm2(tree, num);
      }
      end = std::chrono::high_resolution_clock::now();
      perm2_time = std::chrono::duration_cast<
          std::chrono::microseconds>(end - start).count() /
          random_nums.size();
    }

    std::cout << n << "\t" << all_perms_time 
              << "\t\t\t" << perm1_time 
              << "\t\t\t" << perm2_time << "\n";

    n_values.push_back(n);
    all_perms_times.push_back(all_perms_time);
    perm1_times.push_back(perm1_time);
    perm2_times.push_back(perm2_time);
  }

  savePerformanceDataToFile(n_values, all_perms_times,
                          perm1_times, perm2_times);
  generatePlot();
}

int main() {
  printPermutations({'1', '2', '3'});
  testSpecificPermutations({'1', '2', '3'});
  performanceExperiment();
  return 0;
}
