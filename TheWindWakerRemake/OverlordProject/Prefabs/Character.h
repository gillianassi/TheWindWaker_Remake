#pragma once

struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f,
		float modelScale = .005f,
		float modelHeight = - 1.25f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
		mHeight = modelHeight;
		mScale = modelScale;
	}

	float mHeight{};
	float mScale{};

	float maxMoveSpeed{ 15.f };
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 15.f };


	float crawlSpeedModifier{ 0.4f };
	float rollSpeedModifier{ 1.5f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	float maxCameraDist{ 10.f };
	float minCameraDist{ 4.f };

	float maxCameraHeight{ 3.f };
	float minCameraHeight{ 0.f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	float rollDuration{ 0.6f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
	int actionId_Roll{ -1 };
	int actionId_Crouch{ -1 };
};

class Character : public GameObject
{
public:
	enum class CharacterState
	{
		STAND_IDLE = 0,
		STAND_RUNNING = 1,
		JUMPING = 2,
		Falling = 3,
		CROUCH = 4,
		CRAWL_IDLE =5,
		CRAWL_MOVE = 6,
		ROLLING = 7

	};

	Character(const CharacterDesc& characterDesc, PhysxProxy* pPhysxScene);
	~Character() override = default;

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	void DrawImGui();
	CameraComponent* GetCameraComponent()
	{
		return m_pCameraComponent;
	}

	bool isMovingForwards();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	bool IsStateChanged() {	return m_CurrentState != m_PrevState;}

	XMFLOAT3 GetCurrentDirection() { return m_CurrentDirection; }
	XMFLOAT3 GetFootPosition() { return m_pControllerComponent->GetFootPosition(); }

	ModelComponent* AddModel(ModelComponent* pModelComponent);

	CharacterState m_CurrentState = CharacterState::STAND_IDLE;

private:
	PhysxProxy* m_pPhysxScene{};
	GameObject* m_pModelObject{};

	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};
	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{3.14f};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{},								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)
		m_CameraHeight{2.f},
		m_CameraDist{10},
		m_CurrentMeshRotation{},
		m_ElapsedRollTime{100};


	CharacterState m_PrevState = CharacterState::STAND_IDLE;

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)
};

