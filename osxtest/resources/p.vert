#version 120

uniform mat4 umvp;
uniform float udt;
attribute vec3 attrib_vertexpos;
attribute vec3 attrib_vertexcolor;
varying vec4 frag_color;

void main()
{
    gl_Position = umvp * vec4(attrib_vertexpos.x, attrib_vertexpos.y * sin(udt*0.002 + attrib_vertexpos.x * 0.1), attrib_vertexpos.z, 1.0f);
    //gl_FrontColor = gl_Color;
	frag_color = vec4(attrib_vertexcolor.xyz, 1.0f);
}