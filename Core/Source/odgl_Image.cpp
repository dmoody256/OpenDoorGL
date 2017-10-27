#include "odgl_Include.h"
#include "odgl_Image.h"

#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

namespace OpenDoorGL{

    Image::Image() : 
        _textureID(0),
        _height(0),
        _width(0)
    {

    }

    Image::Image(const char* filepath) : 
        _textureID(0),
        _height(0),
        _width(0)
    {
        LoadImageFromFile(filepath);
    }

    Image::~Image(){


    }

    GLuint Image::GetTextureGPUHandle(){
        return _textureID;
    }

    GLuint Image::LoadImageFromFile(const char* filepath){
        std::string filepathString = filepath;
        std::transform(filepathString.begin(), filepathString.end(), filepathString.begin(), ::tolower);
        if(filepathString.substr(filepathString.find_last_of(".") + 1) == "bmp") {
            LoadBMPFromFile(filepath);
        }
        if(filepathString.substr(filepathString.find_last_of(".") + 1) == "dds") {
            LoadDDSFromFile(filepath);
        }
    }

    #define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
    #define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
    #define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
    
    GLuint Image::LoadDDSFromFile(const char * filepath){
    
        unsigned char header[124];
    
        FILE *fp; 
     
        /* try to open the file */ 
        fp = fopen(filepath, "rb"); 
        if (fp == NULL){
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filepath); getchar(); 
            return 0;
        }
       
        /* verify the type of file */ 
        char filecode[4]; 
        fread(filecode, 1, 4, fp); 
        if (strncmp(filecode, "DDS ", 4) != 0) { 
            fclose(fp); 
            return 0; 
        }
        
        /* get the surface desc */ 
        fread(&header, 124, 1, fp); 
    
        _height      = *(unsigned int*)&(header[8 ]);
        _width	     = *(unsigned int*)&(header[12]);
        unsigned int linearSize	 = *(unsigned int*)&(header[16]);
        unsigned int mipMapCount = *(unsigned int*)&(header[24]);
        unsigned int fourCC      = *(unsigned int*)&(header[80]);
    
     
        unsigned char * buffer;
        unsigned int bufsize;
        /* how big is it going to be including all mipmaps? */ 
        bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
        buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
        fread(buffer, 1, bufsize, fp); 
        /* close the file pointer */ 
        fclose(fp);
    
        unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
        unsigned int format;
        switch(fourCC) 
        { 
        case FOURCC_DXT1: 
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
            break; 
        case FOURCC_DXT3: 
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
            break; 
        case FOURCC_DXT5: 
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
            break; 
        default: 
            free(buffer); 
            return 0; 
        }
    
        // Create one OpenGL texture
        GL_CHECK( glGenTextures(1, &_textureID) );
    
        // "Bind" the newly created texture : all future texture functions will modify this texture
        GL_CHECK( glBindTexture(GL_TEXTURE_2D, _textureID) );
        GL_CHECK( glPixelStorei(GL_UNPACK_ALIGNMENT,1) );	
        
        unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
        unsigned int offset = 0;
    
        /* load the mipmaps */ 
        for (unsigned int level = 0; level < mipMapCount && (_width || _height); ++level) 
        { 
            unsigned int size = ((_width+3)/4)*((_height+3)/4)*blockSize; 
            GL_CHECK(  glCompressedTexImage2D(GL_TEXTURE_2D, level, format, _width, _height,  
                0, size, buffer + offset) ); 
         
            offset += size; 
            _width  /= 2; 
            _height /= 2; 
    
            // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
            if(_width < 1) _width = 1;
            if(_height < 1) _height = 1;
    
        } 
    
        free(buffer); 
    
        return _textureID;
    }

    GLuint Image::LoadBMPFromFile(const char* filepath){

        // Data read from the header of the BMP file
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        // Actual RGB data
        unsigned char * data;

        // Open the file
        FILE * file = fopen(filepath,"rb");
        if (!file){
            printf("%s could not be opened.\n", filepath);
            getchar();
            return 0;
        }

        // Read the header, i.e. the 54 first bytes

        // If less than 54 bytes are read, problem
        if ( fread(header, 1, 54, file)!=54 ){ 
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }
        // A BMP files always begins with "BM"
        if ( header[0]!='B' || header[1]!='M' ){
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }
        // Make sure this is a 24bpp file
        if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
        if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}

        // Read the information about the image
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        _width      = *(int*)&(header[0x12]);
        _height     = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize==0)    imageSize=_width*_height*3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos==0)      dataPos=54; // The BMP header is done that way

        // Create a buffer
        data = new unsigned char [imageSize];

        // Read the actual data from the file into the buffer
        fread(data,1,imageSize,file);

        // Everything is in memory now, the file can be closed.
        fclose (file);

        // Create one OpenGL texture
        GL_CHECK ( glGenTextures(1, &_textureID) );
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        GL_CHECK ( glBindTexture(GL_TEXTURE_2D, _textureID) );

        // Give the image to OpenGL
        GL_CHECK ( glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, data) );

        // OpenGL has now copied the data. Free our own version
        delete [] data;

        // Poor filtering, or ...
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

        // ... nice trilinear filtering ...
        GL_CHECK ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
        GL_CHECK ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
        GL_CHECK ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        GL_CHECK ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR) );
        // ... which requires mipmaps. Generate them automatically.
        GL_CHECK ( glGenerateMipmap(GL_TEXTURE_2D) );

        // Return the ID of the texture we just created
        return _textureID;

    }


}
