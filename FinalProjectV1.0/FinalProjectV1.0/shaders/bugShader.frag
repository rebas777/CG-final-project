#version 330 core


struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 




out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform Material material;

void main()
{

  vec3 result;
      
  result = vec3(texture(material.texture_diffuse1, TexCoords));
  //color = vec4(result, 1.0f);
  color = vec4(lightColor, 1.0);

}

