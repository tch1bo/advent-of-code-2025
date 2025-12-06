#include <charconv>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

std::string read_file(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary);
  auto size = std::filesystem::file_size(path);
  std::string content(size, '\0');
  file.read(content.data(), size);
  return content;
}

int stoi(std::string_view sv) {
  int value;
  auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

  if (ec != std::errc()) { throw std::string("invalid number"); }
  return value;
}

auto parse_items(const std::string &content) {
  return content | std::views::split('\n') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); }) |
         std::views::transform([](std::string_view sv) -> std::vector<int> {
           std::vector<int> v;
           for (char c : sv) { v.push_back(c - '0'); }
           return v;
         });
}

int part_one(const std::string &content) {
  int result = 0;
  auto items = parse_items(content);
  for (const auto &item : items) {
    int max = item[item.size() - 2] * 10 + item.back();
    for (int i = item.size() - 3; i >= 0; --i) {
      max = std::max(max, std::max(item[i] * 10 + max / 10, item[i] * 10 + max % 10));
    }
    result += max;
  }
  return result;
}

int64_t pow10(int64_t e) { return std::pow(10, e); }

int64_t part_two(const std::string &content) {
  int64_t result = 0;
  auto items = parse_items(content);
  for (const auto &item : items) {
    int64_t max = 0;
    for (int64_t i = 1; i <= 12; ++i) { max += item[item.size() - i] * pow10(i - 1); }

    int64_t mult = pow10(11);
    for (int64_t i = item.size() - 13; i >= 0; --i) {
      int64_t cur_max = 0;
      for (int64_t j = 0; j < 12; ++j) {
        int64_t remainder = max / pow10(j + 1) * pow10(j) + (max % pow10(j));
        int64_t cur = item[i] * mult + remainder;

        cur_max = std::max(cur_max, cur);
      }
      max = std::max(max, cur_max);
    }
    result += max;
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
};
