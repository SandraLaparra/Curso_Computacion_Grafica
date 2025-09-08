#version 330 core
in vec3 ourColor;

out vec4 color;

uniform vec3 objetoColor; // color opcional desde C++

void main()
{
    // Si objetoColor == (-1,-1,-1) usamos color de vértice
    if(objetoColor.x < 0.0)
        color = vec4(ourColor, 1.0f);
    else
        color = vec4(objetoColor, 1.0f);
}
