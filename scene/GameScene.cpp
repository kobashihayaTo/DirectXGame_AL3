#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		//ワールドトランスフォーム初期化
		worldTransform_[i].Initialize();
	}

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	float rad;

	float add_x;
	float add_y;

	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		// 度数法を変換
		rad = Angle[i] * XM_PI / 180.0f;

		//円の位置を割り出す
		add_x = cos(rad) * 10.0f;
		add_y = sin(rad) * 10.0f;

		//角度加算
		Angle[i] += 1.0f;

		//中心座標に位置を加算
		worldTransform_[i].translation_.x = 0.0f + add_x;
		worldTransform_[i].translation_.y = 0.0f + add_y;

		//ワールドトランスフォーム初期化
		worldTransform_[i].UpdateMatrix();
	}
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
	model_ = Model::Create();
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