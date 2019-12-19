#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <typeinfo>
#include <map>


using namespace std;


template <typename T1, typename T2>
struct less_second {
  typedef pair<T1, T2> type;
  bool operator ()(type const& a, type const& b) const {
    return a.second < b.second;
  }
};

class Feature {
public:
  virtual ~Feature(){};
  virtual void push_back(int id, double token)=0;
  virtual void push_back(int id, string token)=0;
  virtual void printData()=0;
  virtual void sortData(){  }
  bool hasOrder(){
    return false;
  }
};


class NumericFeature : public Feature
{
public:



  vector<pair<int, double> > data;

  void push_back(int id, string token){}

  void push_back(int id, double token){
    data.push_back(std::make_pair<int,double>(int(id),double(token)));
  }

  void printData(){
    for(auto iter = data.begin(); iter != data.end(); iter++){
      cout<<iter->first<<" "<<iter->second<<"; ";
    }
    cout << '\n';       
  }
  void sortData(){
    sort(data.begin(), data.end(), less_second<int, double>());

  }
  bool hasOrder(){
    return true;
  }



private:

};


class CategoricalFeature : public Feature
{
public:
  map<int, string> data;
  void push_back(int id, double token){}
  void push_back(int id, string token){
    data[id]=token;
  }
  void printData(){

    for(auto iter = data.begin(); iter != data.end(); iter++){
      cout<<iter->first<<" "<<iter->second<<"; ";
    }
    cout << '\n';             
  }
private:

};

class Target : public Feature
{
public:
  map<int, string> data;
  void push_back(int id, double token){

  }
  void push_back(int id, string token){
    data[id]=token;
  }
  void printData(){

    for(auto iter = data.begin(); iter != data.end(); iter++){
      cout<<iter->first<<" "<<iter->second<<"; ";
    }
    cout << '\n';        
  }
private:

};





class Dataset
{
public:
  Dataset(int feature_num, string data_path, string delimiter, bool skipHead) {
  	this->data_path=data_path;
    this->feature_num=feature_num;
    this->delimiter=delimiter;
    this->skipHead=skipHead;
  }


private:
  bool skipHead = false;
  string data_path = "";
  int feature_num = 0;
  string delimiter;
  vector<unique_ptr<Feature>> features;

  bool is_number(const std::string& s)
  {
    char* end = 0;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
  }

public:
  void printInfo(){
   cout << "feature_num: " << this->feature_num << "\n"; 
   cout << "data_path: " << this->data_path << "\n"; 
   cout << "feature size: " << this->features.size() << "\n"; 

   for(int i=0; i < this->features.size(); i++){
    cout << i+1 <<"th features:" << '\n'; 
    features[i]->printData();
    cout << endl;
        // cout << typeid(features[i]).name() << '\n';
          // cout << features[i] << '\n'; 
  }

  for(int i=0; i < this->features.size(); i++){
    cout << i+1 <<"th features:" << '\n'; 
    features[i]->sortData();
    features[i]->printData();
    cout << endl;
        // cout << typeid(features[i]).name() << '\n';
          // cout << features[i] << '\n'; 
  }
}

void init(){

  string line;
  ifstream myfile(data_path);


      //!skipHead
  skipHead=true;

  int line_index=0;
  cout << "begin processing ..." << endl;

  cout << "weather skip headline : " <<skipHead << endl ;
  if (myfile.is_open()){

   while ( getline (myfile,line) )
   {

    if(skipHead&&line_index==0){
      line_index=line_index+1;
      continue;
    } 
    string token;
    size_t pos = 0;

    int i=0;

  // cout << "aaaaa " <<skipHead << endl ;

    while (line.length() > 0) {

      if( (pos = line.find(delimiter)) != string::npos ){
        token = line.substr(0, pos);
      }else{
        pos =line.length()-1;
        token = line;
      }

      // data format の部分を更新したあと　要修正
      if(i+1 == this->feature_num){

        if(i+1>features.size()){
          features.emplace_back(new Target);
        }
        features[i]->push_back(line_index, token);

      }else if(is_number(token)){

            // cout << is_number(token) << endl;
        if(i+1>features.size()){
          features.emplace_back(new NumericFeature);
        }
        double d_value = atof(token.c_str());
        features[i]->push_back(line_index, d_value);
      }else{
        if(i+1>features.size()){
          features.emplace_back(new CategoricalFeature);
        }
        features[i]->push_back(line_index, token);
      }

      line.erase(0, pos + delimiter.length());
      i++;
    }


    line_index=line_index+1;
  }
  myfile.close();
}else cout << "Unable to open file"; 

}

};


