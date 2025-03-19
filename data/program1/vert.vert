#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;


smooth out vec4 theColor;

uniform vec4 voxel_color;
uniform mat4 perspectiveMatrix;


void main() {

    vec4 final_pos=vec4(aPos, 1.0);

    gl_Position = final_pos;
    theColor = vec4(color,1.0);
}