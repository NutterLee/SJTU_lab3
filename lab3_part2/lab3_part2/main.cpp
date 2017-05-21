//做了以大写字母开头的feature
#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include <string>
#include<sstream>
#include<stdlib.h>
#include<time.h>

using namespace std;

//错误处理类
class file_not_exist{};
class num_too_large{};
class num_too_small{};
//map生成，参数是fstream 文件和 要操作的map以及key的文件数
void map_generator(int num,fstream& file, vector<string>& word_collection,map<vector<string>, vector<string>>& collections, map<vector<string>, vector<string>>& head_collections, map<vector<string>, vector<string>>& end_collections);
//随机数生成，生成num1,num2之间的一个随机数，包括num1和num2
int random(int num1, int num2);
//要输出的字符串生成，参数就是collections作为map和字符串长度num
string generate_text(int num, map<vector<string>, vector<string>>& collections, map<vector<string>, vector<string>>& head_collections, map<vector<string>, vector<string>>& end_collections);

int main()
{
	while (true)
	{
		string filename;
		fstream file;
		int num_of_grams = 0;
		map<vector<string>, vector<string>> collections;
		//设置随机数种子
		srand((unsigned int)(time(NULL)));
		try
		{
			//文件输入
			cout << "Input file name? (Enter q to quit)";
			getline(cin, filename);			
			if (filename == "")return 0;
			file.open(filename);
			if (!file.is_open()) throw file_not_exist();
			string word;
			//word_collection存储的是file里面所有的word
			vector <string> word_collection;
			//写入word_collection
			while (file >> word)
			{
				word_collection.push_back(word);
			}

			while (true)
			{
				map<vector<string>, vector<string>>head_collections;
				map<vector<string>, vector<string>>end_collections;
				try
				{
					cout << "Value of N? ";
					cin >> num_of_grams;
					if (num_of_grams < 2) throw num_too_small();
					if (num_of_grams > word_collection.size()) throw num_too_large();
					//接下去是生成部分
					//test done，生成map
					map_generator(num_of_grams, file,word_collection, collections,head_collections,end_collections);
					while (true)
					{
						//生成输出字符串
						cout << endl << "# of random words to generate (0 to quit)?  ";
						int num_to_generate = 0;					
						cin >> num_to_generate;
						if (num_to_generate == 0)
						{
							cout << endl << "Exiting.";
							return 0;
						}
						cout << generate_text(num_to_generate, collections,head_collections,end_collections) << endl;
					}
				}
				catch (num_too_small) { cerr << "the value of N must be greater than 1. " << endl; }
				catch (num_too_large) { cerr << "the value of N should not be greater than the number of words in file" << endl; }
			}
		}
		catch (file_not_exist) { cerr << "the file cannot be found, try again."<<endl; }	
		}	
	return 0;
}

//map的生成函数，num是输入的N的value
void map_generator(int num, fstream & file, vector<string>& word_collection,map<vector<string>, vector<string>>& collections, map<vector<string>, vector<string>>& head_collections, map<vector<string>, vector<string>>& end_collections)
{
	num = num - 1;
	for (int i = 0; i < word_collection.size()+1; i++)
	{
		vector<string>key;
		//写入key值,在使用下标运算时，采用%word_collection.size()使得可以首尾套起来
		vector<string>head_key;
		vector<string>end_key;
		int size = word_collection[i].size();
		//如果单词是以大写字母开头，就存一份到head中间去
		if (word_collection[i][0] >= 'A'&&word_collection[i][0] <= 'Z')
		{
			for (int k = i; k < i + num; k++)
			{
				head_key.push_back(word_collection[k%word_collection.size()]);
			}
			head_collections[head_key].push_back(word_collection[(i + num) % word_collection.size()]);
		}

		//如果以 . ! ?结尾，写一份存入到end中
		if (word_collection[i][size - 1] == '.' || word_collection[i][size - 1] == '!' || word_collection[i][size - 1] == '?')
		{
			for (int k = i; k < i + num; k++)
			{
				end_key.push_back(word_collection[k%word_collection.size()]);
			}
			end_collections[end_key].push_back(word_collection[(i + num) % word_collection.size()]);
		}

		for (int k = i; k < i + num; k++)
		{			
			key.push_back(word_collection[k%word_collection.size()]);
		}
		//写入map,注意要首尾相连
		collections[key] .push_back(word_collection[(i+num)%word_collection.size()]);
	}	
}

//随机数生成函数，生成num1和num2之间的随机数，包括num1和num2
int random(int num1, int num2)
{
	//num1 和 num2 大小无所谓的
	if (num1 == num2) return num1;
	if (num1 > num2)
	{
		int tmp = num1;
		num1 = num2;
		num2 = tmp;
	}	
	return (rand()%(num2-num1+1))+num1;
}

//生成输出文本，num是需要生成的文本的数量
string generate_text(int num, map<vector<string>, vector<string>>& collections, map<vector<string>, vector<string>>& head_collections, map<vector<string>, vector<string>>& end_collections)
{
	string result = "";
	//count记录已经加入字符串的长度
	int count = 0;
	int size_of_key = 0;
	vector<string>tmp_key;
	//先从head_collection里面随机挑选一个作为生成字符串的开头
	//pos指向的是要创建的result的刚开始几个字符的来源的那个key
	map<vector<string>, vector<string>>::iterator pos = head_collections.begin();
	int random_num = random(0, head_collections.size() - 1);
	//使pos指向随机的位置
	for (int i = 0; i < random_num-1; i++)pos++;
	//tmp_key 即是随机访问的key
	tmp_key = pos->first;
	size_of_key = (pos->first).size();
	//先将tmp_key中的内容写入到string中
	for (auto word : tmp_key)
	{
		result = result + word + " ";
		count++;
		if (count >= num)
		{
			result += '.';
			return result;
		}
	}

	//写入字符串中间部分
	while (count < num)
	{
		//随机读取的key对应的值中的 一个值		
		string add_to_key = "";
		//add_to_key中存的是来自于key所对应的若干个值中的随机的一个
		int range = collections[tmp_key].size()-1;
		add_to_key = (collections[tmp_key])[random(0, range)];

		//如果选到的word还是以大写字母开头那就重新选择一个
		while (add_to_key[0]>='A'&&add_to_key[0]<='Z'&&range>0)
			add_to_key = (collections[tmp_key])[random(0, range)];
		if (count < num)
		{
			result = result + add_to_key + " ";
			count++;
		}	
		else return result+'.';
		//对tmp_key的后续处理,删除第一个元素，加上add_to_key
		tmp_key.erase(tmp_key.cbegin());
		tmp_key.push_back(add_to_key);
	}
	if (count < num)
	{
		int end_range = end_collections[tmp_key].size() - 1;
		if (end_range != -1)result = result + " " + end_collections[tmp_key][random(0, end_range)];
		else
		{
			int range = collections[tmp_key].size();
			result = result + " " + collections[tmp_key][random(0, range)] + '.';
		}
	}
	else return result + '.';	
	return result;	
}
