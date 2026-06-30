
// Фрагментний шейдер — запускається для кожного фрагменту
// CPU (VBO)  →  Vertex Shader  →  Fragment Shader  →  екран
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFactor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

     //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // calculate direction of light
    float diff = max(dot(norm, lightDir), 0.0); // calculate diffuse impact of the light on the current fragment
    vec3 diffuse = light.diffuse * (diff * material.diffuse); // diffuse component, the greater the angle between both vectors the darker

    // specular
    vec3 viewDir = normalize(viewPos - FragPos); // calculate view direction
    vec3 reflectDir = reflect(-lightDir, norm); // calculate where to the light is reflected
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //product of viewDir and  make reflectDir and make sure it's not negative
    vec3 specular = light.specular * (spec * material.specular); // calculate final value of specular lighting

    vec4 textureColor = mix(texture(texture1, texCoord),
                    texture(texture2, texCoord),
                    mixFactor);

    FragColor = vec4((ambient + diffuse + specular) * textureColor.rgb, 1.0);
}
