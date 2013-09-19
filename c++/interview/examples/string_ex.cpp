#include<iostream>
#include<map>

using namespace std;

typedef map<char, int> char_counter;

bool ex_1_single(string s) {
  for (int i=0; i<s.length(); i++) {
    for (int j=i+1; j<s.length(); j++) {
      if (s[i] == s[j]) {
	return false;
      }
    }
  }
  return true;
}

string ex_2_reverse(string s) {
  string r(s);
  for (int i=s.length()-1; i>=0; i--) {
    r[r.length()-i-1] = s[i];
  }
  return r;
}

string ex_3_dup(string s) {
  string r(s);
  bool end=false;
  int k=0;
  int j;
  for (int i = 0; i<s.length(); i++) {
    for (j=i+1; j<s.length(); j++) {
      if (s[i]==s[j]) {
	break;
      }
    }
    if(j==s.length()) {
      r[k]=s[i];
      k++;
    }
  }
  r.erase(k,r.length()-k);
  return r;
}

bool anagram(string s, string r) {
  if(s.length()!=r.length())
    return false;
  char_counter c;
  for(int i=0; i<s.length(); i++) {
    if(c.count(s[i]) == 0) {
      c[s[i]]=1;
    } else {
      c[s[i]]+=1; 
    }
  }
  for(int i=0; i<s.length(); i++) {
    if(c.count(r[i])==0) {
      return false;
    } else {
      if( c[r[i]]=1 ) {
	c.erase(r[i]);
      } else {
	r[i]-=1;
      }
    }
  }
  return c.empty();
}

string replace(string val, string s) {
  string res;
  int size = val.length();
  res.resize(s.length()*size);
  int j = 0;
  for(int i=0; i<s.length(); i++) {
    if(!isspace(s[i]))
      res[j++]=s[i];
    else {
      for(int x=0; x<val.length(); x++) {
	res[j++] = val[x];
      }
    }
  }
  return res;
}

void remove_dup2(string *tmp) {
  int i=0,j=0,tail = 1;
  for (i=1; i<tmp->length(); i++) {
    for (j=0; j<tail; j++) {
      if ((*tmp)[i]==(*tmp)[j]) break;
    }
    if (j==tail) {
      (*tmp)[tail] = (*tmp)[i];
      tail++;
    }
  }
  tmp->erase(tail,tmp->length()-tail);
}

int main(int argn, char **argv) {
  cout << ((ex_1_single("cavoli a merenda"))?"True":"False") << endl;
  cout << ((ex_1_single("prova 123"))?"True":"False") << endl;
  cout << ((ex_1_single("prova pippo"))?"True":"False") << endl;
  cout << ((ex_1_single("prova costa"))?"True":"False") << endl;
  cout << ((ex_1_single(""))?"True":"False") << endl;
  cout << ((ex_1_single("prova casta"))?"True":"False") << endl;
  cout << ex_2_reverse(ex_2_reverse("puzza")) << endl;
  cout << ex_2_reverse(ex_2_reverse("mitocondriaco")) << endl;
  cout << ex_3_dup("provap") << endl;
  cout << ex_3_dup("pppppp") << endl;
  cout << ((anagram("prova", "pluto"))?"True":"False") << endl;
  cout << ((anagram("caso", "cosa"))?"True":"False") << endl;
  cout << replace("20%","prova 123 abc") << endl;
  string tmp = "pravaaaaaaaaaaaaaaaaa";
  cout << tmp.length() << endl;
  remove_dup2(&tmp);
  cout << tmp.length() << endl;
  cout << tmp << endl;
}
