#version 330 core

#define lightCount 10

in vec4 vertPos;
in vec4 eyeModel;

in vec3 normalInterp;

in vec4 ambient;
in vec4 color;
in vec4 spec;

in vec4 lightPosition[lightCount];

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

out vec4 out_Color;

void main() {

	out_Color = vec4(0);
	
	for(int i=0; i<lightCount; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		vec3 normal = normalize(normalInterp);

		vec4 LightDirection = vertPos - lightSource[i].Position;
		float Distance = length(LightDirection);  
		LightDirection = normalize(LightDirection);

		vec4 ambientColor = ambient * lightSource[i].Color * lightSource[i].AmbientIntensity;
		vec4 diffuseColor  = vec4(0, 0, 0, 0);                                                 
		vec4 specularColor = vec4(0, 0, 0, 0);

		float DiffuseFactor = dot(normal, vec3(-LightDirection));

		if (DiffuseFactor > 0) {

			diffuseColor = color * lightSource[i].Color * lightSource[i].DiffuseIntensity * DiffuseFactor;

			vec3 VertexToEye = normalize(vec3(eyeModel - vertPos));
			vec3 LightReflect = normalize(reflect(vec3(LightDirection), normal));    
			                
			float SpecularFactor = dot(VertexToEye, LightReflect);   
			                          
			SpecularFactor = pow(SpecularFactor, 255);                               
			if(SpecularFactor > 0.0){
			//SpecularFactor = pow( max(SpecularFactor,0.0), 255);
				specularColor = spec * lightSource[i].Color * lightSource[i].SpecularIntensity * SpecularFactor;
			}
		}

		out_Color += ambientColor + diffuseColor + specularColor;
	}
}
