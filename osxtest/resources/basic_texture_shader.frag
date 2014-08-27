#version 120

varying vec2 frag_uv;
uniform sampler2D tex_0;

void main()
{
    gl_FragColor = texture2D(tex_0, frag_uv);
}
