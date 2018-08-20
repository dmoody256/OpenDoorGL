 #version 330 core 

// Interpolated values from the vertex shaders 
in vec3 fragmentColor; 

// Ouput data 
out vec3 color; 
in vec2 UV;

void main(){ 

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices 
    if(UV.x > 0.9)
    {
        color = vec3(UV.x, UV.y, 1.0); 
    } 
    else
    { 
        color = fragmentColor; 
    }
    //color = fragmentColor;

}