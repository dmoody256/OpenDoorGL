
#include "odgl_Include.hpp"
#include "odgl_ShaderManager.hpp"

#include <sstream>
#include <vector>

namespace OpenDoorGL{

    std::string ShaderManager::colorVertShader;
    std::string ShaderManager::colorFragShader;
    std::string ShaderManager::textureVertShader;
    std::string ShaderManager::textureFragShader;
    std::string ShaderManager::textureModelVertShader;
    std::string ShaderManager::textureModelFragShader;

    const char* ShaderManager::GetTextureModelFragShader(){
        if(textureModelFragShader.size() == 0){
            std::stringstream shader;
            shader <<
            "#version 330 core\n" <<
            "\n" <<
            "// Interpolated values from the vertex shaders\n" <<
            "in vec2 UV;\n" <<
            "in vec3 Position_worldspace;\n" <<
            "in vec3 Normal_cameraspace;\n" <<
            "in vec3 EyeDirection_cameraspace;\n" <<
            "in vec3 LightDirection_cameraspace;\n" <<
            "\n" <<
            "// Ouput data\n" <<
            "out vec3 color;\n" <<
            "\n" <<
            "// Values that stay constant for the whole mesh.\n" <<
            "uniform sampler2D myTextureSampler;\n" <<
            "uniform mat4 MV;\n" <<
            "uniform vec3 LightPosition_worldspace;\n" <<
            "\n" <<
            "void main(){\n" <<
            "\n" <<
            "    // Light emission properties\n" <<
            "    // You probably want to put them as uniforms\n" <<
            "    vec3 LightColor = vec3(1,1,1);\n" <<
            "    float LightPower = 50.0f;\n" <<
            "    \n" <<
            "    // Material properties\n" <<
            "    vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;\n" <<
            "    vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;\n" <<
            "    vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);\n" <<
            "\n" <<
            "    // Distance to the light\n" <<
            "    float distance = length( LightPosition_worldspace - Position_worldspace );\n" <<
            "\n" <<
            "    // Normal of the computed fragment, in camera space\n" <<
            "    vec3 n = normalize( Normal_cameraspace );\n" <<
            "    // Direction of the light (from the fragment to the light)\n" <<
            "    vec3 l = normalize( LightDirection_cameraspace );\n" <<
            "    // Cosine of the angle between the normal and the light direction, \n" <<
            "    // clamped above 0\n" <<
            "    //  - light is at the vertical of the triangle -> 1\n" <<
            "    //  - light is perpendicular to the triangle -> 0\n" <<
            "    //  - light is behind the triangle -> 0\n" <<
            "    float cosTheta = clamp( dot( n,l ), 0,1 );\n" <<
            "    \n" <<
            "    // Eye vector (towards the camera)\n" <<
            "    vec3 E = normalize(EyeDirection_cameraspace);\n" <<
            "    // Direction in which the triangle reflects the light\n" <<
            "    vec3 R = reflect(-l,n);\n" <<
            "    // Cosine of the angle between the Eye vector and the Reflect vector,\n" <<
            "    // clamped to 0\n" <<
            "    //  - Looking into the reflection -> 1\n" <<
            "    //  - Looking elsewhere -> < 1\n" <<
            "    float cosAlpha = clamp( dot( E,R ), 0,1 );\n" <<
            "    \n" <<
            "    color = \n" <<
            "        // Ambient : simulates indirect lighting\n" <<
            "        MaterialAmbientColor +\n" <<
            "        // Diffuse : \"color\" of the object\n" <<
            "        MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +\n" <<
            "        // Specular : reflective highlight, like a mirror\n" <<
            "        MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);\n" <<
            "\n" <<
            "}";
            textureModelFragShader = shader.str();
        }
        return textureModelFragShader.c_str();
    }

    const char* ShaderManager::GetTextureModelVertShader(){
        if(textureModelVertShader.size() == 0){
            std::stringstream shader;
            shader <<
            "#version 330 core\n" <<
            "\n" <<
            "// Input vertex data, different for all executions of this shader.\n" <<
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n" <<
            "layout(location = 1) in vec2 vertexUV;\n" <<
            "layout(location = 2) in vec3 vertexNormal_modelspace;\n" <<
            "\n" <<
            "// Output data ; will be interpolated for each fragment.\n" <<
            "out vec2 UV;\n" <<
            "out vec3 Position_worldspace;\n" <<
            "out vec3 Normal_cameraspace;\n" <<
            "out vec3 EyeDirection_cameraspace;\n" <<
            "out vec3 LightDirection_cameraspace;\n" <<
            "\n" <<
            "// Values that stay constant for the whole mesh.\n" <<
            "uniform mat4 MVP;\n" <<
            "uniform mat4 V;\n" <<
            "uniform mat4 M;\n" <<
            "uniform vec3 LightPosition_worldspace;\n" <<
            "\n" <<
            "void main(){\n" <<
            "\n" <<
            "    // Output position of the vertex, in clip space : MVP * position\n" <<
            "    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n" <<
            "    \n" <<
            "    // Position of the vertex, in worldspace : M * position\n" <<
            "    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;\n" <<
            "    \n" <<
            "    // Vector that goes from the vertex to the camera, in camera space.\n" <<
            "    // In camera space, the camera is at the origin (0,0,0).\n" <<
            "    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;\n" <<
            "    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n" <<
            "\n" <<
            "    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.\n" <<
            "    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n" <<
            "    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n" <<
            "    \n" <<
            "    // Normal of the the vertex, in camera space\n" <<
            "    // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.\n" <<
            "    Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; \n" <<
            "    \n" <<
            "    // UV of the vertex. No special space for this one.\n" <<
            "    UV = vertexUV;\n" <<
            "}";
            textureModelVertShader = shader.str();
        }
        return textureModelVertShader.c_str();
    }


    const char* ShaderManager::getColorFragShader(){
        if(colorFragShader.size() == 0){
            std::stringstream fragShader;
            fragShader <<
            "#version 330 core\n"<< 
            "\n"<<
            "// Interpolated values from the vertex shaders\n"<< 
            "in vec3 fragmentColor;\n"<< 
            "\n"<<
            "// Ouput data\n"<< 
            "out vec3 color;\n"<< 
            "\n"<<
            "void main(){\n"<< 
            "\n"<<
            "    // Output color = color specified in the vertex shader,\n"<< 
            "    // interpolated between all 3 surrounding vertices\n"<< 
            "    color = fragmentColor;\n"<< 
            "\n"<<
            "}\n";
            colorFragShader = fragShader.str();
        }
        return colorFragShader.c_str();
    }

    const char* ShaderManager::getColorVertShader(){
        if(colorVertShader.size() == 0){
            std::stringstream vertShader;
            vertShader <<
            "#version 330 core\n"<< 
            "\n"<<
            "// Input vertex data, different for all executions of this shader.\n"<<
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n"<< 
            "layout(location = 1) in vec3 vertexColor;\n"<< 
            "\n"<<
            "// Output data ; will be interpolated for each fragment.\n"<< 
            "out vec3 fragmentColor;\n"<< 
            "\n"<<
            "// Values that stay constant for the whole mesh.\n"<< 
            "uniform mat4 MVP;\n"<< 
            "\n"<< 
            "void main(){\n"<< 
            "    // Output position of the vertex, in clip space : MVP * position\n"<< 
            "    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"<< 
            "\n"<<
            "    // The color of each vertex will be interpolated\n"<< 
            "    // to produce the color of each fragment\n"<< 
            "    fragmentColor = vertexColor;\n"<< 
            "}\n";
            
            colorVertShader = vertShader.str();
        }
        return colorVertShader.c_str();
    }

    const char* ShaderManager::getTextureFragShader(){
        if(textureFragShader.size() == 0){
            std::stringstream fragShader;
            fragShader <<
            "#version 330 core\n"<< 
            "\n"<<
            "uniform sampler2D myTextureSampler;\n"<< 
            "\n"<<
            "in vec2 UV;\n"<<
            "// Ouput data\n"<< 
            "out vec3 color;\n"<< 
            "\n"<<
            "void main(){\n"<< 
            "\n"<<
            "    // Output color = color specified in the vertex shader,\n"<< 
            "    // interpolated between all 3 surrounding vertices\n"<< 
            "    color = texture( myTextureSampler, UV ).rgb;\n"<< 
            "\n"<<
            "}\n";
            textureFragShader = fragShader.str();
        }
        return textureFragShader.c_str();
    }

    const char* ShaderManager::getTextureVertShader(){
        if(textureVertShader.size() == 0){
            std::stringstream vertShader;
            vertShader <<
            "#version 330 core\n"<< 
            "\n"<<
            "// Input vertex data, different for all executions of this shader.\n"<<
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n"<< 
            "layout(location = 1) in vec2 vertexUV;\n"<<
            "\n"<<
            "out vec2 UV;\n"<<
            "// Values that stay constant for the whole mesh.\n"<< 
            "uniform mat4 MVP;\n"<< 
            "\n"<< 
            "void main(){\n"<< 
            "    // Output position of the vertex, in clip space : MVP * position\n"<< 
            "    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n"<< 
            "    UV = vertexUV;\n"<<
            "}\n";
            
            textureVertShader = vertShader.str();
        }
        return textureVertShader.c_str();
    }

    GLuint ShaderManager::LoadShadersFromString(const char * vertShader,const char * fragShader){
        GLint Result = GL_FALSE;
        int InfoLogLength;
        // Create the shaders
        GLuint VertexShaderID;
        GL_CHECK ( VertexShaderID = glCreateShader(GL_VERTEX_SHADER) );
        GLuint FragmentShaderID;
        GL_CHECK ( FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER) );

        // Compile Vertex Shader
        //printf("Compiling shader : %s\n", vertShader);
        GL_CHECK ( glShaderSource(VertexShaderID, 1, &vertShader , NULL) );
        GL_CHECK ( glCompileShader(VertexShaderID) );

        // Check Vertex Shader
        GL_CHECK ( glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result) );
        GL_CHECK ( glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength) );
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            GL_CHECK ( glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]) );
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        //printf("Compiling shader : %s\n", fragShader);
        GL_CHECK ( glShaderSource(FragmentShaderID, 1, &fragShader , NULL) );
        GL_CHECK ( glCompileShader(FragmentShaderID) );

        // Check Fragment Shader
        GL_CHECK ( glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result) );
        GL_CHECK ( glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength) );
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            GL_CHECK ( glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]) );
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
        //printf("Linking program\n");
        GLuint ProgramID;
        GL_CHECK ( ProgramID = glCreateProgram() );
        GL_CHECK ( glAttachShader(ProgramID, VertexShaderID) );
        GL_CHECK ( glAttachShader(ProgramID, FragmentShaderID) );
        GL_CHECK ( glLinkProgram(ProgramID) );

        // Check the program
        GL_CHECK ( glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result) );
        GL_CHECK ( glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength) );
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            GL_CHECK ( glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]) );
            printf("%s\n", &ProgramErrorMessage[0]);
        }

        
        GL_CHECK ( glDetachShader(ProgramID, VertexShaderID) );
        GL_CHECK ( glDetachShader(ProgramID, FragmentShaderID) );
        
        GL_CHECK ( glDeleteShader(VertexShaderID) );
        GL_CHECK ( glDeleteShader(FragmentShaderID) );

        return ProgramID;
    }

}


