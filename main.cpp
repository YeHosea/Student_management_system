#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstring>
#include <map>
#include <stack>
using namespace std;
struct MNode{
	struct SNode{		
		int flag;			//标志是否为学生
		string xuexiao;		//学校
		string xueyuan;		//学院
		string zhuanye;		//专业
		string banji;		//班级
		string xuehao;		//学号
		string name;		//姓名
		string xingbie;		//性别
		string ywchengji;		//语文成绩
		string sxchengji;		//数学成绩
	}info;
	vector<MNode*>children;
	MNode *parent;
}node;
int posx,posy,zongy;
int id;//增加的学生编号
int addflag=0;	//addflag=1 代表已存在，无需添加 addflag=2 代表添加成功
int modifyflag=0;
int queryflag=0;
map<string,int>mapxuexiao;
map<string,int>mapxueyuan;
map<string,int>mapzhuanye;
map<string,int>mapbanji;
map<string,int>mapxuehao;
map<string,int>mapname;

void gotoxy(short x, short y)
{
	_COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
string MNodeinfo(MNode *node){
	if(node->info.name!="")			return node->info.name;
	else if(node->info.xuexiao!="")	return node->info.xuexiao;
	else if(node->info.xueyuan!="")	return node->info.xueyuan;
	else if(node->info.zhuanye!="")	return node->info.zhuanye;
	else if(node->info.banji!="")	return node->info.banji;
}
class MTree{
private:
	MNode *root;
public:
	void init(MNode *root);
	void insertSNode(MNode *node,int depth);
	void putChild_M(MNode *node,MNode *parent);
	void putChild_S(MNode *node,MNode *parent);
	void putChildren(vector<MNode*> nodes, MNode *parent);
	MNode *findMNode(MNode *root,string e,int flag,int depth);
	void queryName(MNode *node,MNode *e,int depth,int predepth);
	void deleteName(MNode *root,string e);
	void modifyName(MNode *root,string e,int f,string g);
}tree;
stack<MNode*>restack;
int vis[10];
void printbian(int posx){
	gotoxy(0+posx,1);
	for(int i=1;i<=31;i++)cout<<"*";
	for(int j=2;j<=zongy+1+2;j++){
		gotoxy(0+posx,j);cout<<"*";
		gotoxy(30+posx,j);cout<<"*";
	}
	gotoxy(0+posx,zongy+1+3);
	for(int i=1;i<=31;i++)cout<<"*";
}
void printtree(MNode *root,int ceng){
	for(int i=ceng;i<=9;i++)vis[i]=0;
	vector<MNode *>nodes=root->children;
	gotoxy(posx,posy);
	posy++;
	for(int i=1;i<=ceng-1;i++){
		if(i>=1&&vis[i-1]!=1){
			cout<<"│";
		}
		else cout<<" ";
		cout<<"   ";//3个空格
	}
	if(ceng!=0)cout<<"│─";//三个横线
	cout<<' '<<MNodeinfo(root)<<'\n';
	for(auto i:nodes){
		auto x=nodes.back();
		if(MNodeinfo(i)==MNodeinfo(x))vis[ceng]=1;
		printtree(i,ceng+1);
		vis[ceng]=0;
	}
}
void copyMNode(MNode *newroot,MNode *root){
	newroot->parent=root->parent;
	newroot->info=root->info;
	vector<MNode *>nodes=root->children;
	for(int i=0;i<nodes.size();i++){
		MNode *newMNode=new MNode;
		//newMNode->info=nodes[i]->info;
		//newMNode->parent=nodes[i]->parent;
		newroot->children.push_back(newMNode);
		copyMNode(newMNode,root->children[i]);
	}
}
//树初始化
void MTree::init(MNode *root) { this->root = root; }

//插入一个子节点
void MTree::putChild_M(MNode *node, MNode *parent) {
    parent->children.push_back(node);
    node->parent = parent;
}

//插入一个学生节点
void MTree::putChild_S(MNode *node, MNode *parent) {
    parent->children.push_back(node);
    node->parent = parent;
}

//插入一个深度为depth的节点
void MTree::insertSNode(MNode *node,int depth){
	MNode *s;
	addflag=0;//判断此节点是否已经存在
	if(depth>=1)s=findMNode(root,node->info.xuexiao,(depth==1)?1:0,1);
	if(depth>=2)s=findMNode(s,node->info.xueyuan,(depth==2)?1:0,2);
	if(depth>=3)s=findMNode(s,node->info.zhuanye,(depth==3)?1:0,3);
	if(depth>=4)s=findMNode(s,node->info.banji,(depth==4)?1:0,4);
	if(depth>=5)s=findMNode(s,node->info.name,(depth==5)?1:0,5);
	//else putChild_S(node->parent,s->parent);
}

//寻找学生对应信息，如果信息不存在，则新建一个节点
MNode *MTree::findMNode(MNode *root,string e,int flag,int depth){
	vector<MNode *> nodes = root->children;
	for(int i=0;i<nodes.size();i++){
		if(MNodeinfo(nodes[i])==e){	//找到已建立的节点
			if(flag==1)addflag=1;	//如果该节点在对应的深度找到相同名字的节点，则认为该节点已经存在
			return nodes[i];		//返回该存在的节点
		}
	}
	//该节点不存在，新建一个节点
	MNode *a=new MNode;
	switch(depth){//根据新建节点的深度，判断它的类型，标记该名称存在，并相应赋值
		case 1:a->info.xuexiao=e;	mapxuexiao[e]=1;	break;
		case 2:a->info.xueyuan=e;	mapxueyuan[e]=1;	break;
		case 3:a->info.zhuanye=e;	mapzhuanye[e]=1;	break;
		case 4:a->info.banji=e;		mapbanji[e]=1;		break;
		case 5:a->info.name=e;		mapname[e]=1;		break;
	}
	//将该节点插入到树中，并返回该节点
	putChild_M(a,root);
	return a;
}
void MTree::deleteName(MNode *root,string e){
	mapname[e]=0;
	vector<MNode *>nodes=root->children;
	if(nodes.size()==0){
		if(root->info.name==e){
			vector<MNode *>node=(root->parent)->children;
			for(auto i=node.begin();i!=node.end();){
				if((*i)->info.name==e){
					i=node.erase(i);
				}
				else{
					i++;
				}
			}
			(root->parent)->children=node;
			return;
		}
	}
	else{
		for(auto i:nodes){
			deleteName(i,e);
		}
	}
}
void MTree::modifyName(MNode *root,string e,int f,string g){
	if(modifyflag==1)return;
	vector<MNode *>nodes=root->children;
	MNode *newNode=new MNode;
	if(nodes.size()==0){
		if(root->info.name==e){
			newNode->info=root->info;
			switch(f){
				case 1:newNode->info.xuexiao=g;		break;
				case 2:newNode->info.xueyuan=g;		break;
				case 3:newNode->info.zhuanye=g;		break;
				case 4:newNode->info.banji=g;		break;
				case 5:newNode->info.xuehao=g;		break;
				case 6:newNode->info.xingbie=g;		break;
				case 7:newNode->info.ywchengji=g;	break;
				case 8:newNode->info.sxchengji=g;	break;
				case 9:{
					mapname[newNode->info.name]=0;
					newNode->info.name=g;			break;
				}
			}
		}
		tree.deleteName(root->parent,e);
		modifyflag=1;
		tree.insertSNode(newNode,5);
		return;
	}
	else{
		for(auto i:nodes){
			modifyName(i,e,f,g);
		}
	}
}

void MTree::queryName(MNode *root,MNode *e,int depth,int predepth){
	vector<MNode *>nodes=root->children;
	if(queryflag!=0)return;
	for(auto i:nodes){
		switch(predepth){
			case 1:{
				if(i->info.xuexiao==e->info.xuexiao){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"源目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询结果："<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 2:{
				if(i->info.xueyuan==e->info.xueyuan){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"源目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询结果："<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 3:{
				if(i->info.zhuanye==e->info.zhuanye){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"源目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询结果："<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 4:{
				if(i->info.banji==e->info.banji){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"源目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询结果："<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 5:{
				if(i->info.name==e->info.name){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"源目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询结果："<<endl;
						posy++;printtree(i,0);
						posy++;gotoxy(posx+1,posy);cout<<"姓名："<<i->info.name<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"学号："<<i->info.xuehao<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"性别："<<i->info.xingbie<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"语文成绩："<<i->info.ywchengji<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"数学成绩："<<i->info.sxchengji<<endl;
						printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else return;
				}
				break;
			}
		}
	}
	if(queryflag==1||queryflag==-1)return;
	switch(predepth){
		case 1:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询失败"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"查询的学校： ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<" 不存在";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 2:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询失败"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"查询的学院： ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan<<" 不存在";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 3:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询失败"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"查询的专业： ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan<<' '<<e->info.zhuanye<<" 不存在";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 4:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询失败"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"查询的班级： ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan;
			posy++;gotoxy(posx+1,posy);cout<<e->info.zhuanye<<' '<<e->info.banji<<" 不存在";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 5:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"查询失败"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"查询的姓名： ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan;
			posy++;gotoxy(posx+1,posy);cout<<e->info.zhuanye<<' '<<e->info.banji<<' '<<e->info.xuehao<<" 不存在";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
	}
}
int main(){
	id=7;
	MNode lastnode;
	node.parent=nullptr;
	node.info.name=" ";
	tree.init(&node);

	MNode stu[10];
	stu[1].info.flag=1;
	stu[1].info.xuexiao="浙江大学";			 stu[1].info.xueyuan="计算机";
	stu[1].info.zhuanye="计科";				stu[1].info.banji="1班";
	stu[1].info.xuehao="19015001";			stu[1].info.xingbie="男";
	stu[1].info.ywchengji="100";			stu[1].info.sxchengji="99";
	stu[1].info.name="何涛涛";				tree.insertSNode(&stu[1],5);

	stu[2].info.flag=1;
	stu[2].info.xuexiao="浙江大学";		stu[2].info.xueyuan="网络安全";
	stu[2].info.zhuanye="网安";			stu[2].info.banji="2班";
	stu[2].info.xuehao="19017002";		stu[2].info.xingbie="女";
	stu[2].info.ywchengji="30";			stu[2].info.sxchengji="79";
	stu[2].info.name="李桑";			tree.insertSNode(&stu[2],5);
	
	stu[3].info.flag=1;
	stu[3].info.xuexiao="浙江大学";		stu[3].info.xueyuan="网络安全";
	stu[3].info.zhuanye="信安";			stu[3].info.banji="3班";
	stu[3].info.xuehao="19015003";		stu[3].info.xingbie="女";
	stu[3].info.ywchengji="90";			stu[3].info.sxchengji="87";
	stu[3].info.name="兔兔";			tree.insertSNode(&stu[3],5);

	stu[4].info.flag=1;
	stu[4].info.xuexiao="杭电";			stu[4].info.xueyuan="人工智能";
	stu[4].info.zhuanye="机器人";		stu[4].info.banji="1班";
	stu[4].info.xuehao="19021001";		stu[4].info.xingbie="女";
	stu[4].info.ywchengji="95";			stu[4].info.sxchengji="67";
	stu[4].info.name="小王";			tree.insertSNode(&stu[4],5);

	stu[5].info.flag=1;
	stu[5].info.xuexiao="杭电";			stu[5].info.xueyuan="人工智能";
	stu[5].info.zhuanye="智能车";		stu[5].info.banji="1班";
	stu[5].info.xuehao="19021002";		stu[5].info.xingbie="男";
	stu[5].info.ywchengji="91";			stu[5].info.sxchengji="79";
	stu[5].info.name="小张";			tree.insertSNode(&stu[5],5);

	stu[6].info.flag=1;
	stu[6].info.xuexiao="浙江大学";			 stu[6].info.xueyuan="计算机";
	stu[6].info.zhuanye="计科";				stu[6].info.banji="1班";
	stu[6].info.xuehao="19017002";			stu[6].info.xingbie="男";
	stu[6].info.ywchengji="100";			stu[6].info.sxchengji="100";
	stu[6].info.name="小叶";				tree.insertSNode(&stu[6],5);

	stu[7].info.flag=1;
	stu[7].info.xuexiao="浙江大学";			 stu[7].info.xueyuan="计算机";
	stu[7].info.zhuanye="计科";				stu[7].info.banji="1班";
	stu[7].info.xuehao="19017003";			stu[7].info.xingbie="男";
	stu[7].info.ywchengji="80";				stu[7].info.sxchengji="70";
	stu[7].info.name="小皮";				tree.insertSNode(&stu[7],5);

	stu[8].info.flag=1;
	stu[8].info.xuexiao="清华大学";			 stu[8].info.xueyuan="计算机";
	stu[8].info.zhuanye="计科";				stu[8].info.banji="1班";
	stu[8].info.xuehao="19016001";			stu[8].info.xingbie="男";
	stu[8].info.ywchengji="91";				stu[8].info.sxchengji="90";
	stu[8].info.name="小马";				
	//tree.queryName(&node,"wang");
	//tree.queryName(&node,"li");
	system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
	while(1){
		gotoxy(1,zongy+7);cout<<" 请输入操作方式：1.添加 2.删除 3.修改 4.查询 5.更新当前目录 6.撤销 7.结束操作 : ";
		int x;
		while(cin>>x&&(x!=1&&x!=2&&x!=3&&x!=4&&x!=5&&x!=6&&x!=7)){
			cout<<" 输入不符合规则，请重新输入: ";
		}
		switch(x){
			case 1:{
				addshang:
				cout<<" 请输入添加的信息：1.学校 2.学院 3.专业 4.班级 5.自动生成一名新学生 6.手动输入一名新学生:";
				int addInfo;
				while(cin>>addInfo&&(addInfo!=1&&addInfo!=2&&addInfo!=3&&addInfo!=4&&addInfo!=5&&addInfo!=6&&addInfo!=7)){
					cout<<"输入不符合规则，请重新输入: ";
				}
				MNode *newnode=new MNode;
				addflag=0;
				switch(addInfo){//根据添加的信息种类不同而分类
					case 1:{
						cout<<" 请输入您要添加的学校(返回上一级操作输入re）:";
						cin>>newnode->info.xuexiao;
						if(newnode->info.xuexiao=="re")goto addshang;
						break;
					}
					case 2:{
						cout<<" 请输入您要添加的学院的所在学校(返回上一级操作输入re）:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" 请输入您要添加的学院(返回上一级操作输入re）：";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						break;
					}
					case 3:{
						cout<<" 请输入您要添加的专业的所在学校(返回上一级操作输入re）:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" 请输入您要添加的专业的所在学院(返回上一级操作输入re）:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" 请输入您要添加的专业(返回上一级操作输入re）：";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						break;
					}
					case 4:{
						cout<<" 请输入您要添加班级的所在学校(返回上一级操作输入re）:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" 请输入您要添加班级的所在学院(返回上一级操作输入re）:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" 请输入您要添加班级的所在专业(返回上一级操作输入re）:";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						cout<<" 请输入您要添加班级(返回上一级操作输入re）：";cin>>newnode->info.banji;if(newnode->info.banji=="re")goto addshang;
						break;
					}
					case 5:{
						newnode->info=stu[++id].info;
						break;
					}
					case 6:{
						newnode->info.flag=1;
						cout<<" 请输入您要添加学生的所在学校(返回上一级操作输入re）:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" 请输入您要添加学生的所在学院(返回上一级操作输入re）:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" 请输入您要添加学生的所在专业(返回上一级操作输入re）:";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						cout<<" 请输入您要添加学生的所在班级(返回上一级操作输入re）:";cin>>newnode->info.banji;if(newnode->info.banji=="re")goto addshang;
						cout<<" 请输入您要添加学生(返回上一级操作输入re）：";cin>>newnode->info.name;if(newnode->info.banji=="re")goto addshang;
					}
				};
				if(addflag==1){//该节点已经存在
					system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
					posx=40,posy=2;gotoxy(posx+1,posy);cout<<"该节点已经存在，无法添加"<<endl;posy++;printbian(39);zongy=posy;gotoxy(33,zongy/2);cout<<"--->";
					break;
				}

				//撤销操作
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"添加前"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
				tree.insertSNode(newnode,addInfo);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"添加后"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(39);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 2:{
				cout<<" 请输入删除学生姓名： ";//补：重名
				string deleteName;
				while(cin>>deleteName&&mapname[deleteName]==0){
					cout<<endl<<" 该学生不存在，请重新输入: ";
				}
				//撤销操作
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"修改前"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				tree.deleteName(&node,deleteName);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"修改后"<<endl;posy++;printbian(39);printtree(&node,0);zongy=posy;gotoxy(33,zongy/2);cout<<"--->";
				break;
			}	
			case 3:{
				cout<<" 请输入修改信息的学生姓名： ";
				string modifyName;
				while(cin>>modifyName){
					if(mapname[modifyName]==0){
						cout<<endl;
						cout<<" 该学生不存在，请重新输入: ";
					}
					else break;
				}
				cout<<" 请输入修改的信息：1.学校 2.学院 3.专业 4.班级 5.学号 6.性别 7.语文成绩 8.数学成绩 9.姓名： ";
				int modifyInfo;
				while(cin>>modifyInfo&&(modifyInfo!=1&&modifyInfo!=2&&modifyInfo!=3&&modifyInfo!=4&&modifyInfo!=5&&modifyInfo!=6&&modifyInfo!=7&&modifyInfo!=8&&modifyInfo!=9)){
					cout<<"输入不符合规则，请重新输入: ";
				}
				cout<<endl<<" 请输入修改内容为： ";
				string modifyContent;
				cin>>modifyContent;
				modifyflag=0;
				//撤销操作
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"修改前"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				tree.modifyName(&node,modifyName,modifyInfo,modifyContent);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"修改后"<<endl;posy++;printtree(&node,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 4:{
				cout<<" 请输入您要查询的信息：1.学校 2.学院 3.专业 4.班级 5.姓名： ";
				int queryInfo;
				while(cin>>queryInfo&&(queryInfo!=1&&queryInfo!=2&&queryInfo!=3&&queryInfo!=4&&queryInfo!=5&&queryInfo!=6)){
					cout<<" 输入不符合规则，请重新输入: ";
				}
				MNode *newnode=new MNode;//新建一个节点，将所有查询的信息赋值给他，然后进行查询
				queryflag=0;
				switch(queryInfo){//根据添加的信息种类不同而分类
					case 1:{
						cout<<" 请输入您要查询的学校：";
						cin>>newnode->info.xuexiao;
						tree.queryName(&node,newnode,1,1);
						break;
					}
					case 2:{
						cout<<" 请输入您要查询的学院的所在学校：";
						cin>>newnode->info.xuexiao;
						cout<<" 请输入您要查询的学院：";
						cin>>newnode->info.xueyuan;
						tree.queryName(&node,newnode,2,1);
						break;
					}
					case 3:{
						cout<<" 请输入您要查询的专业的所在学校：";
						cin>>newnode->info.xuexiao;
						cout<<" 请输入您要查询的专业的所在学院：";
						cin>>newnode->info.xueyuan;
						cout<<" 请输入您要查询的专业：";
						cin>>newnode->info.zhuanye;
						tree.queryName(&node,newnode,3,1);
						break;
					}
					case 4:{
						cout<<" 请输入您要查询的班级的所在学校：";
						cin>>newnode->info.xuexiao;
						cout<<" 请输入您要查询的班级的所在学院：";
						cin>>newnode->info.xueyuan;
						cout<<" 请输入您要查询的班级的所在专业：";
						cin>>newnode->info.zhuanye;
						cout<<" 请输入您要查询的班级：";
						cin>>newnode->info.banji;
						tree.queryName(&node,newnode,4,1);
						break;
					}
					case 5:{
						cout<<" 请输入您要查询的姓名的所在学校：";
						cin>>newnode->info.xuexiao;
						cout<<" 请输入您要查询的姓名的所在学院：";
						cin>>newnode->info.xueyuan;
						cout<<" 请输入您要查询的姓名的所在专业：";
						cin>>newnode->info.zhuanye;
						cout<<" 请输入您要查询的班级：";
						cin>>newnode->info.banji;
						cout<<" 请输入您要查询的姓名：";
						cin>>newnode->info.name;
						tree.queryName(&node,newnode,5,1);
						break;
					}
				};
				delete(newnode);//防止内存泄漏
				break;
			}
			case 5:{
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"最新目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				break;
			}
			case 6:{
				if(restack.size()==0){
					system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"撤销前"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
					posx=40,posy=2;gotoxy(posx+1,posy);cout<<"无操作可以撤销"<<endl;posy++;printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					break;
				}
				MNode *cheMNode=restack.top();
				restack.pop();
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"撤销前"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				node.parent=nullptr;node.children.clear();
				copyMNode(&node,cheMNode);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"撤销后"<<endl;posy++;
				printtree(&node,0);
				printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 7:{
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"最后目录"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				cout<<endl<<"操作结束"<<endl;
				goto ends;
			}
		}
	}
	ends:
	system("pause");
}