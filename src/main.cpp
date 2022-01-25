#include <fmt/color.h>
#include <fmt/format.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <cstdlib>
#include <ghc/filesystem.hpp>
#include <optional>
#include <string>
namespace fs = ghc::filesystem;
using namespace std::string_literals;

static constexpr std::size_t path_max = 1024;
static const std::string newline = "\n ⦙";
static constexpr std::size_t max_dir_len = 40;

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
  (void)getcwd(dir, path_max);

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

static std::optional<std::string> get_slurm_string()
{
  char* slurm_id = getenv("SLURM_JOB_ID");
  if (slurm_id != NULL) {
    return fmt::format("{}job {}",
                       newline,
                       fmt::format(fg(fmt::color::rebecca_purple),
                                   "{}",
                                   slurm_id));
  }
  return std::nullopt;
}

static std::string get_git_str()
{
  char cmd[path_max] = "git rev-parse --abbrev-ref HEAD 2> /dev/null";
  FILE* fp = popen(cmd, "r");
  if (!fp) {
    std::exit(EXIT_FAILURE);
  }

  char out[path_max];
  (void)fgets(out, sizeof(out), fp);
  pclose(fp);
  for (char* i = out; i != (out + strlen(out)); i++) {
    if (*i == '\n') {
      *i = 0;
      break;
    }
  }
  return std::string{out};
}

static bool is_git_dir()
{
  auto ec = system("git rev-parse 2>/dev/null 1>/dev/null");
  return (not ec) or fs::exists(fs::path(".git"));
}

[[noreturn]]
static void print_shell_setup(fs::path exe)
{
  exe = fs::absolute(exe);
  fmt::print("__my_ps1() {{ PS1=$({}); }}; export PROMPT_COMMAND=__my_ps1;\n", exe.string());
  std::exit(EXIT_SUCCESS);
}

[[noreturn]]
static void help(std::vector<std::string> args)
{
  fmt::print("To make ps1 your shell, evaluate the output of:\n\t$ {} -c\n", args[0]);
  std::exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
  const auto args = std::vector<std::string>(argv, argv+argc);
  if (args.size() > 1 and args[1] == "-c")
    print_shell_setup(args[0]);

  if (args.size() > 1 and args[1] == "-h")
    help(args);

  const auto mach = get_machine_name();
  auto dir = get_dir();
  if (dir.size() > max_dir_len) {
    static const auto sz = dir.size();
    // Number of characters to keep on each side of the dir
    static constexpr std::size_t chars_to_keep = 5;
    const auto start = dir.find("/", 1);
    const auto last = dir.rfind("/");
    dir.replace(start + 1, last - start - 1, "..."s);
  }
  const auto name = get_user_name();
  auto git_str = std::string{""};
  if (is_git_dir()) {
    git_str = fmt::format(" on {}",
                          fmt::format(fg(fmt::color::light_salmon),
                                      "{}",
                                      get_git_str()));
  }
  const auto slurmid = get_slurm_string();
  fmt::print("{} on {} in {}{}{}\n ⤷  ",
             fmt::format(fg(fmt::color::cyan), "{}", name),
             fmt::format(fg(fmt::color::yellow), "{}", mach),
             fmt::format(fg(fmt::color::light_green), "{}", dir),
             git_str,
             (slurmid.has_value() ? slurmid.value() : ""));
  return 0;
}
