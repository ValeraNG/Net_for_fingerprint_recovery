#pragma once
#include <iostream>
#include <random>
using namespace std;
using ushort = unsigned short int;

struct Fragment {
	Fragment() {}
	Fragment(ushort w, ushort h) : width(w), height(h) {}

	auto getPosithion() { return make_pair(x, y); }
	auto getSize() { return make_pair(width, height); }
	void setPosition(ushort posX, ushort posY) { x = posX, y = posY; }
	
	virtual bool operator() (int x, int y) = 0;
	virtual ~Fragment() {}

protected:
	ushort x, y;
	ushort width, height;
};

struct Eleps : public Fragment {
	Eleps() : Fragment() {}
	Eleps(ushort r) : Fragment() { 
		radius = r;
		height = width = r * 2; 
	}
	void setBias(int a, int b) { A = a, B = b; }
	bool operator() (int posX, int posY) {
		auto temp = A * pow(posX - (x + width / 2), 2) + B * pow(posY - (y + height / 2), 2);
		return (radius * (radius - radius / 2) <= temp && temp <= pow(radius, 2));
	}
private:
	ushort radius = 1;
	int A = 1, B = 1;
};

struct Rhombus : public Fragment {
	Rhombus() : Fragment() {}
	Rhombus(ushort w, ushort h) : Fragment(w, h) {}
	bool operator() (int posX, int posY) {
		auto temp = abs(posX - (x + width / 2)) + abs(posY - (y + height / 2));
		return (height / 3 < temp && temp <= height / 2);
	}
};

struct Line : public Fragment {
	Line() : Fragment() {}
	Line(ushort w, ushort h, float d) : 
		Fragment(w, h), angle(d) {}
	bool operator() (int posX, int posY) {
		auto temp = (posX - 2) <= posY && posY <= (posX + 2);
		return temp;
	}
private:
	float angle;
};

int randomInt(int a, int b) {
	static mt19937 gen(random_device{}());
	uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}

Fragment* generatorFragments(ushort min, ushort max) {
	auto rand = randomInt;
	Fragment* ptr = nullptr;
	switch (randomInt(0, 3))
	{
	case 0: {
		auto temp = new Eleps(rand(15, 30));
		temp->setBias(rand(1, 4), rand(1, 2));
		ptr = static_cast<Fragment*>(temp);
		break;
	}
	case 1:
		ptr = new Rhombus(rand(21, 31), rand(31, 41));
		break;
	case 2:
		ptr = new Line(rand(30, 50), rand(30, 50), 1);
		break;
	default: break;
	}
	return ptr;
}
