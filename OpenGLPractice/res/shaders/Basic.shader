#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MPV;

void main() {
    gl_Position = u_MPV * position;
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main(){
    color = u_color;// vec4(.5f, .5f, .5f, .5f);
};