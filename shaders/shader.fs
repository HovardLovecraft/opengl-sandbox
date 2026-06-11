
// Фрагментний шейдер — запускається для кожного пікселя
// CPU (VBO)  →  Vertex Shader  →  Fragment Shader  →  екран
#version 330 core
out vec4 FragColor;
// in vec3 ourColor;
in vec3 ourPosition;

void main()
{
    FragColor = vec4(ourPosition + 0.5, 1.0);    // note how the position value is linearly interpolated to get all the different colors
}
