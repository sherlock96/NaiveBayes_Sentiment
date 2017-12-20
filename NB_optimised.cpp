#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits.h>
#include <string>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <vector>
#include <set>
#include <streambuf>
#include <dirent.h>
#define ll int64_t
#define MOD 1000000007
#define pb push_back
#define ff first
#define ss second
#define FOR(i,n) for(int i=0;i<n;i++)
//const int64_t INF = 1000000000000000000;

using namespace std;

DIR *dir;
struct dirent *ent;

ll arr[100005],freq[100005];
ll t,n,m,x,y,a,b,z,p,q,r,sum=0;
vector<ll> adj[100005],v;

set<ll> st;
string s,s2;

ll N=25000,pos=12500,neg=12500;                                      //number of documents
double prior_pos,prior_neg;

vector<string> dict,dict1,dict2;

unordered_map <string,double> mp1,mp2;

set<string> vocab,stop_word;


int tested_answers[30000],current=0;



int isWordEnd(char c)
{
	if(c=='.' || c==',' || c==' ') return 1;
	else return 0;
}

// void TrainNB(string str[])
// {

// 	//sample

// 	N=25000,pos=12500,neg=12500;


//     prior_pos=pos*1.0/N,prior_neg=neg*1.0/N;


// 	// building vocabulary


// 	for(int i=0;i<N;i++)								   //taking one document at a time
// 	{
// 	    string word="";

// 			for(int j=0;j<=str[i].length();j++)
// 			{
// 				if(j==str[i].length() || isWordEnd(str[i][j])==1)
// 		    	{
// 		    	    //cout<<word<<" ";

// 		    		if(stop_word.find(word)==stop_word.end())
// 		    		{
// 		    			if(i<N/2) dict1.pb(word);			  // add word to vocab of positives
// 						else dict2.pb(word);
// 						vocab.insert(word);				  // add word to vocab of negatives
// 						dict.pb(word);                        // overall dictionary
// 		    		}
// 					word="";
// 				}
// 				else
// 				{
// 					word=word+str[i][j];
// 				}
// 			}
// 	}


// 	/*  for each word in vocabulary calculate its frequency in postive examples and negative examples  */

// 	std::set<string>::iterator it;

// 	for(it=vocab.begin();it!=vocab.end();it++)
// 	{
// 		string word=*it;

// 		int count=0;

// 		for(int j=0;j<dict1.size();j++)
// 		{
// 			if(dict1[j]==word) count++;
// 		}
// 		mp1[word]=count;
//         //if(word=="powerful") cout<<"*** "<<count;
// 		//mp1[word]=log((count+1)/(dict1.size()+vocab.size()));
// 		mp1[word]=(count+1)*1.0/(dict1.size()+vocab.size());

// 		count=0;
// 		for(int j=0;j<dict2.size();j++)
// 		{
// 			if(dict2[j]==word) count++;
// 		}
// 		mp2[word]=count;
//         //if(word=="powerful") cout<<"*** "<<count;
// 		//mp2[word]=log((count+1)/(dict2.size()+vocab.size()));

// 		mp2[word]=(count+1)*1.0/(dict2.size()+vocab.size());

// 	}

// }



// void testNB(string test_string)
// {

// 	// *******  probablity for postive   ************

// 	double prob1=0.5;

// 	int len=test_string.length();

// 	string word="";

// 	//cout<<test_string<<"  ";

// 	for(int i=0;i<=len;i++)
// 	{
// 		if(i==len || isWordEnd(test_string[i])==1)
// 		{
// 		    //cout<<word<<" ";
// 			if(word!="" && vocab.find(word)!=vocab.end())
// 			{
// 			    //cout<<"sdsd"<<prob1<<"  ";
// 			    //cout<<"  pos  *"<<word<<"  "<<mp1[word]<<"   ";
// 				prob1=prob1+mp1[word];
// 				//cout<<prob1<<"  ";
// 			}
// 			word="";
// 		}
// 		else word=word+test_string[i];
// 	}


// 	// *******  probablity for negative   ************


// 	double prob2=0.5;

// 	len=test_string.length();

// 	word="";

// 	for(int i=0;i<=len;i++)
// 	{
// 		if(i==len || isWordEnd(test_string[i])==1)
// 		{
// 			if(word!="" && vocab.find(word)!=vocab.end())
// 			{
// 			    //cout<<"    neg  *"<<word<<"  "<<mp2[word]<<"   ";
// 				prob2=prob2+mp2[word];
// 				//cout<<prob2<<" ";
// 			}
// 			word="";
// 		}
// 		else word=word+test_string[i];
// 	}

//     cout<<prob1<<"  "<<prob2<<endl;

//     if(prob1>prob2)
//     {
//     	tested_answers[current++]=1;
//     }
//     else tested_answers[current++]=-1;

// 	//if(prob1>prob2) return 1;
// 	//else return -1;
// }


int main() {
	// your code goes here


	unordered_map<string,string> map_total;


	std::ifstream input4( "imdb.vocab.txt" );

	string st;

	int cc=0;

	for( std::string st; getline( input4, st ); )
	{
		map_total.insert({to_string(cc),st});
		cc++;
	}

	//cout<<"YOOO "<<cc<<endl;
	// *****  populate stop words in stop_word  *****

	std::ifstream input( "stop_words.txt" );

	string wd,line;


	for( std::string line; getline( input, line ); )
	{
    	stop_word.insert(line);
	}

	cout<<stop_word.size()<<endl;


	//  *******    training the classifier    *******

	string D[30000],word;

	ifstream input1("dataset/train/labeledBow.feat");

	
	
	map<string,double> mp,mp1,mp2;


	while(getline(input1,word))
	{	
		istringstream iss(word);
  		vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());
  		
  		int rating= atoi(results[0].c_str());
  		
  		for(int i=1;i<results.size();i++)
  		{
  			string temp="";
  			string num="";
  			int fl=0;  			
  			for(int j=0;j<results[i].length();j++)
  			{
  				if(results[i][j]==':')
  				{
  					fl=1;
  					continue;
				}
				if(fl)
				num+=results[i][j];
				else
				temp+=results[i][j];
			}
			
			int no= atoi(num.c_str());
			//cout<<temp<<"  "<<no<<"    ";


			//  ****     storing frequencies except for stop words   ****
			string temp2=temp;
			if(map_total.find(temp)!=map_total.end())
			temp2=map_total[temp];
			if(stop_word.find(temp2)==stop_word.end())
			{
				mp[temp]+=no;
				if(rating>5) mp1[temp]+=no;
				else mp2[temp]+=no;
			}
			//else cout<<"Error"<<endl;
			
		}			
		//cout<<endl;	
	}




	ifstream input2("dataset/test/labeledBow.feat");


	// *****          testing the dataset               ******** 



	current=0;
	ll kk=0;

	while(getline(input2,word))
	{	
		istringstream iss(word);
  		vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());
  		
  		int rating= atoi(results[0].c_str());
  		
  		double prob1=0.5,prob2=0.5;

  		for(int i=1;i<results.size();i++)
  		{
  			string temp="";
  			string num="";
  			int fl=0;  			
  			for(int j=0;j<results[i].length();j++)
  			{
  				if(results[i][j]==':')
  				{
  					fl=1;
  					continue;
				}
				if(fl)
				num+=results[i][j];
				else
				temp+=results[i][j];
			}
			
			int no= atoi(num.c_str());
			//cout<<temp<<" "<<"yoo"<<"      ";
			if(mp.find(temp)!=mp.end())
			{
				// prob1=prob1+(mp1[temp]+1)*1.0/(mp[temp]+mp1[temp]);
				// prob2=prob2+(mp2[temp]+1)*1.0/(mp[temp]+mp2[temp]);	
				prob1=prob1+log((mp1[temp]+1)*1.0/(mp.size()+mp1.size()));
				prob2=prob2+log((mp2[temp]+1)*1.0/(mp.size()+mp2.size()));		
			}

		}	
			kk++;
			// if(kk<30)
			// cout<<setprecision(10)<<prob1<<"   "<<setprecision(10)<<prob2<<endl;
			if(prob1>prob2)
			{
				tested_answers[current]=1;
			} 			
			else tested_answers[current]=-1;
			current++;
	}

	cout<<kk<<endl;


	// ****   Calculating accuracy,precision and recall     **********


    ll fp=0,fn=0,tp=0,tn=0,c=0;

	for(int i=0;i<12500;i++)
	{
		if(tested_answers[i]==1) {tp++;c++;}
		else fn++;
	}
	

	for(int i=12500;i<25000;i++)
	{
	    //cout<<tested_answers[i]<<"  ";
		if(tested_answers[i]==-1) {tn++;c++;}
		else fp++;
	}


	double accuracy,precision,recall,frobenius;

	accuracy=(tp+tn)*1.0/25000;
	precision=tp*1.0/(tp+fp);
	recall=tp*1.0/(tp+fn);
	frobenius= (2*recall*precision)*1.0/(recall+precision);

    cout<<tp<<" "<<tn<<" "<<fp<<" "<<fn<<endl;

    //cout<<c<<" ";

    //cout<<setprecision(6)<<c*1.0/2000<<endl;

	cout<<"*****Accuracy is "<<setprecision(6)<<accuracy<<"*****"<<endl;
	cout<<"*****Precision is "<<setprecision(6)<<precision<<"*****"<<endl;
	cout<<"*****Recall is "<<setprecision(6)<<recall<<"*****"<<endl;
	cout<<"*****f-measure is "<<setprecision(6)<<frobenius<<"*****"<<endl;


	return 0;
}
