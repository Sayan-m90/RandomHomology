#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <limits.h>
#include <iomanip>
using namespace std;
int main()
{
	cout<<"Enter file name to remove 'e' for exponent: ";
	string word, inp, e, e2;
	int dim, count = 0;
	cin>>inp;
	cout<<"Enter dimension: ";
	cin>>dim;
	e = inp+".txt";
	e2 = inp+"2.txt";
	ifstream file;
	ofstream file2;
    file.open(e);
    file2.open(e2);
    if (!file.is_open()) 
    {
        cout<<"File does not exist";
        return 0;
    }

    string str3, str4;
    string str2 = "e";

    while (file >> word)
    {
    	//cout<<"\nword: "<<word;
    	//getchar();
        size_t found = word.find(str2);
        
        if (found!= string::npos)
        	{
        		str3 = word.substr(found+1);	//after e
        		str4 = word.substr(0,word.size()-str3.size()-1);	//before e, -1 is so it doesnt include e
        		double val = stod(str4)*pow(10,stof(str3));
                if(count==dim-1)
                    file2 << fixed << std::setprecision (25) << val << endl;
        		else
                    file2 << fixed << std::setprecision (25) << val << " ";

        		cout<<fixed<<std::setprecision (25)<<"original: "<<word<<" before e: "<<str4<<" after e: "<<str3<<" Final val:"<<val<<endl;
        	}
        else	
            {
                if(count==dim-1)
                    file2 << fixed <<std::setprecision (25)<< word << "\n";
                else
                    file2 << fixed <<std::setprecision (25)<< word << " ";
            }

        count++;
        if(count == dim)
        	{
        		
        		count = 0;
        	}
    }
    return 1;
}