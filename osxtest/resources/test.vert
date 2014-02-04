#version 120

uniform mat4 umvp;
varying vec4 frag_color;

void main()
{
    gl_Position = umvp * gl_Vertex;
    gl_FrontColor = gl_Color;
}