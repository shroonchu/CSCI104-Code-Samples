#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "msort.h"
#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
struct ProdRateSorter {
    bool operator()(Product* p1, Product* p2){
        return (p1->getAvgRate() > p2->getAvgRate());
    }
};
struct ReviewSorter {
    bool operator()(Review* r1, Review* r2){
        return (r1->date > r2->date);
    }
};
void displayProducts(vector<Product*>& hits, char usort);
void displayReviews(vector<Review*>& revs);
void displayRecs(ostream& ofile, string currentUser, 
    vector<pair<string,double> >& recs);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  LOGIN username password                " << endl;
    cout << "  LOGOUT                                 " << endl;
    cout << "  AND r/n term term ...                  " << endl;
    cout << "  OR r/n term term ...                   " << endl;
    cout << "  ADD search_hit_number                  " << endl;
    cout << "  VIEWCART                               " << endl;
    cout << "  BUYCART                                " << endl;
    cout << "  ADDREV search_hit_number rating date review_text" << endl;
    cout << "  VIEWREV search_hit_number              " << endl;
    cout << "  REC                                    " << endl;
    cout << "  QUIT new_db_filename                   " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    //add data to keep track of current user
    string curruser;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "LOGIN") {
                string checkuser;
                ss >> checkuser;
                string checkpass;
                ss >> checkpass;
                if(!ds.checkLogin(checkuser,checkpass)){
                    cout << "Invalid login credentials" << endl;
                }
                else{
                    curruser = checkuser;
                    cout << "User logged in" << endl;
                }
            }
            else if( cmd == "LOGOUT") {
                curruser.clear();
            }
            else if( cmd == "AND") {
                string usort;
                ss >> usort;
                if(usort == "r" || usort == "n"){
                    string term;
                    vector<string> terms;
                    while(ss >> term) {
                        term = convToLower(term);
                        terms.push_back(term);
                    }
                    hits = ds.search(terms, 0);
                    displayProducts(hits, usort[0]);
                }
            }
            else if ( cmd == "OR" ) {
                string usort;
                ss >> usort;
                if(usort == "r" || usort == "n"){
                    string term;
                    vector<string> terms;
                    while(ss >> term) {
                        term = convToLower(term);
                        terms.push_back(term);
                    }
                    hits = ds.search(terms, 1);
                    displayProducts(hits, usort[0]);
                }
            }
            //add product at specified index of hit
            //to current user
            else if ( cmd == "ADD" ) {
                if(curruser.empty()){
                    cout << "No current user" << endl;
                }
                else{
                    unsigned index;
                    ss >> index;
                    ds.addToCart(curruser, index, hits);
                }
            }
            //display current user's cart
            else if ( cmd == "VIEWCART" ) {
                if(curruser.empty()){
                    cout << "No current user" << endl;
                }
                else{
                    string username;
                    ss >> username;
                    ds.viewCart(curruser);
                }
            }
            //if item in stock, and if user balance is
            //sufficient, then remove items from user's 
            //cart, decrement item's qty, and subtract
            //price from user's balance
            else if ( cmd == "BUYCART" ) {
                if(curruser.empty()){
                    cout << "No current user" << endl;
                }
                else{
                    ds.buyCart(curruser);
                }
            }
            else if ( cmd == "ADDREV" ) {
                int hitnum, r;
                string d;
                if(curruser.empty()){
                    cout << "No current user" << endl;
                }
                else if(ss>>hitnum && ss>>r && ss>>d 
                    && (hitnum<=(int)hits.size()) 
                    && (hitnum>0) && (r>=1) && (r<=5)){
                    string revtext;
                    getline(ss,revtext);
                    revtext = trim(revtext);
                    //get prodName from hitnum 
                    string prodName = hits[hitnum-1]->getName();
                    //add review into datastore
                    ds.addReview(prodName,r,curruser,d,revtext);
                }
            }
            else if( cmd == "VIEWREV" ) {
                int hitnum;
                if(ss>>hitnum && hitnum>0 
                    && hitnum<=(int)hits.size()){
                    vector<Review*> revs = hits[hitnum-1]->getReviews();
                    displayReviews(revs);

                }
            }
            else if( cmd == "REC" ) {
                if(curruser.empty()){
                    cout << "No current user" << endl;
                }
                else{
                    vector<pair<string,double> > recs;
                    recs = ds.makeSuggestion(curruser);
                    string filename = "rec.txt";
                    ofstream ofile(filename.c_str());
                    displayRecs(ofile,curruser,recs);
                    displayRecs(cout,curruser,recs);
                }
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits, char usort)
{
    int resultNo = 1;
    if(usort == 'r'){
        mergeSort(hits, ProdRateSorter());
    }
    else if(usort == 'n'){
        mergeSort(hits, ProdNameSorter());
    }
    for(vector<Product*>::iterator it = hits.begin(); 
        it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        if(it+1 != hits.end()){
            cout << endl;
        }
        //cout << endl;
        resultNo++;
    }
}

void displayReviews(vector<Review*>& revs){
    mergeSort(revs, ReviewSorter());
    for(vector<Review*>::iterator it=revs.begin(); 
        it!=revs.end(); it++){
        cout << (*it)->date << " ";
        cout << (*it)->rating << " ";
        cout << (*it)->username << " ";
        cout << (*it)->reviewText << endl;
    }
}

void displayRecs(ostream& ofile, string currentUser, 
    vector<pair<string,double> >& recs){
    if(recs.empty()){
        ofile << "No recommendations available" << endl;
        return;
    }
    ofile << currentUser << endl;
    for(unsigned int i=0; i<recs.size(); i++){
        ofile << fixed << setprecision(2) << recs[i].second;
        ofile << " " << recs[i].first << endl;
    }
}