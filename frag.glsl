#version 410 core

out vec4 final_colour;

in vec3 pass_normal;
in vec2 pass_uv;

in float zdistance;

uniform float vreme;

uniform sampler2D albedo;
uniform sampler2D noise;

void main()
{
	//final_colour = vec4(zdistance, zdistance, zdistance, 1.0);
	final_colour = texture(albedo, pass_uv);
	//final_colour = vec4(pass_normal, 1.0);
	//final_colour = vec4(pass_uv.xy, 0.0, 1.0);
}