// Implementation for our MeshManager
#include "MeshManager.h"
#include "MeshData.h"

namespace ITP485
{

// Does nothing of note for now
void MeshManager::Setup()
{

}

// Iterates through the Mesh Map, and deletes all MeshData pointers.
// Then clears out Mesh Map.
void MeshManager::Cleanup()
{

}

// Searches the std::map for the requested mesh. If it exists, that
// MeshData is returned.
// If the MeshData isn't already loaded for it, will construct a MeshData
// using new, add that pointer to the hash map, and then return that pointer
MeshData* MeshManager::GetMeshData(const char* szMeshFile)
{
	return NULL; // fixme
}

// Helper function which hashes the passed string using djb2 algorithm
unsigned int MeshManager::HashString(const char* str)
{
	return 0; // fixme
}

}
