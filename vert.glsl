#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 pass_normal;
out vec2 pass_uv;

out float zdistance;

out vec3 camera_position;

out vec3 to_light_vector;

uniform mat4 projection_m;
uniform mat4 model_m;
uniform mat4 view_m;


uniform vec3 light_position;


void main()
{
	vec4 world_position = model_m * vec4(pos, 1.0);
	vec4 camera_space = view_m * world_position;
	vec4 projected = projection_m * camera_space; 
    gl_Position = projected;
	
	to_light_vector = light_position - world_position.xyz;
	
	camera_position = (view_m * vec4(0.0, 0.0, 0.0, 1.0)).xyz * (-1.0);
	
	pass_normal = (model_m * vec4(normal, 0.0)).xyz;
	pass_uv = uv;
	zdistance = projected.z;
}
