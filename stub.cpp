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

auto split_lines(const std::string &content) {
  return content | std::views::split('\n') | std::views::transform([](auto &&range) {
           return std::string_view(&*range.begin(), std::ranges::distance(range));
         }) |
         std::views::filter([](std::string_view sv) { return !sv.empty(); });
}

int main(int argc, char *argv[]) { return 0; }
