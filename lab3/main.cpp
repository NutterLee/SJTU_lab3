#include<stack>
#include<queue>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>

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

//判断是不是neighbor
//bool is_neighbor(const string word1, const string word2);
bool is_in_ladder2(const string& word, queue<stack<string>>& ladder);

//判断一个word是不是在ladder（stack）里面
bool is_in_ladder(const string word,  stack<string>& ladder);
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
			queue<stack<string>> words;
			stack<string> final_result;

			cout << "Dictionary file name? ";
			cin >> filename;			
			get_dictionary(filename, dictionary);
			
			while (true)
			{
				try
				{
					cout <<endl<< "Word #1 (or Enter q to quit):";
					cin >> word1;
					if (word1 == "q") return 0;
					cout << "Word #2 (or Enter q to quit):";
					cin >> word2;
					if (word2=="q")return 0;

					//不管输入的是大写还是小写，一律转换成小写
					transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
					transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
					//长度不等，相同单词，不是单词，都报错
					if (word1.length() != word2.length()) throw word_length_error();
					if (word1 == word2) throw same_word();
					if (!is_a_word(word1,dictionary) || !is_a_word(word2,dictionary))throw invalid_word();

					//接下来应该是具体处理过程，两个单词之间的ladder建立完毕则进入下一轮循环
					stack<string> word_container;
					bool get_result = false;
					word_container.push(word1);
					words.push(word_container);
					while (!words.empty())
					{
						//vector<string>neighbors;
						stack<string>neighbors;
						stack<string>tmp_stack = words.front();
						words.pop();
						string tmp_word = tmp_stack.top();						
						//生成一个单词的neighbors,并存入名为neighbors的stack中
						for (int i = 0; i < tmp_word.size(); i++)
						{
							for (char character = 'a'; character <= 'z'; character++)
							{
								string new_word;
								//生成替换了一个字母了的new_word
								for (int k = 0; k < tmp_word.size(); k++)
								{
									if (k == i)new_word += character;
									else new_word += tmp_word[k];
								}
								//如果不是有效单词，直接下一个循环
								if (!is_a_word(new_word, dictionary)) continue;
								//是有效单词，如果已经在neighbors里面出现过了，就直接下一个循环								
								//借用一下is_in_ladder函数，如果new_word已经在neighbor里面了 直接下一个循环
								if (is_in_ladder(new_word, neighbors))continue;
								/*
								for (auto s : neighbors)
								{
									if (s == new_word)
									{
										already_in_neighbor = true;
										break;
									}
								}
								if (already_in_neighbor == true) continue;
								*/
								neighbors.push(new_word);

							}
						}
						/*
						for (auto a : neighbors)
						{
							cout << a << endl;
						}
						*/			
						
						//对于每个是tmp_stack的top的neighbor的单词
						while(neighbors.size()!=0)
						{
							string nei_word = neighbors.top();
							neighbors.pop();
							//没有出现在ladder中过
							if (!is_in_ladder(nei_word, tmp_stack))
							{	//如果恰好是word2 那么将手上正在处理的tmp_stack写入final_result，并跳出循环
								if (nei_word == word2)
								{
									get_result = true;
									tmp_stack.push(nei_word);
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
									stack<string> tmp_stack2(tmp_stack);
									tmp_stack2.push(nei_word);
									words.push(tmp_stack2);
								}
							}
						
						}
						if (get_result == true) break;

					}
					//对得到结果的最后处理
					if (final_result.size() == 0) cout << "The ladder from " << word1 << "to " << word2 << " doesn't exist.";
					else
					{
						cout << "A ladder from " << word1 << " to " << word2 << ":" << endl;
						stack<string> out_put;
						while (final_result.size() != 0)
						{
							cout << final_result.top() << " ";
							final_result.pop();
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
	

	//cout << "dictionary got!" << endl;
  //  cout << is_a_word("zsd", dictionary) << endl;
	system("pause");
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
	if (head == middle) return false;
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

/*
bool is_neighbor(const string word1, const string word2)
{
	int differ_num = 0;
	for (int i = 0; i < word1.size(); i++)
	{
		if (word1[i] != word2[i])differ_num++;
	}
	
	
}
*/

bool is_in_ladder(const string word,  stack<string>& ladder)
{
	stack<string>tmp_ladder;
	bool same_word = false;
	while (ladder.size() != 0)
	{
		//如果ladder顶端就是word 直接修改bool然后跳出循环
		if (ladder.top() == word)
		{
			same_word = true;
			break;
		}
		//否则将top的元素推入新的辅助stack中
		tmp_ladder.push(ladder.top());
		//并删除掉原stack的top元素
		ladder.pop();
	}
	//复位stack
	while (tmp_ladder.size() != 0)
	{
		ladder.push(tmp_ladder.top());
		tmp_ladder.pop();
	}	
	if (same_word == true)	return true;
	else return false;
}

bool is_in_ladder2(const string& word, queue<stack<string>>& ladder)
{
	queue<stack<string>> tmp_queue ;
	bool is_in = false;
	while (ladder.size() != 0)
	{
		stack<string> this_stack = ladder.front();
		ladder.pop();
		if (is_in_ladder(word, this_stack))
		{
			is_in = true;
			tmp_queue.push(this_stack);
			break;
		}
		else
		{
			tmp_queue.push(this_stack);
		}
	}
	while (tmp_queue.size() != 0)
	{
		ladder.push(tmp_queue.front());
		tmp_queue.pop();
	}
	if (is_in == true) return true;
	else return false;
}