#version 410

in vec4 frag_normal;
in vec4 frag_position;

out vec4 frag_color;

uniform vec3 ambient_light;

uniform vec3 light_dir;
uniform vec3 light_color;

uniform vec3 material_color;

uniform vec3 eye_pos;

uniform float specular_power;

void main()
{

	vec3 N = normalize(frag_normal.xyz);
	vec3 L = normalize(light_dir);

	vec3 ambient = material_color * ambient_light;

	float d = max(0.0, dot(N, -L));
	vec3 diffuse = vec3(d) * light_color * material_color;

	vec3 E = normalize(eye_pos - frag_position.xyz);
	vec3 R = reflect(L, N);

	//float S = dot(R, E);
	float S = max(0, dot(R, E));

	S = pow(S, specular_power);
	vec3 specular = vec3(S) * light_color * material_color;


	frag_color = vec4(ambient + diffuse + specular, 1);








	/////OLD/////

	//PURE WHITE, NO LIGHT
	//frag_color = vec4(1,1,1,1);

	////COLOUR
	//frag_color = frag_normal;

	/////////////




	////WHITE DIRECTIONAL LIGHT
	//// light is facing diagonally down (-1 x, -1 y)
	//vec3 light_dir = vec3(-1, -1, 0);
	//
	////Calculate Normal for the Surface (with swizzle (.xyz))
	//vec3 N = normalize(frag_normal.xyz);
	//
	//float d = dot(N, -light_dir);
	//d = max(d, 0);
	//
	//vec3 final_diffuse = vec3(d);
	//
	//frag_color = vec4(final_diffuse, 1);
}

