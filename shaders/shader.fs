
// Фрагментний шейдер — запускається для кожного фрагменту
// CPU (VBO)  →  Vertex Shader  →  Fragment Shader  →  екран
#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFactor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ambient
    float ambientStrength = 0.22;
    vec3 ambient = ambientStrength * lightColor;

     //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 1;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 textureColor = mix(texture(texture1, texCoord),
                    texture(texture2, texCoord),
                    mixFactor);

    FragColor = vec4((ambient + diffuse + specular) * textureColor.rgb, 1.0);
}
