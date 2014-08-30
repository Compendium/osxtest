#version 120

uniform mat4 umvp;
uniform float udt;
attribute vec3 attrib_vertexpos;
attribute vec2 attrib_vertexuv;
varying vec2 frag_uv;
varying vec3 frag_pos;

void main()
{
    vec4 pos = umvp * vec4(attrib_vertexpos.x, attrib_vertexpos.y/* * sin(udt*0.002 + attrib_vertexpos.x * 0.1)*/, attrib_vertexpos.z, 1.0f);
    //gl_FrontColor = gl_Color;
	gl_Position = pos;
	frag_pos = pos.xyz;
	frag_uv = attrib_vertexuv;
}