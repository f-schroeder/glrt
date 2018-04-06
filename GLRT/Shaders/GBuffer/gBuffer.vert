#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0f);

out vec4 passPosition;
out vec4 passNormal;
out vec2 passUVCoord;
 
void main(){
    passUVCoord = uv;

    passPosition = modelMatrix * position;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    passNormal = transpose(inverse(modelMatrix)) * normal;
}
