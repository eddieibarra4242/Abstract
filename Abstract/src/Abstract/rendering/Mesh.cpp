#include "Mesh.h"
#include <array>
#include <fstream>
#include <set>

namespace Abstract {

	Mesh::Mesh(const std::string& name, const std::vector<float>& vertexData, size_t vertexSize, const std::vector<uint32_t>& indices)
	{
		this->name = name;
		this->drawCount = indices.size();
		this->vertexSize = vertexSize;

		glGenVertexArrays(1, &this->vertexArrayObject);
		glBindVertexArray(this->vertexArrayObject);

		glGenBuffers(1, &this->vertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vertexData[0]), &vertexData[0], GL_STATIC_DRAW);

		glGenBuffers(1, &this->indexElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		this->instancedTransformArrayBuffer = 0;
		//glGenBuffers(1, &this->instancedTransformArrayBuffer);
		//data to be sent at render time

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		//eventually implement this
		//if (vertexArrayObject != 0)
		//{
		//	glBindVertexArray(vertexArrayObject);
		//	glDeleteBuffers(1, &indexElementBuffer);
		//	glDeleteBuffers(1, &vertexArrayBuffer);
		//	if (instancedTransformArrayBuffer != 0) { glDeleteBuffers(1, &instancedTransformArrayBuffer); }
		//	glBindVertexArray(0);
		//	glDeleteVertexArrays(1, &vertexArrayObject);
		//}
	}

	bool IndexedModel::isValid() const
	{
		return positions.size() == texCoords.size()
			&& texCoords.size() == normals.size()
			&& normals.size() == tangents.size()
			&& vertexData.size() != 0;
	}

	void IndexedModel::addVertex(const Vector3& vert)
	{
		positions.push_back(vert);
	}

	void IndexedModel::addTexCoord(const Vector2& texCoord)
	{
		texCoords.push_back(texCoord);
	}

	void IndexedModel::addNormal(const Vector3& normal)
	{
		normals.push_back(normal);
	}

	void IndexedModel::addTangent(const Vector3& tangent)
	{
		tangents.push_back(tangent);
	}

	IndexedModel IndexedModel::finalize()
	{
		if (isValid())
		{
			return *this;
		}

		if (texCoords.size() == 0)
		{
			for (uint64_t i = texCoords.size(); i < positions.size(); i++)
			{
				texCoords.push_back(Vector2(0.0f, 0.0f));
			}
		}

		if (normals.size() == 0)
		{
			calcNormals();
		}

		if (tangents.size() == 0)
		{
			calcTangents();
		}

		for (uint32_t i = 0; i < positions.size(); i++)
		{
			vertexData.push_back(positions[i].getX());
			vertexData.push_back(positions[i].getY());
			vertexData.push_back(positions[i].getZ());
			vertexData.push_back(texCoords[i].getX());
			vertexData.push_back(texCoords[i].getY());
			vertexData.push_back(normals[i].getX());
			vertexData.push_back(normals[i].getY());
			vertexData.push_back(normals[i].getZ());
			vertexData.push_back(tangents[i].getX());
			vertexData.push_back(tangents[i].getY());
			vertexData.push_back(tangents[i].getZ());
		}

		return *this;
	}

	Mesh IndexedModel::finalizeToMesh()
	{
		finalize();
		return Mesh(name, vertexData, vertexData.size() * sizeof(vertexData[0]) / positions.size(), indices);
	}

	void IndexedModel::addFace(unsigned int vertIndex0, unsigned int vertIndex1, unsigned int vertIndex2)
	{
		indices.push_back(vertIndex0);
		indices.push_back(vertIndex1);
		indices.push_back(vertIndex2);
	}

	void IndexedModel::calcNormals()
	{
		normals.clear();
		normals.reserve(positions.size());

		for (uint64_t i = 0; i < positions.size(); i++)
			normals.push_back(Vector3(0, 0, 0));

		for (uint64_t i = 0; i < indices.size(); i += 3)
		{
			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			Vector3 v1 = positions[i1] - positions[i0];
			Vector3 v2 = positions[i2] - positions[i0];

			Vector3 normal = v1.cross(v2).normalized();

			normals[i0] = normals[i0] + normal;
			normals[i1] = normals[i1] + normal;
			normals[i2] = normals[i2] + normal;
		}

		for (uint64_t i = 0; i < normals.size(); i++)
			normals[i] = normals[i].normalized();
	}

	void IndexedModel::calcTangents()
	{
		tangents.clear();
		tangents.reserve(positions.size());

		for (uint64_t i = 0; i < positions.size(); i++)
			tangents.push_back(Vector3(0, 0, 0));

		for (uint64_t i = 0; i < indices.size(); i += 3)
		{
			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			Vector3 edge1 = positions[i1] - positions[i0];
			Vector3 edge2 = positions[i2] - positions[i0];

			float deltaU1 = texCoords[i1].getX() - texCoords[i0].getX();
			float deltaU2 = texCoords[i2].getX() - texCoords[i0].getX();
			float deltaV1 = texCoords[i1].getY() - texCoords[i0].getY();
			float deltaV2 = texCoords[i2].getY() - texCoords[i0].getY();

			float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
			float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

			Vector3 tangent = Vector3(0, 0, 0);

			tangent.setX(f * (deltaV2 * edge1.getX() - deltaV1 * edge2.getX()));
			tangent.setY(f * (deltaV2 * edge1.getY() - deltaV1 * edge2.getY()));
			tangent.setZ(f * (deltaV2 * edge1.getZ() - deltaV1 * edge2.getZ()));

			tangents[i0] += tangent;
			tangents[i1] += tangent;
			tangents[i2] += tangent;
		}

		for (unsigned int i = 0; i < tangents.size(); i++)
			tangents[i] = tangents[i].normalized();
	}

#pragma region Mesh Loading

	//Maybe move this code into it's own file or class

	std::vector<std::string> split(const std::string& str, const char* c, size_t clen)
	{
		std::string buffer = "";
		std::vector<std::string> tokens;

		for (auto cur : str)
		{
			bool append = true;

			for (uint32_t i = 0; i < clen; i++)
			{
				if (cur == c[i])
				{
					tokens.push_back(buffer);
					buffer = "";
					append = false;
				}
			}

			if (append)
			{
				buffer += cur;
			}
		}

		if (buffer != "")
		{
			tokens.push_back(buffer);
		}

		return tokens;
	}

	struct VData_t
	{
		int data[3];
		uint32_t index;

		constexpr bool operator<(const VData_t& other) const
		{
			for (int i = 2; i >= 0; i--)
			{
				if (data[i] != other.data[i])
				{
					return data[i] < other.data[i];
				}
			}

			return false;
		}
	};

	uint32_t uniqueMeshID = 0;

	IndexedModel RenderTools::loadModelFromObj(const std::string& filename)
	{
		std::vector<Vector3> positionPool;
		std::vector<Vector2> texCoordPool;
		std::vector<Vector3> normalPool;
		std::vector<std::vector<VData_t>> faces;
		std::set<VData_t> createdVertices;
		std::string name = "";

		std::vector<Vector3> positions;
		std::vector<Vector2> texCoords;
		std::vector<Vector3> normals;

		std::ifstream file;

		file.open(filename.c_str());

		if (file.is_open())
		{
			char space = ' ';
			char slash = '/';

			while (file.good())
			{
				std::string line;
				std::getline(file, line);

				std::vector<std::string> tokens = split(line, &space, 1);

				if (tokens.size() == 0 || tokens[0].at(0) == '#')
				{
					continue;
				}

				if (tokens[0] == "o")
				{
					if (name != "")
					{
						DEBUG_WARN("Mesh Loader", "Multiple Models in one Obj not supported! Model Name: %s", tokens[1]);
						break;
					}

					name = tokens[1];
				}
				else if (tokens[0] == "v")
				{
					positionPool.push_back(Vector3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
				}
				else if (tokens[0] == "vt")
				{
					texCoordPool.push_back(Vector2(std::stof(tokens[1]), std::stof(tokens[2])));
				}
				else if (tokens[0] == "vn")
				{
					normalPool.push_back(Vector3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
				}
				else if (tokens[0] == "f")
				{
					std::vector<VData_t> face;

					for (uint32_t i = 1; i < tokens.size(); i++)
					{
						std::vector<std::string> t = split(tokens[i], &slash, 1);
						VData_t vertex;

						for (uint32_t j = 0; j < 3; j++)
						{
							if (j < t.size() && t[j] != "")
							{
								vertex.data[j] = std::stoi(t[j]) - 1;
							}
							else
							{
								vertex.data[j] = -1;
							}
						}

						std::set<VData_t>::iterator res = createdVertices.find(vertex);

						if (res == createdVertices.end())
						{
							vertex.index = positions.size();

							positions.push_back(vertex.data[0] != -1 ? positionPool[vertex.data[0]] : Vector3());
							texCoords.push_back(vertex.data[1] != -1 ? texCoordPool[vertex.data[1]] : Vector2());

							if (normalPool.size() != 0)
							{
								normals.push_back(normalPool[vertex.data[2]]);
							}

							createdVertices.insert(vertex);
						}
						else
						{
							vertex.index = res->index;
						}

						face.push_back(vertex);
					}

					faces.push_back(face);
				}
			}
		}
		else
		{
			DEBUG_ERROR("Mesh Loader", "Obj file '%s' could not be found!", filename.c_str());
			return IndexedModel("");
		}

		std::vector<uint32_t> indices;

		for (std::vector<std::vector<VData_t>>::iterator it = faces.begin(); it != faces.end(); ++it)
		{
			std::vector<VData_t>& face = (*it);
			int firstIndex = face[0].index;

			for (uint32_t ind = 1; ind < face.size() - 1; ind++)
			{
				indices.push_back(firstIndex);
				indices.push_back(face[ind].index);
				indices.push_back(face[ind + 1].index);
			}
		}

		if (name == "")
		{
			name += (uniqueMeshID++);
		}

		if (normals.size() != 0)
		{
			return IndexedModel(name, indices, positions, texCoords, normals);
		}

		return IndexedModel(name, indices, positions, texCoords);
	}

#pragma endregion
}