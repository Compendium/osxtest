#version 120

uniform mat4 umvp;
uniform float udt;
uniform vec4 tint;

attribute vec3 attrib_vertexpos;
attribute vec2 attrib_texpos;

varying vec4 frag_color;
varying vec2 frag_texpos;
varying vec4 frag_tint;

void main()
{
    gl_Position = umvp * vec4(attrib_vertexpos.x, attrib_vertexpos.y , attrib_vertexpos.z, 1.0f);
    gl_FrontColor = gl_Color;
    frag_texpos = attrib_texpos;
	frag_tint = tint;
}