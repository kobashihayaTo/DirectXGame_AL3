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

		for (size_t j = 0; j < _countof(worldTransform_); j++) {

			for (size_t k = 0; k < _countof(worldTransform_); k++) {

				worldTransform_[i][j][k].scale_ = {1.0f, 1.0f, 1.0f};

				//平行移動を設定
				worldTransform_[i][j][k].translation_ = {
				  -12.0f + j * 3.0f, -12.0f + i * 3.0f, 0.0f + k * 4.0f};

				//ワールドトランスフォーム初期化
				worldTransform_[i][j][k].Initialize();
			}
		}
	}

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {}

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

		for (size_t j = 0; j < _countof(worldTransform_); j++) {

			for (size_t k = 0; k < _countof(worldTransform_); k++) {

				model_->Draw(worldTransform_[i][j][k], viewProjection_, textureHandle_);
			}
		}
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