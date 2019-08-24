#version 410 core

out vec4 final_colour;

in vec3 pass_normal;
in vec2 pass_uv;

in float zdistance;

in vec3 camera_position;

in vec3 to_light_vector;

uniform float vreme;

uniform vec3 light_colour;

uniform sampler2D albedo;
uniform sampler2D noise;

void main()
{
	
	float ambient_intensity = 0.1;
	
	vec3 unit_normal = normalize(pass_normal);
	vec3 unit_to_light = normalize(to_light_vector);
	
	float diffuse_intensity = clamp(dot(unit_to_light, unit_normal) + ambient_intensity, 0.0, 1.0);

	vec3 albedo_colour = texture(albedo, pass_uv).rgb;
	
	vec3 diffuse_colour = light_colour * diffuse_intensity;
	final_colour = vec4(diffuse_colour, 1.0) * vec4(albedo_colour, 1.0);
	

}