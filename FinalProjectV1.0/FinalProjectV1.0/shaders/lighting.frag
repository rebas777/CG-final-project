#version 330 core
out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;//光源的位置
uniform vec3 viewPos;//视点的位置

void main()
{
  //1. ambient light
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  //2. diffuse light
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  //3. specular light
  float specularStrength = 0.5f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);//给定入射光方向和法线方向，计算反射光方向
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//视线方向与反射线方向的n次幂(n为发光值)
  vec3 specular = specularStrength * spec * lightColor;

  //combine
  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}
