#version 120

attribute vec3 position;
attribute vec3 color;

varying vec3 MyColor;
  
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0f);
    MyColor = color;
}
