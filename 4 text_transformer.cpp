#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int counter_of_accurate_substring (string base, string to_find, vector<int> &positions){
    int count=0;
    int flag_1=1;
    int flag_2=1;
    for (int i=0; i<=base.length()-to_find.length();){
        size_t position = base.find(to_find, i);
        if (position == std::string::npos){
            break;
        }
        else{
            flag_1=1;
            flag_2=1;
            if (position!=0){
                if ((65<=base[position-1] && base[position-1]<=90) || (97<=base[position-1] && base[position-1]<=122)){
                    flag_1=0;
                }
            }
            if ((position+to_find.length())<(base.length())){
                if ((65<=base[position+to_find.length()] && base[position+to_find.length()]<=90) || (97<=base[position+to_find.length()] && base[position+to_find.length()]<=122)){
                    flag_2=0;
                }
            }
            if ((flag_1+flag_2)==2){
                ++count;
                positions.push_back(position);
            }
        }
        i=position+1;
    }
    return count;
}

void erasing_of_all_substring(string &base, string to_erase, vector<int> positions, int counter_of_erase){
    for (int i=0; i<counter_of_erase; i++){
        base.replace(positions[i],to_erase.length(),"");
    }
}

void replacing_of_all_substring(string &base, string to_replace, string for_replace, vector<int> positions, int counter){
    for (int i=0; i<counter ; i++){
        base.replace(positions[i],to_replace.length(),for_replace);
    }
}
int main()
{
    char c='x';
    vector <string> sentences;
    string s_to_replace,s_for_replace,s_for_erase;
    cin>>s_to_replace>>s_for_replace>>s_for_erase;
    getchar();

    while ((cin.peek()!=EOF)){
        string s = "" ;
        while ((c!='.') && (c!='!') && (c!='?')) {
            c=getchar();
            if (c==EOF){
                break;
            }
            s.push_back(c);
        }
        sentences.push_back(s);
        if (c==EOF){
            break;
        }
        c=getchar();
    }
    for (string s: sentences){
        vector<int> positions_to_erase;
        int counter_of_erase=counter_of_accurate_substring(s,s_for_erase,positions_to_erase);
        if (counter_of_erase==0){
            vector<int> positions_to_replace;
            int k=counter_of_accurate_substring(s,s_to_replace,positions_to_replace);
            replacing_of_all_substring(s,s_to_replace,s_for_replace,positions_to_replace,k);
            cout<<s<<' ';
        }
        else{
            erasing_of_all_substring(s,s_for_erase,positions_to_erase,counter_of_erase);
        }
    }
    return 0;
}
