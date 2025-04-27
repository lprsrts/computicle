static const std::string VERTEX_SHADER_CODE = R"(
#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 MVP;
out vec4 fragColor;
void main() {
    gl_Position = MVP * vec4(position.xy, 0.0, 1.0);
    gl_PointSize = 2.0;
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
