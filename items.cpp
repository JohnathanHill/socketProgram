#include "items.h"
/*
Item::Item( int max, std::string name, int my_units, int itemPrice){
    maxBid = max;
    item_desc = name;
    units = my_units;
    currBid = 0;
    itemPrice = price;

}
*/

Item::Item(){}

Item::~Item(){
    // d-structor
}

void Item::storeInitializer(){ //reads from text file and stores into vector
    int counter = 1;
    std::string word;
    textFile.open("items.txt");
    if(textFile.is_open()){
         while(!textFile.eof()){
            textFile >> word;
            std::string demo = "Item";
            demo += std::to_string(counter);
            //std::cout << word << std::endl;
            if(word == demo){
                myItems.push_back(item()); //pushes struct into vector
                myItems[counter - 1].description = word; //stores item name in myItem[index] of the struct

                textFile >> word;
                //std:: cout << atoi(word.c_str()) << std::endl;

                myItems[counter - 1].unit = atoi(word.c_str()); //converts string to int and stores in myItem[index] of the struct as int
                textFile >> word;
                //std:: cout << atoi(word.c_str()) << std::endl;
                myItems[counter - 1].price = atoi(word.c_str()); //converts string to int and stores in myItem[index] of the struct as int
                counter++;  //changes index of the vector
            }
        }
    }
    else{
        std::cout << "File didn't open" << std::endl;
    }

}   

void Item::printStore(){
    for(int i = 0; i < myItems.size(); i++){ //loops thru index printing struct per index
        std::cout << myItems[i].description << " " << myItems[i].unit << " " << myItems[i].price << std::endl;
    }
}

