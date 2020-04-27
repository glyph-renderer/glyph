/*----------------------------------------------------------------------------*/
/* glyphRenderer.cpp - Multidimensional Glyph Rendering			      */
/*                                                 			      */
/*					        			      */
/*----------------------------------------------------------------------------*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define NPIX  1025
#define MAX_SZ 100000
#define NO_GLYPHS 4

GLfloat proj_matrix[] = {
		1.667f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.667f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-0.833f, -0.833f, 0.0f, 1.0f
	};
GLubyte cat_colormap[] = {
    		78,121,167, 242,142,44, 225,87,89, 118,183,178, 89,161,79,
		237,201,73,175,122,161, 255,157,167, 156,117,95, 186,176,171
	};
int	cat_map_sz = 10;
GLubyte data_colormap[] = {
		44,123,182, 171,217,233, 255,255,191, 253,174,97, 215,25,28
	};
int	data_map_sz = 5;
int	pos_sz = 2;
/*----------------------------------------------------------------------------*/
GLuint makeShader( const char* shader_filename, GLenum shaderType ) {
	char sdr_str[MAX_SZ];
	FILE* sdr_file = fopen( shader_filename, "r" );
	size_t shdr_sz = fread( sdr_str, 1, MAX_SZ - 1, sdr_file );
	sdr_str[shdr_sz] = '\0';
	fclose( sdr_file );
  
	GLuint shader_handle   = glCreateShader( shaderType );
	const GLchar *shader_ptr = (const GLchar *)sdr_str;
	glShaderSource( shader_handle, 1, &shader_ptr, NULL );
	glCompileShader( shader_handle );

	return shader_handle;
}
/*----------------------------------------------------------------------------*/
GLuint loadShader(const char* vert_shad_filen, const char* frag_shad_filen,
		  const char* geom_shad_filen ) {
	GLuint shader_program = glCreateProgram();
	GLuint vert_shad_handle = makeShader( vert_shad_filen,
					      GL_VERTEX_SHADER );
	GLuint frag_shad_handle = makeShader( frag_shad_filen,
					      GL_FRAGMENT_SHADER );
	GLuint geom_shad_handle = makeShader( geom_shad_filen,
					      GL_GEOMETRY_SHADER );

	glAttachShader( shader_program, vert_shad_handle );
	glAttachShader( shader_program, frag_shad_handle );
	glAttachShader( shader_program, geom_shad_handle );

	glLinkProgram( shader_program );
	glDeleteShader( vert_shad_handle );
	glDeleteShader( frag_shad_handle );
	glDeleteShader( geom_shad_handle );

	glLinkProgram(shader_program);
	int matrix_location = glGetUniformLocation( shader_program,
						    "matrix" );
	int cat_tex_location = glGetUniformLocation( shader_program,
						     "cat_texture" );
	int attr_tex_location = glGetUniformLocation( shader_program,
						      "attr_texture" );
	glUseProgram( shader_program );
	glUniformMatrix4fv( matrix_location, 1, GL_FALSE, proj_matrix );
	glUniform1i(cat_tex_location, 0);
	glUniform1i(attr_tex_location,  1);

        GLuint cat_textureID;
        glGenTextures(1, &cat_textureID);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_1D, cat_textureID);
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, cat_map_sz, 0,
		      GL_RGB, GL_UNSIGNED_BYTE, cat_colormap );
	glGenerateMipmap( GL_TEXTURE_1D );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	GLuint attr_textureID;
        glGenTextures(1, &attr_textureID);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_1D, attr_textureID);
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, data_map_sz, 0, 
		      GL_RGB, GL_UNSIGNED_BYTE, data_colormap );
	glGenerateMipmap( GL_TEXTURE_1D );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	return shader_program;
}
/*----------------------------------------------------------------------------*/
void reloadShader(
	GLuint* program,
	const char* vert_shad_filen,
	const char* frag_shad_filen,
	const char* geom_shad_filen ) {

	GLuint reloaded_program = loadShader(
	vert_shad_filen, frag_shad_filen, geom_shad_filen );
	if ( reloaded_program ) {
	    glDeleteProgram( *program );
	    *program = reloaded_program;
	}
}
/*----------------------------------------------------------------------------*/
void randomArray(float* array, GLuint* buffer, int attrib_pos) {
	for (int i=0;i<NO_GLYPHS;i++) {
		array[i] = float(rand()) / float(RAND_MAX);
	}
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, NO_GLYPHS * sizeof(float),
		     array, GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_pos, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attrib_pos);
}
/*----------------------------------------------------------------------------*/
void draw(GLFWwindow* window, GLuint vao) {
	GLuint shader_program = loadShader( "vert.glsl",
					    "frag.glsl", "geom.glsl" );

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, NO_GLYPHS);
		glfwPollEvents();
		glfwSwapBuffers(window);
		if ( glfwGetKey( window, GLFW_KEY_B ) ) {
			reloadShader( &shader_program, "vert.glsl",
				      "frag.glsl", "geom.glsl" );
		}
		if ( glfwGetKey( window, GLFW_KEY_C ) ) {
			reloadShader( &shader_program, "vert.glsl",
				      "frag.glsl", "geomAlt.glsl" );
		}
		if ( glfwGetKey( window, GLFW_KEY_S ) ) {
			reloadShader( &shader_program, "vert.glsl",
				      "frag.glsl", "geomStar.glsl" );
		}
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
	}
}
/*----------------------------------------------------------------------------*/
int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(NPIX, NPIX,
					      "Glyph Plotter", NULL, NULL);
	glfwMakeContextCurrent( window );
	glewExperimental = GL_TRUE;
	glewInit();
 
	float points[pos_sz*NO_GLYPHS];
	float cols[NO_GLYPHS];
	float varA[NO_GLYPHS];
	float varB[NO_GLYPHS];
	float varC[NO_GLYPHS];
	float varD[NO_GLYPHS];
	float varE[NO_GLYPHS];
	float varF[NO_GLYPHS];
	for (int i=0;i<NO_GLYPHS;i++) {
		cols[i] = rand() % 10 + 1;
		points[i*pos_sz] = float(rand()) / float(RAND_MAX);
		points[i*pos_sz+1] = float(rand()) / float(RAND_MAX);
	}
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, pos_sz* NO_GLYPHS * sizeof(float),
		     points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, pos_sz, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
  
	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, NO_GLYPHS * sizeof(float),
		     cols, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	GLuint varA_vbo = 0;
	glGenBuffers(1, &varA_vbo);
	randomArray(varA, &varA_vbo,2);
	GLuint varB_vbo = 0;
	glGenBuffers(1, &varB_vbo);
	randomArray(varB, &varB_vbo,3);
	GLuint varC_vbo = 0;
	glGenBuffers(1, &varC_vbo);
	randomArray(varC, &varC_vbo,4);
	GLuint varD_vbo = 0;
	glGenBuffers(1, &varD_vbo);
	randomArray(varD, &varD_vbo,5);
	GLuint varE_vbo = 0;
	glGenBuffers(1, &varE_vbo);
	randomArray(varE, &varE_vbo,6);
	GLuint varF_vbo = 0;
	glGenBuffers(1, &varF_vbo);
	randomArray(varF, &varF_vbo,7);

	draw(window, vao);
	glfwTerminate();
	return 0;
}