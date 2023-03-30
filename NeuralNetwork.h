#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
using namespace std;
using ushort = unsigned short int;

float randomFloat(float a, float b) {
	static mt19937 gen(random_device{}());
	uniform_real_distribution<float> dis(a, b);
	return dis(gen);
}

class NeuralNetwork {
public:
	NeuralNetwork(ushort input, vector<ushort> hidden, ushort output) {
		settingLayers(input, hidden, output);
		settingMatrixWeights();
		settingArr_fX();
		initMatrixTW();
	}
	NeuralNetwork(const string path) { readFromFile(path); }

	void setInputFloat(float* input) { arr_fX[0] = input; }
	void goNetwork() {
		for (ushort pos = 0; pos < (sizeL - 1); ++pos) {
			auto& matrix = weights[pos];
			ushort width = layers[pos];
			ushort height = layers[pos + 1];
			for (size_t row = 0; row < height; ++row) {
				float sum = sumWF(arr_fX[pos], matrix[row], width);
				arr_fX[pos + 1][row] = sigmoida(sum);
			}
		}
	}

	void education(float* answer) {
		reference = answer;
		auto pos = sizeL - 1;
		education(pos, &NeuralNetwork::getErrorOut);
		for (pos = sizeL - 2; pos > 0; --pos) education(pos, &NeuralNetwork::getErrorHidden);
	}
	void get_answerUint8_t(uint8_t* arr) {
		auto pos = sizeL - 1;
		auto size = layers[pos];
		for (size_t i = 0; i < size; ++i) arr[i] = uint8_t(arr_fX[pos][i] * 255);
	}

	/*-----test-----*/
	void readFromFile(const string path) {
		ifstream in{ path };
		if (!in) {
			cout << "No file: " << path;
			return;
		}
		in >> sizeL;
		layers = new ushort[sizeL];
		for (size_t i = 0; i < sizeL; ++i) in >> layers[i];
		settingArr_fX();
		initMatrixTW();
		weights = new float** [sizeL - 1];
		for (size_t idM = 0; idM < sizeL - 1; ++idM) {
			ushort width = layers[idM] + 1;
			ushort height = layers[idM + 1];
			auto matrix = weights[idM] = new float* [height];
			for (size_t i = 0; i < layers[idM + 1]; ++i) {
				matrix[i] = new float[width];
				for (size_t j = 0; j < layers[idM] + 1; ++j) in >> matrix[i][j];
			}
		}
	}
	void writeToFile(const string path) {
		ofstream of{ path };
		if (!of) {
			cout << "No file: " << path;
			return;
		}
		of << sizeL << endl;
		for (size_t i = 0; i < sizeL; ++i) of << layers[i] << ' ';
		of << endl;
		for (size_t idM = 0; idM < sizeL - 1; idM++) {
			for (size_t i = 0; i < layers[idM + 1]; i++) {
				for (size_t j = 0; j < layers[idM] + 1; j++)
					of << weights[idM][i][j] << ' ';
				of << endl;
			}
			of << endl;
		}
	}

private:
	/*-----education-----*/
	float getErrorOut(ushort pos, ushort idN) { return arr_fX[pos][idN] - reference[idN]; }
	float getErrorHidden(ushort pos, ushort idN) {
		float error = 0;
		auto countNnext = layers[pos + 1];
		for (ushort i = 0; i < countNnext; ++i)
			error += matrixTW[i][idN] * arr_fX[pos + 1][i];
		return error;
	}
	void setWeight(ushort pos, ushort idN, float d) {
		auto matrix = weights[pos - 1];
		auto countPrevN = layers[pos - 1];
		for (ushort idW = 0; idW < countPrevN; ++idW) {
			float curentW = matrix[idN][idW];
			float fXin = arr_fX[pos - 1][idW];
			float newW = curentW - n * d * fXin;
			swap(matrixTW[idN][idW] = newW, matrix[idN][idW]);
		}
		matrix[idN][countPrevN] -= n * d;	//B
	}
	void education(ushort pos, auto fError) {
		auto countCurentN = layers[pos];
		for (ushort idN = 0; idN < countCurentN; ++idN) {
			float fXout = arr_fX[pos][idN];
			float e = (this->*fError)(pos, idN);
			float d = e * fXout * (1 - fXout);
			setWeight(pos, idN, d);
			arr_fX[pos][idN] = d;
		}
	}
	float* getInOut(uint8_t* arr, ushort size) {
		auto temp = new float[size];
		for (ushort i = 0; i < size; ++i) temp[i] = (static_cast<float>(arr[i]) / 255);
		return temp;
	}

	/*-----goNetwork-----*/
	inline float sigmoida(float x) { return 1 / (1 + exp(-x)); }
	float sumWF(float* arrF, float* arrW, ushort size) {
		float sum = 0;
		for (ushort i = 0; i < size; ++i) sum += arrF[i] * arrW[i];
		return (sum + arrW[size]);
	}

	/*-----settings-----*/
	void settingLayers(ushort in, vector<ushort>& hidd, ushort out) {
		layers = new ushort[sizeL = (ushort)hidd.size() + 2];
		for (ushort i = 0; i < hidd.size(); ++i) layers[i + 1] = hidd[i];
		layers[sizeL - 1] = out;
		layers[0] = in;
	}
	float** setMatrixWeights(ushort width, ushort height) {
		auto matrix = new float* [height];
		for (ushort row = 0; row < height; ++row) {
			matrix[row] = new float[width];
			for (ushort col = 0; col < width; ++col) matrix[row][col] = randomFloat(-0.5, 0.5);
		}
		return matrix;
	}
	void settingMatrixWeights() {
		weights = new float** [sizeL - 1];
		for (ushort idMatrix = 0; idMatrix < (sizeL - 1); ++idMatrix) {
			ushort width = layers[idMatrix] + 1;
			ushort height = layers[idMatrix + 1];
			weights[idMatrix] = setMatrixWeights(width, height);
		}
	}
	void settingArr_fX() {
		arr_fX = new float* [sizeL];
		for (ushort i = 1; i < sizeL; ++i) arr_fX[i] = new float[layers[i]];
		arr_fX[0] = nullptr;
	}
	void initMatrixTW() {
		ushort maxWidth = 0;
		for (ushort i = 1; i < sizeL; ++i) {
			sizeTM = max(sizeTM, layers[i]);
			maxWidth = max(maxWidth, layers[i - 1]);
		}
		matrixTW = new float* [sizeTM];
		for (ushort i = 0; i < sizeTM; ++i) matrixTW[i] = new float[maxWidth];
	}

	/*-----options-----*/
	ushort sizeL = 0;
	ushort* layers = nullptr;
	float** arr_fX = nullptr;
	float*** weights = nullptr;
	float** matrixTW = nullptr;
	ushort sizeTM = 0;
	float n = 0.001f;
	float* reference = nullptr;
};