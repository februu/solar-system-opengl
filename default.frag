#version 330 core

// Outputs colors
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 transformedNormal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;
// Uniform to check if the model has an internal light source
uniform bool isLightSource;

vec4 light()
{
    if (isLightSource) {
        return texture(diffuse0, texCoord) * lightColor;
    } else {

        // ambient lighting
        float ambient = 0.20f;

        // diffuse lighting
        vec3 normal = transformedNormal;
        vec3 lightDirection = normalize(lightPos - crntPos);
        float diffuse = max(dot(normal, lightDirection), 0.0f);

        // specular lighting
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
        float specular = specAmount * specularLight;

        return (texture(diffuse0, texCoord) * (diffuse + ambient) + specular) * lightColor;
    }
}

void main()
{
    FragColor = light();
}
