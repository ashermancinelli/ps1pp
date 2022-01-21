#include <fmt/color.h>
#include <fmt/format.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <cstdlib>
#include <ghc/filesystem.hpp>
#include <string>
namespace fs = ghc::filesystem;

static constexpr std::size_t path_max = 1024;

static std::string get_machine_name()
{
  struct utsname buffer;

  if (uname(&buffer) < 0) {
    std::exit(EXIT_FAILURE);
  }

  return buffer.nodename;
}

static std::string get_user_name()
{
  char name[path_max];
  getlogin_r(name, path_max);
  return std::string{name};
}

static std::string get_dir()
{
  char dir[path_max];
  getcwd(dir, path_max);

  char* _home = getenv("HOME");
  if (nullptr == _home) {
    const auto d = fs::path(dir).string();
    return d;
  } else {
    const auto home = fs::canonical(fs::path(_home)).string();
    auto cwd = fs::canonical(fs::path(dir)).string();
    const auto is_under_home =
        std::equal(home.begin(), home.end(), cwd.begin());
    if (is_under_home) {
      cwd.replace(0, home.size(), "~");
      return cwd;
    }
    return cwd;
  }
}

int main()
{
  const auto mach = get_machine_name();
  const auto dir = get_dir();
  const auto name = get_user_name();
  fmt::print("{} on {} in {}\n $ ",
             fmt::format(fg(fmt::color::cyan), "{}", name),
             fmt::format(fg(fmt::color::yellow), "{}", mach),
             fmt::format(fg(fmt::color::light_green), "{}", dir));
  return 0;
}
