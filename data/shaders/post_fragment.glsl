#version 410

in vec2 frag_texcoord;

out vec4 frag_color;

uniform sampler2D input_texture;

vec4 BoxBlur()
{

}

void main()
{
	frag_color = texture(input_texture, frag_texcoord);
}