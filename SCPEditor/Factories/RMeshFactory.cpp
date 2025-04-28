// Fill out your copyright notice in the Description page of Project Settings.

#include "Factories/RMeshFactory.h"

#include "cbreader/rmesh.h"

#include "Engine/Polys.h"
#include "EditorFramework/AssetImportData.h"
#include "Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/FeedbackContext.h"
#include "RawMesh.h"
#include "PhysicsEngine/BodySetup.h"

DEFINE_LOG_CATEGORY(LogRMeshFactory);

// SCP rooms are scaled weirdly, so we change their scale here to work fine with UE
constexpr float ROOM_SCALE = 0.6f;

URMeshFactory::URMeshFactory()
{
	bCreateNew = false;
	bText = false;
	bEditorImport = true;
	Formats.Add(TEXT("rmesh;Room Mesh File"));
	SupportedClass = UStaticMesh::StaticClass();
}

UObject* URMeshFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& FileName, const TCHAR* Parms,
										  FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, Parms);

	const FString FilePath = FPaths::ConvertRelativePathToFull(FileName);

	rmesh::RMesh File;
	if (!File.Read(TCHAR_TO_UTF8(*FilePath)))
	{
		UE_LOG(LogRMeshFactory, Error, TEXT("Failed to import RMesh: %s!"), *FilePath);
		Warn->Log(ELogVerbosity::Error, TEXT("Failed to import RMesh!"));
		return nullptr;
	}

	UStaticMesh* StaticMesh = CreateStaticMesh(InParent, InName, Flags, File);

	if (!StaticMesh)
	{
		UE_LOG(LogRMeshFactory, Error, TEXT("Failed to import RMesh: %s!"), *FilePath);
		Warn->Log(ELogVerbosity::Error, TEXT("Failed to import RMesh!"));
		return nullptr;
	}

	StaticMesh->MarkPackageDirty();
	StaticMesh->PostEditChange();

	StaticMesh->AssetImportData->Update(CurrentFilename, FileHash.IsValid() ? &FileHash : nullptr);

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, StaticMesh);

	return StaticMesh;
}

UStaticMesh* URMeshFactory::CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags, rmesh::RMesh& RMesh)
{
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InParent, InName, RF_Public | RF_Standalone);
	StaticMesh->AddToRoot();

	FRawMesh RawMesh;
	RawMesh.Empty();

	rmesh::Mesh& DrawnMesh = RMesh.drawnMesh;

	// Create drawn mesh
	int VertexOffset = 0;
	for (rmesh::Surface& Surface : DrawnMesh.surfaces)
	{
		int TextureIndex = 0;

		for (int i = 0; i < 2; i++)
		{
			rmesh::Texture& Texture = Surface.textures[i];
			FString TextureName = FPaths::GetBaseFilename(UTF8_TO_TCHAR(Texture.textureName.c_str()));

			// Ignore the texture if the name is empty or if it's a lightmap texture
			if (TextureName.IsEmpty() || TextureName.Contains("_lm"))
			{
				continue;
			}

			FStaticMaterial StaticMaterial = FStaticMaterial(nullptr, *TextureName);

			// Check to see if the material already exists, if not add it to the material
			int FoundIndex = StaticMesh->GetStaticMaterials().Find(StaticMaterial);
			if (FoundIndex != INDEX_NONE)
			{
				TextureIndex = FoundIndex;
			}
			else
			{
				TextureIndex = StaticMesh->GetStaticMaterials().Add(StaticMaterial);
			}
		}

		for (rmesh::Vertex& Vertex : Surface.vertices)
		{
			FVector3f MeshVertex;

			MeshVertex.X = Vertex.vertex.z * ROOM_SCALE;
			MeshVertex.Y = Vertex.vertex.x * ROOM_SCALE;
			MeshVertex.Z = Vertex.vertex.y * ROOM_SCALE;

			RawMesh.VertexPositions.Add(MeshVertex);
		}

		for (cbtypes::Triangle& Triangle : Surface.triangles)
		{
			// Reverse the winding
			int Index1 = Triangle.index3;
			int Index2 = Triangle.index2;
			int Index3 = Triangle.index1;

			RawMesh.WedgeIndices.Add(Index1 + VertexOffset);
			RawMesh.WedgeIndices.Add(Index2 + VertexOffset);
			RawMesh.WedgeIndices.Add(Index3 + VertexOffset);

			cbtypes::Vector2 UV1 = Surface.vertices.at(Index1).uv;
			cbtypes::Vector2 UV2 = Surface.vertices.at(Index2).uv;
			cbtypes::Vector2 UV3 = Surface.vertices.at(Index3).uv;

			RawMesh.WedgeTexCoords[0].Add(FVector2f(UV1.x, UV1.y));
			RawMesh.WedgeTexCoords[0].Add(FVector2f(UV2.x, UV2.y));
			RawMesh.WedgeTexCoords[0].Add(FVector2f(UV3.x, UV3.y));

			RawMesh.WedgeTangentX.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentX.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentX.Add(FVector3f::ZeroVector);

			RawMesh.WedgeTangentY.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentY.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentY.Add(FVector3f::ZeroVector);

			RawMesh.WedgeTangentZ.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentZ.Add(FVector3f::ZeroVector);
			RawMesh.WedgeTangentZ.Add(FVector3f::ZeroVector);

			RawMesh.FaceMaterialIndices.Add(TextureIndex);
			RawMesh.FaceSmoothingMasks.Add(0);
		}

		VertexOffset += Surface.vertices.size();
	}

	FStaticMeshSourceModel& SourceModel = StaticMesh->AddSourceModel();
	SourceModel.BuildSettings.bRecomputeNormals = true;
	SourceModel.BuildSettings.bRecomputeTangents = true;
	SourceModel.BuildSettings.bUseMikkTSpace = false;
	SourceModel.BuildSettings.bGenerateLightmapUVs = true;
	SourceModel.BuildSettings.bBuildReversedIndexBuffer = false;
	SourceModel.BuildSettings.bUseFullPrecisionUVs = true;
	SourceModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
	SourceModel.RawMeshBulkData->SaveRawMesh(RawMesh);

	StaticMesh->SetLightingGuid(FGuid::NewGuid());
	StaticMesh->SetLightMapResolution(512);
	StaticMesh->SetLightMapCoordinateIndex(1);

	// Setup collision
	/// @todo Handle RMesh collision, or maybe don't
	StaticMesh->CreateBodySetup();
	UBodySetup* Body = StaticMesh->GetBodySetup();
	Body->CollisionTraceFlag = CTF_UseComplexAsSimple;

#if 0
	for (rmesh::Surface& Surface : RMesh.collisionMesh.surfaces)
	{
		FKConvexElem ConvexElem;

		for (rmesh::Vertex& Vertex : Surface.vertices)
		{
			FVector MeshVertex;

			MeshVertex.X = Vertex.vertex.z * ROOM_SCALE;
			MeshVertex.Y = Vertex.vertex.x * ROOM_SCALE;
			MeshVertex.Z = Vertex.vertex.y * ROOM_SCALE;

			ConvexElem.VertexData.Add(MeshVertex);
		}

		for (cbtypes::Triangle& Triangle : Surface.triangles)
		{
			// Reverse the winding
			int Index1 = Triangle.index3;
			int Index2 = Triangle.index2;
			int Index3 = Triangle.index1;

			ConvexElem.IndexData.Add(Index1);
			ConvexElem.IndexData.Add(Index2);
			ConvexElem.IndexData.Add(Index3);
		}

		Body->AggGeom.ConvexElems.Add(ConvexElem);
	}
#endif

	TArray<FText> OutErrors;
	StaticMesh->Build(false, &OutErrors);

	return StaticMesh;
}
