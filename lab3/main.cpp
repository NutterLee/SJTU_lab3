//做了第二个单词不是dictionary中有效的单词的部分
#include<stack>
#include<queue>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<set>
#include<unordered_set>
#include<cctype>
#include<algorithm>

using namespace std;

//文件写入到vector模块
//filename是用户输入的文件的名字，vector是存储word的
//判断文件是否存在，不存在抛出异常，（要求重新输入在调用这个函数的那一层实现）
void get_dictionary(const string& filename, unordered_set<string>& dictionary);


//错误处理类
class invalid_word{};
class word_length_error{};
class same_word{};
class file_not_found{};

int main()
{	
	unordered_set<string>dictionary;
	string filename;
	string word1;
	string word2;	
	while (true)
	{
		try
		{
			cout << "Dictionary file name? ";
			cin >> filename;			
			get_dictionary(filename, dictionary);

			while (true)
			{
				queue<stack<string>> words;
				//final_result存储的是最后的结果
				stack<string> final_result;
				//word_collection 是存储已经使用过的单词，因为要大量查找，用unordered_set比较好
				unordered_set<string>word_collection;
				try
				{
					cout <<endl<< "Word #1 (or Enter q to quit):";
					cin >> word1;
					if (word1 == "q")
					{
						cout << "Have a nice day."<<endl;
						return 0;
					}

					cout << "Word #2 (or Enter q to quit):";
					cin >> word2;
					if (word2 == "q")
					{
						cout << "Have a nice day."<<endl;
						return 0;
					}

					//不管输入的是大写还是小写，一律转换成小写
					transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
					transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
					//长度不等，相同单词，word1不是单词，都报错
					if (word1.length() != word2.length()) throw word_length_error();
					if (word1 == word2) throw same_word();
					if (dictionary.find(word1)==dictionary.end() || dictionary.find(word2)==dictionary.end())throw invalid_word();

					//接下来应该是具体处理过程，两个单词之间的ladder建立完毕则进入下一轮循环
					stack<string> word_container;
					bool get_result = false;
					word_container.push(word1);
					words.push(word_container);
					while (!words.empty())
					{
						stack<string>tmp_stack = words.front();
						words.pop();
						string tmp_word = tmp_stack.top();						
						//生成一个单词的neighbors,并存入名为neighbors的stack中
						for (int i = 0; i < tmp_word.size(); i++)
						{
							for (char character = 'a'; character <= 'z'; character++)
							{
								string new_word (tmp_word);
								//生成替换了一个字母了的new_word
								new_word[i] = character;
								//如果不是有效单词，直接下一个循环
								if (dictionary.find(new_word)==dictionary.end()) continue;								
								if (new_word == tmp_word) continue;
								if (word_collection.find(new_word) != word_collection.end()) continue;	
								else word_collection.insert(new_word);
									//如果恰好是word2 那么将手上正在处理的tmp_stack写入final_result，并跳出循环
								if (new_word == word2)
									{
										get_result = true;
										tmp_stack.push(new_word);
										while (tmp_stack.size() != 0)
										{
											final_result.push(tmp_stack.top());
											tmp_stack.pop();
										}
										break;
									}
								else
									{
										//否则将走到这一步的stack拷贝一份后保存到queue中
										stack<string> tmp_stack2=tmp_stack;
										tmp_stack2.push(new_word);
										words.push(tmp_stack2);
									}
								
								//是有效单词，如果已经在neighbors里面出现过了，就直接下一个循环								
								//借用一下is_in_ladder函数，如果new_word已经在neighbor里面了 直接下一个循环
							//	if (is_in_ladder(new_word, neighbors))continue;
								//neighbors.push(new_word);
							}
						}								
						//对于每个是tmp_stack的top的neighbor的单词
						if (get_result == true) break;
					}

					//对得到结果的最后处理，输出结果
					//如果final_result是空的那说明没有找到
					if (final_result.size() == 0) cout << "The ladder from " << word1 << " to " << word2 << " doesn't exist.";
					else
					{
						cout << "A ladder from " << word2 << " to " << word1<< ":" << endl;
						stack<string> out_put;
						while (final_result.size() != 0)
						{
							out_put.push( final_result.top());
							final_result.pop();
						}
						while (out_put.size() != 0)
						{
							cout << out_put.top() << " ";
							out_put.pop();
						}
					}		
				}
				catch (word_length_error) { cout << "the words should have the same length, try again" << endl; }
				catch (same_word) { cout << "the words should be different, try again" << endl; }
				catch (invalid_word) { cout << "the words must be valid, try again" << endl; }
			}
		}
		catch (file_not_found) { cout << "the file cannot be found, try again" << endl; }
	}
	return 0;
}

//dictionary的写入程序，将dictionary的每一个单词写入到set中
void get_dictionary(const string & filename, unordered_set<string>& dictionary)
{
	ifstream file(filename);
	if (!file.is_open()) throw file_not_found();
	string word;
	while (getline(file,word))
	{
		dictionary.insert(word);
	}
}
