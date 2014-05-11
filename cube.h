#ifndef _BLENDER_VUFORIA_EXPORT__CUBE_H
#define _BLENDER_VUFORIA_EXPORT__CUBE_H


#ifndef _BLENDER_VUFORIA_EXPORT_OBJECT_STRUCT_
#define _BLENDER_VUFORIA_EXPORT_OBJECT_STRUCT_

struct BlenderVuforiaExportObject
{
	unsigned int numVertices;
	const float * vertices;
	const float * normals;
	const float * colors;
	const float * texCoords;

	unsigned int numIndices;
	const unsigned short * indices;

	const float * transform;
};

#endif

#define _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES 24
#define _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_INDICES (12 * 3)

static const float _BlenderVuforiaExportObject_cubeVertices[ _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES * 3 ] =
{
	63.999968f, 63.999968f, -63.999968f,
	63.999968f, -63.999968f, -63.999968f,
	-63.999968f, -63.999968f, -63.999968f,
	-63.999968f, 63.999968f, -63.999968f,
	63.999968f, 63.999904f, 63.999968f,
	-63.999968f, 63.999968f, 63.999968f,
	-63.999968f, -63.999968f, 63.999968f,
	63.999904f, -64.000032f, 63.999968f,
	63.999968f, 63.999968f, -63.999968f,
	63.999968f, 63.999904f, 63.999968f,
	63.999904f, -64.000032f, 63.999968f,
	63.999968f, -63.999968f, -63.999968f,
	63.999968f, -63.999968f, -63.999968f,
	63.999904f, -64.000032f, 63.999968f,
	-63.999968f, -63.999968f, 63.999968f,
	-63.999968f, -63.999968f, -63.999968f,
	-63.999968f, -63.999968f, -63.999968f,
	-63.999968f, -63.999968f, 63.999968f,
	-63.999968f, 63.999968f, 63.999968f,
	-63.999968f, 63.999968f, -63.999968f,
	63.999968f, 63.999904f, 63.999968f,
	63.999968f, 63.999968f, -63.999968f,
	-63.999968f, 63.999968f, -63.999968f,
	-63.999968f, 63.999968f, 63.999968f,

};

static const float _BlenderVuforiaExportObject_cubeTexCoords[ _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES * 2 ] =
{
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
	0.000000f, 0.000000f,
};

static const float _BlenderVuforiaExportObject_cubeNormals[ _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES * 3 ] =
{
	0.000000f, 0.000000f, -1.000000f,
	0.000000f, 0.000000f, -1.000000f,
	0.000000f, 0.000000f, -1.000000f,
	0.000000f, 0.000000f, -1.000000f,
	0.000000f, -0.000000f, 1.000000f,
	0.000000f, -0.000000f, 1.000000f,
	0.000000f, -0.000000f, 1.000000f,
	0.000000f, -0.000000f, 1.000000f,
	1.000000f, -0.000000f, 0.000000f,
	1.000000f, -0.000000f, 0.000000f,
	1.000000f, -0.000000f, 0.000000f,
	1.000000f, -0.000000f, 0.000000f,
	-0.000000f, -1.000000f, -0.000000f,
	-0.000000f, -1.000000f, -0.000000f,
	-0.000000f, -1.000000f, -0.000000f,
	-0.000000f, -1.000000f, -0.000000f,
	-1.000000f, 0.000000f, -0.000000f,
	-1.000000f, 0.000000f, -0.000000f,
	-1.000000f, 0.000000f, -0.000000f,
	-1.000000f, 0.000000f, -0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
};

static const float _BlenderVuforiaExportObject_cubeColors[ _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES * 3 ] =
{
	0.000000f, 0.000000f, 1.000000f,
	0.000000f, 1.000000f, 0.000000f,
	1.000000f, 1.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
	0.000000f, 0.000000f, 0.000000f,
	1.000000f, 0.000000f, 0.000000f,
	0.000000f, 0.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
	1.000000f, 0.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	1.000000f, 0.000000f, 0.000000f,
	0.000000f, 0.000000f, 0.000000f,
	1.000000f, 1.000000f, 1.000000f,
	0.000000f, 0.000000f, 1.000000f,
	0.000000f, 0.000000f, 0.000000f,
	1.000000f, 0.000000f, 0.000000f,
	0.000000f, 1.000000f, 0.000000f,
	1.000000f, 1.000000f, 1.000000f,
	0.000000f, 0.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
	1.000000f, 1.000000f, 1.000000f,
};

static const unsigned short _BlenderVuforiaExportObject_cubeIndices[ _BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_INDICES ] =
{
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	16, 18, 19,
	20, 21, 22,
	20, 22, 23,
};

static const float _BlenderVuforiaExportObject_cubeTransform[ 16 ] =
{
	0.500000f, 0.000000f, 0.000000f, 0.000000f,
	0.000000f, 0.500000f, 0.000000f, 0.000000f,
	0.000000f, 0.000000f, 0.500000f, 0.000000f,
	0.000000f, 0.000000f, 0.000000f, 1.000000f,
};

static const BlenderVuforiaExportObject cubeObject =
{
	_BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_VERTICES,
	_BlenderVuforiaExportObject_cubeVertices,
	_BlenderVuforiaExportObject_cubeNormals,
	_BlenderVuforiaExportObject_cubeColors,
	_BlenderVuforiaExportObject_cubeTexCoords,
	_BLENDER_VUFORIA_EXPORT__CUBE_OBJECT_NUM_INDICES,
	_BlenderVuforiaExportObject_cubeIndices,
	_BlenderVuforiaExportObject_cubeTransform,
};


#endif
