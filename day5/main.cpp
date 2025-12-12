#include <algorithm>
#include <charconv>
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

int64_t stoi(std::string_view sv) {
  int64_t value;
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

int64_t part_one(const std::string &content) {
  int64_t result = 0;
  std::vector<std::pair<int64_t, int64_t>> fresh;
  auto is_fresh = [&fresh](int64_t ingredient) -> bool {
    for (const auto &[start, end] : fresh) {
      if (ingredient >= start && ingredient <= end) { return true; }
    }
    return false;
  };
  for (const auto &item : parse_items(content)) {
    std::size_t pos = item.find('-');
    if (pos != std::string::npos) {
      fresh.emplace_back(stoi(item.substr(0, pos)), stoi(item.substr(pos + 1)));
      continue;
    }

    if (is_fresh(stoi(item))) { ++result; }
  }
  return result;
}

int64_t part_two(const std::string &content) {
  int64_t result = 0;
  using P = std::pair<int64_t, int64_t>;
  std::vector<P> V;
  for (const auto &item : parse_items(content)) {
    std::size_t pos = item.find('-');
    if (pos == std::string::npos) { break; }
    V.emplace_back(stoi(item.substr(0, pos)), stoi(item.substr(pos + 1)));
  }
  std::sort(V.begin(), V.end());

  std::vector<bool> merged(V.size(), false);

  for (int i = 0; i < V.size(); ++i) {
    auto [start, end] = V[i];
    for (int j = i + 1; j < V.size(); ++j) {
      if (V[j].first > end) {
        break;
      }
      end = std::max(end, V[j].second);
      ++i;
      // V[i] = merge V[i] with all
    }
    result += end - start + 1;
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
