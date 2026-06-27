
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
    vec3 lightDir = normalize(lightPos - FragPos); // calculate direction of light
    float diff = max(dot(norm, lightDir), 0.0); // calculate diffuse impact of the light on the current fragment
    vec3 diffuse = diff * lightColor; // diffuse component, the greater the angle between both vectors the darker

    // specular
    float specularStrength = 1;
    vec3 viewDir = normalize(viewPos - FragPos); // calculate view direction
    vec3 reflectDir = reflect(-lightDir, norm); // calculate where to the light is reflected
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); //product of viewDir and  make reflectDir and make sure it's not negative
    vec3 specular = specularStrength * spec * lightColor; // calculate final value of specular lighting

    vec4 textureColor = mix(texture(texture1, texCoord),
                    texture(texture2, texCoord),
                    mixFactor);

    FragColor = vec4((ambient + diffuse + specular) * textureColor.rgb, 1.0);
}
