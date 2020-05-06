#pragma once

#include <GL/glew.h>
#include "../ecs/EcsComponent.h"
#include "../math/Vector.h"
#include "../core/Platform.h"

namespace Abstract
{

	struct ABSTRACT_API Mesh : public ECSComponent<Mesh>
	{
	public:
		Mesh(const std::string& name, const std::vector<float>& vertexData, size_t vertexSize, const std::vector<uint32_t>& indices);
		~Mesh();

		std::string name					      = "";
		GLuint      vertexArrayObject			  = 0;
		GLuint      indexElementBuffer			  = 0;
		GLuint      vertexArrayBuffer			  = 0;
		GLuint      instancedTransformArrayBuffer = 0;
		uint32_t    drawCount                     = 0;
		size_t      vertexSize					  = 0;
	};

	class ABSTRACT_API IndexedModel
	{
	public:
		IndexedModel(const std::string& name) {}
		IndexedModel(const std::string& name, const std::vector<unsigned int> indices, const std::vector<Vector3>& positions, const std::vector<Vector2>& texCoords,
			const std::vector<Vector3>& normals = std::vector<Vector3>(), const std::vector<Vector3>& tangents = std::vector<Vector3>()) :
			name(name),
			indices(indices),
			positions(positions),
			texCoords(texCoords),
			normals(normals),
			tangents(tangents) {}

		~IndexedModel() { }

		bool isValid() const;
		void calcNormals();
		void calcTangents();

		IndexedModel finalize();
		Mesh finalizeToMesh();

		void addVertex(const Vector3& vert);
		inline void addVertex(float x, float y, float z) { addVertex(Vector3(x, y, z)); }

		void addTexCoord(const Vector2& texCoord);
		inline void addTexCoord(float x, float y) { addTexCoord(Vector2(x, y)); }

		void addNormal(const Vector3& normal);
		inline void addNormal(float x, float y, float z) { addNormal(Vector3(x, y, z)); }

		void addTangent(const Vector3& tangent);
		inline void addTangent(float x, float y, float z) { addTangent(Vector3(x, y, z)); }

		void addFace(unsigned int vertIndex0, unsigned int vertIndex1, unsigned int vertIndex2);

		inline const std::vector<unsigned int>& getIndices() const { return indices; }
		inline const std::vector<Vector3>& getPositions()   const { return positions; }
		inline const std::vector<Vector2>& getTexCoords()   const { return texCoords; }
		inline const std::vector<Vector3>& getNormals()     const { return normals; }
		inline const std::vector<Vector3>& getTangents()    const { return tangents; }

		inline const std::vector<float>& getVertexData() const { return vertexData; }
	private:
		std::string name;

		std::vector<unsigned int> indices;
		std::vector<float> vertexData;
		std::vector<Vector3> positions;
		std::vector<Vector2> texCoords;
		std::vector<Vector3> normals;
		std::vector<Vector3> tangents;
	};

	namespace RenderTools {
		extern IndexedModel ABSTRACT_API loadModelFromObj(const std::string& filename);
	}
}