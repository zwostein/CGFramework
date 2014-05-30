#include "Pixelator.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include <stdlib.h>

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include "suzanne.h"
#include "logo.h"
#include "cube.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

struct Face
{
	Vertex a;
	Vertex b;
	Vertex c;
};

typedef unsigned short zBuffer_t;
std::vector<zBuffer_t> zBuffer;

glm::vec3 lightAmbientColor( 0.05f, 0.05f, 0.05f );
glm::vec3    lightDirection( 1.0f, -1.0f,  0.0f );
glm::vec3        lightColor( 1.0f,  1.0f,  1.0f );


void clear( Pixelator & p, const glm::vec4 & color = glm::vec4( 0, 0, 0, 0 ), float depth = 1.0f )
{
	for( unsigned int i = 0; i < p.getWidth() * p.getHeight(); ++i )
		zBuffer[ i ] = (zBuffer_t)(depth * (float)std::numeric_limits<zBuffer_t>::max());
	glm::vec4 clampedColor = glm::clamp( color, 0.0f, 1.0f ) * 255.0f;
	p.clear( (unsigned char)clampedColor.r,
		 (unsigned char)clampedColor.g,
		 (unsigned char)clampedColor.b,
		 (unsigned char)clampedColor.a );
}


void setPixel( Pixelator & p, const glm::vec3 & position, const glm::vec4 & color )
{
	// ignore if outside viewport or outside depth range
	if( position.x >= (float)p.getWidth() || position.y >= (float)p.getHeight() ||
	    position.x < 0.0f || position.y < 0.0f ||
	    position.z < 0.0f || position.z > 1.0f )
		return;

	unsigned int index = (unsigned int)position.x + (unsigned int)position.y * p.getWidth();

	zBuffer_t newZ = (zBuffer_t)(position.z * (float)std::numeric_limits<zBuffer_t>::max());
	if( newZ < zBuffer[ index ] )
	{
		zBuffer[ index ] = newZ;
		glm::vec4 clampedColor = glm::clamp( color, 0.0f, 1.0f ) * 255.0f;
		p.setPixel( (unsigned int)position.x,
			    (unsigned int)position.y,
			    (unsigned char)clampedColor.r,
			    (unsigned char)clampedColor.g,
			    (unsigned char)clampedColor.b,
			    (unsigned char)clampedColor.a );
	}
}


void drawLine( Pixelator & p, const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & ac, const glm::vec3 & bc )
{
	glm::vec3 delta_a_b = b - a;
	glm::vec3 delta_ac_bc = bc - ac;
	float steps = std::max( fabs(delta_a_b.x), fabs(delta_a_b.y) );
	glm::vec3 inc_a_b( delta_a_b.x / steps, delta_a_b.y / steps, delta_a_b.z / steps );
	glm::vec3 inc_ac_bc( delta_ac_bc.r / steps, delta_ac_bc.g / steps, delta_ac_bc.b / steps );
	glm::vec3 current_a_b = a;
	glm::vec3 current_ac_bc = ac;
	for( int i=0; i<steps; i++ )
	{
		setPixel( p, current_a_b, glm::vec4( current_ac_bc, 1.0f ) );
		current_a_b += inc_a_b;
		current_ac_bc += inc_ac_bc;
	}
}


void drawTriangle( Pixelator & p, const Face & tf )
{
	// get the bounding box of the triangle
	int maxX = (int)std::max( tf.a.position.x, std::max( tf.b.position.x, tf.c.position.x ) );
	int minX = (int)std::min( tf.a.position.x, std::min( tf.b.position.x, tf.c.position.x ) );
	int maxY = (int)std::max( tf.a.position.y, std::max( tf.b.position.y, tf.c.position.y ) );
	int minY = (int)std::min( tf.a.position.y, std::min( tf.b.position.y, tf.c.position.y ) );

	// clamp bounding box to viewport size
	maxX = glm::clamp<int>( maxX, 0, p.getWidth()-1 );
	minX = glm::clamp<int>( minX, 0, p.getWidth()-1 );
	maxY = glm::clamp<int>( maxY, 0, p.getHeight()-1 );
	minY = glm::clamp<int>( minY, 0, p.getHeight()-1 );

	for( int x = minX; x <= maxX; x++ )
	{
		for( int y = minY; y <= maxY; y++ )
		{
			float den = 1.0f / ( (tf.b.position.y - tf.c.position.y) * (tf.a.position.x - tf.c.position.x)
			                   + (tf.c.position.x - tf.b.position.x) * (tf.a.position.y - tf.c.position.y) );

			float u = ( (tf.b.position.y - tf.c.position.y) * ((float)x - tf.c.position.x)
			          + (tf.c.position.x - tf.b.position.x) * ((float)y - tf.c.position.y) ) * den;

			float v = ( (tf.c.position.y - tf.a.position.y) * ((float)x - tf.c.position.x)
			          + (tf.a.position.x - tf.c.position.x) * ((float)y - tf.c.position.y) ) * den;

			float w = 1.0f - u - v;

			if( (u >= -std::numeric_limits<float>::epsilon()) &&
			    (v >= -std::numeric_limits<float>::epsilon()) &&
			    (u + v <= 1.0f + std::numeric_limits<float>::epsilon()) )
			{ // inside triangle - calculate interpolated vertex attributes at x, y
				// current position and interpolated z value
				glm::vec3 position(
					x,
					y,
					tf.a.position.z*u + tf.b.position.z*v + tf.c.position.z*w
				);

				// interpolated vertex color
				glm::vec3 vcolor(
					tf.a.color.r * u + tf.b.color.r * v + tf.c.color.r * w,
					tf.a.color.g * u + tf.b.color.g * v + tf.c.color.g * w,
					tf.a.color.b * u + tf.b.color.b * v + tf.c.color.b * w
				);

				// interpolated normal
				glm::vec3 normal(
					tf.a.normal.x * u + tf.b.normal.x * v + tf.c.normal.x * w,
					tf.a.normal.y * u + tf.b.normal.y * v + tf.c.normal.y * w,
					tf.a.normal.z * u + tf.b.normal.z * v + tf.c.normal.z * w
				);

				// diffuse lighting color
				float d = glm::dot( glm::normalize(normal), glm::normalize(lightDirection) );
//				d = glm::clamp( d, 0.0f, 1.0f ); // "correct" diffuse lighting - if pixel is facing away from light, it is black
				d = d * 0.5f + 0.5f; // alternate diffuse lighting - causes pixels facing away from light still beeing lit slightly
				glm::vec3 diffuse = lightColor * d;

				glm::vec3 finalColor = vcolor * diffuse + lightAmbientColor;

				setPixel( p, position, glm::vec4( finalColor, 1.0f ) );
			}
		}
	}
}


Vertex getVertex( const BlenderVuforiaExportObject & o, unsigned int vertexIndex )
{
	Vertex v;
	unsigned int i = o.indices[vertexIndex] * 3; // lookup vector index and stride 3 elements (x,y,z)
	v.position = glm::vec3( o.vertices[i], o.vertices[i+1], o.vertices[i+2] );
	v.normal   = glm::vec3( o.normals[i],  o.normals[i+1],  o.normals[i+2] );
	v.color    = glm::vec3( o.colors[i],   o.colors[i+1],   o.colors[i+2] );
	return v;
}


Face getFace( const BlenderVuforiaExportObject & o, unsigned int faceIndex )
{
	Face f;
	unsigned int i = faceIndex * 3; // stride 3 vertices (triangle)
	f.a = getVertex( o, i );
	f.b = getVertex( o, i+1 );
	f.c = getVertex( o, i+2 );
	return f;
}


unsigned int getNumFaces( const BlenderVuforiaExportObject & o )
{
	return o.numIndices / 3;
}


glm::vec3 cc2ndc( const glm::vec4 & cc )
{
	return glm::vec3( cc.x/cc.w, cc.y/cc.w, cc.z/cc.w );
}


glm::vec3 ndc2wc( const glm::vec3 & ndc, const Pixelator & p )
{
	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportWidth = (float)p.getWidth();
	float viewportHeight = (float)p.getHeight();
	float depthNear = 0.0f;
	float depthFar = 1.0f;
	return glm::vec3(
		( viewportWidth * ndc.x ) / 2.0f + ( viewportX + viewportWidth / 2.0f ),
		( viewportHeight * ndc.y ) / 2.0f + ( viewportY + viewportHeight / 2.0f ),
		( (depthFar-depthNear) * ndc.z ) / 2.0f + (depthFar+depthNear) / 2.0f
	);
}


void rasterizeObject( Pixelator & p, const glm::mat4 & mvp, const BlenderVuforiaExportObject & o )
{
	// add object specific transformation (usually identity)
	const glm::mat4 mymvp = mvp * glm::make_mat4( o.transform );

	// rasterize each triangle face
	for( unsigned int i = 0; i<getNumFaces( o ); i++ )
	{
		Face f = getFace( o, i );

		// homogenous vertices
		glm::vec4 va( f.a.position, 1.0f );
		glm::vec4 vb( f.b.position, 1.0f );
		glm::vec4 vc( f.c.position, 1.0f );

		// to clip coordinates
		glm::vec4 ca = mymvp * va;
		glm::vec4 cb = mymvp * vb;
		glm::vec4 cc = mymvp * vc;

		// (very) simple clipping - discards complete triangle
		if( ca.z <= std::numeric_limits<float>::epsilon() ||
		    cb.z <= std::numeric_limits<float>::epsilon() ||
		    cc.z <= std::numeric_limits<float>::epsilon() )
			continue;

		// to device coordinates
		glm::vec3 da = cc2ndc( ca );
		glm::vec3 db = cc2ndc( cb );
		glm::vec3 dc = cc2ndc( cc );

		// to window coordinates
		glm::vec3 wa = ndc2wc( da, p );
		glm::vec3 wb = ndc2wc( db, p );
		glm::vec3 wc = ndc2wc( dc, p );

		// write back transformed coordinates to face
		f.a.position = wa;
		f.b.position = wb;
		f.c.position = wc;

		drawTriangle( p, f );
	}
}


void rasterizeLine( Pixelator & p, const glm::mat4 & mvp,
                    const glm::vec3 & a, const glm::vec3 & b,
                    const glm::vec3 & ac, const glm::vec3 & bc )
{
	// homogenous vertices
	glm::vec4 va( a, 1.0f );
	glm::vec4 vb( b, 1.0f );

	// to clip coordinates
	glm::vec4 ca = mvp * va;
	glm::vec4 cb = mvp * vb;

	// to device coordinates
	glm::vec3 da = cc2ndc( ca );
	glm::vec3 db = cc2ndc( cb );

	// to window coordinates
	glm::vec3 wa = ndc2wc( da, p );
	glm::vec3 wb = ndc2wc( db, p );

	drawLine( p, wa, wb, ac, bc );
}


int main( int argc, char ** argv )
{
	(void)argc;
	(void)argv;

	Pixelator p( 512, 512, 128, 128 );
	zBuffer.resize( p.getWidth() * p.getHeight() );

	while( !glfwWindowShouldClose( p.getWindow() ) )
	{
		static float alpha = 0.0f;       // left mouse button x-axis
		static float beta = 0.0f;       // left mouse button y-axis
		static float distance = 80.0f; // right mouse button y-axis
		static float fov = 90.0f;     // right mouse button x-axis
		static float res = 6;        // middle mouse button y-axis


		////////////////////////////////
		// handle user input

		double mouseX, mouseY;
		glfwGetCursorPos( p.getWindow(), &mouseX, &mouseY );

		static double lastMouseXDown=0, lastMouseYDown=0;
		static bool lastButton = false;
		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) && !lastButton )
		{
			lastButton = true;
			lastMouseXDown = mouseX;
			lastMouseYDown = mouseY;
		}
		else if( !glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) )
			lastButton = false;

		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) )
		{
			alpha += -(float)(lastMouseXDown - mouseX) / 200.0f;
			beta += -(float)(lastMouseYDown - mouseY) / 200.0f;
		}

		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_RIGHT ) )
		{
			fov += -(float)(lastMouseXDown - mouseX) / 10.0f;
			distance += -(float)(lastMouseYDown - mouseY) / 10.0f;
			std::cout << "FOV: " << fov << "\tDistance: " << distance << std::endl;
		}

		static int lastRes = 6;
		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_MIDDLE ) )
		{
			res += -(float)(lastMouseYDown - mouseY) / 80.0f;
		}

		if( res < 4 )
			res = 4;
		else if( res > 9 )
			res = 9;

		if( lastRes != (int)res )
		{
			unsigned int resolution = 1 << (int)res;
			p.resize( resolution, resolution );
			zBuffer.resize( p.getWidth() * p.getHeight() );
			lastRes = (int)res;
			std::cout << "Resolution: " << resolution << "x" << resolution << std::endl;
		}

		lastMouseXDown = mouseX;
		lastMouseYDown = mouseY;


		////////////////////////////////
		// drawing

		glm::mat4 model( 1.0f );
		model *= glm::rotate( alpha, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		model *= glm::rotate( beta, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		glm::mat4 view = glm::lookAt(
			glm::vec3( 0.0f, -distance, 0.0f ), // camera position
			glm::vec3( 0.0f, 0.0f, 0.0f ),     // look at
			glm::vec3( 0.0f, 0.0f, 1.0f )     // up
		);

		glm::mat4 projection = glm::perspective(
			glm::radians<float>( fov ), // FOV
			1.0f,                      // aspect ratio
			1.0f,                     // near
			200.0f                   // far
		);

		// final model view projection matrix
		glm::mat4 mvp = projection * view * model;

		static float angle = 0.0f;
		angle += 0.01f;
		// let light circle around model
		lightDirection.x = sin( angle );
		lightDirection.y = cos( angle );
		// change light color a bit
		lightColor.r = sin( angle * 2.0f ) * 0.25f + 0.75f;
		lightColor.g = sin( angle * 2.5f ) * 0.25f + 0.75f;
		lightColor.b = sin( angle * 2.75f ) * 0.25f + 0.75f;

		// clear color and depth buffer
		clear( p );

		// draw model
//		rasterizeObject( p, mvp, cubeObject );
		rasterizeObject( p, mvp, suzanneObject);
//		rasterizeObject( p, mvp, waveObject );

		// draw coordinate markers         from                     to                 from color             to color
		rasterizeLine( p, mvp, glm::vec3( 0, 0, 0 ), glm::vec3( 60,  0,  0 ), glm::vec3( 1, 1, 1 ), glm::vec3( 1, 0, 0 ) );
		rasterizeLine( p, mvp, glm::vec3( 0, 0, 0 ), glm::vec3(  0, 60,  0 ), glm::vec3( 1, 1, 1 ), glm::vec3( 0, 1, 0 ) );
		rasterizeLine( p, mvp, glm::vec3( 0, 0, 0 ), glm::vec3(  0,  0, 60 ), glm::vec3( 1, 1, 1 ), glm::vec3( 0, 0, 1 ) );

		p.update();
	}

	return 0;
}
