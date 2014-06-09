#version 120

varying vec4 frag_color;
varying vec2 frag_texpos;
varying vec4 frag_tint;
uniform sampler2D colorMap;

void main()
{
    gl_FragColor = frag_tint * texture2D(colorMap, frag_texpos) + vec4(0.1);
}