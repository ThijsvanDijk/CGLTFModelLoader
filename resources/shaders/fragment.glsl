#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

vec3 CalcDirLight(vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(vec3(0.2f, 0.3f, 0.3f));    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // Combine
    vec3 ambient = vec3(0.1f, 0.1f, 0.1f) * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(0.4f, 0.4f, 0.4f) * diff * vec3(1.0, 1.0, 1.0);
    vec3 specular = vec3(0.5f, 0.5f, 0.5f) * spec * vec3(1.0, 1.0, 1.0);
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(norm, viewDir);
    FragColor = vec4(result, 1.0f);
}