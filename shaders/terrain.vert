#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

uniform mat4 projection_m;
uniform mat4 model_m;
uniform mat4 view_m;

uniform vec3 light_position;

out float height;
out vec2 pass_uv;
out vec3 pass_normal;


out vec3 to_light_vector;

uniform sampler2D height_map;
uniform float height_factor;

uniform vec3 player_position;

out float pass_player_distance;

void main()
{

	float sample_width = 0.01;
	float sample_width2 = sample_width * sample_width;
	
	
	float heightc = texture(height_map, uv).r;
	float heightl = texture(height_map, uv + vec2(-sample_width, 0.0)).r;
	float heightr = texture(height_map, uv + vec2(sample_width, 0.0)).r;
	float heightu = texture(height_map, uv + vec2(0.0, sample_width)).r;
	float heightd = texture(height_map, uv + vec2(0.0, -sample_width)).r;
	
	pass_normal.x = 0.0;
	pass_normal.y = 1.0;
	pass_normal.z = 0.0;
	
	float ldiff = heightc - heightl;
	float ldiff2 = ldiff * ldiff;
	
	float rdiff = heightc - heightr;
	float rdiff2 = rdiff * rdiff;
	
	
	if(heightc < heightl && heightc > heightr)
		pass_normal.x = 1.0;
	
	if(heightc > heightl && heightc < heightr)
		pass_normal.x = -1.0;
		
	if(rdiff2 < sample_width2 && ldiff2 < sample_width2)
		pass_normal.x = 0.0;
	
	

	float udiff = heightc - heightu;
	float udiff2 = udiff * udiff;
	
	float ddiff = heightc - heightd;
	float ddiff2 = ddiff * ddiff;

	
	if(heightc < heightu && heightc > heightd)
		pass_normal.z = 1.0;
	
	if(heightc > heightu && heightc < heightd)
		pass_normal.z = -1.0;
	
	if(ddiff2 < sample_width2 && udiff2 < sample_width2)
		pass_normal.z = 0.0;
	
	
	
	vec3 p2 = pos;
	p2.y = heightc * height_factor;
	
	vec4 world_position = model_m * vec4(p2, 1.0);
	vec4 camera_space = view_m * world_position;
	vec4 projected = projection_m * camera_space; 
    gl_Position = projected;
	
	pass_player_distance = length(player_position - world_position.xyz);
		
	to_light_vector = light_position - world_position.xyz;
	
	pass_normal = (model_m * vec4(pass_normal, 0.0)).xyz;
	
	height = heightc;
	pass_uv = uv;
}
