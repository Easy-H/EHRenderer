#pragma once;

class SimpleInputClass {
public:
	SimpleInputClass();
	SimpleInputClass(const SimpleInputClass&);
	~SimpleInputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool _keys[256];
};