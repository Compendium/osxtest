#version 120

varying vec4 frag_color;
varying vec2 frag_texpos;
uniform sampler2D colorMap;

void main()
{
    //gl_FragColor = vec4(1.0);//gl_Color;
    gl_FragColor = texture2D(colorMap, frag_texpos);
}