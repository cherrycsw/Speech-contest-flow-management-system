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

//演讲比赛流程管理系统需求分析-比赛规则：
//学校举行一场演讲比赛，共有12个人参加，比赛共两轮，第一轮为淘汰赛，第二轮为决赛
//每名选手都有对应的编号，如10001-10012
//比赛方式：分组比赛，每组6人
//第一轮分为两个小组，整体按选手编号进行抽签后的顺序进行演讲
//十个评委分别给每名选手打分，去除最高分和最低分，求的平均分为本轮选手的成绩
//当小组演讲完后，淘汰组内排名最后的三名选手，前三名晋级，进入下一轮比赛
//第二轮为决赛，前三名胜出
//每轮比赛过后需要显示晋级选手的信息

//程序功能：
//开始演讲比赛：完成整届比赛的流程，每个比赛阶段需要给用户一个提示，用户按任意键后继续下一个阶段
//查看往届记录：查看之前比赛前三名结果，每次比赛都会记录到文件中，文件用.csv后缀名保存
//清空比赛记录：将文件中数据清空
//退出比赛程序：可以退出当前程序



//演讲选手类
class Speaker {
public:
	string m_Name;
	double m_Score[2];//最多有两轮的平均分
public:
	Speaker() {

	}
	Speaker(string name) {
		this->m_Name = name;
		this->m_Score[0] = 0;
		this->m_Score[1] = 0;
	}
};

//演讲系统管理类
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
	deque<int> v1;//比赛选手容器存放12人
	deque<int> v2;//第一轮晋级容器6人
	vector<int> victory;//胜利前3名容器
	map<int, Speaker> m_Speaker;//存放编号及对应的选手
	int m_Index;//比赛轮次
};

//SpeechManager构造函数
SpeechManager::SpeechManager() {
	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m_Speaker.clear();
	this->m_Index = 1;
	this->createSpeaker();//创建选手
}

//菜单函数
void SpeechManager::menu() {
	cout << "***************************************" << endl;
	cout << "***********欢迎参加演讲比赛！**********" << endl;
	cout << "************1.开始演讲比赛*************" << endl;
	cout << "************2.查看往届记录*************" << endl;
	cout << "************3.清空比赛记录*************" << endl;
	cout << "************0.退出比赛程序*************" << endl;
	cout << "***************************************" << endl;
}

//开始演讲比赛函数
void SpeechManager::startSpeechRace() {
	cout << "第<<" << this->m_Index << ">>轮比赛选手正在抽签" << endl;
	random_shuffle(this->v1.begin(), this->v1.end());
	cout << "----------------------------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	for (deque<int>::iterator it = this->v1.begin(); it != this->v1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
	system("pause");
	cout << "------------第<<" << this->m_Index << ">>轮比赛正式开始！------------" << endl;
	multimap<double, int, greater<double>> groupScore1;//key分数 valuea编号 存储第一组
	multimap<double, int, greater<double>> groupScore2;//key分数 valuea编号  存储第二组
	for (int i = 0; i < v1.size();i++) {
		deque<double> temp;
		temp.clear();
		for (int i = 0; i < 10;i++) {//十名评委对第一组打分
			double score = (rand() % 401 + 600)/10.f;//随机生成60-100的小数
			temp.push_back(score);
		}
		sort(temp.begin(),temp.end());//从小到大排序
		temp.pop_back();//去掉最高分
		temp.pop_front();//去掉最低分
		double avg=accumulate(temp.begin(),temp.end(),0.0f)/8;//获取平均分
		map<int,Speaker>::iterator pos=this->m_Speaker.find(this->v1[i]);
		(*pos).second.m_Score[0] = avg;
		if (i<=5) {
			groupScore1.insert(make_pair(avg, v1[i]));
		}
		else {
			groupScore2.insert(make_pair(avg, v1[i]));
		}
	}
	cout << "第一小组比赛名次：" << endl;
	int count = 0;
	for (multimap<double, int, greater<double>>::iterator it2 = groupScore1.begin(); it2 != groupScore1.end(); it2++) {
		cout << "编号：" << (*it2).second << "  姓名：" << (*(this->m_Speaker.find((*it2).second))).second.m_Name << "  成绩：" << (*it2).first << endl;
		if (count < 3) {
			this->v2.push_back((*it2).second);//第一组取前三名到v2
		}
		count++;
	}
	cout << "第二小组比赛名次：" << endl;
	count = 0;
	for (multimap<double, int, greater<double>>::iterator it3 = groupScore2.begin(); it3 != groupScore2.end(); it3++) {
		cout << "编号：" << (*it3).second << "  姓名：" << (*(this->m_Speaker.find((*it3).second))).second.m_Name << "  成绩：" << (*it3).first << endl;
		if (count < 3) {
			this->v2.push_back((*it3).second);//第一组取前三名到v2
		}
		count++;
	}
	cout << "------------第<<" << this->m_Index << ">>轮比赛完毕！------------" << endl;
	system("pause");
	cout << "------------第<<" << this->m_Index << ">>轮晋级选手信息如下：------------" << endl;
	for (deque<int>::iterator it4 = this->v2.begin(); it4 != v2.end(); it4++) {
		cout << "编号：" << *it4 << "  姓名：" << (*(this->m_Speaker.find(*it4))).second.m_Name << "  成绩：" << (*(this->m_Speaker.find(*it4))).second.m_Score[0] << endl;
	}
	this->m_Index = 2;
	system("pause");
	system("cls");
	cout << "第<<" << this->m_Index << ">>轮比赛选手正在抽签" << endl;
	random_shuffle(this->v2.begin(), this->v2.end());
	cout << "----------------------------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	for (deque<int>::iterator it = this->v2.begin(); it != this->v2.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "----------------------------------------" << endl;
	system("pause");
	cout << "------------第<<" << this->m_Index << ">>轮比赛正式开始！------------" << endl;
	multimap<double, int, greater<double>> groupScore3;//key分数 valuea编号 存储第一组
	for (int i = 0; i < v2.size(); i++) {
		deque<double> temp;
		temp.clear();
		for (int i = 0; i < 10; i++) {//十名评委对第一二组打分
			double score = (rand() % 401 + 600) / 10.f;//随机生成60-100的小数
			temp.push_back(score);
		}
		sort(temp.begin(), temp.end());//从小到大排序
		temp.pop_back();//去掉最高分
		temp.pop_front();//去掉最低分
		double avg = accumulate(temp.begin(), temp.end(), 0.0f) / 8;//获取平均分
		map<int, Speaker>::iterator pos = this->m_Speaker.find(this->v2[i]);
		(*pos).second.m_Score[1] = avg;
		groupScore3.insert(make_pair(avg, v2[i]));
	}
	cout << "最终比赛名次：" << endl;
	count = 0;
	for (multimap<double, int, greater<double>>::iterator it5 = groupScore3.begin(); it5 != groupScore3.end(); it5++) {
		cout << "编号：" << (*it5).second << "  姓名：" << (*(this->m_Speaker.find((*it5).second))).second.m_Name << "  成绩：" << (*it5).first << endl;
		if (count < 3) {
			this->victory.push_back((*it5).second);//第一组取前三名到victory
		}
		count++;
	}
	
	cout << "------------第<<" << this->m_Index << ">>轮比赛完毕！------------" << endl;
	system("pause");
	cout << "------------冠军选手信息如下：------------" << endl;
	for (vector<int>::iterator it6 = this->victory.begin(); it6 != victory.end(); it6++) {
		cout << "编号：" << *it6 << "  姓名：" << (*(this->m_Speaker.find(*it6))).second.m_Name << "  成绩：" << (*(this->m_Speaker.find(*it6))).second.m_Score[1] << endl;
	}
	cout << "比赛结束！" << endl;
	this->saveRecord();
	system("pause");
	system("cls");

}

//查看往届记录函数
void SpeechManager::viewPastRecords() {
	ifstream ifs;
	ifs.open("speech.csv", ios::in);//读取文件
	if (!ifs.is_open()) {
		cout << "文件不存在" << endl;
		ifs.close();
		return;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "文件为空！" << endl;
		ifs.close();
		return;
	}
	//文件不为空
	ifs.putback(ch);//读取的单个字符放回去
	string data;
	int index = 0;
	multimap<int, vector<string>> outmap;//存放往届记录map容器
	outmap.clear();
	while (ifs>>data) {//一行行读
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
		cout << "第" << num1<< "届比赛记录如下：" << endl;
		cout << "冠军：" << endl;
		cout << "选手编号：" << (*it).second[0] << "  姓名：" << (*it).second[1] << "  分数：" <<(*it).second[2] << endl;
		cout << "亚军：" << endl;
		cout << "选手编号：" << (*it).second[3] << "  姓名：" << (*it).second[4] << "  分数：" << (*it).second[5] << endl;
		cout << "季军：" << endl;
		cout << "选手编号：" << (*it).second[6] << "  姓名：" << (*it).second[7] << "  分数：" << (*it).second[8] << endl;
		num1++;
	}
	

}

//清空比赛记录函数
void SpeechManager::clearAll() {
	cout << "确认清空？" << endl;
	cout << "1、确认" << endl;
	cout << "2、返回" << endl;
	int select;
	cin >> select;
	if (select == 1) {
		//删除原有文件，生成新的空文件
		ofstream ofs;
		ofs.open("speech.csv",ios::trunc);//如果文件存在删除并重创
		ofs.close();
	}
}

//创建选手函数
void SpeechManager::createSpeaker() {
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.size();i++) {
		string nameHead = "选手";
		string name=nameHead+nameSeed[i];
		Speaker speaker(name);
		this->v1.push_back(10001 + i);
		this->m_Speaker.insert(make_pair(10001+i,speaker));
	}
	
}

//保存记录函数
void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("speech.csv",ios::out|ios::app);//以追加的方式写文件
	//将每个人数据写入文件中
	for (vector<int>::iterator it = this->victory.begin(); it != this->victory.end();it++) {
		ofs << *it << "," << this->m_Speaker[*it].m_Name << "," << this->m_Speaker[*it].m_Score[1] << "," ;
	}
	ofs << endl;
	ofs.close();
	cout << "记录已保存" << endl;
	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m_Index = 1;
	this->createSpeaker();
}


//SpeechManager析构函数
SpeechManager::~SpeechManager() {

}

int main() 
{
	srand((unsigned int)time(NULL));//产生随机数种子
	SpeechManager sm;
	while (true) {
		sm.menu();
		cout << "请输入您的选择：" << endl;
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
			cout << "输入有误！请在0-3中选择" << endl;
			system("pause");
			system("cls");
			break;
		}
	}

}