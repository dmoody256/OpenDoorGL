 #version 330 core 

// Ouput data 
out vec3 color; 

uniform vec3 cubeColor;

void main(){ 

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices 
    color = cubeColor; 

}