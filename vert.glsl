#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 pass_normal;
out vec2 pass_uv;

out float zdistance;

uniform mat4 projection_m;
uniform mat4 model_m;
uniform mat4 view_m;


void main()
{
	vec4 world_position = model_m * vec4(pos, 1.0);
	vec4 projected = projection_m * world_position; 
    gl_Position = projected;
	pass_normal = normal;
	pass_uv = uv;
	zdistance = projected.z;
}
