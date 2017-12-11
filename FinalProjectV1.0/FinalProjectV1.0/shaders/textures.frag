#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
//uniform float mixValue

void main()
{
    vec2 processed_TexCoord = TexCoord;
    processed_TexCoord.y *= -1;//纹理上下颠倒
    color = mix(texture(ourTexture1,TexCoord),texture(ourTexture2,processed_TexCoord),0.2);
    //根据第三个参数进行线性插值，0.2意味着第一个纹理占80%，第二个纹理占20%
}
