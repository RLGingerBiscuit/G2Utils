#include "G2/ItemInfo.hpp"

#include <regex>

static std::regex s_colour_tag_regex(R"(<\/?GlobalColor\.[^>]+>(.+?)<\/>)");
static std::regex s_newline_regex(R"((\r?\n)+)");
static std::regex s_pipe_regex(R"(\|)");

auto ItemInfo::plain_description() -> std::string & {
  if (!m_plain_description) {
    auto description =
        std::regex_replace(m_description, s_colour_tag_regex, "$1");
    description = std::regex_replace(description, s_newline_regex, " ");
    description = std::regex_replace(description, s_pipe_regex, " ");
    m_plain_description = std::move(description);
  }
  return *m_plain_description;
}

auto ItemInfo::plain_description() const -> const std::string & {
  return const_cast<ItemInfo *>(this)->plain_description();
}
