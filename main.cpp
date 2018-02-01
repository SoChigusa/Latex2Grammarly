
/*
 * Project: Latex2Grammerly
 * File: main.cpp
 * Author: So Chigusa
 *
 * Created on 2018/1/31
 */

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int replacePos(string & arg_buffer, vector<int> arg_pos, const string arg_str) {
  if(arg_pos.size() % 2 == 0) {
    for(int i = arg_pos.size()-1; i >= 1; i=i-2) {
      arg_buffer.replace(arg_pos[i-1], arg_pos[i]-arg_pos[i-1]+1, arg_str);
    }
  } else { return -1; }
  return 0;
}

int replaceLatex(string & arg_buffer, const string arg_fbegin, const string arg_fend, const string arg_str) {
  int currentPos = -1;
  vector<int> posMath;
  string strTarget = arg_fbegin;
  while((currentPos = (int)arg_buffer.find(strTarget, currentPos+1)) != -1) {
    posMath.push_back(currentPos);
    if(strTarget == arg_fbegin) { strTarget = arg_fend; }
    else { strTarget = arg_fbegin; }
  }

  if(replacePos(arg_buffer, posMath, arg_str) == -1) {
    cout << "Mismatch between beginFlag=" << arg_fbegin << " and endFlag=" << arg_fend << " ";
    return -1;
  }
  return 0;
}

int main(int argc, char** argv) {
  ifstream ifs;
  ofstream ofs;
  if (argc == 3) {
    ifs.open(argv[1]);
    ofs.open(argv[2]);
  } else {
    cout << "1st argument: input filename, 2nd argument: output filename." << endl;
    return 0;
  }
  if (!(ifs && ofs)) {
    throw runtime_error("File open error.");
  }

  /*
   * line status
   * 0: normal
   * 1: comment (start from %)
   * 2: align environment
   * 3: empty line
   */
  int lineStatus = 0;
  int nLine = 0;
  string strBufferLine;
  stringstream tmp;
  while(getline(ifs, strBufferLine)) {
    nLine++;
    while(strBufferLine[0] == ' ') { strBufferLine.erase(strBufferLine.begin()); }
    if(lineStatus == 0 && (int)strBufferLine.find("\\begin{align}") != -1) {
      lineStatus = 2;
      tmp << "[equation]";
    }
    else if(lineStatus == 0 && strBufferLine[0] == '%') { lineStatus = 1; }
    else if(lineStatus == 0 && strBufferLine.empty()) { lineStatus = 3; }
    
    if(lineStatus == 0) {
      tmp << strBufferLine << " ";
    } else if(lineStatus == 1) {
      lineStatus = 0;
    } else if(lineStatus == 3) {
      strBufferLine = tmp.str();
      if(replaceLatex(strBufferLine, "$", "$", "[expression]") == -1 ||
	 replaceLatex(strBufferLine, "\\cite{", "}", "[citation]") == -1 ||
	 replaceLatex(strBufferLine, "\\rem{", "}", "") == -1) {
	cout << "in line: " << nLine << endl;
    	return -1;
      }
      ofs << strBufferLine << endl;
      ofs << endl;
      tmp.str("");
      tmp.clear(stringstream::goodbit);
      lineStatus = 0;
    }

    if(lineStatus == 2 && (int)strBufferLine.find("\\end{align}") != -1) {
      lineStatus = 0;
      ifstream::pos_type pos = ifs.tellg();
      getline(ifs, strBufferLine);
      ifs.seekg(pos);
      if(islower(strBufferLine[0]) != 0) { tmp << ", "; }
      else { tmp << ". "; }
    }
  }

  return 0;
}

