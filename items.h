#ifndef ITEMS_H
#define ITEMS_H
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

struct item{
    std::string description;
    int unit;
    int price;
};

class Item{
    private:
        int currBid, units, lastbidID, maxBid, price;
        std::string item_desc;
        std::ifstream textFile;
        std::vector<item> myItems;

    public:
        Item();
        ~Item();


        int getCurrBid() { return currBid; }
        void setCurrBid(int val) { currBid = val; }
        int getLastBidID() { return lastbidID;}
        void setLastBidID(int val) { lastbidID = val;}
        int getMaxBid(){ return maxBid;}
        void setMaxBid(int val) { maxBid = val;}
       
        //items
        std::string getDescription() { return item_desc;}
        void setDescripton( std::string val) { item_desc= val;}
        int getUnits() { return units; }
        void setUnits(int val) { units = val; }
        int getPrice(){ return price; }
        void setPrice(int itemNum) { price = itemNum;} 


        void storeInitializer();
        void printStore();

};

#endif