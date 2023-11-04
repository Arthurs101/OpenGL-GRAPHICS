#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;


vec4 toonShading(vec3 lightDir, vec3 normal) {
    // controls how big the area that is lit up is
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // ambient lighting
    float ambient = 0.20f;

    // diffuse lighting with thresholding
    float diffuseThreshold = 0.4; // Adjust this threshold value
    float diffuse = max(dot(normalize(normal), normalize(lightDir)), 0.0);
    diffuse = step(diffuseThreshold, diffuse);

    // specular lighting
    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDir, normalize(normal));
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    // calculates the intensity of the crntPos based on its angle to the center of the light cone
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    // Sample diffuse texture
    vec3 diffuseColor = texture(diffuse0, texCoord).rgb;

    return vec4((diffuse * inten + ambient) * lightColor.rgb * diffuseColor, 1.0) + vec4(specular * inten * lightColor.rgb, 1.0);
}

void main() {
    // Normal is already interpolated from the vertex shader
    vec3 normal = normalize(Normal);

    // Light direction from fragment to light position
    vec3 lightDirection = normalize(lightPos - crntPos);

    // Calculate the toon shaded color
    vec4 toonColor = toonShading(lightDirection, normal);

    // outputs final color
    FragColor = toonColor;
}
