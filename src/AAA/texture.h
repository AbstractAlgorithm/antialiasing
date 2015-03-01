#ifndef _core_texture_h_
#define _core_texture_h_

#include "core.h"
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Texture2D
////////////////////////////////////////////////////////////////////////////////

class Texture2D
{
private:
    GLuint m_texture;
public:
    uint16_t
        m_width, m_height,
        m_internalFormat    = GL_RGBA,
        m_format            = GL_RGBA,
        m_magFilter         = GL_NEAREST,
        m_minFilter         = GL_NEAREST,
        m_wrapS             = GL_REPEAT,
        m_wrapT             = GL_REPEAT;

    Texture2D(uint16_t width = 0, uint16_t height = 0)
        : m_width(width)
        , m_height(height)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture2D()
    {
        glDeleteTextures(1, &m_texture);
    }

    GLuint texture() const { return m_texture; }

    void fill(byte* data)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
        GL_ERROR_CHECK;
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    static Texture2D* loadBMP(const char* filename)
    {
        unsigned char header[54]; // Each BMP file begins by a 54-bytes header
        unsigned int dataPos;     // Position in the file where the actual data begins
        unsigned int width, height;
        unsigned int imageSize;   // = width*height*3
        // Actual RGB data
        unsigned char* data;

        // Open the file
        FILE * file = fopen(filename, "rb");
        if (!file)
        {
            printf("ERROR: %s could not be opened.\n", filename);
            return 0;
        }

        if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
            printf("Not a correct BMP file.\n");
            return 0;
        }

        if (header[0] != 'B' || header[1] != 'M'){
            printf("Not a correct BMP file.\n");
            return 0;
        }

        // Read ints from the byte array
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

        // Create a buffer
        data = new unsigned char[imageSize];

        // Read the actual data from the file into the buffer
        fread(data, 1, imageSize, file);

        //Everything is in memory now, the file can be closed
        fclose(file);

        Texture2D* t = new Texture2D(width, height);
        t->m_internalFormat = GL_RGB;
        t->m_format = GL_BGR;
        t->fill(data);

        delete[] data;
        return t;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureArray
////////////////////////////////////////////////////////////////////////////////

class TextureArray
{
private:
    GLuint m_texture;
public:
    uint16_t
        m_width, m_height,
        m_layerCount,
        m_internalFormat    = GL_RGBA,
        m_format            = GL_RGBA,
		m_magFilter			= GL_LINEAR,
        m_minFilter         = GL_LINEAR,
        m_wrapS             = GL_REPEAT,
        m_wrapT             = GL_REPEAT;
	bool
		m_mipmap			= false;

    TextureArray(uint16_t width = 0, uint16_t height = 0, uint16_t layers=1, bool mipmap = false)
        : m_width(width)
        , m_height(height)
		, m_mipmap(mipmap)
        , m_layerCount(layers)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_mipmap ? log2((float)(m_width > m_height ? m_width : m_height)) : 1, GL_RGB8, m_width, m_height, m_layerCount);
        GL_ERROR_CHECK;

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, m_wrapS);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, m_wrapT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, m_mipmap ? GL_LINEAR_MIPMAP_LINEAR : m_minFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, m_magFilter);
		GL_ERROR_CHECK;

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    ~TextureArray()
    {
        glDeleteTextures(1, &m_texture);
    }

    GLuint texture() const { return m_texture; }

    void fill(byte* data)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_width, m_height, m_layerCount, m_format , GL_UNSIGNED_BYTE, data);
		GL_ERROR_CHECK;

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		GL_ERROR_CHECK;
        
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

	static TextureArray* loadBMPs(const char** filenames, uint16_t cnt, uint16_t w, uint16_t h, bool mip = false)
    {
        TextureArray* t = new TextureArray(w, h, cnt, mip);
        t->m_internalFormat = GL_RGB8;
        t->m_format = GL_BGR;
        // Actual RGB data
        unsigned char* data = new unsigned char[w*h * 3 * cnt];
		unsigned int imageSize = w*h*3;

        for (uint16_t i = 0; i < cnt; i++)
        {
            unsigned char header[54]; // Each BMP file begins by a 54-bytes header
            unsigned int dataPos;     // Position in the file where the actual data begins
            unsigned int width, height;
			

            // Open the file
            FILE * file = fopen(filenames[i], "rb");
            if (!file)
            {
                printf("ERROR: %s could not be opened.\n", filenames[i]);
                return 0;
            }

            if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
                printf("Not a correct BMP file.\n");
                return 0;
            }

            if (header[0] != 'B' || header[1] != 'M'){
                printf("Not a correct BMP file.\n");
                return 0;
            }

            // Read ints from the byte array
            /*dataPos = *(int*)&(header[0x0A]);
            imageSize = *(int*)&(header[0x22]);
            width = *(int*)&(header[0x12]);
            height = *(int*)&(header[0x16]);

            // Some BMP files are misformatted, guess missing information
            if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
            if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
			*/

            // Read the actual data from the file into the buffer
            fread(data + (w*h * 3 * i), 1, imageSize, file);
			//fread()

            //Everything is in memory now, the file can be closed
            fclose(file);
        }

        t->fill(data);
        delete[] data;

        return t;
    }
};


#endif