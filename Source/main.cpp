//
//  main.cpp
//  Klephtica
//
//  Created by Carlos Sousa and Joao Bajouco on 2014-11-25.
//  Copyright (c) 2014 primatas. All rights reserved.
//

#include <iostream>

#if __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h> // Usar gl3
#include <SDL2/SDL.h>
#elif __linux__
#include <SDL.h>
#include <GL/gl.h>
#endif

using namespace std;

// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
void Display_InitGL();
//function to reset our viewport after a window resize
int Display_SetViewport( int width, int height);
void Display_Render(SDL_Renderer* displayRenderer, GLdouble ax, GLdouble ay );
void rotateViewport(SDL_Renderer* displayRenderer, int mov_x, int mov_y);

void printVersion();

int
main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow;
    SDL_Renderer* displayRenderer;
    SDL_RendererInfo displayRendererInfo;
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    /*TODO: Check that we have OpenGL */
    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        /*TODO: Handle this. We have no render surface and not accelerated. */
    }
    SDL_Event event;
    SDL_GL_SwapWindow(displayWindow);
    Display_InitGL();
	Display_SetViewport(800, 600);
	glLoadIdentity();
	Display_Render(displayRenderer, 0.0 ,0.0);


    bool done=false;
    bool click = false, stopped=true;
    
    //imprime infos
    printVersion();

    int mov_x=0, mov_y=0;



    while(!done) {
    	while(SDL_PollEvent(&event)) {
    			if (event.type == SDL_QUIT) {
    				done=true;
    			}

    			if(event.type == SDL_KEYDOWN){
    				switch(event.key.keysym.sym){
    				        case SDLK_LEFT:
    				        	rotateViewport(displayRenderer, -1.0, 0.0);
    				          break;
    				        case SDLK_RIGHT:
    				        	rotateViewport(displayRenderer, 1.0, 0.0);
    				          break;
    				        case SDLK_UP:
    				        	rotateViewport(displayRenderer, 0.0, -1.0);
    				          break;
    				        case SDLK_DOWN:
    				        	rotateViewport(displayRenderer, 0.0, 1.0);
    				          break;
							default:
							break;
    				      }
    			}




    			if (event.type == SDL_MOUSEBUTTONDOWN) {
    				mov_x = event.motion.x;
    				mov_y = event.motion.y;
    				click = true;
    				//cout << "0### "<< mov_x <<" :: "<< mov_y <<" ###" << endl;
    			}
    			if (event.type == SDL_MOUSEMOTION && click) {
    				if(stopped){
    					//cout << "1### "<< mov_x << "-" << event.motion.x << " :: "<< mov_y <<"-" << event.motion.y << " ###" << endl;

    					//stopped = false;
    					rotateViewport(displayRenderer, mov_x - event.motion.x, mov_y - event.motion.y);
    					mov_x = event.motion.x;
						mov_y = event.motion.y;
						//cout << "2### " << mov_x << ":" << mov_y << " ###" << endl;
    				}


    			}
    			if (event.type == SDL_MOUSEBUTTONUP) {
    			    click = false;
    			}
    	}
		// fill the screen with black color
		//SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

		// update the screen buffer
		//SDL_Flip(screen);
    }

    SDL_Quit();
    return 0;
}


// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan( fovY / 360 * pi ) * zNear;
	fW = fH * aspect;

	glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void Display_InitGL()
{
    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

/* function to reset our viewport after a window resize */
int Display_SetViewport( int width, int height)
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 ) {
        height = 1;
    }

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 10, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    //gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    perspectiveGL( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return 1;
}

void Display_Render(SDL_Renderer* displayRenderer, GLdouble ax, GLdouble ay )
{
	glTranslatef( 0.0f, 0.0f, -6.0f );
	glRotated(ay , 1.0f, 0.0f, 0.0f);
	glRotated(ax , 0.0f, 1.0f, 0.0f);
	glTranslatef( 0.0f, 0.0f, 6.0f );

	glPushMatrix();
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    //glLoadIdentity();
    glTranslatef( -1.5f, 0.0f, -6.0f );

    glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
      glVertex3f(  0.0f,  1.0f, 0.0f ); /* Top */
      glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
      glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
    glEnd( );                           /* Finished Drawing The Triangle */

    /* Move Right 3 Units */
    glTranslatef( 3.0f, 0.0f, 0.0f );

    //solid_sphere();

    glBegin( GL_QUADS );                /* Draw A Quad */
      glVertex3f( -1.0f,  1.0f, 0.0f ); /* Top Left */
      glVertex3f(  1.0f,  1.0f, 0.0f ); /* Top Right */
      glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
      glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
    glEnd( );                           /* Done Drawing The Quad */

    glPopMatrix();

    SDL_RenderPresent(displayRenderer);
}

void rotateViewport(SDL_Renderer* displayRenderer, int mov_x, int mov_y){
	while (mov_x != 0 || mov_y != 0){
		//SDL_Delay(5);
		//cout << mov_x << ":" << mov_y << endl;
		if(mov_x > 0){
			Display_Render(displayRenderer, -1.0 ,0.0);
			mov_x = mov_x - 1;
		}
		if(mov_x < 0){
			Display_Render(displayRenderer, 1.0 ,0.0);
			mov_x = mov_x + 1;
		}
		if(mov_y > 0){
			Display_Render(displayRenderer, 0.0 ,-1.0);
			mov_y = mov_y - 1;
		}
		if(mov_y < 0){
			Display_Render(displayRenderer, 0.0 ,1.0);
			mov_y = mov_y + 1;
		}




	}
}

void printVersion(){
	cout << endl;
    cout << "\t\t###############################" << endl;
    cout << "\t\t# Primatas Game Alpha Version #" << endl;
    cout << "\t\t###############################" << endl << endl;
    cout << "#OpenGL" << endl;
    cout << "\tGL_VERSION\t" << glGetString(GL_VERSION) << endl;
    cout << "\tGL_VENDOR\t" << glGetString(GL_VENDOR) << endl;
    cout << "\tGL_RENDERER\t" << glGetString(GL_RENDERER) << endl;
    
    //mais info -> http://www.libsdl.org/tmp/docs-1.3/_s_d_l__version_8h.html
    cout << "#SDL" << endl;
    SDL_version compile, runtime;
	SDL_VERSION(&compile); // Prints the compile time version
	SDL_GetVersion(&runtime); // Prints the runtime version
	cout << "\tSDL_VERSION\t" << (int)runtime.major << "." << (int)runtime.minor << "." << (int)runtime.patch
		<< " (" << (int)compile.major << "." << (int)compile.minor << "." << (int)compile.patch << ")\t" << " runtime (compile-time)" << endl;
}


