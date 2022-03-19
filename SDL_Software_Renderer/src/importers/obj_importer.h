#include <iostream>
#include <fstream>
#include "../vector3.h"
#include <vector>

class ObjImporter
{
public:
	static const long BUCKET_VERTEX_COUNT = 2048;
	static const long MAX_MODELS = 264;

	struct Vector3Bucket { Vector3 values[BUCKET_VERTEX_COUNT]; };

	struct OBJModel
	{
		std::vector<Vector3Bucket>* vertexBuckets = new std::vector<Vector3Bucket>();
		long vertexCount = 0;
		int bucketCount = 0;
	};

	struct OBJData
	{
		OBJModel models[MAX_MODELS] = {};
		int modelCount = 0;
	};

#pragma warning(push)
#pragma warning(disable:4996)
	static OBJData* Import(const char* filePath)
	{
		const int lineIDSize = 2;
		const int bufferSize = 256;
		char buffer[bufferSize];

		int vertexCount = 0;
		int modelCount = 0;

		OBJData* data = new OBJData();

		OBJModel tmpModel;
		tmpModel.vertexBuckets->reserve(64);
		std::vector<Vector3Bucket>::iterator bucketVertexIterator = tmpModel.vertexBuckets->begin();
		Vector3Bucket tmpBucket;
		Vector3 tmpVertex;

		int tmpVertexCount = 0;
		int tmpVertexInBucketCount = 0;

		FILE* file = fopen(filePath, "r");
		bool eofReached = false;
		while(eofReached == false)
		{
			char* result = fgets(buffer, bufferSize, file);
			if (strncmp(buffer, "g ", lineIDSize) == 0 || strncmp(buffer, "o ", lineIDSize) == 0) // Treat groups and objects as models for now
			{
				if (tmpVertexCount != 0)
				{
					bucketVertexIterator = tmpModel.vertexBuckets->emplace(bucketVertexIterator, tmpBucket);
					tmpModel.vertexCount = tmpVertexCount;
					tmpModel.bucketCount = tmpModel.vertexBuckets->size();

					data->models[data->modelCount++] = tmpModel;
					tmpModel.vertexBuckets = new std::vector<Vector3Bucket>();

					tmpVertexCount = 0;
					tmpVertexInBucketCount = 0;

					bucketVertexIterator = tmpModel.vertexBuckets->begin();
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
				tmpBucket.values[tmpVertexInBucketCount++] = tmpVertex;
				++tmpVertexCount;

				if (tmpVertexInBucketCount == BUCKET_VERTEX_COUNT)
				{
					bucketVertexIterator = tmpModel.vertexBuckets->emplace(bucketVertexIterator, tmpBucket);
					tmpVertexInBucketCount = 0;
				}

			}
			else if (result == NULL)
			{
				bucketVertexIterator = tmpModel.vertexBuckets->emplace(bucketVertexIterator, tmpBucket);
				tmpModel.vertexCount = tmpVertexCount;
				tmpModel.bucketCount = tmpModel.vertexBuckets->size();

				data->models[data->modelCount++] = tmpModel;

				tmpVertexCount = 0;
				tmpVertexInBucketCount = 0;

				eofReached = true;

				delete(result);
			}
			else continue;		
		}

		delete (file);
		return data;
	}
#pragma warning(pop)

};
