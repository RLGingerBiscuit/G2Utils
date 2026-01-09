#include "UI/RichText.hpp"

#include <optional>
#include <string>

#include <imgui.h>
#include <spdlog/spdlog.h>

namespace UI {

auto OEIRichText(const std::string &str, const Theme &theme) -> void {
  std::optional<ImVec4> run_colour = std::nullopt;
  size_t run_start = 0;
  size_t run_end = 0;

  auto render_run = [](const char *str, size_t start, size_t end,
                       std::optional<ImVec4> colour) {
    if (colour)
      ImGui::PushStyleColor(ImGuiCol_Text, *colour);
    ImGui::TextUnformatted(str + start, str + end);
    ImGui::SameLine(0, 0);
    if (colour)
      ImGui::PopStyleColor();
  };

  for (size_t i = 0; i < str.length(); ++i) {
    auto c = str[i];

    switch (c) {
    case '\n': {
      // Newline
      run_end = i;
      render_run(str.c_str(), run_start, run_end, run_colour);
      ImGui::NewLine();
      run_start = i + 1;
    }; break;

    case '<': {
      // Tag
      run_end = i;
      render_run(str.c_str(), run_start, run_end, run_colour);

      auto tag_start = i;
      auto tag_end = str.find('>', tag_start);
      if (tag_end == std::string::npos)
        goto loop_end;

      run_start = tag_end + 1;

      auto tag_content = str.substr(tag_start + 1, tag_end - tag_start - 1);
      if (tag_content.starts_with("GlobalColor.")) {
        auto colour_name = tag_content.substr(strlen("GlobalColor."));
        if (colour_name == "") {
          spdlog::warn("Empty colour tag");
        }

#define X(name)                                                                \
  else if (colour_name == #name) {                                             \
    run_colour =                                                               \
        ImVec4{theme.name[0], theme.name[1], theme.name[2], theme.name[3]};    \
  }
        THEME_COLOR_TABLE()
#undef X

        else {
          spdlog::warn("Unknown colour tag: {}", colour_name);
          run_colour = std::nullopt;
        }
      } else if (tag_content == "/") {
        run_colour = std::nullopt;
      }

      i = tag_end;
    }; break;
    }
  }

loop_end:

  // Render any remaining text
  if (run_start < str.length())
    render_run(str.c_str(), run_start, str.length(), run_colour);

  ImGui::NewLine();
}

}; // namespace UI
