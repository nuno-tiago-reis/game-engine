#version 330 core

in vec2 Position;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec2 TextureCoordinate;
 
void main(void) {

  gl_Position = vec4(Position, 0.0, 1.0);

  TextureCoordinate = (Position + 1.0) / 2.0;
}