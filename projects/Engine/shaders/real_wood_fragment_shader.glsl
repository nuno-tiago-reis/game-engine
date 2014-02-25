#version 330 core

#define lightCount 10
#pragma optionNV(unroll all)

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

uniform sampler3D Noise;
uniform float NoiseScale;

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
	
	vec3 scaledPos =  (mat * out_Position * NoiseScale).xyz;
	float snoise = 2.0 * texture(Noise, scaledPos).r - 1.0;
	float r = fract(rfreq * (out_Position.z*NoiseScale) + rescala * snoise);
	float invMax = pow(rsharp, rsharp / (rsharp - 1.0))/(rsharp - 1.0);
	float ring = invMax * (r - pow(r, rsharp));
	float lerp = ring + snoise;
	
	vec4 noiseColor = mix(escuro, claro, lerp);

	out_Color = vec4(0);

	for(int i=0; i<lightCount; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		vec3 Normal = normalize(out_Normal);

		vec4 LightDirection = out_Position - lightSource[i].Position;
		float Distance = length(LightDirection);  
		LightDirection = normalize(LightDirection);

		float LightIntensity = 1.0 / (lightSource[i].ConstantAttenuation + lightSource[i].LinearAttenuation * Distance + lightSource[i].ExponentialAttenuation * Distance * Distance);
		
		vec4 AmbientColor = noiseColor * lightSource[i].Color * lightSource[i].AmbientIntensity;
		vec4 DiffuseColor = noiseColor * lightSource[i].Color * lightSource[i].DiffuseIntensity;
		vec4 SpecularColor = noiseColor * lightSource[i].Color * lightSource[i].SpecularIntensity;

		/*float DiffuseFactor = max(dot(Normal,vec3(-LightDirection)), 0.0);
		DiffuseColor = DiffuseColor * DiffuseFactor;
		float SpecularFactor = 0.0;

		if(DiffuseFactor > 0.0) {

			vec3 ViewDirection = normalize(vec3(-out_Position));
			vec3 HalfDirection = normalize(vec3(LightDirection) + ViewDirection);

			float SpecularAngle = max(dot(ViewDirection, Normal), 0.0);

			SpecularFactor = pow(SpecularAngle, 16.0);
		}*/
		
		float DiffuseFactor = dot(Normal, vec3(-LightDirection));
		DiffuseColor = DiffuseColor * DiffuseFactor;

		if (DiffuseFactor > 0) {

			vec3 VertexToEye = normalize(vec3(out_CameraPosition - out_Position));
			vec3 LightReflect = normalize(reflect(vec3(LightDirection), Normal));    
			                
			float SpecularAngle = dot(VertexToEye, LightReflect);   
			                          
			float SpecularFactor = pow(SpecularAngle, 0);                               
			if(SpecularFactor > 0.0)
				SpecularColor = SpecularColor * SpecularFactor;
		}

		out_Color += AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
	}
}