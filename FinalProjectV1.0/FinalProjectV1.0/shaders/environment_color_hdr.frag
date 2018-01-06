#version 330 core


struct LightProperty {
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec3 our_color;

uniform vec3 objectColor;
uniform vec3 lightPos[100];
uniform int activeLightNum;
uniform vec3 viewPos;//视点的位置
uniform LightProperty lightProperty;
uniform LightProperty centerLightProperty;
uniform vec3 globalAmbient;
uniform vec3 centerLightPos;
uniform int centerNum;
uniform float exposure; // hdr曝光算法的曝光值

vec3 CalcPointLight(LightProperty lightProperty, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

  const float gamma = 2.2;

  vec3 result;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 norm = normalize(Normal);
    
  for(int i = 0; i < activeLightNum; i++)
      result += CalcPointLight(lightProperty, lightPos[i], norm, FragPos, viewDir);

  result += CalcPointLight(centerLightProperty, centerLightPos, norm, FragPos, viewDir);
  result += globalAmbient;
      
  vec3 hdrColor = result * our_color;
  
  // 曝光色调映射
  vec3 mapped = vec3(1.0) - exp(hdrColor * exposure);
  // Gamma矫正
  mapped = pow(mapped, vec3(1.0/gamma));

  color = vec4(mapped, 1.0);
}


vec3 CalcPointLight(LightProperty lightProperty, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(lightPos - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 15); // can set mat.shininess
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (lightProperty.constant + lightProperty.linear * distance 
      + lightProperty.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = lightProperty.ambient; 
    vec3 diffuse = lightProperty.diffuse * diff;
    vec3 specular = lightProperty.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);

}