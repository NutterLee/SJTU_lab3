#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include <string>
#include<sstream>
#include<stdlib.h>
#include<time.h>

using namespace std;

//��������
class file_not_exist{};
class num_too_large{};
class num_too_small{};
//map���ɣ�������fstream �ļ��� Ҫ������map�Լ�key���ļ���
void map_generator(int num,fstream& file, vector<string>& word_collection,map<vector<string>, vector<string>>& collections);
//��������ɣ�����num1,num2֮���һ�������������num1��num2
int random(int num1, int num2);
//Ҫ������ַ������ɣ���������collections��Ϊmap���ַ�������num
string generate_text(int num, map<vector<string>, vector<string>>& collections);

int main()
{
	while (true)
	{
		string filename;
		fstream file;
		int num_of_grams = 0;
		map<vector<string>, vector<string>> collections;
		//�������������
		srand((unsigned int)(time(NULL)));
		try
		{
			//�ļ�����
			cout << "Input file name? (Enter q to quit)";
			cin >> filename;
			if (filename == "q")return 0;
			file.open(filename);
			if (!file.is_open()) throw file_not_exist();
			string word;
			//word_collection�洢����file�������е�word
			vector <string> word_collection;
			//д��word_collection
			while (file >> word)
			{
				word_collection.push_back(word);
			}

			while (true)
			{
				try
				{
					cout << "Value of N? ";
					cin >> num_of_grams;
					if (num_of_grams < 2) throw num_too_small();
					if (num_of_grams > word_collection.size()) throw num_too_large();
					//����ȥ�����ɲ���
					//test done������map
					map_generator(num_of_grams, file,word_collection, collections);
					while (true)
					{
						//��������ַ���
						cout << endl << "# of random words to generate (0 to quit)?  ";
						int num_to_generate = 0;
						cin >> num_to_generate;
						if (num_to_generate == 0)
						{
							cout << endl << "Exiting.";
							return 0;
						}
						cout << "..." << generate_text(num_to_generate, collections) << "..." << endl;
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

void map_generator(int num, fstream & file, vector<string>& word_collection,map<vector<string>, vector<string>>& collections)
{
	num = num - 1;

	for (int i = 0; i < word_collection.size()+1; i++)
	{
		vector<string>key;
		//д��keyֵ,��ʹ���±�����ʱ������%word_collection.size()ʹ�ÿ�����β������
		for (int k = i; k < i + num; k++)
		{			
			key.push_back(word_collection[k%word_collection.size()]);
		}
		//д��map,ע��Ҫ��β����
		collections[key] .push_back(word_collection[(i+num)%word_collection.size()]);
	}	
}

int random(int num1, int num2)
{
	//num1 �� num2 ��С����ν��
	if (num1 == num2) return num1;
	if (num1 > num2)
	{
		int tmp = num1;
		num1 = num2;
		num2 = tmp;
	}	
	return (rand()%(num2-num1+1))+num1;
}

string generate_text(int num, map<vector<string>, vector<string>>& collections)
{
	string result = "";
	int count = 0;
	vector<string>tmp_key;
	//posָ�����Ҫ������result�ĸտ�ʼ�����ַ�����Դ���Ǹ�key
	map<vector<string>, vector<string>>::iterator pos = collections.begin();
	int random_num = random(0, collections.size() - 1);
	//ʹposָ�������λ��
	for (int i = 0; i < random_num; i++)pos++;
	//tmp_key ����������ʵ�key
	tmp_key = pos->first;
	for (auto word : tmp_key)
	{
		result = result + word + " ";
		count++;
		if (count >= num) return result;
	}
	while (count < num)
	{
		//�����ȡ��key��Ӧ��ֵ�е� һ��ֵ		
		string add_to_key = "";
		//add_to_key�д����������key����Ӧ�����ɸ�ֵ�е������һ��
		int range = collections[tmp_key].size()-1;
		add_to_key = (collections[tmp_key])[random(0, range)];
		if (count < num)
		{
			result = result + add_to_key + " ";
			count++;
		}		
		//��tmp_key�ĺ�������,ɾ����һ��Ԫ�أ�����add_to_key
		tmp_key.erase(tmp_key.cbegin());
		tmp_key.push_back(add_to_key);
	}
	return result;	
}
