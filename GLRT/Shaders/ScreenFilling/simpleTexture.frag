#version 450

in vec2 passUVCoord;

uniform sampler2D colortexture;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(colortexture, passUVCoord);
	//fragmentColor = vec4( 1, 0, 0, 1);
}