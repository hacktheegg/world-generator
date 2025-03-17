#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColour;

out vec4 vertexColour;

void main() {
  gl_Position = aPos;
  vertexColour = aColour;
}
