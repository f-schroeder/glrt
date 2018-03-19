#version 450

in vec4 passPosition;
in vec3 passNormal;
in vec2 passUVCoord;

uniform vec4 diffuseColor;

uniform bool useColorTexture;
uniform sampler2D colortexture;

uniform uint materialIndex = uint(0);

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;
 
void main(){  
    positionOutput = passPosition;
	positionOutput.w = float(materialIndex);
    normalOutput = vec4((gl_FrontFacing ? 1.0f : -1.0f) * normalize(passNormal), 1.0f);
	colorOutput = useColorTexture ? texture(colortexture, passUVCoord) : diffuseColor;
}
