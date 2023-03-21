#shader vertexShader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 oTexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    oTexCoord = aTexCoord;
}

/*-------------------------------------------------------------------------------*/

#shader fragmentShader
#version 330 core

in vec2 oTexCoord;

uniform sampler2D uTexture;

out vec4 oFragColor;

void main()
{
    oFragColor = texture(uTexture, oTexCoord);
}