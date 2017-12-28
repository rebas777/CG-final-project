#version 330 core


struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 


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

uniform vec3 objectColor;
/*
 动态多光源的处理：
     shader 最多支持50个光源的渲染，少于50个则由 activeLightNum 规约。
*/
uniform vec3 viewPos;//视点的位置
uniform vec3 lightPos[2];
uniform LightProperty lightProperty;
uniform Material material;


vec3 CalcPointLight(LightProperty lightProperty, vec3 lightPos,  Material mat, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

  vec3 result;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 norm = normalize(Normal);
    
  /*for(int i = 0; i < activeLightNum; i++)
      result += CalcPointLight(lightProperty, lightPos[i], material, norm, FragPos, viewDir);*/

  result = CalcPointLight(lightProperty, lightPos[0], material, norm, FragPos, viewDir)*objectColor
      + CalcPointLight(lightProperty, lightPos[1], material, norm, FragPos, viewDir)*objectColor;
      
  color = vec4(result, 1.0f);

}


vec3 CalcPointLight(LightProperty lightProperty, vec3 lightPos,  Material mat, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(lightPos - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // can set mat.shininess
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (lightProperty.constant + lightProperty.linear * distance 
      + lightProperty.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = lightProperty.ambient; //* vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = lightProperty.diffuse * diff; //* vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = lightProperty.specular * spec; //* vec3(texture(mat.texture_specular1, TexCoords));
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}