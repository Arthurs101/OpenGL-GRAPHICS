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

uniform float time;  // Time variable provided from the application


void main()
{
    
    // Create a simple static effect using time and coordinates
    float red = fract(sin(time + crntPos.x * 10.0) * 43758.5453);
    float green = fract(sin(time + crntPos.y * 10.0) * 43758.5453);
    float blue = fract(cos(time + length(crntPos) * 10.0) * 43758.5453);

	FragColor = texture(diffuse0, texCoord) + vec4(red,green,blue,1.0);
}
