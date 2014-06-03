#version 120

varying vec4 frag_color;
varying vec2 frag_texpos;
varying vec4 frag_tint;
uniform sampler2D colorMap;

void main()
{
    //gl_FragColor = vec4(1.0);//gl_Color;
    //if(texture2D(colorMap, frag_texpos).a == 0) discard;
    gl_FragColor = frag_tint * texture2D(colorMap, frag_texpos);
}