#version 450

in vec4 passPosition;
in vec4 passNormal;
in vec2 passUVCoord;

uniform vec4 diffuseColor = vec4(1,0,1,1);

uniform bool useColorTexture = false;
uniform sampler2D colortexture;

uniform uint materialIndex = uint(0);

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;
 
void main(){  
    positionOutput = passPosition;
	positionOutput.w = float(materialIndex);
    normalOutput = (gl_FrontFacing ? 1.0f : -1.0f) * normalize(passNormal);
	colorOutput = useColorTexture ? texture(colortexture, passUVCoord) : diffuseColor;
}
