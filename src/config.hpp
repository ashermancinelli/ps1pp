#pragma once
#include <fmt/color.h>
#include <fmt/format.h>

/*
 * Edit this file to change the colors and format string used for the prompt.
 *
 * NOTE: the git string will not always be present (eg if the user is not in a
 * git directory), so we need a separate formatting string for it.
 *
 */
static const std::string git_format_str = " on {branch}";
static const std::string format_str = "{name} on {mach} in {dir}{git} ({ec})\n ⤜  ";
static constexpr auto name_c = fmt::color::cyan;
static constexpr auto mach_c = fmt::color::yellow;
static constexpr auto dir_c = fmt::color::light_green;
static constexpr auto git_c = fmt::color::light_salmon;

std::string format_ec(int ec) {
  return fmt::format(fg(ec ? fmt::color::fire_brick : fmt::color::green), "{}", ec);
}
