#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() { }

GameObject::~GameObject() { }

bool GameObject::IsActive() const {
    return mActive;
}

float GameObject::GetRotation() const {
    return mRotation;
}

Position GameObject::GetPosition() const {
    return mPosition;
}

void GameObject::SetRotation(float rot) {
    mRotation = rot;
}

void GameObject::SetPosition(Position pos) {
    mPosition = pos;
}

void GameObject::AddRot(float rot) {
    mRotation += rot;
}

void GameObject::AddPos(Position vec) {
    mPosition.x += vec.x;
    mPosition.y += vec.y;
}

void GameObject::Update(float deltaTime) { 
    if (false == mActive or nullptr == mRenderer) {
        return;
    }

    mRenderer->Update(deltaTime);
}

void GameObject::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    if (false == mActive or nullptr == mRenderer) {
        return;
    }

    mRenderer->Render(renderTarget, mPosition, mRotation);
}
