#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));//FragPos和Normal着两个值
    //都是先乘以 model 再传给frag shader，因为要转换到世界空间
    Normal = mat3(transpose(inverse(model))) * normal;//这里对model进行了神秘的处理
    //是为了防止非等比例放缩后引起的发现与平面不垂直现象
}
