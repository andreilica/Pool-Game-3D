#version 330

uniform sampler2D texture;

in vec3 world_position;
in vec3 world_normal;
in vec2 tex_coord;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ka;
uniform float material_ks;
uniform int material_shininess;
uniform float attenuation_factor;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 colorAfterTexture = texture2D(texture, tex_coord);

	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );

	vec4 ambient_light = material_ka * colorAfterTexture;
	float diffuse_light = material_kd * max(dot(world_normal, L), 0);
	float specular_light = material_ks *  pow ( max(dot(world_normal, H), 0), material_shininess);

	if(diffuse_light <= 0){
		specular_light = 0;
	}

	float distanceToLight = length(light_position - world_position);
	float attenuation = 1.0 / (1.0 + attenuation_factor * pow(distanceToLight, 2));

	vec4 light_color = ambient_light + attenuation * (diffuse_light + specular_light);
	out_color = light_color;
}