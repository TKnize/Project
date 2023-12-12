#include <jetson-inference/imageNet.h>
#include <jetson-utils/loadImage.h>
#include <iostream>
#include <vector>
#include <thread>;

using namespace std;

struct recipe{
public:
	vector<string> ingredients;
	string publisher;
	string name;
	string culture;
	short age;
};


int main() {

	static char command;
	
	printf("---------------------------Recipe-Indentifier---------------------------\n\n");
	
	printf("                                Commands\n\n");
	
	printf("		M         - Start Program in Machine Learing Mode.\n\n");

	printf("		R         - Enter a Recipe.\n\n");
	
	printf("		H         - Help menu.\n\n");
	
	printf("		L         - Learn about the process.\n\n");
	
	printf("------------------------------------------------------------------------\n\n");

	printf("Enter Command :"); cin>>command; cout<<"\n";
	goto commands;
commands:
	switch(command) {
		case 'M' : {
			short ammount;

			printf("How many images would you like to test? : "); cin>>ammount; printf("\n"); cin.ignore();
			string* images = new string[ammount];
			for(int i = 0; i < ammount; i++) {
				cout<<"Select and enter the path of an image from the food_dataset. (e.g. /home/nvidia/Project/food_dataset/kiwi/Image_2.jpg) : "; getline(cin, images[i]); cout<<"\n";
			}

	vector<string> lists;

	// load the GoogleNet image recognition network with TensorRT
	// you can use imageNet::RESNET_18 to load ResNet-18 model instead
	char* strarr[4] = {"--model=resnet18.onnx", "--input_blob=input_0", "--output_blob=output_0", "--labels=labels.txt"};
	imageNet* net = imageNet::Create(4, strarr);

	for(int c = 0; c < ammount; ++c)
	{
		// retrieve the image filename from the array of command line args
		string imgFilename = images[c];

		// these variables will store the image data pointer and dimensions
		uchar3* imgPtr = NULL;   // shared CPU/GPU pointer to image
		int imgWidth   = 0;      // width of the image (in pixels)
		int imgHeight  = 0;      // height of the image (in pixels)
			
		// load the image from disk as uchar3 RGB (24 bits per pixel)
		if( !loadImage(imgFilename.c_str(), &imgPtr, &imgWidth, &imgHeight) )
		{
			printf("failed to load image '%s'\n", imgFilename);
			return 0;
		}

		// check to make sure that the network model loaded properly
		if( !net )
		{
			printf("failed to load image recognition network\n");
			return 0;
		}

		// this variable will store the confidence of the classification (between 0 and 1)
		float confidence = 0.0;

		// classify the image, return the object class index (or -1 on error)
		const int classIndex = net->Classify(imgPtr, imgWidth, imgHeight, &confidence);

		// retrieve the name/description of the object class index
			const char* classDescription = net->GetClassDesc(classIndex);
			string classdes(classDescription);

		// make sure a valid classification result was returned	
		if( classIndex >= 0 )
		{
			lists.push_back(classdes);
			// print out the classification results
			printf("image is recognized as '%s' (class #%i) with %f%% confidence\n", 
				classDescription, classIndex, confidence * 100.0f);
		}
		else
		{
			// if Classify() returned < 0, an error occurred
			printf("failed to classify image\n");
		}

	}
	for(int veccount = 0; veccount < lists.size(); ++veccount) cout<<lists[veccount]<<"\n\n";
	// free the network's resources before shutting down
	delete net;
	}
	default: {
		return 0;
	}
	}
	return 0;
}