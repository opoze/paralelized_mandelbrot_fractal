#version 120

varying vec3 MyColor;
  
void main()
{
	gl_FragColor = vec4(MyColor, 1.0f);
}
