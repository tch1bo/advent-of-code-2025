#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

std::string read_file(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary);
  auto size = std::filesystem::file_size(path);
  std::string content(size, '\0');
  file.read(content.data(), size);
  return content;
}

auto parse_items(const std::string &content) {
  return content | std::views::split('\n') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); }) |
         std::views::transform([](std::string_view sv) -> std::vector<bool> {
           std::vector<bool> v(sv.length(), false);
           for (int i = 0; i < sv.length(); ++i) { v[i] = (sv[i] == '@'); }
           return v;
         });
}

auto all_neighbours(const std::vector<std::vector<bool>> &map, int i, int j) {
  return std::views::cartesian_product(std::views::iota(-1, 2), std::views::iota(-1, 2)) |
         std::views::transform([i, j](auto t) -> std::pair<int, int> {
           return std::make_pair(i + std::get<0>(t), j + std::get<1>(t));
         }) |
         std::views::filter([&map, i, j](auto p) -> bool {
           return p.first >= 0 && p.first < map.size() && p.second >= 0 &&
                  p.second < map[0].size() && !(p.first == i && p.second == j);
         });
}

bool is_removable(const std::vector<std::vector<bool>> &map, int i, int j) {
  if (!map[i][j]) { return false; }

  int count = 0;
  for (const auto &[ni, nj] : all_neighbours(map, i, j)) {
    if (map[ni][nj]) { ++count; }
  }
  return (count < 4);
}

int part_one(const std::string &content) {
  int result = 0;
  std::vector<std::vector<bool>> map;
  for (auto item : parse_items(content)) { map.emplace_back(item); };

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[0].size(); ++j) { result += int(is_removable(map, i, j)); }
  }
  return result;
}

int part_two(const std::string &content) {
  int result = 0;
  std::vector<std::vector<bool>> map;
  for (auto item : parse_items(content)) { map.emplace_back(item); };

  std::vector<std::pair<int, int>> q;
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[0].size(); ++j) {
      if (is_removable(map, i, j)) { q.emplace_back(i, j); }
    }
  }

  while (!q.empty()) {
    result += q.size();

    std::set<std::pair<int, int>> neighbours;
    for (const auto &[i, j] : q) {
      map[i][j] = false;
      for (const auto &n : all_neighbours(map, i, j)) { neighbours.emplace(n); }
    }
    q.clear();
    for (const auto &[i, j] : neighbours) {
      if (is_removable(map, i, j)) { q.emplace_back(i, j); }
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "please provide exactly one argument\n";
    return 1;
  }
  auto content = read_file(argv[1]);
  if (argv[1][0] == '1') {
    std::cout << part_one(content) << "\n";
  } else {
    std::cout << part_two(content) << "\n";
  }
  return 0;
}
