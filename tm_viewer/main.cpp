//
// Date       : 26/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/util/common_expr.h"
#include "tm_parse/util/logging.h"

// clang-format off
#include "GLFW/glfw3.h"
#include "gl/GL.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "tm_parse/parser/rules/primary/mod_definition.h"
// clang-format on

using namespace tm_parse;
using namespace tm_parse::logging;

namespace {

GLFWwindow* main_window{nullptr};
ImGuiContext* imgui_context{nullptr};

#define FORMAT(...) (std::format(__VA_ARGS__).c_str())

// clang-format off
struct LogMessage { LogLevel Level; str Message; };
// clang-format on

size_t max_log_messages{1000};
std::vector<LogMessage> log_messages{};
bool messages_added{false};
bool auto_clear{false};

std::string text_editor_content = R"(
create_mod = (
  author = "Test Author",
  version = (1, 0)
)

set $pc baz (
  1,
  0,
  ( A = 10,
    B = 20
  )
)

set $foo $baz

)";

float parse_timeout = 0.0F;
std::unique_ptr<Parser> parser{nullptr};
std::unique_ptr<ParserRule> root_node{nullptr};
std::string selected_node_text{};

void initialise(void);
void terminate(void);
void begin_frame(void);
void end_frame(void);

// TODO: Implement text editor, parse tree, and parse tree selection views

void draw_text_editor(void);
void draw_parse_tree_view(void);
void draw_parse_tree_selection_view(void);
void draw_log_view(void);

void parse_text_content(void);
ImVec4 get_colour_log_level(LogLevel level);

}  // namespace

int main() {
    add_log_callback([](LogLevel level, std::string_view msg, const SrcLoc&) -> void {
        bool has_space = (log_messages.size() + 1) < max_log_messages;

        if (!has_space && auto_clear) {
            log_messages.clear();
        } else if (!has_space) {
            return;
        }

        log_messages.emplace_back(
            level,
            std::format("[{:>5}] {}", get_log_level_name(level, true), msg)
        );
        messages_added = true;
    });

    try {
        initialise();
    } catch (std::exception& e) {
        LOG_ERR("Failed to initialise - {}", e.what());
        return -1;
    }

    LOG_INFO("Starting application loop");
    while (!glfwWindowShouldClose(main_window)) {
        begin_frame();
        ImGui::ShowDemoWindow();

        // In order
        draw_text_editor();
        draw_parse_tree_view();
        draw_parse_tree_selection_view();
        draw_log_view();

        end_frame();

        parse_timeout = std::max(parse_timeout - ImGui::GetIO().DeltaTime, -1.0F);

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

namespace {

void glfw_error_callback(int error, const char* description) {
    LOG_ERR("{}, {}", error, description);
}

void initialise(void) {
    // Mostly copied from: _deps\imgui-src\examples\example_glfw_opengl3\main.cpp
    const char* glsl_version = "#version 330 core";

    if (!glfwInit()) {
        throw std::runtime_error{"Failed to initialize GLFW"};
    }

    text_editor_content.reserve(8192);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    main_window = glfwCreateWindow(1280, 720, "Text Mod Viewer", nullptr, nullptr);
    if (!main_window) {
        throw std::runtime_error{"Failed to create GLFW window"};
    }
    glfwMakeContextCurrent(main_window);
    glfwSwapInterval(1);
    LOG_INFO("GLFW initialised");

    IMGUI_CHECKVERSION();
    imgui_context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0F;
        style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    ImGui_ImplGlfw_InitForOpenGL(main_window, true);
    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        throw std::runtime_error{"Failed to initialize ImGui::OpenGL"};
    }

    LOG_INFO("ImGui initialised");
}

void terminate(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(main_window);
    glfwTerminate();
}

void begin_frame(void) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
}

void end_frame(void) {
    ImGui::Render();
    int display_w{1280};
    int display_h{720};
    glfwGetFramebufferSize(main_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.15F, 0.15F, 0.15F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(main_window);
}

////////////////////////////////////////////////////////////////////////////////
// | DRAW FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

#define FAST_EXIT_BEGIN(title)  \
    if (!ImGui::Begin(title)) { \
        ImGui::End();           \
        return;                 \
    }

void draw_text_editor(void) {
    FAST_EXIT_BEGIN("Text Editor");
    ImGui::BeginChild("##text_editor_controls", ImVec2(0.0F, 0.0F), ImGuiChildFlags_AutoResizeY);

    if (ImGui::Button("Parse")) {
        parse_text_content();
    }

    ImGui::EndChild();

    auto size = ImGui::GetContentRegionAvail();
    ImGui::InputTextMultiline("##text_editor", &text_editor_content, size);

    bool is_ctrl = ImGui::GetIO().KeyCtrl;
    bool parse_key_down = ImGui::IsKeyPressed(ImGuiKey_S) || ImGui::IsKeyPressed(ImGuiKey_R);
    if (is_ctrl && parse_key_down && parse_timeout < 0.0F) {
        parse_text_content();
        parse_timeout = 0.75F;
    }

    ImGui::End();
}

void draw_parse_tree_view(void) {
    FAST_EXIT_BEGIN("Parse Tree");

    if (root_node == nullptr) {
        ImGui::Text("Content has not been parsed");
        ImGui::End();
        return;
    }

    struct TreeNode {
        const ParserRule* Rule;
        bool IsOpen;
    };
    std::deque<TreeNode> parent_stack{};

    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 5.0F);
    root_node->visit([&parent_stack](const ParserRule& node) {
        auto draw_node = [&node, &parent_stack] {
            ImGui::PushID(&node);
            if (ImGui::TreeNodeEx(FORMAT("{}", node.rule_name()))) {
                parent_stack.emplace_back(&node, true);
            } else {
                parent_stack.emplace_back(&node, false);
            }
            ImGui::PopID();

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                LOG_INFO("Updating Selection - {}", node.rule_name());
                // Bit of a hack but text selection can crash if the new size is less than the
                // current. Has to do with selecting text that is outside the new buffers bounds.
                size_t max_size = std::max(selected_node_text.length(), node.full_text().length());
                selected_node_text = std::string{node.full_text()} + '\n';
                selected_node_text.resize(max_size, ' ');
            }
        };

        // pop and close all nodes of a lower depth
        int my_depth = node.get_depth();
        while (!parent_stack.empty() && parent_stack.back().Rule->get_depth() >= my_depth) {
            if (parent_stack.back().IsOpen) {
                ImGui::TreePop();
            }
            parent_stack.pop_back();
        }

        // push root nodes or push child nodes of open nodes
        if (parent_stack.empty() || parent_stack.back().IsOpen) {
            draw_node();
        }
    });

    // any remianing nodes left open need to be cleaned up
    while (!parent_stack.empty()) {
        if (parent_stack.back().IsOpen) {
            ImGui::TreePop();
        }
        parent_stack.pop_back();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

void draw_parse_tree_selection_view(void) {
    FAST_EXIT_BEGIN("Selection Rule Text");
    auto size = ImGui::GetContentRegionAvail();
    ImGui::InputTextMultiline(
        "##selected_node",
        &selected_node_text,
        size,
        ImGuiInputTextFlags_AlwaysOverwrite | ImGuiInputTextFlags_ReadOnly
    );
    ImGui::End();
}

////////////////////////////////////////////////////////////////////////////////
// | LOG VIEW |
////////////////////////////////////////////////////////////////////////////////

void draw_log_view(void) {
    FAST_EXIT_BEGIN("Log");

    // Force the console to flush
    if (messages_added) {
        std::cout << std::flush;
        std::cerr << std::flush;
        messages_added = false;
    }

    if (ImGui::Button("Clear")) {
        log_messages.clear();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Auto-Clear", &auto_clear);

    ImGui::SameLine();
    ImGui::Text(FORMAT(
        "Count: {}/{} Delta: {:.3f} FPS: {:.3f}",
        log_messages.size(),
        max_log_messages,
        ImGui::GetIO().DeltaTime,
        ImGui::GetIO().Framerate
    ));

    ImGui::BeginChild("##error_scroll_view", ImVec2(0.0F, 0.0F), ImGuiChildFlags_Borders);

    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(log_messages.size()));
    while (clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
            const auto& msg = log_messages.at(line_no);
            ImGui::TextColored(get_colour_log_level(msg.Level), msg.Message.c_str());
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

////////////////////////////////////////////////////////////////////////////////
// | HELPERS |
////////////////////////////////////////////////////////////////////////////////

void parse_text_content(void) {
    parser = nullptr;
    root_node = nullptr;

    try {
        parser = std::make_unique<Parser>(text_editor_content);
        root_node = parser->parse();
        LOG_INFO("Content has been parsed successfully");
    } catch (const TokenError& e) {
        e.log_error();
    } catch (const std::exception& e) {
        LOG_ERR("Failed to parse text content - {}", e.what());
    }
}

ImVec4 get_colour_log_level(LogLevel level) {
    // clang-format off
    switch (level) {
        case LogLevel::Info:  return ImVec4{0.0F, 1.0F, 0.0F, 1.0F};
        case LogLevel::Trace: return ImVec4{0.0F, 1.0F, 0.1F, 1.0F};
        case LogLevel::Warn:  return ImVec4{1.0F, 1.0F, 0.0F, 1.0F};
        case LogLevel::Err:   return ImVec4{1.0F, 0.0F, 0.0F, 1.0F};
        default:              return ImVec4{0.0F, 0.0F, 1.0F, 1.0F};
    }
    // clang-format on
}

}  // namespace