#include <bits/stdc++.h>
#include <tuple>
#include <unordered_map>
#define MAXN 1300

using namespace std;

typedef unsigned long long hashValue;
typedef pair<hashValue, hashValue> P;

#define MAXN 1300

class OCRErrorReduction{
    struct DoubleHash {
        hashValue powers1[MAXN], powers2[MAXN], values1[MAXN], values2[MAXN], prime1, prime2;

        DoubleHash(const string &word, const hashValue base1 = 419, const hashValue base2 = 521, const hashValue prime1 = 82517999, const hashValue prime2 = 76824091) {
            precalcPowers(powers1, base1, prime1);
            precalcPowers(powers2, base2, prime2);

            precalcValues(values1, base1, prime1, powers1, word);
            precalcValues(values2, base2, prime2, powers2, word);

            this->prime1 = prime1;
            this->prime2 = prime2;
        }

        void precalcPowers(hashValue *powers, const hashValue base, const hashValue prime) {
            powers[0] = 1;
            for(int i = 1; i < MAXN; i++) {
                powers[i] = powers[i-1] * base;
                powers[i] %= prime;
            }
        }

        void precalcValues(hashValue *values, const hashValue base, const hashValue prime, hashValue *powers, const string &word) {
            int tam = ((int) word.size());
            values[0] = word[0] + 307;
            for(int i = 1; i < tam; i++) {
                values[i] = ( word[i] + 307 ) + (values[i-1] * base);
                values[i] %= prime;
            }
        }

        hashValue hashSubstr(int l, int r) {
            if(l == 0) return values1[r] + values2[r];
            hashValue r1, r2;	

            r1 = (values1[r] - (values1[l-1] * powers1[r - l + 1])%this->prime1 + this->prime1) % this->prime1;
            r2 = (values2[r] - (values2[l-1] * powers2[r - l + 1])%this->prime2 + this->prime2) % this->prime2;

            return r2 + r1;
        }
    };

    typedef struct node{
        string word;
        unordered_map< hashValue, int > before, after;
        node(string word){
            this->word = word;
        }
        node(){
            this->word = "";
        }
    } node;

    unordered_map<hashValue, node> nodes;
    
    public:
        int initialize(vector <string> keyphrases) {

            // Build base words
            for (string line : keyphrases) {
                string before = "";
                string tmp = "";
                
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ' ') {
                        hashValue hash = DoubleHash(tmp).hashSubstr(0, tmp.length());

                        if (nodes.find(hash) == nodes.end()) {
                            node newNode = node(tmp);
                            nodes.insert(make_pair(hash, newNode));

                            if (before != "") {
                                hashValue beforeHash = DoubleHash(before).hashSubstr(0, before.length());
                                if (nodes.at(hash).before.find(beforeHash) == nodes[hash].before.end()){
                                    nodes.at(hash).before.insert(make_pair(beforeHash, 1));
                                } else {
                                    nodes[hash].before[beforeHash] += 1;
                                }
                            }
                        }

                        before = tmp;
                        tmp = "";
                    }
                    tmp += line[i];
                }                
            }

            return 0;
        }

        string clean(string paragraph){
            string word = "";
            vector<string> words;
            vector<string> cleaned;

            for (int i = 0; i < paragraph.length(); i++) {
                if (paragraph[i] == ' ') {
                    if (word != "") {
                        words.push_back(word);
                    }
                    word = "";
                }
                word += paragraph[i];
            }

            hashValue error = -1;

            for (int i = 0; i < words.size(); i++) {
                hashValue currHash = DoubleHash(word).hashSubstr(0, word.length());

                if (error != -1) {
                    string bestString = "";
                    if (nodes.find(currHash) != nodes.end()) {

                        unordered_map<hashValue, int>:: iterator itr;
                        int maxFreq = 0;
                        hashValue best = -1;

                        for (itr = nodes[currHash].before.begin(); itr != nodes[currHash].before.end(); itr++) {
                            if (itr->second > maxFreq){
                                maxFreq = itr->second;
                                best = itr->first;
                            }
                        }

                        bestString = nodes[best].word;
                    }

                    if (bestString == "") bestString = words[i-1];
                    cleaned.push_back(bestString);

                    error = -1;
                } 

                // Found error 
                if (nodes.find(currHash) == nodes.end()) {
                   error = currHash;
                } else {
                    cleaned.push_back(words[i]);
                }                
            }

            string cleaned_paragraph = "";
            for (int i = 0; i < cleaned.size(); i++) {
                string s = cleaned[i];
                cleaned_paragraph += s;
                if (i != cleaned.size() - 1) {
                    cleaned_paragraph += " ";
                }
            }
            return cleaned_paragraph;
        }


};

