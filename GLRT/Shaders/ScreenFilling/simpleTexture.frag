#version 450
#extension GL_ARB_bindless_texture : require

layout(bindless_sampler) uniform sampler2D sfq_texture;

in vec2 uvCoord;
out vec4 fragmentColor;

void main() {
    fragmentColor = texture(sfq_texture, uvCoord);
	//fragmentColor = vec4(uvCoord, 0, 1);
}