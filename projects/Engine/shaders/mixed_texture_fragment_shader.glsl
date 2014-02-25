#version 330 core

#define lightCount 10

in vec4 out_Position;
in vec4 out_CameraPosition;

in vec2 out_TextureUV;

in vec3 out_Normal;

in vec4 out_Ambient;
in vec4 out_Diffuse;
in vec4 out_Specular;

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

uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec4 out_Color;

void main() {

	out_Color = vec4(0);
	
	for(int i=0; i<lightCount; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		vec3 Normal = normalize(out_Normal);

		vec4 LightDirection = out_Position - lightSource[i].Position;
		float Distance = length(LightDirection);  
		LightDirection = normalize(LightDirection);

		float LightIntensity = 1.0 / (lightSource[i].ConstantAttenuation + lightSource[i].LinearAttenuation * Distance + lightSource[i].ExponentialAttenuation * Distance * Distance);

		vec4 AmbientColor = out_Ambient * lightSource[i].Color * lightSource[i].AmbientIntensity;
		vec4 DiffuseColor = vec4(0, 0, 0, 0);                                            
		vec4 SpecularColor = vec4(0, 0, 0, 0);
		
		vec4 TextureColor0 = texture2D(Texture0, out_TextureUV);
		vec4 TextureColor1 = texture2D(Texture1, out_TextureUV);       

		vec4 TextureColor = TextureColor0 * 0.5 + TextureColor1 * 0.5;

		float DiffuseFactor = dot(Normal, vec3(-LightDirection));

		if (DiffuseFactor > 0) {

			DiffuseColor = (out_Diffuse + TextureColor) * lightSource[i].Color * lightSource[i].DiffuseIntensity * DiffuseFactor;

			vec3 VertexToEye = normalize(vec3(out_CameraPosition - out_Position));
			vec3 LightReflect = normalize(reflect(vec3(LightDirection), Normal));    
			                
			float SpecularAngle = dot(VertexToEye, LightReflect);   
			                          
			float SpecularFactor = pow(SpecularAngle, 255);                               
			if(SpecularFactor > 0.0)
				SpecularColor = out_Specular * lightSource[i].Color * lightSource[i].SpecularIntensity * SpecularFactor;
		}

		out_Color += AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
	}
}