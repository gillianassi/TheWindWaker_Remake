#pragma once

class BoneObject final : public GameObject
{
public:
	BoneObject(BaseMaterial* pMaterial, float length = 5.f);
	virtual ~BoneObject() override = default;
	BoneObject(const BoneObject& other) = delete;
	BoneObject(BoneObject&& other) noexcept = delete;
	BoneObject& operator=(const BoneObject& other) = delete;
	BoneObject& operator=(BoneObject&& other) noexcept = delete;

	void AddChildBone(BoneObject* pBone);

	const XMFLOAT4X4& GetBindPose() const { return m_BindPose; }
	void CalculateBindPose();
protected:
	virtual void Initialize(const SceneContext&) override;

private:
	float m_Length;
	BaseMaterial* m_pMaterial;

	XMFLOAT4X4 m_BindPose;
};