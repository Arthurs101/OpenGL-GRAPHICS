#version 330 core

out vec4 FragColor;

uniform float time;  // Time variable provided from the application

void main()
{
    // Calculate color values based on time and fragment coordinates
    float red = 0.5 + 0.5 * sin(time + gl_FragCoord.x / 100.0);
    float green = 0.5 + 0.5 * sin(time + gl_FragCoord.y / 100.0);
    float blue = 0.5 + 0.5 * cos(time + length(gl_FragCoord.xy) / 100.0);

    // Output the final color
    FragColor = vec4(red, green, blue, 1.0);
}
