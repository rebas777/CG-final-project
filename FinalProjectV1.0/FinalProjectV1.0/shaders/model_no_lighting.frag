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

uniform Material material;



void main()
{
      
  color = vec4(texture(texture_diffuse1, TexCoords));

}
