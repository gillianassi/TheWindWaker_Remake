#pragma once
//Resharper Disable All

	class PostFade : public PostProcessingMaterial
	{
	public:
		PostFade();
		~PostFade() override = default;
		PostFade(const PostFade& other) = delete;
		PostFade(PostFade&& other) noexcept = delete;
		PostFade& operator=(const PostFade& other) = delete;
		PostFade& operator=(PostFade&& other) noexcept = delete;
		
		void SetFadeWeight(float bias) const;

	protected:
		void Initialize(const GameContext& /*gameContext*/) override {}
	};

