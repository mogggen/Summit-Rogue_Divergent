#include "config.h"
#include "render/MeshResource.h"
#include <stdio.h>
#include <inttypes.h>

MeshResource::MeshResource(Vertex vertices[], uint32_t verticesLength, uint32_t indices[], uint32_t indicesLength) : indices(indicesLength)
{
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticesLength, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesLength, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshResource::render()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 7));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 9));

	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::shared_ptr<MeshResource> MeshResource::Cube()
{
	V4 top(0, 255, 0, 100);		  // red
	V4 back(128, 66, 128, 100);	  // gray
	V4 left(0, 0, 255, 100);	  // blue
	V4 right(255, 0, 0, 100);	  // red
	V4 front(255, 165, 0, 100);	  // orange
	V4 bottom(64, 224, 208, 100); // turquoise

	float factor = .01f;
	top = top * factor;
	back = back * factor;
	left = left * factor;
	right = right * factor;
	front = front * factor;
	bottom = bottom * factor;

	Vertex vertices[] = // world points
		{
			// Mesh:	(0,1)	(1,1)	Tex:(0,0)	(1,0)
			//		(0,0)	(1,0)		(0,1)	(1,1)

			// xyz
			// 000
			Vertex // back : 0
			{
				V3(-.5f, -.5f, -.5f), // position, will transform later with the projection matrix
				back,				  // raw color 0-1 : black-white
				V2(1, 1)			  // texture position, when the indicies are added, combined with this data, the orientation and size of the texture will make sense
			},
			Vertex // left : 1
			{
				V3(-.5f, -.5f, -.5f),
				left,
				V2(0, 1)},
			Vertex // bottom : 2
			{
				V3(-.5f, -.5f, -.5f),
				bottom,
				V2(1, 0)},

			// 100
			Vertex // back : 3
			{
				V3(.5f, -.5f, -.5f),
				back,
				V2(0, 1)},
			Vertex // right : 4
			{
				V3(.5f, -.5f, -.5f),
				right,
				V2(1, 1)},
			Vertex // bottom : 5
			{
				V3(.5f, -.5f, -.5f),
				bottom,
				V2(0, 0)},

			// 010
			Vertex // back : 6
			{
				V3(-.5f, .5f, -.5f),
				back,
				V2(1, 0)},
			Vertex // left : 7
			{
				V3(-.5f, .5f, -.5f),
				left,
				V2(0, 0)},
			Vertex // top : 8
			{
				V3(-.5f, .5f, -.5f),
				top,
				V2(0, 0)},

			// 110
			Vertex // back : 9
			{
				V3(.5f, .5f, -.5f),
				back,
				V2(0, 0)},
			Vertex // right : 10
			{
				V3(.5f, .5f, -.5f),
				right,
				V2(1, 0)},
			Vertex // top : 11
			{
				V3(.5f, .5f, -.5f),
				top,
				V2(1, 0)},

			// 001
			Vertex // left : 12
			{
				V3(-.5f, -.5f, .5f),
				left,
				V2(1, 1)},
			Vertex // front : 13
			{
				V3(-.5f, -.5f, .5f),
				front,
				V2(0, 1)},
			Vertex // bottom : 14
			{
				V3(-.5f, -.5f, .5f),
				bottom,
				V2(1, 1)},

			// 101
			Vertex // right : 15
			{
				V3(.5f, -.5f, .5f),
				right,
				V2(0, 1)},
			Vertex // front : 16
			{
				V3(.5f, -.5f, .5f),
				front,
				V2(1, 1)},
			Vertex // bottom : 17
			{
				V3(.5f, -.5f, .5f),
				bottom,
				V2(0, 1)},

			// 011
			Vertex // left : 18
			{
				V3(-.5f, .5f, .5f),
				left,
				V2(1, 0)},
			Vertex // front : 19
			{
				V3(-.5f, .5f, .5f),
				front,
				V2(0, 0)},
			Vertex // top : 20
			{
				V3(-.5f, .5f, .5f),
				top,
				V2(0, 1)},

			// 111
			Vertex // right : 21
			{
				V3(.5f, .5f, .5f),
				right,
				V2(0, 0)},
			Vertex // front : 22
			{
				V3(.5f, .5f, .5f),
				front,
				V2(1, 0)},
			Vertex // top : 23
			{
				V3(.5f, .5f, .5f),
				top,
				V2(1, 1)},
		};

	uint32_t indices[] // World point's relations to form triangles and surfaces with razterisation
		{
			0, 3, 6,
			3, 6, 9, // back

			1, 12, 7,
			7, 12, 18, // left

			4, 15, 10,
			10, 15, 21, // right

			13, 16, 19,
			16, 19, 22, // front

			20, 8, 23,
			8, 23, 11, // top

			2, 5, 14,
			5, 14, 17, // bottom
		};

	return std::make_shared<MeshResource>(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(uint64_t));
}

std::shared_ptr<MeshResource> MeshResource::LoadObj(const char *pathToFile)
{
	char buf[1024];
	FILE *fs;
#ifndef __linux
	fopen_s(&fs, pathToFile, "r"); // textures/sphere.obj
#else
	fs = fopen64(pathToFile, "r"); // "textures/sphere.obj"
#endif

	unsigned long long verticesUsed = 0ull;
	std::vector<uint32_t> indices;
	std::vector<V3> coords;
	std::vector<V2> texels;
	std::vector<V3> normals;
	std::vector<Vertex> vertices; // complete package

	if (fs)
	{
		while (true)
		{
			int foo = fscanf(fs, "%1024s", buf); // reads one word at a time and places it at buf[0] with a trailing '\0'
			if (foo <= 0)
			{
				break;
			}

			if (buf[0] == 'v' && buf[1] == '\0')
			{
				V3 nextCoordinate;
				if (fscanf(fs, "%f %f %f", &nextCoordinate.x, &nextCoordinate.y, &nextCoordinate.z) == 3)
				{
					coords.push_back(nextCoordinate);
				}
				else
				{
					std::cerr << "missing arguments in vertex, expected 3" << std::endl;
				}
			}

			else if (buf[0] == 'v' && buf[1] == 't' && buf[2] == '\0')
			{
				V2 nextTexel;
				if (fscanf(fs, "%f %f", &nextTexel.x, &nextTexel.y) == 2)
				{
					texels.push_back(nextTexel);
				}
				else
				{
					std::cerr << "missing arguments in texel, expected 2" << std::endl;
				}
			}

			else if (buf[0] == 'v' && buf[1] == 'n' && buf[2] == '\0')
			{
				V3 nextNormal;
				if (fscanf(fs, "%f %f %f", &nextNormal.x, &nextNormal.y, &nextNormal.z) == 3)
				{
					normals.push_back(nextNormal);
				}
				else
				{
					std::cerr << "missing arguments in texel, expected 3" << std::endl;
				}
			}

			else if (buf[0] == 'f' && buf[1] == '\0')
			{
				char pos[4][64];
				uint8_t argc = fscanf(fs, "%s %s %s %s", &pos[0], &pos[1], &pos[2], &pos[3]);

				uint32_t listOfIndices[4][3];

				if (argc == 4 && pos[3][0] != 'f' && pos[3][0] != '#')
				{
					for (size_t i = 0; i < 4; i++)
					{
						if (sscanf(pos[i], "%lu"
										   "/ %lu"
										   "/ %lu"
										   "/",
								   &listOfIndices[i][0], &listOfIndices[i][1], &listOfIndices[i][2]) != 3)
							break;
							vertices.push_back(Vertex{
								coords[(listOfIndices[i][0]) - 1],
								V4(1, 1, 1, 1),
								texels[(listOfIndices[i][1]) - 1],
								normals[(listOfIndices[i][2]) - 1],
							});
					}

					float dist1 = (vertices[vertices.size() - 4].pos - vertices[vertices.size() - 2].pos).Length();
					float dist2 = (vertices[vertices.size() - 3].pos - vertices[vertices.size() - 1].pos).Length();
					if (dist1 > dist2)
					{
						indices.push_back(vertices.size() - 4);
						indices.push_back(vertices.size() - 3);
						indices.push_back(vertices.size() - 1);

						indices.push_back(vertices.size() - 3);
						indices.push_back(vertices.size() - 2);
						indices.push_back(vertices.size() - 1);
					}
					else
					{
						indices.push_back(vertices.size() - 4);
						indices.push_back(vertices.size() - 3);
						indices.push_back(vertices.size() - 2);

						indices.push_back(vertices.size() - 4);
						indices.push_back(vertices.size() - 2);
						indices.push_back(vertices.size() - 1);
					}
				}
				else if (argc == 3)
				{
					for (size_t i = 0; i < 3; i++)
					{
						if (sscanf(pos[i], "%lu"
										   "/ %lu"
										   "/ %lu"
										   "/",
								   &listOfIndices[i][0], &listOfIndices[i][1], &listOfIndices[i][2]) != 3)
								   break;

							vertices.push_back(Vertex{
								coords[listOfIndices[i][0] - 1],
								V4(1, 1, 1, 1),
								texels[listOfIndices[i][1] - 1],
								normals[listOfIndices[i][2] - 1],
							});
						indices.push_back(vertices.size() - 1);
					}
				}
			}
		}
	}
	else
	{
		printf("file not found with path \"./%s\"\n", pathToFile);
	}
	fclose(fs);
	printf("loaded %s\n", pathToFile);
	return std::make_shared<MeshResource>(&vertices[0], vertices.size(), &indices[0], indices.size());
}

/// <summary>
/// Destroy glBuffers
/// </summary>
void MeshResource::Destroy()
{
	glDeleteBuffers(1, &this->indexBuffer);
	glDeleteBuffers(1, &this->vertexBuffer);
}

MeshResource::~MeshResource()
{
	Destroy();
}