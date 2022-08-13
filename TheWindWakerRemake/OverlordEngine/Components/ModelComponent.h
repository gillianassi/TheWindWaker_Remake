#pragma once
#include "BaseComponent.h"

class MeshFilter;
class ModelAnimator;
struct SubMeshFilter;

class ModelComponent : public BaseComponent
{
public:
	ModelComponent(const std::wstring&  assetFile, bool castShadows = true);
	~ModelComponent() override;

	ModelComponent(const ModelComponent& other) = delete;
	ModelComponent(ModelComponent&& other) noexcept = delete;
	ModelComponent& operator=(const ModelComponent& other) = delete;
	ModelComponent& operator=(ModelComponent&& other) noexcept = delete;

	void SetMaterial(BaseMaterial* pMaterial, UINT8 submeshId = 0, bool isDrawnLate = false);
	void SetMaterial(UINT materialId, UINT8 submeshId = 0, bool isDrawnLate = false);

	ModelAnimator* GetAnimator() const { return m_pAnimator; }
	bool HasAnimator() const { return m_pAnimator != nullptr; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext&) override;
	void Draw(const SceneContext& sceneContext) override;



	void ShadowMapDraw(const SceneContext& sceneContext) override; //update_W9

private:

	bool IsLateSubmesh(const SubMeshFilter* subMesh);
	void DrawSubmesh(const SubMeshFilter* subMesh, const SceneContext& sceneContext);
	std::wstring m_AssetFile{};
	MeshFilter* m_pMeshFilter{};

	std::vector<BaseMaterial*> m_Materials{};
	BaseMaterial* m_pDefaultMaterial{};

	bool m_MaterialChanged{};

	ModelAnimator* m_pAnimator{};

	std::vector<UINT8> m_LateSubmeshIDs{};

	//W9
	bool m_CastShadows{ true };
};
