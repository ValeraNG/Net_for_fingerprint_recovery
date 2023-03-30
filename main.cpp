#include <iostream>
#include <filesystem>
#include "BMP.h"
#include "MyFormat.h"
#include "NeuralNetwork.h"
using namespace std;
using namespace std::filesystem;

//int main() {
//	const int WIDTH = 90, HEIGHT = 97;
//	const int fragmentW = 40, fragmentH = 50;
//	int posX = (WIDTH - fragmentW) / 2;
//	int posY = (HEIGHT - fragmentH) / 2;
//	MyFormat data(WIDTH, HEIGHT);
//
//	auto inputSize = fragmentW * fragmentH;
//	auto outputSize = inputSize;
//
//	NeuralNetwork net("Net/net.txt");
//
//	auto in = new float[inputSize];
//	auto out = new float[outputSize];
//	auto answer = new uint8_t[outputSize];
//
//	data.read("TestNet/data.txt");
//	data.datArrFragment(out, posX, posY, fragmentW, fragmentH);
//
//	auto fragmentPtr = generatorFragments(20, 20);
//	if (fragmentPtr) {
//		int minPx = posX / 2;
//		int minPy = posY / 2;
//		int maxPx = fragmentW;
//		int maxPy = fragmentH;
//		fragmentPtr->setPosition(randomInt(minPx, maxPx), randomInt(minPy, maxPy));
//		data.drawFragment(*fragmentPtr);
//	}
//
//	data.drawFragment(posX, posY, fragmentW, fragmentH, answer);
//
//	BMP8 bmp(WIDTH, HEIGHT);
//	bmp.readMyFormatUint8_t(data.getData());
//	bmp.write("result.bmp");
//	BMP8::opean_file("result.bmp");
//}

/*-----education-----*/
//int main() {
//	const int WIDTH = 90, HEIGHT = 97;
//	const int fragmentW = 40, fragmentH = 50;
//	int posX = (WIDTH - fragmentW) / 2;
//	int posY = (HEIGHT - fragmentH) / 2;
//	string directory = "D:\\Data\\Converted\\uint8_tBin90x97";
//	MyFormat data(WIDTH, HEIGHT);
//
//	auto inputSize = fragmentW * fragmentH;
//	auto outputSize = inputSize;
//	NeuralNetwork net(inputSize, { 20000 }, outputSize);
//
//	auto in = new float[inputSize];
//	auto out = new float[outputSize];
//
//	int count = 0;
//	for (auto& p : directory_iterator(directory)) {
//		auto path = p.path().generic_string();
//		cout << count << ")\t" << path << endl;
//
//		/////////////////////////////////////////
//		data.read(path);
//		data.datArrFragment(out, posX, posY, fragmentW, fragmentH);
//
//		auto fragmentPtr = generatorFragments(20, 20);
//		if (fragmentPtr) {
//			int minPx = posX / 2;
//			int minPy = posY / 2;
//			int maxPx = fragmentW;
//			int maxPy = fragmentH;
//			fragmentPtr->setPosition(randomInt(minPx, maxPx), randomInt(minPy, maxPy));
//			data.drawFragment(*fragmentPtr);
//		}
//
//		data.datArrFragment(in, posX, posY, fragmentW, fragmentH);
//
//		net.setInputFloat(in);
//		net.goNetwork();
//		net.education(out);
//		/////////////////////////////////////////
//
//		if (!(++count % 100)) net.writeToFile("Net/net.txt");
//	}
//
//	net.writeToFile("Net/net,txt");
//}

/*-----network performance test-----*/
int main() {
	const int WIDTH = 90, HEIGHT = 97;
	const int fragmentW = 40, fragmentH = 50;
	int posX = (WIDTH - fragmentW) / 2;
	int posY = (HEIGHT - fragmentH) / 2;

	MyFormat data(WIDTH, HEIGHT);
	data.read("TestNet/data.txt");

	auto inputSize = fragmentW * fragmentH;
	auto outputSize = inputSize;
	NeuralNetwork net(inputSize, { 300, 300, 300 }, outputSize);

	auto in = new float[inputSize];
	auto out = new float[outputSize];
	auto answer = new uint8_t[outputSize];
	
	for (size_t i = 0; i < 100; i++)
	{
		cout << i << endl;
		////////////////////////////////////////////////////////////
		data.datArrFragment(in, posX, posY, fragmentW, fragmentH);
		data.datArrFragment(out, posX, posY, fragmentW, fragmentH);

		net.setInputFloat(in);
		net.goNetwork();
		net.education(out);
		////////////////////////////////////////////////////////////
	}

	net.setInputFloat(in);
	net.goNetwork();
	net.get_answerUint8_t(answer);
	data.drawFragment(posX, posY, fragmentW, fragmentH, answer);

	BMP8 bmp(WIDTH, HEIGHT);
	bmp.readMyFormatUint8_t(data.getData());
	bmp.write("result.bmp");
	BMP8::opean_file("result.bmp");
}

/*-----holst-----*/
//int main() {
//	const int WIDTH = 90, HEIGHT = 97;
//	const int fragmentW = 40, fragmentH = 50;
//	int posX = (WIDTH - fragmentW) / 2;
//	int posY = (HEIGHT - fragmentH) / 2;
//
//	MyFormat data(WIDTH, HEIGHT);
//	data.read("Holst/data.txt");
//	data.drawFragmentINVERS(posX, posY, fragmentW, fragmentH);
//
//	auto fragmentPtr = generatorFragments(20, 20);
//	if (fragmentPtr) {
//		int minPx = posX / 2;
//		int minPy = posY / 2;
//		int maxPx = fragmentW;
//		int maxPy = fragmentH;
//		fragmentPtr->setPosition(randomInt(minPx, maxPx), randomInt(minPy, maxPy));
//		data.drawFragment(*fragmentPtr);
//		/*-----range-----*/
//		//for (size_t i = minPx; i < maxPy; i++)
//		//{
//		//	for (size_t j = minPy; j < maxPx; j++)
//		//	{
//		//		fragmentPtr->setPosition(j, i);
//		//		data.drawFragment(*fragmentPtr);
//		//	}
//		//}
//	}
//
//	BMP8 bmp(WIDTH, HEIGHT);
//	bmp.readMyFormatUint8_t(data.getData());
//	bmp.write("result.bmp");
//	BMP8::opean_file("result.bmp");
//}