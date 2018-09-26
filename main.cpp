#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <iterator>
#include <cstring>
#include <regex>

using namespace std;


bool isIntInString(const std::string &in_str);

int calcRes(const string &key, const map<string, vector<string>> &mapParsedString);

vector<string> split(const string &str, const string &symbolForSplit);

set<string> getStringSetFromFile(string filename);

void splitByControlSymbols(vector<string> inputDataVector, map<string, vector<string>> &mapParsedString);

void printMapWithData(const map<string, vector<string>> &mapParsedString);

static int recCounter = 0;

int main(int argc, char *argv[]) {
    string filename;

    if (argc != 0 && argv[1] != 0) {
        cout << "argv0 = " << argv[1] << endl;
        filename = argv[1];
    }

    //ловим exception несуществующего файла
    try {
        set<string> setOfStringInFile = getStringSetFromFile(filename);
        vector<string> vectorWithInputData/*[setOfStringInFile.size()]*/;

        for (auto it = setOfStringInFile.begin(); it != setOfStringInFile.end(); ++it) {
            string ans = *it;
            vectorWithInputData.push_back(ans);
        }
        map<string, vector<string>> mapParsedString;

        splitByControlSymbols(vectorWithInputData, mapParsedString);
//    printMapWithData(mapParsedString);


        ofstream resultFile("res_output.txt");
        for (auto elem: mapParsedString) {
            int result = 0;
            result = calcRes(elem.first, mapParsedString);
            cout << "result key: " << elem.first << endl;
            cout << "result: " << result << "" << "\n" << endl;
            resultFile << elem.first << " = " << result << "\n" << endl;
        }
        cout << "recCounter: " << recCounter << endl;
        resultFile.close();
    } catch (exception &ex) {
        cout << ex.what() << "!\n";
    }

    return 0;
}


int calcRes(const string &key, const map<string, vector<string>> &mapParsedString) {

    int calcValueForThisKey = 0;
    vector<string> resVec = mapParsedString.at(key);
    recCounter++;
    //TODO определить глубину рекурсии
    if (recCounter > 100) {
        throw runtime_error("Recursive error");
    }

// TODO заменить на try/except
    if (mapParsedString.empty()) {
        return -100;
    }

    if (resVec.size() == 1 && isIntInString(*resVec.begin())) {
//        cout << "key = " << key << ", value = " << stoi(*resVec.begin()) << endl;
        return stoi(*resVec.begin());
    }
    for (vector<string>::const_iterator resVecIter = resVec.begin();
         resVecIter != resVec.end(); ++resVecIter) {
//        cout << "current vector element" << *resVecIter << endl;
        if (isIntInString(*resVecIter)) {
            calcValueForThisKey += stoi(*resVecIter);
        }

        if (!isIntInString(*resVecIter)) {
            if (0 == mapParsedString.count(*resVecIter)) {
                return -1000; //признак ошибки,что такого имени параметра не существует
            }
            calcValueForThisKey += calcRes(*resVecIter, mapParsedString);
        }
    }
    return calcValueForThisKey;
}

//рубим по символу
vector<string> split(const string &str, const string &symbolForSplit) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(symbolForSplit, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
//        cout << "token: " << token << endl;
        if (!token.empty()) tokens.push_back(token);
        prev = pos + symbolForSplit.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool isIntInString(const std::string &in_str) {
    size_t size = in_str.size();
    if (size == 0) return false;
    int it = in_str.find_first_not_of("1234567890");

    if (it != std::string::npos)     // Нашли символ, который не число
    {
        if (size == 1 || size <= (it + 1) || it != 0)
            return false;
        if (in_str.at(0) != '-')
            return false;

        it = in_str.find_first_not_of("1234567890", it + 1);

        if (it != std::string::npos) // Нашли еще один символ, который не число
            return false;
    }
    return true;
}

set<string> getStringSetFromFile(string filename) {
    //set  - потому что дубликаты нам не надо обрабатывать
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("FileNotFound");
    }
    string line = "";
    set<string> setOfStringInFile;
    while (getline(inputFile, line)) {
        if (line == "") {
            continue;
        }
        line.erase(remove(line.begin(), line.end(), ' '), line.end());//надо удалить пробелы
        setOfStringInFile.insert(line);
    }
    inputFile.close();
    return setOfStringInFile;
}

void splitByControlSymbols(vector<string> inputDataVector, map<string, vector<string>> &mapParsedString) {
    //дробим сначала по =, а потом по +.получаем пары param_name
    for (vector<string>::const_iterator vecIter = inputDataVector.begin();
         vecIter != inputDataVector.end(); ++vecIter) {
//        cout << *vecIter << endl;
        string res = *vecIter;
//        cout << "string: " << res << endl;
        vector<string> parsedStr = split(res, "=");
        //сплит по + вектор с числами/переменными
        vector<string> valueWithoutPlusSymbol = split(*(parsedStr.begin() + 1), "+");
//        cout << "parsed: " << *parsedStr.begin() << " " << *(parsedStr.begin() + 1) << endl;
        mapParsedString.insert(pair<string, vector<string>>(*parsedStr.begin(), valueWithoutPlusSymbol));
    }
}

void printMapWithData(const map<string, vector<string>> &mapParsedString) {
    for (auto elem: mapParsedString) {
        cout << "map key: " << elem.first << endl;
        vector<string> vecOfVal = elem.second;
        for (vector<string>::const_iterator vecOfValIter = vecOfVal.begin();
             vecOfValIter != vecOfVal.end(); ++vecOfValIter) {
            cout << *vecOfValIter << " ";
        }
        cout << endl;
    }
}




