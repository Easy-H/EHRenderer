#include "FrustumClass.hpp"

FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass&)
{
}

FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(XMMATRIX viewMatrix, XMMATRIX projectionMatrix, float screenDepth)
{
	XMFLOAT4X4 projMatrix, matrix;

	XMStoreFloat4x4(&projMatrix, projectionMatrix);

	float zMinimum = -projMatrix._43 / projMatrix._33;
	float r = screenDepth / (screenDepth - zMinimum);
	projMatrix._33 = r;
	projMatrix._43 = -r * zMinimum;

	projectionMatrix = XMLoadFloat4x4(&projMatrix);

	XMMATRIX finalMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	XMStoreFloat4x4(&matrix, finalMatrix);

    _planes[0].x = matrix._13;
    _planes[0].y = matrix._23;
    _planes[0].z = matrix._33;
    _planes[0].w = matrix._43;

    // Normalize it.
    float t = (float)sqrt((_planes[0].x * _planes[0].x) + (_planes[0].y * _planes[0].y) + (_planes[0].z * _planes[0].z));
    _planes[0].x /= t;
    _planes[0].y /= t;
    _planes[0].z /= t;
    _planes[0].w /= t;

    // Calculate the far plane of frustum.
    _planes[1].x = matrix._14 - matrix._13;
    _planes[1].y = matrix._24 - matrix._23;
    _planes[1].z = matrix._34 - matrix._33;
    _planes[1].w = matrix._44 - matrix._43;

    // Normalize it.
    t = (float)sqrt((_planes[1].x * _planes[1].x) + (_planes[1].y * _planes[1].y) + (_planes[1].z * _planes[1].z));
    _planes[1].x /= t;
    _planes[1].y /= t;
    _planes[1].z /= t;
    _planes[1].w /= t;

    // Calculate the left plane of frustum.
    _planes[2].x = matrix._14 + matrix._11;
    _planes[2].y = matrix._24 + matrix._21;
    _planes[2].z = matrix._34 + matrix._31;
    _planes[2].w = matrix._44 + matrix._41;

    // Normalize it.
    t = (float)sqrt((_planes[2].x * _planes[2].x) + (_planes[2].y * _planes[2].y) + (_planes[2].z * _planes[2].z));
    _planes[2].x /= t;
    _planes[2].y /= t;
    _planes[2].z /= t;
    _planes[2].w /= t;

    // Calculate the right plane of frustum.
    _planes[3].x = matrix._14 - matrix._11;
    _planes[3].y = matrix._24 - matrix._21;
    _planes[3].z = matrix._34 - matrix._31;
    _planes[3].w = matrix._44 - matrix._41;

    // Normalize it.
    t = (float)sqrt((_planes[3].x * _planes[3].x) + (_planes[3].y * _planes[3].y) + (_planes[3].z * _planes[3].z));
    _planes[3].x /= t;
    _planes[3].y /= t;
    _planes[3].z /= t;
    _planes[3].w /= t;

    // Calculate the top plane of frustum.
    _planes[4].x = matrix._14 - matrix._12;
    _planes[4].y = matrix._24 - matrix._22;
    _planes[4].z = matrix._34 - matrix._32;
    _planes[4].w = matrix._44 - matrix._42;

    // Normalize it.
    t = (float)sqrt((_planes[4].x * _planes[4].x) + (_planes[4].y * _planes[4].y) + (_planes[4].z * _planes[4].z));
    _planes[4].x /= t;
    _planes[4].y /= t;
    _planes[4].z /= t;
    _planes[4].w /= t;

    // Calculate the bottom plane of frustum.
    _planes[5].x = matrix._14 + matrix._12;
    _planes[5].y = matrix._24 + matrix._22;
    _planes[5].z = matrix._34 + matrix._32;
    _planes[5].w = matrix._44 + matrix._42;

    // Normalize it.
    t = (float)sqrt((_planes[5].x * _planes[5].x) + (_planes[5].y * _planes[5].y) + (_planes[5].z * _planes[5].z));
    _planes[5].x /= t;
    _planes[5].y /= t;
    _planes[5].z /= t;
    _planes[5].w /= t;

}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
    for (int i = 0; i < 6; i++) {
        if (((_planes[i].x * x) + (_planes[i].y * y) + (_planes[i].z * z) + _planes[i].w) < 0.0f) {
            return false;
        }
    }
    return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
    // Check if any one point of the cube is in the view frustum.
    for (int i = 0; i < 6; i++)
    {
        if (_planes[i].x * (xCenter - radius) +
            _planes[i].y * (yCenter - radius) +
            _planes[i].z * (zCenter - radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + radius) +
            _planes[i].y * (yCenter - radius) +
            _planes[i].z * (zCenter - radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - radius) +
            _planes[i].y * (yCenter + radius) +
            _planes[i].z * (zCenter - radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + radius) +
            _planes[i].y * (yCenter + radius) +
            _planes[i].z * (zCenter - radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - radius) +
            _planes[i].y * (yCenter - radius) +
            _planes[i].z * (zCenter + radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + radius) +
            _planes[i].y * (yCenter - radius) +
            _planes[i].z * (zCenter + radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - radius) +
            _planes[i].y * (yCenter + radius) +
            _planes[i].z * (zCenter + radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + radius) +
            _planes[i].y * (yCenter + radius) +
            _planes[i].z * (zCenter + radius) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        return false;
    }

    return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
    for (int i = 0; i < 6; i++) {
        if (((_planes[i].x * xCenter) + (_planes[i].y * yCenter)
            + (_planes[i].z * zCenter) + _planes[i].w) < -radius) {
            return false;
        }
    }

    return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter,
    float xSize, float ySize, float zSize)
{
    for (int i = 0; i < 6; i++)
    {
        if (_planes[i].x * (xCenter - xSize) +
            _planes[i].y * (yCenter - ySize) +
            _planes[i].z * (zCenter - zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + xSize) +
            _planes[i].y * (yCenter - ySize) +
            _planes[i].z * (zCenter - zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - xSize) +
            _planes[i].y * (yCenter + ySize) +
            _planes[i].z * (zCenter - zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - xSize) +
            _planes[i].y * (yCenter - ySize) +
            _planes[i].z * (zCenter + zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + xSize) +
            _planes[i].y * (yCenter + ySize) +
            _planes[i].z * (zCenter - zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + xSize) +
            _planes[i].y * (yCenter - ySize) +
            _planes[i].z * (zCenter + zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter - xSize) +
            _planes[i].y * (yCenter + ySize) +
            _planes[i].z * (zCenter + zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        if (_planes[i].x * (xCenter + xSize) +
            _planes[i].y * (yCenter + ySize) +
            _planes[i].z * (zCenter + zSize) + _planes[i].w >= 0.0f)
        {
            continue;
        }

        return false;
    }

    return true;
}
