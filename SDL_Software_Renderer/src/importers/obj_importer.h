#include <iostream>
#include <fstream>
#include "../vector3.h"
#include <vector>

class ObjImporter
{
	static const long BUCKET_VERTEX_COUNT = 128;
	static const long MAX_MODELS = 8;

	struct Vector3Bucket { Vector3 values[BUCKET_VERTEX_COUNT]; };

public:
	struct OBJModel
	{
		std::vector<Vector3Bucket>* vertexBuckets = new std::vector<Vector3Bucket>();
		long vertexCount = 0;
		int vertexBucketCount = 0;

		int vertexIndices[BUCKET_VERTEX_COUNT * 3] = {};
	};

	struct OBJData
	{
		OBJModel models[MAX_MODELS] = {};
		int modelCount = 0;
	};

#pragma warning(push)
#pragma warning(disable:4996)
	static void Import(const char* filePath)
	{
		const int lineIDSize = 2;
		const int bufferSize = 256;
		char buffer[bufferSize];

		int vertexCount = 0;
		int modelCount = 0;

		OBJData* data = new OBJData();

		OBJModel tmpModel;
		tmpModel.vertexBuckets = new std::vector<Vector3Bucket>(128);

		Vector3Bucket tmpBucket;

		int tmpVertexCount = 0;
		int tmpBucketVertexCount = 0;
		int tmpBucketCount = 0;

		Vector3 tmpVertex;

		FILE* file = fopen(filePath, "r");
		bool eofReached = false;
		while(eofReached == false)
		{
			char* result = fgets(buffer, bufferSize, file);
			if (strncmp(buffer, "g ", lineIDSize) == 0 || strncmp(buffer, "o ", lineIDSize) == 0) // Treat groups and objects as models for now
			{
				if (tmpVertexCount != 0)
				{
					tmpModel.vertexBuckets->push_back(tmpBucket);
					tmpModel.vertexCount = tmpVertexCount;
					tmpModel.vertexBucketCount = tmpBucketCount;

					data->models[data->modelCount++] = tmpModel;

					tmpVertexCount = 0;
					tmpBucketVertexCount = 0;
					tmpBucketCount = 1;
				}
			}
			else if (strncmp(buffer, "vt", lineIDSize) == 0)
			{

			}
			else if (strncmp(buffer, "vn", lineIDSize) == 0)
			{

			}
			else if (strncmp(buffer, "v ", lineIDSize) == 0)
			{
				sscanf(buffer + lineIDSize, "%f %f %f\n", &tmpVertex.x, &tmpVertex.y, &tmpVertex.z);
				tmpBucket.values[tmpBucketVertexCount++] = tmpVertex;

				if (tmpBucketVertexCount == BUCKET_VERTEX_COUNT)
				{
					tmpBucketVertexCount = 0;
					++tmpBucketCount;
				}
			}
			else if (result == NULL)
			{
				tmpModel.vertexBuckets->push_back(tmpBucket);
				tmpModel.vertexCount = tmpVertexCount;
				tmpModel.vertexBucketCount = tmpBucketCount;

				data->models[data->modelCount++] = tmpModel;

				tmpVertexCount = 0;
				tmpBucketVertexCount = 0;
				tmpBucketCount = 1;

				eofReached = true;

				delete(result);
			}
			else continue;		
		}

		delete (file);
	}
#pragma warning(pop)

};
