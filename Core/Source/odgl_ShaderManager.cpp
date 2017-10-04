
#include "odgl_ShaderManager.h"

#include <sstream>
#include <vector>



namespace OpenDoorGL{

    std::string ShaderManager::colorVertShader;
    std::string ShaderManager::colorFragShader;

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

    GLuint ShaderManager::LoadShadersFromString(const char * vertShader,const char * fragShader){
        GLint Result = GL_FALSE;
        int InfoLogLength;
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Compile Vertex Shader
        printf("Compiling shader : %s\n", vertShader);
        glShaderSource(VertexShaderID, 1, &vertShader , NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        printf("Compiling shader : %s\n", fragShader);
        glShaderSource(FragmentShaderID, 1, &fragShader , NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
        printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }

        
        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);
        
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

}


