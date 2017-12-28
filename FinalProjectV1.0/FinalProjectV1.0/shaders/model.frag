#version 330 core
out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//视点的位置
uniform vec3 globalAmbientColor;
uniform sampler2D texture_diffuse1;

void main()
{
  //1. ambient light
  //float ambientStrength = 0.1f;
  vec3 ambient = lightColor;
  float globalAmbientStrength = 0.3f;
  ambient = ambient + globalAmbientStrength * globalAmbientColor;

  //2. diffuse light
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor; //* vec3(texture(texture_diffuse1, TexCoords))

  //3. specular light
  //float specularStrength = 0.5f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);//给定入射光方向和法线方向，计算反射光方向
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//视线方向与反射线方向的n次幂(n为发光值)
  vec3 specular = spec * lightColor;

  //combine
  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}
