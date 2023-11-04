#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;

// Time uniform provided from the application
uniform float time;

void main()
{
    // Calculate oscillations for x and z coordinates
    float radius = 6.0; // Radius of the circle
    float frequency = 2.0; // Number of oscillations
    float amplitude = 5.0; // Amplitude of the oscillation

    float xOffset = amplitude * sin(frequency * time);
    float zOffset = amplitude * cos(frequency * time);
    float yOffset = amplitude * cos(frequency * time);
    // Apply oscillations to the vertex position
    vec3 oscillatedPosition = vec3(aPos.x + (xOffset-yOffset)*aPos.x, aPos.y*(xOffset+yOffset), aPos.z + zOffset);

    // Create a rotation matrix for the twisting effect around the y-axis
    float angle = time; // Twist angle (can be adjusted for speed)
    mat3 rotationMatrix = mat3(
        cos(angle), 0, sin(angle),
        0, 1, 0,
        -sin(angle), 0, cos(angle)
    );

    // Apply the rotation to the oscillated position
    vec3 twistedPosition = rotationMatrix * oscillatedPosition;

    // Calculates current position
    crntPos = vec3(model * vec4(twistedPosition, 1.0f));
    // Assigns the normal from the Vertex Data to "Normal"
    Normal = aNormal;
    // Assigns the colors from the Vertex Data to "color"
    color = aColor;
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
    texCoord = aTex;

    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}
