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

auto parse_items(const std::string &content) {
  return content | std::views::split('\n') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); });
}

int part_one(const std::string &content) {
  int result = 0;
  for (const auto &item : parse_items(content)) {}
  return result;
}

int part_two(const std::string &content) {
  int result = 0;
  for (const auto &item : parse_items(content)) {}
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
