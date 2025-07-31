// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

AKratosController::AKratosController()
{
	KratosTeamID = FGenericTeamId(0);
}

FGenericTeamId AKratosController::GetGenericTeamId() const
{
	return KratosTeamID;
}
