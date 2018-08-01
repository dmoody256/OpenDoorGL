#version 330 core 

uniform sampler2D myTextureSampler; 

in vec2 UV;
// Ouput data 
out vec3 color; 

void main(){ 

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices 
    color = texture( myTextureSampler, UV ).rgb; 

}