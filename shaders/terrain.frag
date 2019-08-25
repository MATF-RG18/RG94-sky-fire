#version 410 core

out vec4 final_colour;

in float height;
in vec2 pass_uv;
in vec3 pass_normal;

in vec3 to_light_vector;

in float pass_player_distance;

uniform vec3 light_colour;


void main()
{

	if(pass_player_distance < 2.0)
		discard;
	

	vec3 unit_normal = normalize(pass_normal);
	vec3 unit_to_light = normalize(to_light_vector);
	
	float ambient_intensity = 0.3;
	
	float brightness = clamp(dot(unit_normal, unit_to_light), ambient_intensity, 1.0);
	
	
	
	

	vec3 rock_colour = vec3(0.543, 0.269, 0.074);
	vec3 grass_colour = vec3(0.132, 0.543, 0.132);

	vec3 albedo_colour = grass_colour * (1 - height) + rock_colour * height;
	
	vec3 fc = (light_colour * brightness) * albedo_colour * clamp(height + ambient_intensity, 0.0, 1.0);
	
	final_colour = vec4(fc, 1.0);
	if(pass_player_distance < 2.1)
		final_colour = vec4(0.87, 1.0, 1.0, 1.0);
	
}