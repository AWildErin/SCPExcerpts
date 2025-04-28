// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "RMeshFactory.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRMeshFactory, Log, All);

namespace rmesh
{
struct RMesh;
}

/**
 * @class URMeshFactory
 * Importer class for RMesh Files
 */
UCLASS()
class SCPEDITOR_API URMeshFactory : public UFactory
{
	GENERATED_BODY()

public:
	URMeshFactory();

	// Begin UFactory Interface
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& FileName, const TCHAR* Parms,
									   FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	// End UFactory Interface

private:
	UStaticMesh* CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags, rmesh::RMesh& RMesh);
};
