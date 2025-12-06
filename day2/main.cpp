#include <charconv>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <utility>

std::string read_file(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary);
  auto size = std::filesystem::file_size(path);
  std::string content(size, '\0');
  file.read(content.data(), size);
  return content;
}

uint64_t stoi(std::string_view sv) {
  uint64_t value;
  auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

  if (ec != std::errc()) { throw std::string("invalid number"); }
  return value;
}

struct Item {
  uint64_t l_int;
  std::string_view l_str;
  uint64_t r_int;
  std::string_view r_str;
};

auto parse_items(const std::string &content) {
  return content | std::views::split(',') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); }) |
         std::views::transform([](std::string_view sv) -> Item {
           auto index = sv.find('-');
           auto l_str = sv.substr(0, index);
           auto r_str = sv.substr(index + 1);
           return Item{.l_int = stoi(l_str), .l_str = l_str, .r_int = stoi(r_str), .r_str = r_str};
         });
}

uint64_t sum_invalid_numbers(
    int num_reps,
    int seq_len,
    uint64_t min,
    uint64_t max,
    std::unordered_set<uint64_t> &seen) {
  uint64_t result = 0;

  uint64_t min_sequence = std::pow(10, seq_len - 1);
  uint64_t max_sequence = std::pow(10, seq_len);
  for (uint64_t seq = min_sequence; seq < max_sequence; ++seq) {
    uint64_t num = 0;
    for (int i = 0; i < num_reps; ++i) { num += seq * std::pow(10, i * seq_len); }
    if (min <= num && num <= max) {
      if (seen.insert(num).second) { result += num; }
    }
  }

  return result;
}

uint64_t part_one(const std::string &content) {
  uint64_t result = 0;
  auto items = parse_items(content);
  for (const auto &item : items) {
    std::unordered_set<uint64_t> seen;
    for (int length = item.l_str.length(); length <= item.r_str.length(); ++length) {
      if (length % 2 == 0) {
        result += sum_invalid_numbers(2, length / 2, item.l_int, item.r_int, seen);
      }
    }
  }
  return result;
}

uint64_t part_two(const std::string &content) {
  uint64_t result = 0;
  auto items = parse_items(content);
  for (const auto &item : items) {
    std::unordered_set<uint64_t> seen;
    for (int length = item.l_str.length(); length <= item.r_str.length(); ++length) {
      for (int divisor = 1; divisor < length; ++divisor) {
        if (length % divisor == 0) {
          result += sum_invalid_numbers(length / divisor, divisor, item.l_int, item.r_int, seen);
        }
      }
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
