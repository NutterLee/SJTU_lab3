#include<stack>
#include<queue>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

//字典中查找word模块
//由于几乎不需要改变字典文件，采用vector存储
//所给的字典文件已经排好序，所以直接用二分查找
//word1是待查找的单词，dictionary是存储word的vector
bool is_a_word(const string& word1, const vector<string>& dictionary);


//文件写入到vector模块
//filename是用户输入的文件的名字，vector是存储word的
//判断文件是否存在，不存在抛出异常，（要求重新输入在调用这个函数的那一层实现）
void get_dictionary(const string& filename, vector<string>& dictionary);

//错误处理类
class invalid_word{};
class word_length_error{};
class same_word{};
class file_not_found{};

int main()
{
	vector<string>dictionary;
	string filename;
	string word1;
	string word2;
	while (true)
	{
		try
		{
			cout << "Dictionary file name? ";
			cin >> filename;
			cout << endl;
			get_dictionary(filename, dictionary);
			while (true)
			{
				try
				{
					cout << "Word #1 (or Enter q to quit):";
					cin >> word1;
					if (word1 == "q") return 0;
					cout << "Word #2 (or Enter q to quit):";
					cin >> word2;
					if (word2=="q")return 0;
					if (word1.length() != word2.length()) throw word_length_error();
					if (word1 == word2) throw same_word();
					if (!is_a_word(word1,dictionary) || !is_a_word(word2,dictionary))throw invalid_word();
					//接下来应该是具体处理过程，两个单词之间的ladder建立完毕则进入下一轮循环
				}
				catch (word_length_error) { cout << "the words should have the same length" << endl; }
				catch (same_word) { cout << "the words should be different" << endl; }
				catch (invalid_word) { cout << "the words must be valid" << endl; }
			}
		}
		catch (file_not_found) { cout << "the file cannot be found, try again" << endl; }
	}

	//cout << "dictionary got!" << endl;
    //cout << is_a_word("good", dictionary) << endl;

}

bool is_a_word(const string & word1, const vector<string>& dictionary)
{
	int head = 0;
	int end = dictionary.size() - 1;
	int middle = (head + end) / 2;
	while (head !=middle)
	{
		if (word1>dictionary[middle])
		{
			head = middle;
			middle = (head + end) / 2;
		}
		else if (word1 == dictionary[middle])return true;
		else
		{
			end = middle;
			middle = (head + end) / 2;
		}
	}



}

void get_dictionary(const string & filename, vector<string>& dictionary)
{
	ifstream file(filename);
	if (!file.is_open()) throw file_not_found();
	string word;
	while (getline(file,word))
	{
		dictionary.push_back(word);
	}
}
