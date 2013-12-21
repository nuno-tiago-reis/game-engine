#version 330 core

#define lightCount 10

in vec4 color;
in vec3 normalInterp;
in vec4 ambient;
in vec4 spec;
in vec3 vertPos;

struct LightSource {

	vec4 Position;
	vec4 Direction;

	vec4 Color;

	float CutOff;

	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;

	float ConstantAttenuation;
	float LinearAttenuation;
	float ExponentialAttenuation;

	int lightType;
};

layout(std140) uniform LightSources {

	LightSource lightSource[10];
};

uniform sampler3D Noise;
uniform float noiseScale;

out vec4 out_Color;

void main() {
	vec4 escuro=vec4(0.4, 0.2, 0.07, 1.0);
	vec4 claro=vec4(0.6, 0.3, 0.1, 1.0);
	float rfreq=20;
	float rescala=3.14;
	float rsharp=4352.0;
	
	//rotation matrix
	mat4 mat;
	mat[0]=vec4(0.75,-0.21651,0.625,0);
	mat[1]=vec4(0.43301,0.875,-0.21651,0);
	mat[2]=vec4(-0.5,0.43301,0.75,0);
	mat[3]=vec4(0,0,0,1);
	
	vec3 scaledPos =  (mat * vec4(vertPos,1.0) * noiseScale).xyz;
	float snoise = 2.0 * texture(Noise, scaledPos).r - 1.0;
	float r = fract(rfreq * (vertPos.z*noiseScale) + rescala * snoise);
	float invMax = pow(rsharp, rsharp / (rsharp - 1.0))/(rsharp - 1.0);
	float ring = invMax * (r - pow(r, rsharp));
	float lerp = ring + snoise;
	
	vec4 cor = mix(escuro, claro, lerp);

	out_Color = vec4(0);
	for(int i=0; i<lightCount; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		vec3 ambientColor = vec3(cor) * vec3(lightSource[i].Color) * lightSource[i].AmbientIntensity;
		vec3 diffuseColor = vec3(cor) * vec3(lightSource[i].Color) * lightSource[i].DiffuseIntensity;
		vec3 specularColor = vec3(cor) * vec3(lightSource[i].Color) * lightSource[i].SpecularIntensity;

		vec3 normal = normalize(normalInterp);

		vec3 lightDir = normalize(vec3(lightSource[i].Position) - vertPos);

		float lambertian = max(dot(lightDir,normal), 0.0);

		float specular = 0.0;

		if(lambertian > 0.0) {

			vec3 viewDir = normalize(-vertPos);
			vec3 halfDir = normalize(lightDir + viewDir);
			float specAngle = max(dot(halfDir, normal), 0.0);
			specular = pow(specAngle, 16.0);
		}

		out_Color += vec4(ambientColor + lambertian * diffuseColor + specular * specularColor, 1.0);
	}

	/*
	vec3 lightPos = vec3(1.0,1.0,1.0);
	vec3 ambientColor = vec3(0.1, 0.0, 0.0);
	vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
	vec3 specColor = vec3(1.0, 1.0, 1.0);

	vec3 normal = normalize(normalInterp);
	vec3 lightDir = normalize(lightPos - vertPos);

	float lambertian = max(dot(lightDir,normal), 0.0);
	float specular = 0.0;
	if(lambertian > 0.0) {
		vec3 viewDir = normalize(-vertPos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, 16.0);  
	}
	out_Color = vec4(ambientColor + lambertian * diffuseColor + specular * specColor, 1.0);
	*/
}