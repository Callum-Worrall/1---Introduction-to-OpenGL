#version 410

in vec2 frag_texcoord;

out vec4 frag_color;

uniform sampler2D perlin_texture;

uniform float scale;
uniform float persistence;

uniform float min;
uniform float max;

void main()
{
	
	vec4 red = vec4(1, 0, 0, 1);
	vec4 green = vec4(0, 1, 0, 1);
	vec4 blue = vec4(0, 0, 1, 1);

	vec4 white = vec4(1, 1, 1, 1);
	vec4 black = vec4(0, 0, 0, 1);



	float perlin_sample = texture(perlin_texture, frag_texcoord).r;
	
	//perlin_sample *= (persistence) * (scale / 10);
	perlin_sample -= min;
	perlin_sample /= (max - min);

	//perlin_sample * min;

	if(perlin_sample > 0.5)
	{
		if(perlin_sample > 0.75)
		{
			if(perlin_sample > 0.875)
			{
				if(perlin_sample > 1)
				{
					frag_color = white;
				}
				else
					frag_color = mix(black, white, 0.875);
			}
			else
				frag_color = mix(black, white, 0.75);
		}
		else
			frag_color = mix(black, white, 0.5);
	}
	else
	{
		if(perlin_sample > 0.25 * perlin_sample)
		{
			frag_color = mix(black, white, 0.25);
		}
		else
			frag_color = black;
	}
	frag_color = vec4(perlin_sample);
	frag_color.a = 1;
}