static const std::string FRAGMENT_SHADER_CODE = R"(
#version 330 core
in vec4 fragColor;
out vec4 color;
void main() {
    color = fragColor;
}
)";
