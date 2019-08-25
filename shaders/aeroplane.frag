#version 410 core

out vec4 final_colour;

in vec3 pass_normal;
in vec2 pass_uv;

in float zdistance;

in vec3 camera_position;

in vec3 to_light_vector;
in vec3 to_camera_vector;

uniform float vreme;

uniform vec3 light_colour;

uniform sampler2D albedo_sampler;
uniform sampler2D normal_sampler;
uniform sampler2D metalness_sampler;


void main()
{

	float shine_damper = 0.4;
	float reflectivity = texture(metalness_sampler, pass_uv).r;
	if(reflectivity < 0.1) shine_damper = 1.0;
	
	float ambient_intensity = 0.3;
	
	vec3 unit_normal = normalize(pass_normal);
	vec3 unit_to_light = normalize(to_light_vector);
	
	float diffuse_intensity = clamp(dot(unit_to_light, unit_normal), ambient_intensity, 1.0);
	
	vec3 unit_vector_to_camera = normalize(to_camera_vector);
	
	vec3 light_direction = -unit_to_light;
	
	vec3 reflected_light_direction = reflect(light_direction, unit_normal);
	
	float specular_factor = dot(reflected_light_direction, unit_vector_to_camera);
	specular_factor = max(specular_factor, 0.0);
	float damped_factor = pow(specular_factor, shine_damper);
	
	vec3 final_specular = damped_factor * reflectivity * light_colour;

	vec3 albedo_colour = texture(albedo_sampler, pass_uv).rgb;
	
	vec3 diffuse_colour = light_colour * diffuse_intensity;
	final_colour = vec4(diffuse_colour, 0.0) * vec4(albedo_colour, 1.0) + vec4(final_specular, 0.0);
	

}