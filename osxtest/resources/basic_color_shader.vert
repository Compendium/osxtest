#version 120

uniform mat4 umvp;
uniform float udt;
attribute vec3 attrib_vertexpos;
attribute vec3 attrib_vertexcolor;
varying vec4 frag_color;

void main()
{
    gl_Position = umvp * vec4(attrib_vertexpos.x, attrib_vertexpos.y, attrib_vertexpos.z, 1.0f);
	frag_color = vec4(attrib_vertexcolor.xyz, 1.0f);
}