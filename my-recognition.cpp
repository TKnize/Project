#include <jetson-inference/imageNet.h>
#include <jetson-utils/loadImage.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct recipe{
public:
	vector<string> ingredients;
	string publisher;
	string name;
	string description;
	recipe(vector<string> ingredients, string publisher, string name, string description) : ingredients(ingredients), publisher(publisher), name(name), description(description) {};
};
vector<recipe> recipes;

static char Commander;

void sayHello();
int Commanded(char instruction);
int VisualNet();
void RecipeAdder();
void UserRecipes();
vector<int> UserInfo(string name);
int UserIndex(string commandType, string name);
int valueMatcher(vector<string> VectIngredients);
void recall();
void recipeRestore();
void Latp();

int main() {
	recipeRestore();
	sayHello();
	return 0;
}


void sayHello(){

	printf("---------------------------Recipe-Indentifier---------------------------\n\n");
	
	printf("                                Commands\n\n");
	
	printf("		M         - Start Program in Machine Learing Mode.\n\n");

	printf("		R         - Enter a Recipe.\n\n");

	printf("		V         - View User Recipe(s)\n\n");
	
	printf("		L         - Learn about the process.\n\n");
	
	printf("------------------------------------------------------------------------\n\n");


	printf("Enter Command : ");

	cin>>Commander;

	cout<<"\n";

	Commanded(Commander);
}


int Commanded(char instruction) {
	switch(instruction) {
		case 'M' : {
			VisualNet();
			break;
		}
		case 'R' : {
			RecipeAdder();
			break;
		}
		case 'V' : {
			UserRecipes();
			break;
		}
		case 'L' : {
			Latp();
		}
	}
	return 0;
}

void Latp() {
	printf("------------------------Learn-About-The-Proccess-------------------------\n\n");
	
	printf("                               Commands\n\n");
	
	printf("	    Created by Trevor (Student) & Lucas (Teacher)\n\n");
	
	printf("This Project succeeds in pairing ingredients with their corrisponding recipes.\n\n");

	printf("This Project \"does not\" have great - stated, decent- success rates - don't\n          expect the ingredients to be guessed correctly\n\n");

	printf("                   The system works by taking an image - \n   from the food_dataset folder (/home/nvidia/Project/food_dataset)- \n  and putting it through an algorithm where - through trial and error- \n       it can formulate answers by giving it a preset base of answers.");
	
	printf("\n\n--------------------------------------------------------------------------\n\n");

	cout<<"\n";
	recall();
}

int VisualNet() {
				short ammount;

				printf("How many images would you like to test? : "); 
				cin>>ammount; 
				printf("\n");
				cin.ignore();
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
	int fV = valueMatcher(lists);
	if(fV != -1) {
		cout<<"--Applicable-recipes--\n\nPublisher: "<<recipes[fV].publisher<<"\n\nRecipe: "<<recipes[fV].name<<"\n\nIngredients:";
		for(int i = 0; i < recipes[fV].ingredients.size(); i++) cout<<" "<<recipes[fV].ingredients[i]<<",";
		cout<<"\n\nDescription: "<<recipes[fV].description;
	}
	else{printf("Failed to find viable recipe");}

	delete net;
	recall();
}

void RecipeAdder() {
			printf("Cooking is a philosophy; it's not a recipe (Marco P White)\n");

			printf("Allow others to discover this philosophy through your recipes!");

			string name;
			while(true) {
				printf("\nFirstly, please enter your name : ");
				cin>>name;
				if(name != "user" || name != "users") break;
				printf("Name cannot be used. Try again.");
			}

			string reciname;
			printf("What is the name of the recipe you wish to submit - all recipes are local to your device and therefore your program only : ");
			cin>>reciname;

			vector<string> reciingred;
			int counter = 1;
			while(true) {
				string ingredient;
				char breaksig;
				

				printf("\nWhat is the name of #%d the ingredient? : ", counter);
				std::cin>> ingredient;
				reciingred.push_back(ingredient);
				counter++;

				char Value;
				while(true){
					printf("\n\nAdd another ingredient (y/n) [must be single character response; y for yes | n for no] : ");
					cin>>Value;
				if(Value == 'n' || Value == 'N') {
					breaksig = 'y';
					break;
				}
				else if(Value == 'y' || Value == 'Y') break;
				printf("Answer not within the viable spectrum. Try Again\n");
				}
				if(breaksig == 'y') break;
			}

			string description;
			printf("Add a short, one sentence description of your recipe : ");
			cin>>description;

			string path = "/home/nvidia/Project/Users/" + name;
			fstream fsile(path, ios::in|ios::app);

			string data;
			fsile>>data;
			fsile.close();

			fsile.open(path, ios::out|ios::app);
			fsile<<"o";

			if(data == "") {
				fstream file("users.txt", ios::out|ios::app);
				if(!file.is_open()) cerr<<"Error opening file. Program Crash";

				file<<name+"\n";

				file.close();
			}

			std::fstream ffile("recipes.txt", ios::out|ios::app);

			if(!ffile.is_open()){
				cerr<<"Error opening file. Program Crash";
			}


			ffile<<"\n{\n"+name+"\n|\n";
			ffile<<reciname+"\n|\n";

			for(int i = 0; i < reciingred.size(); i++) ffile<< reciingred[i]+"\n";

			ffile<<"|\n"+description+"\n";

			ffile.close();
			void recipeRestore();
			printf("\nThank you for submitting your recipe.\n\n");

			recall();
}

void UserRecipes() {
	string value;
	printf("User Recipe(s) Index\n");
	printf("Enter the User who's recipe(s) you want to view - if your not sure which user enter /users : ");
	cin>>value;

	if(value == "/users") {
		UserIndex("NULL", "NULL");
		printf("\n\n");
	}

	else if(UserIndex("match", value) != 0){
		vector<int> rsp = UserInfo(value);
		int va = rsp.size();
		if(va != 0) {
			printf("User Exists!\n\n");
			cout<<"--"<<value<<"'s-Recipes\n\n";
			for(int fV = 0; fV < rsp.size(); fV++) {
				cout<<"Publisher: "<<recipes[fV].publisher<<"\n\nRecipe: "<<recipes[fV].name<<"\n\nIngredients:";
				for(int i = 0; i < recipes[fV].ingredients.size(); i++) cout<<" "<<recipes[fV].ingredients[i]<<",";
				cout<<"\n\nDescription: "<<recipes[fV].description<<"\n\n\n";
			}
		}
		else { printf("User Exists! But isn't recorded in the database.\n\n"); }
	}

	else{printf("Error, value given is erroneous.\n");}
	
}

vector<int> UserInfo(string name) {
	vector<int> indexes;
	for(int i = 0; i < recipes.size(); i++) {
		if(recipes[i].publisher == name) indexes.push_back(i);
	}
	return indexes;
}

void recipeRestore() {
	fstream ffile("recipes.txt", ios::in);
	while(!ffile.eof()){
		string data;
		std::getline(ffile, data);
		if(data == "{"){
			vector<string> ingredients;
			string publisher;
			string name;
			string description;
			int breakCounter = 0;
			while(true || !ffile.eof()) {
				breakCounter++;
				
				std::getline(ffile, data);
				
				if(breakCounter == 1) {
					if(data == "|") { breakCounter++; break; }
					publisher = data;
				}

				else if(breakCounter == 3) {
					if(data == "|") { breakCounter++; break; }
					name = data;
				}

				else if(breakCounter == 5) {
					ingredients.push_back(data);
					while(data != "|") {
						std::getline(ffile, data);
						if(data == "|") break;
						ingredients.push_back(data);
					}
					breakCounter++;
				}

				else if(breakCounter == 7) {
					description = data;
					break;
				}
			}
			
			recipes.push_back(recipe(ingredients, publisher, name, description));

		}
	}
	ffile.close();
}

int UserIndex(string commandType, string name) {
	vector<string> match;
	fstream ffile("users.txt", ios::in);
	cout<<"\n\n\n";
		while(true) {
			string names;
			ffile>>names;
			if(names == "") {
				break;
			}
			if(commandType == "NULL") cout<<"           "<<names<<"\n\n";
			match.push_back(names);
		}
		if(match.size() == 0) printf("No users are currently indexed, be the first!\n");

		if(commandType == "NULL") {
		char repitionValue;
		while(true){
			printf("\n\nDo you wish to check User Recipe(s) (y/n) [must be single character response; y for yes | n for no] : ");
			cin>>repitionValue;
			cout<<"\n\n\n";
			if(repitionValue == 'y' || repitionValue == 'Y') {
				UserRecipes();
			}
			else if(repitionValue == 'n' || repitionValue == 'N') break;
			printf("\n\nAnswer not within the viable spectrum. Try Again\n\n");
		}
		recall();
		}
}

int valueMatcher(vector<string> VectIngredients) {
	fstream ffile("recipes.txt", ios::in);
	while(!ffile.eof()){
		string data;
		std::getline(ffile, data);
		if(data == "{"){
			vector<string> ingredients;
			string publisher;
			string name;
			string description;
			int breakCounter = 0;
			while(true || !ffile.eof()) {
				breakCounter++;
				
				std::getline(ffile, data);
				
				if(breakCounter == 1) {
					if(data == "|") { breakCounter++; break; }
					publisher = data;
				}

				else if(breakCounter == 3) {
					if(data == "|") { breakCounter++; break; }
					name = data;
				}

				else if(breakCounter == 5) {
					ingredients.push_back(data);
					while(data != "|") {
						std::getline(ffile, data);
						if(data == "|") break;
						ingredients.push_back(data);
					}
					breakCounter++;
				}

				else if(breakCounter == 7) {
					description = data;
					break;
				}
			}
			
			recipes.push_back(recipe(ingredients, publisher, name, description));

		}
	}
	ffile.close();

	int reciIndex;
	int success = 0;
	int sizeofVectIngredients = VectIngredients.size() - 1;
	int sizeofRecipes = recipes.size() - 1;

	for(reciIndex = 0; reciIndex < recipes.size(); reciIndex++) {
		success = 0;
		for(int i = 0; i < VectIngredients.size(); i++) {
			if(VectIngredients.size() != recipes[reciIndex].ingredients.size()) break;
			else if(recipes[reciIndex].ingredients[i] == VectIngredients[i]) {
				success++;
			}
		}
		if(success == VectIngredients.size()) break;
	}
	if(success == VectIngredients.size()) return reciIndex;
	return -1;
}

void recall() {
char repitionValue;
		while(true){
			printf("\n\nDo you wish to continue your session? (y/n) [must be single character response; y for yes | n for no] : ");
			cin>>repitionValue;
			if(repitionValue == 'y' || repitionValue == 'Y') {
				printf("\n\n\n\n");
				sayHello();
			}
			else if(repitionValue == 'n' || repitionValue == 'N') break;
			printf("\n\nAnswer not within the viable spectrum. Try Again\n\n");
		}
}
