
// Фрагментний шейдер — запускається для кожного фрагменту
// CPU (VBO)  →  Vertex Shader  →  Fragment Shader  →  екран
#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFactor;
uniform vec3 lightColor;

void main()
{
    vec4 textureColor = mix(texture(texture1, texCoord),
                    texture(texture2, texCoord),
                    mixFactor);
    FragColor = vec4(lightColor * textureColor.rgb, 1.0);
}
