#include<iostream>
#include<string>
#include<deque>
#include<vector>
#include<map>
#include<algorithm>
#include<fstream>
#include<time.h>
#include<numeric>
using namespace std;

//�ݽ��������̹���ϵͳ�������-��������
//ѧУ����һ���ݽ�����������12���˲μӣ����������֣���һ��Ϊ��̭�����ڶ���Ϊ����
//ÿ��ѡ�ֶ��ж�Ӧ�ı�ţ���10001-10012
//������ʽ�����������ÿ��6��
//��һ�ַ�Ϊ����С�飬���尴ѡ�ֱ�Ž��г�ǩ���˳������ݽ�
//ʮ����ί�ֱ��ÿ��ѡ�ִ�֣�ȥ����߷ֺ���ͷ֣����ƽ����Ϊ����ѡ�ֵĳɼ�
//��С���ݽ������̭����������������ѡ�֣�ǰ����������������һ�ֱ���
//�ڶ���Ϊ������ǰ����ʤ��
//ÿ�ֱ���������Ҫ��ʾ����ѡ�ֵ���Ϣ

//�����ܣ�
//��ʼ�ݽ����������������������̣�ÿ�������׶���Ҫ���û�һ����ʾ���û���������������һ���׶�
//�鿴�����¼���鿴֮ǰ����ǰ���������ÿ�α��������¼���ļ��У��ļ���.csv��׺������
//��ձ�����¼�����ļ����������
//�˳��������򣺿����˳���ǰ����



//�ݽ�ѡ����
class Speaker {
public:
	string m_Name;
	double m_Score[2];//��������ֵ�ƽ����
public:
	Speaker() {

	}
	Speaker(string name) {
		this->m_Name = name;
		this->m_Score[0] = 0;
		this->m_Score[1] = 0;
	}
};

//�ݽ�ϵͳ������
class SpeechManager {
public:
	SpeechManager();
	void menu();
	void startSpeechRace();
	void viewPastRecords();
	void clearAll();
	void createSpeaker();
	void saveRecord();
	~SpeechManager();
public:
	deque<int> v1;//����ѡ���������12��
	deque<int> v2;//��һ�ֽ�������6��
	vector<int> victory;//ʤ��ǰ3������
	map<int, Speaker> m_Speaker;//��ű�ż���Ӧ��ѡ��
	int m_Index;//�����ִ�
};

//SpeechManager���캯��
SpeechManager::SpeechManager() {
	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m_Speaker.clear();
	this->m_Index = 1;
	this->createSpeaker();//����ѡ��
}

//�˵�����
void SpeechManager::menu() {
	cout << "***************************************" << endl;
	cout << "***********��ӭ�μ��ݽ�������**********" << endl;
	cout << "************1.��ʼ�ݽ�����*************" << endl;
	cout << "************2.�鿴�����¼*************" << endl;
	cout << "************3.��ձ�����¼*************" << endl;
	cout << "************0.�˳���������*************" << endl;
	cout << "***************************************" << endl;
}

//��ʼ�ݽ���������
void SpeechManager::startSpeechRace() {
	cout << "��<<" << this->m_Index << ">>�ֱ���ѡ�����ڳ�ǩ" << endl;
	random_shuffle(this->v1.begin(), this->v1.end());
	cout << "----------------------------------------" << endl;
	cout << "��ǩ���ݽ�˳�����£�" << endl;
	for (deque<int>::iterator it = this->v1.begin(); it != this->v1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
	system("pause");
	cout << "------------��<<" << this->m_Index << ">>�ֱ�����ʽ��ʼ��------------" << endl;
	multimap<double, int, greater<double>> groupScore1;//key���� valuea��� �洢��һ��
	multimap<double, int, greater<double>> groupScore2;//key���� valuea���  �洢�ڶ���
	for (int i = 0; i < v1.size();i++) {
		deque<double> temp;
		temp.clear();
		for (int i = 0; i < 10;i++) {//ʮ����ί�Ե�һ����
			double score = (rand() % 401 + 600)/10.f;//�������60-100��С��
			temp.push_back(score);
		}
		sort(temp.begin(),temp.end());//��С��������
		temp.pop_back();//ȥ����߷�
		temp.pop_front();//ȥ����ͷ�
		double avg=accumulate(temp.begin(),temp.end(),0.0f)/8;//��ȡƽ����
		map<int,Speaker>::iterator pos=this->m_Speaker.find(this->v1[i]);
		(*pos).second.m_Score[0] = avg;
		if (i<=5) {
			groupScore1.insert(make_pair(avg, v1[i]));
		}
		else {
			groupScore2.insert(make_pair(avg, v1[i]));
		}
	}
	cout << "��һС��������Σ�" << endl;
	int count = 0;
	for (multimap<double, int, greater<double>>::iterator it2 = groupScore1.begin(); it2 != groupScore1.end(); it2++) {
		cout << "��ţ�" << (*it2).second << "  ������" << (*(this->m_Speaker.find((*it2).second))).second.m_Name << "  �ɼ���" << (*it2).first << endl;
		if (count < 3) {
			this->v2.push_back((*it2).second);//��һ��ȡǰ������v2
		}
		count++;
	}
	cout << "�ڶ�С��������Σ�" << endl;
	count = 0;
	for (multimap<double, int, greater<double>>::iterator it3 = groupScore2.begin(); it3 != groupScore2.end(); it3++) {
		cout << "��ţ�" << (*it3).second << "  ������" << (*(this->m_Speaker.find((*it3).second))).second.m_Name << "  �ɼ���" << (*it3).first << endl;
		if (count < 3) {
			this->v2.push_back((*it3).second);//��һ��ȡǰ������v2
		}
		count++;
	}
	cout << "------------��<<" << this->m_Index << ">>�ֱ�����ϣ�------------" << endl;
	system("pause");
	cout << "------------��<<" << this->m_Index << ">>�ֽ���ѡ����Ϣ���£�------------" << endl;
	for (deque<int>::iterator it4 = this->v2.begin(); it4 != v2.end(); it4++) {
		cout << "��ţ�" << *it4 << "  ������" << (*(this->m_Speaker.find(*it4))).second.m_Name << "  �ɼ���" << (*(this->m_Speaker.find(*it4))).second.m_Score[0] << endl;
	}
	this->m_Index = 2;
	system("pause");
	system("cls");
	cout << "��<<" << this->m_Index << ">>�ֱ���ѡ�����ڳ�ǩ" << endl;
	random_shuffle(this->v2.begin(), this->v2.end());
	cout << "----------------------------------------" << endl;
	cout << "��ǩ���ݽ�˳�����£�" << endl;
	for (deque<int>::iterator it = this->v2.begin(); it != this->v2.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
	system("pause");
	cout << "------------��<<" << this->m_Index << ">>�ֱ�����ʽ��ʼ��------------" << endl;
	multimap<double, int, greater<double>> groupScore3;//key���� valuea��� �洢��һ��
	for (int i = 0; i < v2.size(); i++) {
		deque<double> temp;
		temp.clear();
		for (int i = 0; i < 10; i++) {//ʮ����ί�Ե�һ������
			double score = (rand() % 401 + 600) / 10.f;//�������60-100��С��
			temp.push_back(score);
		}
		sort(temp.begin(), temp.end());//��С��������
		temp.pop_back();//ȥ����߷�
		temp.pop_front();//ȥ����ͷ�
		double avg = accumulate(temp.begin(), temp.end(), 0.0f) / 8;//��ȡƽ����
		map<int, Speaker>::iterator pos = this->m_Speaker.find(this->v2[i]);
		(*pos).second.m_Score[1] = avg;
		groupScore3.insert(make_pair(avg, v2[i]));
	}
	cout << "���ձ������Σ�" << endl;
	count = 0;
	for (multimap<double, int, greater<double>>::iterator it5 = groupScore3.begin(); it5 != groupScore3.end(); it5++) {
		cout << "��ţ�" << (*it5).second << "  ������" << (*(this->m_Speaker.find((*it5).second))).second.m_Name << "  �ɼ���" << (*it5).first << endl;
		if (count < 3) {
			this->victory.push_back((*it5).second);//��һ��ȡǰ������victory
		}
		count++;
	}
	
	cout << "------------��<<" << this->m_Index << ">>�ֱ�����ϣ�------------" << endl;
	system("pause");
	cout << "------------�ھ�ѡ����Ϣ���£�------------" << endl;
	for (vector<int>::iterator it6 = this->victory.begin(); it6 != victory.end(); it6++) {
		cout << "��ţ�" << *it6 << "  ������" << (*(this->m_Speaker.find(*it6))).second.m_Name << "  �ɼ���" << (*(this->m_Speaker.find(*it6))).second.m_Score[1] << endl;
	}
	cout << "����������" << endl;
	this->saveRecord();
	system("pause");
	system("cls");

}

//�鿴�����¼����
void SpeechManager::viewPastRecords() {
	ifstream ifs;
	ifs.open("speech.csv", ios::in);//��ȡ�ļ�
	if (!ifs.is_open()) {
		cout << "�ļ�������" << endl;
		ifs.close();
		return;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "�ļ�Ϊ�գ�" << endl;
		ifs.close();
		return;
	}
	//�ļ���Ϊ��
	ifs.putback(ch);//��ȡ�ĵ����ַ��Ż�ȥ
	string data;
	int index = 0;
	multimap<int, vector<string>> outmap;//��������¼map����
	outmap.clear();
	while (ifs>>data) {//һ���ж�
		string data1 = data;
		string subData="";
		int findIndex=0;
		int pos = -1;
		int num = 0;
		vector<string> v;
		while (true) {
			pos = pos + num + 1;
			findIndex = data1.find(",", pos);
			num = findIndex - pos;
			if (findIndex!=-1) {
				subData = data1.substr(pos, num); 
				v.push_back(subData);
			}
			else {
				break;
			}
			
		}
		outmap.insert(make_pair(index, v));
		index++;
	}
	int num1 = 1;
	for (multimap<int, vector<string>>::iterator it = outmap.begin(); it != outmap.end();it++) {
		cout << "��" << num1<< "�������¼���£�" << endl;
		cout << "�ھ���" << endl;
		cout << "ѡ�ֱ�ţ�" << (*it).second[0] << "  ������" << (*it).second[1] << "  ������" <<(*it).second[2] << endl;
		cout << "�Ǿ���" << endl;
		cout << "ѡ�ֱ�ţ�" << (*it).second[3] << "  ������" << (*it).second[4] << "  ������" << (*it).second[5] << endl;
		cout << "������" << endl;
		cout << "ѡ�ֱ�ţ�" << (*it).second[6] << "  ������" << (*it).second[7] << "  ������" << (*it).second[8] << endl;
		num1++;
	}
	

}

//��ձ�����¼����
void SpeechManager::clearAll() {
	cout << "ȷ����գ�" << endl;
	cout << "1��ȷ��" << endl;
	cout << "2������" << endl;
	int select;
	cin >> select;
	if (select == 1) {
		//ɾ��ԭ���ļ��������µĿ��ļ�
		ofstream ofs;
		ofs.open("speech.csv",ios::trunc);//����ļ�����ɾ�����ش�
		ofs.close();
	}
}

//����ѡ�ֺ���
void SpeechManager::createSpeaker() {
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.size();i++) {
		string nameHead = "ѡ��";
		string name=nameHead+nameSeed[i];
		Speaker speaker(name);
		this->v1.push_back(10001 + i);
		this->m_Speaker.insert(make_pair(10001+i,speaker));
	}
	
}

//�����¼����
void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("speech.csv",ios::out|ios::app);//��׷�ӵķ�ʽд�ļ�
	//��ÿ��������д���ļ���
	for (vector<int>::iterator it = this->victory.begin(); it != this->victory.end();it++) {
		ofs << *it << "," << this->m_Speaker[*it].m_Name << "," << this->m_Speaker[*it].m_Score[1] << "," ;
	}
	ofs << endl;
	ofs.close();
	cout << "��¼�ѱ���" << endl;
	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m_Index = 1;
	this->createSpeaker();
}


//SpeechManager��������
SpeechManager::~SpeechManager() {

}

int main() 
{
	srand((unsigned int)time(NULL));//�������������
	SpeechManager sm;
	while (true) {
		sm.menu();
		cout << "����������ѡ��" << endl;
		int choice;
		cin >> choice;
		switch (choice) {
		case 1:
			sm.startSpeechRace();
			break;
		case 2:
			sm.viewPastRecords();
			system("pause");
			system("cls");
			break;
		case 3:
			sm.clearAll();
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "������������0-3��ѡ��" << endl;
			system("pause");
			system("cls");
			break;
		}
	}

}