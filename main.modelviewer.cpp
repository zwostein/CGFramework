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


typedef unsigned short zBuffer_t;
std::vector<zBuffer_t> zBuffer;


glm::vec3 lightDirection(1,-1,0);


template<typename T> T clamped( const T & value, const T & min, const T & max )
{
	if( value < min )
		return min;
	if( value > max )
		return max;
	return value;
}


void clear( Pixelator & p, const glm::vec4 & color = glm::vec4(0,0,0,0), float depth = 1.0f )
{
	for( unsigned int i = 0; i < p.getWidth() * p.getHeight(); ++i )
		zBuffer[ i ] = (zBuffer_t)(depth * (float)std::numeric_limits<zBuffer_t>::max());
	p.clear( (unsigned char)(color.r*255.0f), (unsigned char)(color.g*255.0f), (unsigned char)(color.b*255.0f), (unsigned char)(color.a*255.0f) );
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
		p.setPixel( (unsigned int)position.x, (unsigned int)position.y, (unsigned char)(color.r*255.0f), (unsigned char)(color.g*255.0f), (unsigned char)(color.b*255.0f), (unsigned char)(color.a*255.0f) );
	}
}


void drawLine( Pixelator & p, const glm::vec3 & a, const glm::vec3 & b, const glm::vec4 & ac, const glm::vec4 & bc )
{
	glm::vec3 delta_a_b = b - a;
	glm::vec4 delta_ac_bc = bc - ac;
	float steps = std::max( fabs(delta_a_b.x), fabs(delta_a_b.y) );
	glm::vec3 inc_a_b( delta_a_b.x / steps, delta_a_b.y / steps, delta_a_b.z / steps );
	glm::vec4 inc_ac_bc( delta_ac_bc.r / steps, delta_ac_bc.g / steps, delta_ac_bc.b / steps, delta_ac_bc.a / steps );
	glm::vec3 current_a_b = a;
	glm::vec4 current_ac_bc = ac;
	for( int i=0; i<steps; i++ )
	{
		setPixel( p, current_a_b, current_ac_bc );
		current_a_b += inc_a_b;
		current_ac_bc += inc_ac_bc;
	}
}


void drawTriangle( Pixelator & p,
	const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c,
	const glm::vec3 & an, const glm::vec3 & bn, const glm::vec3 & cn,
	const glm::vec4 & ac, const glm::vec4 & bc, const glm::vec4 & cc )
{
	// get the bounding box of the triangle
	int maxX = (int)std::max( a.x, std::max( b.x, c.x ) );
	int minX = (int)std::min( a.x, std::min( b.x, c.x ) );
	int maxY = (int)std::max( a.y, std::max( b.y, c.y ) );
	int minY = (int)std::min( a.y, std::min( b.y, c.y ) );

	// clamp bounding box to viewport size
	maxX = clamped<int>( maxX, 0, p.getWidth()-1 );
	minX = clamped<int>( minX, 0, p.getWidth()-1 );
	maxY = clamped<int>( maxY, 0, p.getHeight()-1 );
	minY = clamped<int>( minY, 0, p.getHeight()-1 );

	for( int x = minX; x <= maxX; x++ )
	{
		for( int y = minY; y <= maxY; y++ )
		{
			float den = 1.0f / ( (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y) );
			float u = ( (b.y - c.y) * ((float)x - c.x) + (c.x - b.x) * ((float)y - c.y) ) * den;
			float v = ( (c.y - a.y) * ((float)x - c.x) + (a.x - c.x) * ((float)y - c.y) ) * den;

			float w = 1.0f - u - v;

			if( (u >= -std::numeric_limits<float>::epsilon()) &&
			    (v >= -std::numeric_limits<float>::epsilon()) &&
			    (u + v <= 1.0 + std::numeric_limits<float>::epsilon()) )
			{ // inside triangle - calculate interpolated vertex attributes at x, y
				// current position and interpolated z value
				glm::vec3 position(
					x, y, a.z*u + b.z*v + c.z*w
				);
				// interpolated normal
				glm::vec3 normal(
					an.x * u + bn.x * v + cn.x * w,
					an.y * u + bn.y * v + cn.y * w,
					an.z * u + bn.z * v + cn.z * w
				);
				// lighting color
				float d = glm::dot( glm::normalize(normal), glm::normalize(lightDirection) );
//				d = clamped( d, 0.0f, 1.0f );
				d = d * 0.5f + 0.5f;
				glm::vec4 diffuse( d, d, d, 1.0f );
				// interpolated vertex color
				glm::vec4 vcolor(
					ac.r * u + bc.r * v + cc.r * w,
					ac.g * u + bc.g * v + cc.g * w,
					ac.b * u + bc.b * v + cc.b * w,
					ac.a * u + bc.a * v + cc.a * w
				);
				setPixel( p, position, vcolor * diffuse );
			}
		}
	}
}


glm::vec3 getVertex( const BlenderVuforiaExportObject & o, unsigned int index )
{
	return glm::vec3(
		o.vertices[o.indices[index]*3],
		o.vertices[o.indices[index]*3+1],
		o.vertices[o.indices[index]*3+2]
	);
}


glm::vec3 getNormal( const BlenderVuforiaExportObject & o, unsigned int index )
{
	return glm::vec3(
		o.normals[o.indices[index]*3],
		o.normals[o.indices[index]*3+1],
		o.normals[o.indices[index]*3+2]
	);
}


glm::vec3 getColor( const BlenderVuforiaExportObject & o, unsigned int index )
{
	return glm::vec3(
		o.colors[o.indices[index]*3],
		o.colors[o.indices[index]*3+1],
		o.colors[o.indices[index]*3+2]
	);
}


glm::vec3 cc2ndc( const glm::vec4 & cc )
{
	return glm::vec3( cc.x/cc.w, cc.y/cc.w, cc.z/cc.w );
}


glm::vec3 ndc2wc( const glm::vec3 & ndc, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float depthNear, float depthFar )
{
	return glm::vec3(
		(viewportWidth *ndc.x)/2.0f + (viewportX + viewportWidth /2.0f),
		(viewportHeight*ndc.y)/2.0f + (viewportY + viewportHeight/2.0f),
		((depthFar-depthNear)*ndc.z)/2.0f + (depthFar+depthNear)/2.0f
	);
}


void rasterizeObject( Pixelator & p, const BlenderVuforiaExportObject & o, const glm::mat4 & modelview, const glm::mat4 & projection )
{
	// calculate model-view-projection matrix and add object specific transformation (usually identity)
	glm::mat4 mvp = projection * modelview * glm::make_mat4( o.transform );

	// rasterize object indices as triangles
	for( unsigned int i = 0; i<o.numIndices; i+=3 )
	{
		// homogenous vertices
		glm::vec4 v0( getVertex( o, i ), 1.0f );
		glm::vec4 v1( getVertex( o, i+1 ), 1.0f );
		glm::vec4 v2( getVertex( o, i+2 ), 1.0f );

		// to clip coordinates
		glm::vec4 c0 = mvp * v0;
		glm::vec4 c1 = mvp * v1;
		glm::vec4 c2 = mvp * v2;

		// to device coordinates
		glm::vec3 d0 = cc2ndc( c0 );
		glm::vec3 d1 = cc2ndc( c1 );
		glm::vec3 d2 = cc2ndc( c2 );

		// to window coordinates
		glm::vec3 w0 = ndc2wc( d0, 0.0f, 0.0f, (float)p.getWidth(), (float)p.getHeight(), 0.0f, 1.0f );
		glm::vec3 w1 = ndc2wc( d1, 0.0f, 0.0f, (float)p.getWidth(), (float)p.getHeight(), 0.0f, 1.0f );
		glm::vec3 w2 = ndc2wc( d2, 0.0f, 0.0f, (float)p.getWidth(), (float)p.getHeight(), 0.0f, 1.0f );

		// vertex colors
		glm::vec4 col0( getColor( o, i ), 1.0f );
		glm::vec4 col1( getColor( o, i+1 ), 1.0f );
		glm::vec4 col2( getColor( o, i+2 ), 1.0f );

		// vertex normals
		glm::vec3 n0( getNormal( o, i ) );
		glm::vec3 n1( getNormal( o, i+1 ) );
		glm::vec3 n2( getNormal( o, i+2 ) );

		drawTriangle( p, w0, w1, w2, n0, n1, n2, col0, col1, col2 );
	}
}


void rasterizeLine( Pixelator & p,
	const glm::vec3 & a,
	const glm::vec3 & b,
	const glm::vec4 & ac,
	const glm::vec4 & bc,
	const glm::mat4 & modelview, const glm::mat4 & projection )
{
	// calculate model-view-projection matrix
	glm::mat4 mvp = projection * modelview;

	// homogenous vertices
	glm::vec4 v0( a, 1.0f );
	glm::vec4 v1( b, 1.0f );

	// to clip coordinates
	glm::vec4 c0 = mvp * v0;
	glm::vec4 c1 = mvp * v1;

	// to device coordinates
	glm::vec3 d0 = cc2ndc( c0 );
	glm::vec3 d1 = cc2ndc( c1 );

	// to window coordinates
	glm::vec3 w0 = ndc2wc( d0, 0.0f, 0.0f, (float)p.getWidth(), (float)p.getHeight(), 0.0f, 1.0f );
	glm::vec3 w1 = ndc2wc( d1, 0.0f, 0.0f, (float)p.getWidth(), (float)p.getHeight(), 0.0f, 1.0f );

	drawLine( p, w0, w1, ac, bc );
}


int main( int argc, char ** argv )
{
	(void)argc;
	(void)argv;

	Pixelator p( 512, 512, 128, 128 );
	zBuffer.resize( p.getWidth() * p.getHeight() );

	while( !glfwWindowShouldClose( p.getWindow() ) )
	{
		static float alpha = 0.0f;
		static float beta = 0.0f;
		static float distance = 80.0f;
		static float fov = 90.0f;
		static float res = 6;


		////////////////////////////////
		// handle user input

		double mouseX, mouseY;
		glfwGetCursorPos( p.getWindow(), &mouseX, &mouseY );

		static double lastMouseXDownLeft=0, lastMouseYDownLeft=0;
		static bool lastButtonLeft = false;
		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) && !lastButtonLeft )
		{
			lastButtonLeft = true;
			lastMouseXDownLeft = mouseX;
			lastMouseYDownLeft = mouseY;
		}
		else if( !glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) )
			lastButtonLeft = false;

		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_LEFT ) )
		{
			alpha += -(float)(lastMouseXDownLeft - mouseX) / 200.0f;
			beta += -(float)(lastMouseYDownLeft - mouseY) / 200.0f;
		}

		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_RIGHT ) )
		{
			fov += -(float)(lastMouseXDownLeft - mouseX) / 10.0f;
			distance += -(float)(lastMouseYDownLeft - mouseY) / 10.0f;
			std::cout << "FOV: " << fov << "\tDistance: " << distance << std::endl;
		}

		static int lastRes = 6;
		if( glfwGetMouseButton( p.getWindow(), GLFW_MOUSE_BUTTON_MIDDLE ) )
		{
			res += -(float)(lastMouseYDownLeft - mouseY) / 80.0f;
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

		lastMouseXDownLeft = mouseX;
		lastMouseYDownLeft = mouseY;


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
			glm::radians<float>(fov), // FOV
			1.0f,                    // aspect ratio
			1.0f,                   // near
			200.0f                 // far
		);

		static float angle = 0.0f;
		angle += 0.01f;
		lightDirection.x = sin(angle);
		lightDirection.y = cos(angle);

		// clear color and depth buffer
		clear( p );

		// draw model
//		rasterizeObject( p, cubeObject, view * model, projection );
		rasterizeObject( p, suzanneObject, view * model, projection );
//		rasterizeObject( p, waveObject, view * model, projection );

		// draw coordinate markers
		rasterizeLine( p, glm::vec3(0,0,0), glm::vec3(60,0,0), glm::vec4(1,1,1,1), glm::vec4(1,0,0,1), view * model, projection );
		rasterizeLine( p, glm::vec3(0,0,0), glm::vec3(0,60,0), glm::vec4(1,1,1,1), glm::vec4(0,1,0,1), view * model, projection );
		rasterizeLine( p, glm::vec3(0,0,0), glm::vec3(0,0,60), glm::vec4(1,1,1,1), glm::vec4(0,0,1,1), view * model, projection );

		p.update();
	}

	return 0;
}
