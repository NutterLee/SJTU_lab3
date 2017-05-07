#include<stack>
#include<queue>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>

using namespace std;

//�ֵ��в���wordģ��
//���ڼ�������Ҫ�ı��ֵ��ļ�������vector�洢
//�������ֵ��ļ��Ѿ��ź�������ֱ���ö��ֲ���
//word1�Ǵ����ҵĵ��ʣ�dictionary�Ǵ洢word��vector
bool is_a_word(const string& word1, const vector<string>& dictionary);


//�ļ�д�뵽vectorģ��
//filename���û�������ļ������֣�vector�Ǵ洢word��
//�ж��ļ��Ƿ���ڣ��������׳��쳣����Ҫ�����������ڵ��������������һ��ʵ�֣�
void get_dictionary(const string& filename, vector<string>& dictionary);

//�ж��ǲ���neighbor
//bool is_neighbor(const string word1, const string word2);
bool is_in_ladder2(const string& word, queue<stack<string>>& ladder);

//�ж�һ��word�ǲ�����ladder��stack������
bool is_in_ladder(const string word,  stack<string>& ladder);
//��������
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

					//����������Ǵ�д����Сд��һ��ת����Сд
					transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
					transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
					//���Ȳ��ȣ���ͬ���ʣ����ǵ��ʣ�������
					if (word1.length() != word2.length()) throw word_length_error();
					if (word1 == word2) throw same_word();
					if (!is_a_word(word1,dictionary) || !is_a_word(word2,dictionary))throw invalid_word();

					//������Ӧ���Ǿ��崦����̣���������֮���ladder��������������һ��ѭ��
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
						//����һ�����ʵ�neighbors,��������Ϊneighbors��stack��
						for (int i = 0; i < tmp_word.size(); i++)
						{
							for (char character = 'a'; character <= 'z'; character++)
							{
								string new_word;
								//�����滻��һ����ĸ�˵�new_word
								for (int k = 0; k < tmp_word.size(); k++)
								{
									if (k == i)new_word += character;
									else new_word += tmp_word[k];
								}
								//���������Ч���ʣ�ֱ����һ��ѭ��
								if (!is_a_word(new_word, dictionary)) continue;
								//����Ч���ʣ�����Ѿ���neighbors������ֹ��ˣ���ֱ����һ��ѭ��								
								//����һ��is_in_ladder���������new_word�Ѿ���neighbor������ ֱ����һ��ѭ��
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
						
						//����ÿ����tmp_stack��top��neighbor�ĵ���
						while(neighbors.size()!=0)
						{
							string nei_word = neighbors.top();
							neighbors.pop();
							//û�г�����ladder�й�
							if (!is_in_ladder(nei_word, tmp_stack))
							{	//���ǡ����word2 ��ô���������ڴ����tmp_stackд��final_result��������ѭ��
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
									//�����ߵ���һ����stack����һ�ݺ󱣴浽queue��
									stack<string> tmp_stack2(tmp_stack);
									tmp_stack2.push(nei_word);
									words.push(tmp_stack2);
								}
							}
						
						}
						if (get_result == true) break;

					}
					//�Եõ�����������
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
		//���ladder���˾���word ֱ���޸�boolȻ������ѭ��
		if (ladder.top() == word)
		{
			same_word = true;
			break;
		}
		//����top��Ԫ�������µĸ���stack��
		tmp_ladder.push(ladder.top());
		//��ɾ����ԭstack��topԪ��
		ladder.pop();
	}
	//��λstack
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