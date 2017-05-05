#include<stack>
#include<queue>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>

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
					//������Ӧ���Ǿ��崦����̣���������֮���ladder��������������һ��ѭ��
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
