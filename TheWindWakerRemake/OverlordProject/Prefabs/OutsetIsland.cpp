#include "stdafx.h"

#include "Materials/shadow/DiffuseMaterial_Shadow.h"
#include "Materials/water/FoamMaterial.h"
#include "Materials/water/ShoreWaterMaterial.h"
#include "Materials/Water/BaseWaterMaterial.h"
#include "Materials/Water/ShadeWaterMaterial.h"
#include "Materials/Water/EdgeFoamMaterial.h"
#include "Materials/Water/WetSandMaterial.h"
#include "Materials/ColorMaterial.h"
#include "OutsetIsland.h"

OutsetIsland::OutsetIsland()
{}

void OutsetIsland::Initialize(const SceneContext&)
{


	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	// outset Island
	const auto pStoneMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pStoneMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Stone.png");
	const auto pCliffMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pCliffMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Cliff.png");
	const auto pForestMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pForestMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Forest.png");
	const auto pSandMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pSandMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Sand.png");
	const auto pWoodMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pWoodMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Wood.png");
	const auto pRoofMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pRoofMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/Roof.png");
	const auto pStonePatchMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pStonePatchMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/StonePatch.png");
	const auto pHouseMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pHouseMaterial->SetDiffuseTexture(L"Textures/OutsetIsland/House.png");

	const auto pWindowMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pWindowMat->SetColor(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
	// Sea
	m_pFoammaterialOuter = MaterialManager::Get()->CreateMaterial<FoamMaterial>();
	m_pFoammaterialOuter->SetUVSpeed(XMFLOAT2(0.4f, 0.3f));
	m_pFoammaterialOuter->SetFoamColor(XMFLOAT4(.85f, .91f, 1.f, 1.f));
	m_pFoammaterialOuter->SetFoamTexture(L"Textures/Sea/Shore2.png"); // give the material without opacity

	m_pFoammaterialInner = MaterialManager::Get()->CreateMaterial<FoamMaterial>();
	m_pFoammaterialInner->UseGradientOpacity(false);
	m_pFoammaterialInner->SetDepthBias(0.0001f);
	m_pFoammaterialInner->UseShoreFoam(true);
	m_pFoammaterialInner->SetUVSpeed(XMFLOAT2(0.4f, 0.3f));
	m_pFoammaterialInner->SetFoamColor(XMFLOAT4(.85f, .91f, 1.f, 1.f));
	m_pFoammaterialInner->SetFoamTexture(L"Textures/Sea/Shore3.png"); // Give material with opacity
	
	m_pEdgeFoam = MaterialManager::Get()->CreateMaterial<EdgeFoamMaterial>();
	m_pEdgeFoam->SetFoamColor(XMFLOAT4(.85f, .91f, 1.f, 1.f));
	m_pEdgeFoam->SetDepthBias(0.0002f);

	m_pWaterShade = MaterialManager::Get()->CreateMaterial<ShadeWaterMaterial>();
	m_pWaterShade->SetDepthBias(0.0001f);

	m_pWetSandMaterial = MaterialManager::Get()->CreateMaterial<WetSandMaterial>();
	m_pWetSandMaterial->SetRetractSpeed(0.3f);
	


	m_pShoreWaterMaterial = MaterialManager::Get()->CreateMaterial<ShoreWaterMaterial>();
	m_pShoreWaterMaterial->SetUVSpeed(XMFLOAT2(0.4f, 0.3f));
	m_pShoreWaterMaterial->SetWaterColor(XMFLOAT4(0.05f, 0.42f, 0.71f, 1.f));
	m_pShoreWaterMaterial->SetFoamColor(XMFLOAT4(.85f, .91f, 1.f, 1.f));



	const auto pWaterMaterial = MaterialManager::Get()->CreateMaterial<BaseWaterMaterial>();
	pWaterMaterial->SetColor(DirectX::XMFLOAT4(0.05f, 0.42f, 0.71f, 1.f));

	//Ground Mesh
	//***********

	const auto pIslandObj = AddChild(new GameObject());

	const auto pOutsetIslandModel = pIslandObj->AddComponent(new ModelComponent(L"Meshes/FullOutsetIsland.ovm", false));
	pOutsetIslandModel->SetMaterial(pWindowMat, 0);
	pOutsetIslandModel->SetMaterial(pWaterMaterial, 8, true); // base water
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pWetSandMaterial, 6, true); // inner wave
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pFoammaterialOuter, 3, true);// outer wave
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pShoreWaterMaterial, 4, true); // sinus wave
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pFoammaterialInner, 7, true); //  wet sand
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pWaterShade, 5, true); // under bridge
	pOutsetIslandModel->SetMaterial((BaseMaterial*)m_pEdgeFoam, 2, true); // edge foam
	pOutsetIslandModel->SetMaterial(pWaterMaterial, 1); // pond ripple
	pOutsetIslandModel->SetMaterial(pForestMaterial, 9, true);
	pOutsetIslandModel->SetMaterial(pRoofMaterial, 10);
	pOutsetIslandModel->SetMaterial(pHouseMaterial, 11);
	pOutsetIslandModel->SetMaterial(pWoodMaterial, 12);
	pOutsetIslandModel->SetMaterial(pStoneMaterial, 13);
	pOutsetIslandModel->SetMaterial(pCliffMaterial, 14);
	pOutsetIslandModel->SetMaterial(pSandMaterial, 15);
	pOutsetIslandModel->SetMaterial(pStonePatchMaterial, 16);

	const auto pOutsetIslandActor = pIslandObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/FullOutsetIsland.ovpt");
	pOutsetIslandActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ .005f,.005f,.005f })), *pDefaultMaterial);

	pIslandObj->GetTransform()->Scale(.005f, .005f, .005f);
	pIslandObj->GetTransform()->Translate(0.f, -3.f, 0.f);

}

void OutsetIsland::Update(const SceneContext& sceneContext)
{
	m_pFoammaterialOuter->SetTotalTime(sceneContext.pGameTime->GetTotal());
	m_pFoammaterialInner->SetTotalTime(sceneContext.pGameTime->GetTotal());
	m_pShoreWaterMaterial->SetTotalTime(sceneContext.pGameTime->GetTotal());
	m_pEdgeFoam->SetTotalTime(sceneContext.pGameTime->GetTotal());
	m_pWaterShade->SetTotalTime(sceneContext.pGameTime->GetTotal());
	m_pWetSandMaterial->SetTotalTime(sceneContext.pGameTime->GetTotal());

}

void OutsetIsland::DrawImGui()
{
	m_pEdgeFoam->DrawImGui();
}