#version 450

layout (location = 0) in vec2 vertex;

out vec2 uvCoord;

void main() {
    uvCoord = vertex * 0.5f + 0.5f;
    gl_Position = vec4(vertex, 0.0f, 1.0f);
}