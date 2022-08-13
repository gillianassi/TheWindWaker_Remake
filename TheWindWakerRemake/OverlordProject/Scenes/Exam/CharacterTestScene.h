#pragma once
class Character;

class CharacterTestScene : public GameScene
{
public:
	CharacterTestScene() :GameScene(L"CharacterTestScene") {}
	~CharacterTestScene() override = default;
	CharacterTestScene(const CharacterTestScene & other) = delete;
	CharacterTestScene(CharacterTestScene && other) noexcept = delete;
	CharacterTestScene& operator=(const CharacterTestScene & other) = delete;
	CharacterTestScene& operator=(CharacterTestScene && other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;

private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterRoll,
		CharacterCrouch
	};

	Character* m_pCharacter{};
};

