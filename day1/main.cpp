#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <utility>

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

enum Direction {
  kLeft = 0,
  kRight = 1,
};
using Item = std::pair<Direction, int>;

auto parse_items(const std::string &content) {
  return content | std::views::split('\n') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); }) |
         std::views::transform([](std::string_view sv) -> Item {
           return std::make_pair(
               sv[0] == 'L' ? Direction::kLeft : Direction::kRight,
               stoi(sv.substr(1)));
         });
}

int part_one(const std::string &content) {
  int position = 50;
  int num_zeros = 0;

  auto items = parse_items(content);
  for (const auto &item : items) {
    int num = item.second % 100;
    position += item.first == Direction::kLeft ? -num : num;
    position %= 100;
    if (!position) { ++num_zeros; }
    if (position < 0) { position += 100; }
  }
  return num_zeros;
}

int part_two(const std::string &content) {
  int position = 50;
  int num_zeros = 0;
  auto items = parse_items(content);

  for (const auto &item : items) {
    int div = item.second / 100;
    int mod = item.second % 100;
    if (mod != 0) {
      bool started_at_zero = (position == 0);
      position = (position + (item.first == Direction::kLeft ? -mod : mod));
      if (!started_at_zero && (position <= 0 || position >= 100)) { num_zeros += 1; }
      if (position < 0) {
        position += 100;
      } else if (position >= 100) {
        position -= 100;
      }
    }
    num_zeros += div;
  }
  return num_zeros;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "please provide exactly one argument\n";
    return 1;
  }
  auto content = read_file(argv[1]);
  if (argv[1][0] == '1') {
    std::cout << "part 1: " << part_one(content) << "\n";
  } else {
    std::cout << "part 2:\n" << part_two(content) << "\n";
  }

  return 0;
}
