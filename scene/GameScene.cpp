#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	/*delete model_;*/
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	////ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937 engine(seed_gen());
	//乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, XM_2PI);

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		// X,Y,Z方向のスケーリングを設定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};

		// X, Y, Z軸周りの回転角を設定 XMConvertToRadians()
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

		// X, Y, Z軸周りの平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}
	//カメラ視点座標を設定
	viewProjection_.eye = {0, 0, -50};
	viewProjection_.target = {10, 0, 0};
	viewProjection_.up = {1, 0, 0};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

std::string strDebug = std::string("eye:") + std::to_string(viewProjection_.eye.x) +
	                       std::to_string(viewProjection_.eye.y) +
	                       std::to_string(viewProjection_.eye.z);
	debugText_->Print(strDebug, 0, 0, 1.0f);
std::string strDebug2 = std::string("target:") + std::to_string(viewProjection_.target.x) +
	                   std::to_string(viewProjection_.target.y) +
	                   std::to_string(viewProjection_.target.z);
debugText_->Print(strDebug2, 0, 30, 1.0f);
std::string strDebug3 = std::string("up:") + std::to_string(viewProjection_.up.x) +
	                    std::to_string(viewProjection_.up.y) +
	                    std::to_string(viewProjection_.up.z);
debugText_->Print(strDebug3, 0, 60, 1.0f);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
