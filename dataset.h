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

  vector<pair<int, int> > data_binned;

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


  void printBinnedData(){
    for(auto iter = data_binned.begin(); iter != data_binned.end(); iter++){
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

  vector<double> calculate_split_points(int bin_num){
    int split_point_num=bin_num-1;
    int bin_data_num=ceil((double)this->data.size()/bin_num);
    printf("feature split info: num_data: %lu bin_num:%d bin_data_num:%d\n",data.size(),bin_num,bin_data_num);
    vector<double> split_points;
    for(int i = 1; i <= split_point_num; i++){
      split_points.push_back(data[i*bin_data_num-1].second);
    }
    // assign data

      int label=0;
    for(int i = 0; i<bin_num; i++){
      int start=i*bin_data_num; 
      int end=(i+1)*bin_data_num;
      if(i==bin_num-1){
        end=data.size();
      }
      for(int t=start; t<end; t++){
        data_binned.push_back(std::make_pair<int,int>(int(data[t].first),int(label)));
      }
      // printf("start%d end%d",start,end);
      
        label=label+1;
    }
    return split_points;
  }

  void assign_data(int bin_num, vector<double> split_points){
    // ! label generic 
    int label=0;
  }


private:

};


class CategoricalFeature : public Feature
{
public:
  vector<pair<int, string> > data;
  void push_back(int id, double token){}
  void push_back(int id, string token){
    data.push_back(std::make_pair<int,string>(int(id),string(token)));
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
  vector<pair<int, string> > data;
  void push_back(int id, double token){
  }
  void push_back(int id, string token){
    data.push_back(std::make_pair<int,string>(int(id),string(token)));
  }
  void printData(){
    // for (int i=0; i<data.size(); i++) { 
    //   cout << data[i].first << " "<< data[i].second << endl; 
    // } 
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

void make_bins(int bin_num){

  vector<vector<double>> feature_split_points;
  //! data format のupdate によって要修正
  for (int i = 0; i < 4; ++i)
  {
    NumericFeature * child = dynamic_cast<NumericFeature*>(features[i].get());
    vector<double> split_points = child->calculate_split_points(bin_num);
    feature_split_points.push_back(split_points);
    child->printBinnedData();
  }


}

void init(){

  string line;
  ifstream myfile(data_path);
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
        printf("%d\n",i );
        features[i]->push_back(line_index, token);

      }else if(is_number(token)){
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


