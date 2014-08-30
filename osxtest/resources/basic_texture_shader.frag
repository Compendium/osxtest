#version 120

varying vec2 frag_uv;
varying vec3 frag_pos;
uniform sampler2D tex_0;

void main()
{
	
	
	float ndcDepth = frag_pos.z;
	float clipDepth = ndcDepth / 6;
	gl_FragColor = vec4(vec3((clipDepth * 0.5) + 0.5), 1.0);
	
    //gl_FragColor = texture2D(tex_0, frag_uv);
}
