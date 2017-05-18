#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <string.h>
#include <vector>

using namespace std;

int book_system_time;              //虚拟时间
int Cur_log_denti=0;               //全局变量，当前登陆区分，1表示管理员，0表示没有登陆，client登陆值是读者编号的int类型

int stringtoint(string s)//基本函数
{
    int num;
    stringstream ss(s);
    ss>>num;
    return num;
}

string inttostring(int i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}

typedef struct book_singal                   //书结构,全部是静态信息
{

    string type;                             //书的类型
    int buytime;                             //书的购买时间，也就是书被录入图书馆的时间
    string name;                             //书名
    string author;                           //书的作者
    string press;                            //出版社
    double price;                            //书价格
    int borrowpermission;                    //书的借阅权限，1代表老师和学生均可以借阅，0代表只有老师可以借阅
    int state;                               //书的状态，1代表该书还在图书馆中，0代表该书已被借阅,2代表已经删除,3被预约
    string id;                               //书的编号，由code()函数生成
    void introduction();                      //书的简介，生成不用输入
} sinbook;

void introduction(book_singal b)
{
    cout<<"this book has a name of "<<b.name
        <<",wroten by "<<b.author
        <<",type is "<<b.type
        <<",press is "<<b.press<<endl;
}

typedef struct lib_client
{
    //人员结构
    string name;                            //客户姓名
    string cli_pass_word;                    //客户密码
    int client_identity;                    //身份信息用来区分老师和学生，0代表老师，1代表学生,2代表管理员
    string institute;                       //客户所在学院
    int creat_time;                          //账户创建时间
    int borrownum;                          //客户已借阅图书数量
    string borrowid[5];                     //客户已借图书的id
    int orderbooknum;
    string orderbookid[5];                   //预约的图书ID
    double Arrears_money;                     //借书欠款
    int honestlevel;                         //诚信等级
    string clid;                              //id
} boclient;

typedef struct lib_comm_list
{
    string bor_book_id;                       //借书的id/预约的id
    string bor_client_id;                     //借书人的id
    int bor_time;                              //借书的时间/预约时间
    int bor_term;                               //借书时长/预约时间
    string bor_way;                               //借书方式（管理员/用户）/预约方式（管理员/用户自己）
    int bookfine;                               //产生罚款/预约不填
    int retu_book;                              //还书没？还书保存还书时间.没换0/预约过期或者失效了失效时间
	int type_comm;                              //借书还是预约,借书1，预约2
} borrow_mass;


class book_shelf
{
public:
    book_shelf();                             //构造函数
    ~book_shelf();                            //析构函数
    vector<sinbook> booklib;                 //书的总数量
    void buybook();                          //图书入库
    //void initializebook();                   //从文件初始化图书
    void renewbook(string renewid,int renew_choice);                        //更新修改信息
    void search_book_shelf(int num,string inf);         //查询模块
    sinbook & id_to_book(string bokide);            //根据bookid返回book

    vector<boclient> suclient;          //存人员信息
    void new_client();                        //加入人员
    //void initializecli();                    //初始化
    void renewclient(string renewid,int renew_choice);                       //更新人员信息
    void search_client_inf(int num,string inf);       //查询模块
    boclient & id_to_client(string cli_gr);             //根据id返回人员

    //借书模块
    vector<borrow_mass> bor_record;                    //交易记录
    void statistic();                                   //交易记录本月统计
    void borr_book(string bookid,string clinid);       //借书
    void back_book(string bookid);                                  //还书
    void order_book(string bookid, string clinid);                                 //预约
    void order_dele_book(string clinid);                            //预约显示
	void delet_book(string bookid);                                         //删除
    int judg_ord_suc(string okid,string inid);      //判断有没有预约书


    //显示模块
    void displaybook(sinbook boid);                      //显示书，一本
    void displaybook();                                  //显示所有书
    void display_client(boclient cl_id);                 //显示顾客
    void display_client();                               //显示所有顾客
};

book_shelf::~book_shelf()
{
    ofstream fout(".//document//time_single.txt");//对时间的保存
    fout<<book_system_time;
    fout.clear();
    fout.close();

    ofstream dout(".//document//book_she_text.txt");//对书的保存
    for(int i=0; i<(int)booklib.size(); i++)
    {
        dout<<booklib[i].type<<endl;
        dout<<booklib[i].name<<endl;
        dout<<booklib[i].buytime<<endl;
        dout<<booklib[i].author<<endl;
        dout<<booklib[i].press<<endl;
        dout<<booklib[i].price<<endl;
        dout<<booklib[i].borrowpermission<<endl;
        dout<<booklib[i].state<<endl;
        dout<<booklib[i].id<<endl;
    }
    dout.clear();
    dout.close();

    ofstream gout(".//document//client_he_text.txt");//对客户的保存
    for(int i=0; i<(int)suclient.size(); i++)
    {
        gout<<suclient[i].name<<endl;
        gout<<suclient[i].cli_pass_word<<endl;
        gout<<suclient[i].client_identity<<endl;
        gout<<suclient[i].institute<<endl;
        gout<<suclient[i].creat_time<<endl;

        gout<<suclient[i].borrownum<<endl;
        for(int j=0; j<suclient[i].borrownum; j++)
            gout<<suclient[i].borrowid[j]<<endl;

        gout<<suclient[i].orderbooknum<<endl;
        for(int j=0; j<suclient[i].orderbooknum; j++)
            gout<<suclient[i].orderbookid[j]<<endl;
        gout<<suclient[i].Arrears_money<<endl;
        gout<<suclient[i].honestlevel<<endl;
        gout<<suclient[i].clid<<endl;
    }
    gout.clear();
    gout.close();


	ofstream rout(".//document//log_file.txt");//交易记录的保存
	for (int i = 0;i<(int)bor_record.size();i++)
	{
		rout << bor_record[i].bor_book_id << endl;
		rout << bor_record[i].bor_client_id << endl;
		rout << bor_record[i].bor_time << endl;
		rout << bor_record[i].bor_term << endl;
		rout << bor_record[i].bor_way << endl;
		rout << bor_record[i].bookfine << endl;
		rout << bor_record[i].retu_book << endl;
		rout << bor_record[i].type_comm << endl;
	}
	rout.clear();
	rout.close();
	cout << "good night!" << endl;
}

book_shelf::book_shelf()
{

    fstream fin;
    fin.open(".//document//book_she_text.txt",fstream::in);                         //读取图书基本信息
    string str;
    sinbook temmem;
    fin.seekg(0,ios_base::end);
    int twmp=fin.tellg();
    if(twmp!=0){
        fin.seekg(0,ios_base::beg);
    while(!fin.eof())
    {

        getline(fin,temmem.type);
        fin>>temmem.buytime;
        getline(fin,str);
        getline(fin,temmem.name);
        getline(fin,temmem.author);
        getline(fin,temmem.press);
        fin>>temmem.price;
        getline(fin,str);
        fin>>temmem.borrowpermission;
        getline(fin,str);
        fin>>temmem.state;
        getline(fin,str);

        getline(fin,temmem.id);

        booklib.push_back(temmem);//memory the information of temp book
    }
    }
    fin.clear();
    fin.close();

    fstream dfin(".//document//client_he_text.txt",fstream::in);//人员信息
    boclient temclie;

    dfin.seekg(0,ios_base::end);
    int tmp=dfin.tellg();
    if(tmp!=0){
        dfin.seekg(0,ios_base::beg);

        while(!dfin.eof())
        {
            getline(dfin,temclie.name);
            getline(dfin,temclie.cli_pass_word);
            dfin>>temclie.client_identity;

            getline(dfin,str);

            getline(dfin,temclie.institute);
            dfin>>temclie.creat_time;
            getline(dfin,str);
            dfin>>temclie.borrownum;
            getline(dfin,str);
            if(temclie.borrownum>5||temclie.borrownum<-1)
                temclie.borrownum=0;
            for(int j=0; j<temclie.borrownum; j++)
              {
                 getline(dfin,temclie.borrowid[j]);
                   cout<<"ttt";
              }

            dfin>>temclie.orderbooknum;
            getline(dfin,str);
            if(temclie.orderbooknum>5||temclie.orderbooknum<-1)
                temclie.orderbooknum=0;
            for(int j=0; j<temclie.orderbooknum; j++)
               getline(dfin,temclie.orderbookid[j]);
            dfin>>temclie.Arrears_money;
            getline(dfin,str);
            dfin>>temclie.honestlevel;
            getline(dfin,str);
            getline(dfin,temclie.clid);

            suclient.push_back(temclie);
        }

    }
    dfin.clear();
    dfin.close();

    fstream gfin(".//document//log_file.txt");
    gfin.seekg(0,ios_base::end);
    int ewmp=gfin.tellg();
    if(ewmp!=0){
        gfin.seekg(0,ios_base::beg);

	borrow_mass temp_log;
	while (!gfin.eof())
	{
		getline(gfin, temp_log.bor_book_id);
		getline(gfin, temp_log.bor_client_id);
		gfin >> temp_log.bor_time;
		getline(gfin, str);
		gfin >> temp_log.bor_term;
		getline(gfin, str);
		getline(gfin, temp_log.bor_way);
		gfin >> temp_log.bookfine;
		getline(gfin, str);
		gfin >> temp_log.retu_book;
		getline(gfin, str);
		gfin >> temp_log.type_comm;
		getline(gfin, str);
		//if((temp_log.type_comm==2)&&(temp_log.bor_time-book_system_time>temp_log.bor_term))
        //{
        //    temp_log.retu_book=book_system_time;
        //    cout<<temp_log.bor_client_id<<"order"<<temp_log.bor_book_id<<"have over time!"<<endl;
        //}
		bor_record.push_back(temp_log);
	}

	gfin.clear();
	gfin.close();
    }
    cout << "Ready!" << endl;
}



sinbook & book_shelf::id_to_book(string bokide)        //书的id对应书结构
{
    for(int i=0;i<(int)booklib.size(); i++)
    {
        if(bokide==booklib[i].id)
            return booklib[i];
    }
    cout<<"no book!"<<endl;
}

boclient & book_shelf::id_to_client(string cli_gr)
{
    for(int i=0; i<(int)suclient.size(); i++)
    {
        if(cli_gr==suclient[i].clid)
            return suclient[i];
    }
    cout<<"no body!"<<endl;
}

int book_shelf::judg_ord_suc(string okid,string inid)
{
    for(int i=0; i<id_to_client(inid).orderbooknum; i++)
    {
        if(id_to_client(inid).orderbookid[i]==okid&&(id_to_book(okid).state==3))
            return 1;
    }
    return 0;
}

void book_shelf::statistic()
{
    int bonum,renum,unret,monu;//统计借书量，还书量，没换书的数目，营业额
    bonum=0;
    renum=0;
    unret=0;
    monu=0;
    for(int i=0;i<(int)bor_record.size();i++)
    {
        if((bor_record[i].type_comm==1)&&(book_system_time-bor_record[i].bor_time<300))
            ++bonum;
        if((bor_record[i].type_comm==1)&&((book_system_time-bor_record[i].retu_book<300)))
           ++renum;
        if(bor_record[i].retu_book==0)
            ++unret;
        monu=monu+bor_record[i].bookfine;
    }
    cout<<"The number of books borrowed this month is:"<<bonum<<endl;
    cout<<"The number of books returned this month is:"<<renum<<endl;
    cout<<"The number of books unreturned this month is:"<<unret<<endl;
    cout<<"This month's fine income is:"<<monu<<endl;
}

void book_shelf::borr_book(string bookid,string clinid)
{
    if(id_to_client(clinid).honestlevel>5)                       //决策树
        cout<<"You are on the blacklist!"<<endl;
    else
    {
        if(id_to_client(clinid).Arrears_money>10.0)
        {
            cout<<"You already owe too much money!"<<endl;//此处可以添加还钱链接！以后添加吧！并不是主要程序，不优先
        }
        else
        {
            if(id_to_client(clinid).borrownum>4)
            {
                cout<<"You have borrowed too many books!"<<endl;//此处可以添加还书链接！
            }
            else
            {

                //
                if(id_to_book(bookid).state==1)
                {

                    if(id_to_book(bookid).borrowpermission==0&&id_to_client(clinid).client_identity==1)
                        cout<<"You don't have access to books. This book is for teachers!"<<endl;
                    else
                    {
                        borrow_mass exml;
                        exml.bor_book_id=bookid;
                        exml.bor_client_id=clinid;
                        exml.bor_time=book_system_time;
                        cout<<"please input the term you want borrow:";
                        cin>>exml.bor_term;                    //可以加一个输入检查
                        exml.bookfine=0;
						exml.type_comm = 1;
                        exml.retu_book=0;
						if (Cur_log_denti == 1)
							exml.bor_way = "admin";//后期写登陆以后可以写服务类型
						else
							exml.bor_way = "yourself";
                        bor_record.push_back(exml);//填写借书记录完毕，开始修改信息

                        id_to_client(clinid).borrowid[id_to_client(clinid).borrownum]=bookid;
                        ++(id_to_client(clinid).borrownum);//修改完顾客信息

                        id_to_book(bookid).state=0;
                        cout<<"You have been successful by self-help books, enjoy reading, enjoy life!"<<endl;
                    }
                   //judg_ord_suc(bookid,clinid)=1;
                }
                else if(judg_ord_suc(bookid,clinid)==1)//预约成功
                {
                    borrow_mass exml;
                    exml.bor_book_id=bookid;
                    exml.bor_client_id=clinid;
                    exml.bor_time=book_system_time;
                    cout<<"please input the term you want borrow:";
                    cin>>exml.bor_term;//可以加一个输入检查
                    exml.bookfine=0;
                    exml.retu_book=0;
					exml.type_comm = 1;
					if (Cur_log_denti == 1)
						exml.bor_way = "admin";//后期写登陆以后可以写服务类型
					else
						exml.bor_way = "yourself";//后期写登陆以后可以写服务类型
                    bor_record.push_back(exml);//填写借书记录完毕，开始修改信息

                    for(int i=0;i<id_to_client(clinid).orderbooknum;i++)//从预约菜单中删除借的书
                    {
                        if(id_to_client(clinid).orderbookid[i]==bookid)
                            id_to_client(clinid).orderbookid[i]=id_to_client(clinid).orderbookid[id_to_client(clinid).orderbooknum];
                    }
                    --id_to_client(clinid).orderbooknum;
                    id_to_client(clinid).borrowid[id_to_client(clinid).borrownum]=bookid;
                    ++id_to_client(clinid).borrownum;//修改完顾客信息

                    id_to_book(bookid).state=0;
                    cout<<"You have been successful by self-help books, enjoy reading, enjoy life!"<<endl;
                }
                else
                    cout<<"There is no book in the library! So sorry!"<<endl;
            }
        }
    }
}

void book_shelf::back_book(string bookid)
{
	int temp_c=0;
	for (int i = 0;i<(int)bor_record.size();i++)//
	{
		if (bor_record[i].bor_book_id == bookid&&bor_record[i].type_comm == 1&&bor_record[i].retu_book==0)
		{
			string temp_client = bor_record[i].bor_client_id;
			//修改人
			for (int j = 0;j < id_to_client(temp_client).borrownum;j++)
			{
				if (id_to_client(temp_client).borrowid[j] == bookid)
				{
					id_to_client(temp_client).borrowid[j] = id_to_client(temp_client).borrowid[id_to_client(temp_client).borrownum - 1];
					break;
				}
			}
			--(id_to_client(temp_client).borrownum);
			if (((book_system_time - bor_record[i].bor_time) - bor_record[i].bor_term) > 0)
			++(id_to_client(temp_client).honestlevel);
			id_to_client(temp_client).Arrears_money += ((book_system_time - bor_record[i].bor_time) - (bor_record[i].bor_term))*id_to_book(bookid).price / 50;
			//修改书
			id_to_book(bookid).state = 1;

			//修改记录
			bor_record[i].retu_book = book_system_time;
			temp_c=1;
			break;
		}
	}
	if(temp_c==0)
	cout<<"no information!"<<endl;
	else
        cout<<"good game! guy!"<<endl;
}

void book_shelf::delet_book(string bookid)
{
	vector<sinbook>::iterator it;
	int i = 0;
	for (it = booklib.begin();it != booklib.end();i++)
	{
		if (booklib[i].id == bookid)
		{
			it = booklib.erase(it);
			break;
		}
		++it;
	}
	cout << "success delete!" << endl;
}

void book_shelf::order_book(string bookid, string clinid)
{
	if (id_to_client(clinid).honestlevel>5)                       //决策树
		cout << "You are on the blacklist!" << endl;
	else
	{
		if (id_to_client(clinid).Arrears_money > 10.0)
		{
			cout << "You already owe too much money!" << endl;//此处可以添加还钱链接！以后添加吧！并不是主要程序，不优先
		}
		else
		{
			if (id_to_book(bookid).state == 1)
			{
				cout << "The book you want to book is in the library,1:goto borrow,0:quit:";
				int temp;
				cin >> temp;
				if (temp == 1)
					borr_book(bookid, clinid);
			}
			else
			{
				if (id_to_book(bookid).state == 2)
					cout << "sorry,this book had been deleted!" << endl;
				else
				{
					id_to_client(clinid).orderbookid[id_to_client(clinid).orderbooknum] = bookid;
					++(id_to_client(clinid).orderbooknum);//对客户的影响

					borrow_mass exml;//产生一个预约记录
					exml.bor_book_id = bookid;
					exml.bor_client_id = clinid;
					exml.bor_time = book_system_time;
					exml.bor_term = 300;
					if (Cur_log_denti == 1)
						exml.bor_way = "admin";//后期写登陆以后可以写服务类型
					else
						exml.bor_way = "yourself";
					exml.bookfine = 0;
					exml.retu_book = 0;
					exml.type_comm = 2;
					bor_record.push_back(exml);//填写预约记录完毕
					cout << "order success!" << endl;
				}
			}
		}
	}
}

void book_shelf::order_dele_book(string clinid)
{
    for(int i=0;i<(int)id_to_client(clinid).orderbooknum;i++)
    {
        if(id_to_book(id_to_client(clinid).orderbookid[i]).state==1)
        {

            cout<<"you order book"<<id_to_client(clinid).orderbookid[i]<<"arrive,input 1 to borrow,others ignore:";
            //清理人

            int tempp;
            cin>>tempp;
            if(tempp==1)
                borr_book(id_to_client(clinid).orderbookid[i],clinid);
            string w;
            w=id_to_client(clinid).orderbookid[i];
            id_to_client(clinid).orderbookid[i]=id_to_client(clinid).orderbookid[id_to_client(clinid).orderbooknum-1];
            --id_to_client(clinid).orderbooknum;

            for(int j=0;j<(int)bor_record.size();j++)
            {
                if(bor_record[j].type_comm==2&&bor_record[j].bor_book_id==w)
                    bor_record[j].retu_book=book_system_time;
            }//销毁所有有关的图书记录
        }
    }
}

void book_shelf::displaybook(sinbook boid)
{
    cout<<"\ntype:"<<boid.type<<"\tbuytime:"<<boid.buytime
        <<"\tname:"<<boid.name<<"\tauthor:"<<boid.author
        <<"\tpress:"<<boid.press<<"\tprice:"<<boid.price
        <<"\tbookid:"<<boid.id<<endl;
}

void book_shelf::displaybook()
{
    int i=0;
    for(i;i<(int)booklib.size();i++)
    {
        displaybook(booklib[i]);
    }
}

void book_shelf::display_client(boclient cl_id)
{
    cout<<"\nname:"<<cl_id.name;
    if(cl_id.client_identity==0)
        cout<<"\ttype:teacher";
    else if(cl_id.client_identity==2)
        cout<<"\ttype:manager";
    else
        cout<<"\ttype:student";
    cout<<"\tinstitute:"<<cl_id.institute<<"\tcreattime:"<<cl_id.creat_time
        <<"\tid:"<<cl_id.clid<<"\thonest tevel:"<<cl_id.honestlevel<<endl;
    if(cl_id.borrownum>0)
    {
        cout<<"you have borrowed "<<cl_id.borrownum<<"book,interesting!"<<endl;
        for(int i=0; i<cl_id.borrownum; i++)
            displaybook(id_to_book(cl_id.borrowid[i]));
    }
    else
        cout<<"no borrow information!"<<endl;
    if(cl_id.orderbooknum==0)
        cout<<"no order book information!"<<endl;
    else
    {
        cout<<"you have ordered "<<cl_id.orderbooknum<<"book,interesting!"<<endl;
        for(int i=0; i<cl_id.orderbooknum; i++)
        {
            if(id_to_book(cl_id.orderbookid[i]).state==1)
                cout<<"your order book"<<id_to_book(cl_id.orderbookid[i]).name<<"are ready!"<<endl;
            else if(id_to_book(cl_id.orderbookid[i]).state==2)
                cout<<"your order book"<<id_to_book(cl_id.orderbookid[i]).name<<"have been delete!"<<endl;
            else if(id_to_book(cl_id.orderbookid[i]).state==0)
                cout<<"your order book"<<id_to_book(cl_id.orderbookid[i]).name<<"have been borrow by others!"<<endl;
        }
    }
}

void book_shelf::display_client()
{
    int i=0;
    for(i;i<(int)suclient.size();i++)
    {
        display_client(suclient[i]);
    }
}

unsigned int RSHash(string mas)              //hash函数
{
    const char *str = mas.c_str();
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash  & 0x3FFF);
}

string code(sinbook bbkk)                    //最大允许有10本相同的书
{
    string exmname;
    exmname=bbkk.name;
    string exmpress;
    exmpress=bbkk.press;
    fstream fin(".//document//code_dictionary.txt");
    int namecode=0;
    while(!fin.eof())
    {
        string rubbish2;
        string ename;
        string epress;
        string eauthor;
        getline(fin,ename);
        getline(fin,epress);
        getline(fin,eauthor);
        if(ename==exmname&&epress==exmpress&&eauthor==bbkk.author)
        {
            string enumber;
            getline(fin,enumber);
            namecode=stringtoint(enumber)-(stringtoint(enumber)/10)*10+1;
            getline(fin,rubbish2);
        }
        else
            getline(fin,rubbish2);
    }
    fin.close();
    bbkk.id=inttostring(RSHash(exmname))+inttostring(RSHash(exmpress))+inttostring(namecode);
    ofstream fout(".//document//code_dictionary.txt",ios::out|ios::app);
    fout<<bbkk.name<<endl;
    fout<<bbkk.press<<endl;
    fout<<bbkk.author<<endl;
    fout<<bbkk.id<<endl;
    fout.close();
    return bbkk.id;
}

string codclient(boclient boktemp)           //人员编码函数
{
    string exmname;
    exmname=boktemp.name;//kehuxingming
    string exmins;
    exmins=boktemp.institute;//kehuxueyuan
    fstream fin(".//document//code_dic_cli.txt");
    int namecode=0;
    while(!fin.eof())
    {
        string rubbish;
        string ename;
        string epress;
        getline(fin,ename);
        getline(fin,epress);
        if(ename==exmname&&epress==exmins)
        {
            string enumber;
            getline(fin,enumber);
            namecode=stringtoint(enumber)-(stringtoint(enumber)/10)*10+1;
        }
        else
            getline(fin,rubbish);
    }
    fin.close();
    boktemp.clid=inttostring(RSHash(exmname))+inttostring(RSHash(exmins))+inttostring(namecode);
    ofstream fout(".//document//code_dic_cli.txt",ios::out|ios::app);
    fout<<boktemp.name<<endl;
    fout<<boktemp.institute<<endl;
    fout<<boktemp.clid<<endl;
    fout.close();
    return boktemp.clid;
}

void book_shelf::buybook()                            //买书会存书
{
    sinbook temp_book;
    cout<<"Please input book type:";
    cin>>temp_book.type;
    cout<<"\nplease input name:";
    cin>>temp_book.name;
    cout<<"\nplease input author:";
    cin>>temp_book.author;
    cout<<"\nplease input press:";
    cin>>temp_book.press;
    cout<<"\nplease input price:";
    cin>>setprecision(2)>>temp_book.price;
    cout<<"\nplease input borrowpermission:";
    cin>>temp_book.borrowpermission;
    temp_book.state=1;
    temp_book.id=code(temp_book);
    temp_book.buytime=book_system_time;

    booklib.push_back(temp_book);                  //add vector
    ofstream fout(".//document//buy_information.txt",ios::out|ios::app);//买书记录
    fout<<temp_book.type<<endl;
    fout<<temp_book.buytime<<endl;
    fout<<temp_book.name<<endl;
    fout<<temp_book.author<<endl;
    fout<<temp_book.press<<endl;
    fout<<temp_book.price<<endl;
    fout<<temp_book.borrowpermission<<endl;
    fout<<temp_book.state<<endl;
    fout<<temp_book.id<<endl;
    fout.close();
}

void book_shelf::new_client()                         //新建人员信息
{
    boclient tempcli;
    cout<<"please input the client name:";
    cin>>tempcli.name;
    cout<<"please input the client password:";
    cin>>tempcli.cli_pass_word;
    cout<<"please input the client client_identity:";
    cin>>tempcli.client_identity;
    cout<<"please input the client institute:";
    cin>>tempcli.institute;
    tempcli.creat_time=book_system_time;
    tempcli.borrownum=0;
    tempcli.orderbooknum=0;
    tempcli.Arrears_money=0.0;
    tempcli.honestlevel=1;
    tempcli.clid=codclient(tempcli);
    cout<<tempcli.clid<<endl;
    suclient.push_back(tempcli);
    ofstream fout(".//document//client_lar_shelf.txt",ios::out|ios::app);//买书记录
    fout<<tempcli.name<<endl;
    fout<<tempcli.client_identity<<endl;
    fout<<tempcli.institute<<endl;
    fout<<tempcli.creat_time<<endl;
    fout<<tempcli.borrownum<<endl;
    fout<<tempcli.orderbooknum<<endl;
    fout<<tempcli.Arrears_money<<endl;
    fout<<tempcli.honestlevel<<endl;
    fout<<tempcli.clid<<endl;
    fout.close();
}


void book_shelf::renewbook(string renewid,int renew_choice)               //更新图书信息
{
    int searbookinf=-1;
    for(int i=0; (i<(int)booklib.size())&&searbookinf==-1; i++)
    {
        if(booklib[i].id==renewid)
            searbookinf=i;
    }
    if(searbookinf!=-1)
    {

        switch (renew_choice)
        {
        case 1:
        {
            cout<<"Please input book type:";
            cin>>booklib[searbookinf].type;
        }
        case 2:
        {
            cout<<"Please input book name:";
            cin>>booklib[searbookinf].name;
        }
        case 3:
        {
            cout<<"Please input book author:";
            cin>>booklib[searbookinf].author;
        }
        case 4:
        {
            cout<<"Please input book press:";
            cin>>booklib[searbookinf].press;
        }
        case 5:
        {
            cout<<"Please input book price:";
            cin>>booklib[searbookinf].price;
        }
        }
    }
    else
        cout<<"error id to search"<<endl;
}

void book_shelf::renewclient(string renewid,int renew_choice)
{
    int searcliinf=-1;
    for(int i=0; (i<(int)suclient.size())&&(searcliinf==-1); i++)
    {
        if(suclient[i].clid==renewid)
            searcliinf=i;
    }
    if(searcliinf!=-1)
    {
        switch (renew_choice)//有不同的功能
        {
        case 1:
        {
            cout<<"please input the new name:";
            cin>>suclient[searcliinf].name;
        }
        case 2:
        {
            cout<<"please input the new institude:";
            cin>>suclient[searcliinf].institute;
        }
        case 3:                                //还款，特殊功能
        {
            cout<<"please input the money:";
            int num;
            cin>>num;

            suclient[searcliinf].Arrears_money=suclient[searcliinf].Arrears_money+num;
        }
        case 4:
        {
            cout<<"please input the new password:";
            cin>>suclient[searcliinf].cli_pass_word;
        }
        }
    }
    else
        cout<<"error id of client"<<endl;
}


void book_shelf::search_book_shelf(int num,string inf)
{
    switch (num)
    {
    case 1:           //查询类型
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].type==inf)
            {
                cout<<booklib[i].id<<endl;
                cout<<booklib[i].name<<endl;
            }
        }
    }
    case 2:
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].buytime==stringtoint(inf))
            {
                cout<<booklib[i].id<<endl;
                cout<<booklib[i].name<<endl;
            }
        }
    }
    case 3:
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].name==inf)
            {
                cout<<booklib[i].id<<endl;
            }
        }
    }
    case 4:
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].author==inf)
            {
                cout<<booklib[i].id<<endl;
                cout<<booklib[i].name<<endl;
            }
        }
    }
    case 5:
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].press==inf)
            {
                cout<<booklib[i].id<<endl;
                cout<<booklib[i].name<<endl;
            }
        }
    }
    case 6:                                      //出版社
    {
        for(int i=0; i<(int)booklib.size(); i++)
        {
            if(booklib[i].price==stringtoint(inf))
            {
                cout<<booklib[i].id<<endl;
                cout<<booklib[i].name<<endl;
            }
        }
    }
    default:
        cout<<"error infomation"<<endl;
    }

    //可以构建查询记录，构建热搜榜，文件
}

void book_shelf::search_client_inf(int num,string inf)
{
    switch (num)
    {
    case 1:
    {
        for(int i=0; i<(int)suclient.size(); i++)
        {
            if(suclient[i].name==inf)
                cout<<suclient[i].clid<<endl;
        }
    }
    case 2:
    {
        for(int i=0; i<(int)suclient.size(); i++)
        {
            if(suclient[i].institute==inf)
                cout<<suclient[i].clid<<endl;
        }
    }
    default:
        cout<<"error choice!"<<endl;
    }
}
typedef struct __THREAD_DATA
{
    int localtime;
}THREAD_DATA;

HANDLE g_hMutex = NULL;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    while(true)
    {
        WaitForSingleObject(g_hMutex, INFINITE);
		book_system_time++;
        Sleep(1000);
        ReleaseMutex(g_hMutex);
    }
    return 0L;
}

int main(void)
{
    //cout<<"hhhh"<<endl;
    book_shelf library;
    int acc;
    string pasword;
    cout<<"                                          Welcome to the BUPT library management system"<<endl;

	//线程隔离区，登陆在前，操作在while循环中
	g_hMutex = CreateMutex(NULL, FALSE, NULL);

	fstream fin(".//document//time_single.txt");
	string book_system_time1;
	getline(fin, book_system_time1);
	stringstream ss(book_system_time1);
	ss >> book_system_time;
	fin.close();
	THREAD_DATA threadData;
	threadData.localtime=0;

	HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, &threadData, 0, NULL);//线程启动，时间运行
	CloseHandle(hThread1);
	//xianchenggeli
	cout<<"do you want to add new id?1: add,others: no"<<endl;
	int temp_num_sta;
	cin>>temp_num_sta;
	if(temp_num_sta==1)
	library.new_client();
reload_systerm:
    while(true)
    {
         cout<<"Please enter your account number:";
         cin>>acc;

		 if (acc == -1)
			 return 0;//直接退出程序
         cout<<"Please enter your password:";
         cin>>pasword;
        string exm_name;
        exm_name=inttostring(acc);
        int judg_temp=0;
        for(int i=0;i<(int)library.suclient.size();i++)
        {
            if(exm_name==library.suclient[i].clid)
                if(pasword==library.suclient[i].cli_pass_word)
                    {
					pasword = library.suclient[i].clid;
                        judg_temp=1;
                        break;//跳出登陆循环
                    }
        }
        if(judg_temp==1)
            break;
        cout<<"Wrong login or password, please login again,If you want to close the program,cin>>-1"<<endl;
    }
    if(library.id_to_client(pasword).client_identity==2)
    {
        Cur_log_denti=1;
        cout<<"You are the administrator!"<<endl;
    }
    else
        Cur_log_denti=stringtoint(pasword);//成功赋值当前登陆人员，
	if (Cur_log_denti == 1)//管理员的登陆
	{
		int choice_syst=0;

		while (true)
		{
			cout << "\tcin>>1.所有图书 " << endl;
			cout << "\tcin>>2.查询图书 " << endl;
			cout << "\tcin>>3.增加图书 " << endl;
			cout << "\tcin>>4.删除图书 " << endl;
			cout << "\tcin>>5.借阅图书 " << endl;
			cout << "\tcin>>6.归还图书 " << endl;
			cout << "\tcin>>7.修改图书 " << endl;
			cout << "\tcin>>8.本月统计 " << endl;//300次sleep后算一个月

			cout << "\tcin>>7.修改图书 " << endl;
			cout << "\tcin>>9.退出登录，重新登陆" << endl;
			cout << "\tcin>>10.退出登录，关闭系统" << endl;
			cout << "\tcin>>11.创建账户 " << endl;
			cout << "\tcin>>12.更改账户 " << endl;
			cout << "\tcin>>13.充值账户 " << endl;
			cin >> choice_syst;
			switch (choice_syst)
			{
			case 1:
			{
				library.displaybook();
				break;
			}
			case 2://查询
			{
				cout << "please input search information" << endl;
				int temp_choice;
				WaitForSingleObject(g_hMutex, INFINITE);
				cin >> temp_choice;
				string temp_inf;
				cin >> temp_inf;
				ReleaseMutex(g_hMutex);
				library.search_book_shelf(temp_choice, temp_inf);
				break;
            }
			case 3://增加图书
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				library.buybook();
				ReleaseMutex(g_hMutex);
				break;
			}
			case 4://删除图书
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				string temp_nam_book;
				cout << "input the id you want delete!" << endl;
				cin>> temp_nam_book;
				library.delet_book(temp_nam_book);
				ReleaseMutex(g_hMutex);
				break;
			}
			case 5:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				cout << "you need input the name youwant borrow" << endl;
				string temp_b,temp_c;
				cin.clear();
				cout<<"please input the bookid:";
				cin >> temp_b;
                cin.clear();
                cout<<"please input the client id:";
				cin >> temp_c;
				library.borr_book(temp_b,temp_c);
				ReleaseMutex(g_hMutex);
                break;
			}
			case 6:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				cout << "input the name you want return!" << endl;
				string temp_b;
				cin>>temp_b;
				library.back_book(temp_b);
				ReleaseMutex(g_hMutex);
                break;
			}
			case 7:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				int temp_cho;
				string temp_b;
				cin >> temp_cho;
				cin >> temp_b;
				library.renewbook(temp_b,temp_cho);
				ReleaseMutex(g_hMutex);
				break;
			}
			case  8:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				library.statistic();
				ReleaseMutex(g_hMutex);
				break;
			}
			case 9:
			{
				goto reload_systerm;
				break;
			}
			case 10:
			{
				return 0;

			}
			case 11:
                {
                WaitForSingleObject(g_hMutex, INFINITE);
				library.new_client();
				ReleaseMutex(g_hMutex);
				break;
                }
            case 12:
                {
                    WaitForSingleObject(g_hMutex, INFINITE);
                    cout<<"please input renewid:";
                    string temp_cli;
                    cin>>temp_cli;
                    cout<<"please input choice:";
                    cout<<"1:name;2:institute:";
                    int num;
                    cin>>num;
				library.renewclient(temp_cli,num);
				ReleaseMutex(g_hMutex);
				break;
                }
            case 13:
                {
                    WaitForSingleObject(g_hMutex, INFINITE);
                    cout<<"please input renewid:";
                    string temp_cli;
                    cin>>temp_cli;
                    library.renewclient(temp_cli,3);
                    ReleaseMutex(g_hMutex);
                }
			default:
			{
				cout << "Please re-enter" << endl;
				cin>>choice_syst;
				break;
			}
			}
		}

	}
	else {
        library.order_dele_book(inttostring(Cur_log_denti));
		while (true)
		{
			cout << "\tcin>>1.所有图书目录 " << endl;
			cout << "\tcin>>2.查询图书 " << endl;
			cout << "\tcin>>3.借阅图书 " << endl;
			cout << "\tcin>>4.归还图书 " << endl;
			cout << "\tcin>>5.预约图书 " << endl;
			cout << "\tcin>>6.退出登录，重新登陆" << endl;
			cout << "\tcin>>7.退出登录，关闭系统" << endl;
			int choice_syst;
			cin >> choice_syst;
			switch (choice_syst)
			{
			case 1:
			{
				library.displaybook();
				break;
			}
			case 2:
			{
				cout << "please input search information" << endl;
				int temp_choice;
				WaitForSingleObject(g_hMutex, INFINITE);
				cin >> temp_choice;
				string temp_inf;
				cin >> temp_inf;
				ReleaseMutex(g_hMutex);cin >> temp_choice;
				library.search_book_shelf(temp_choice, temp_inf);
				break;
			}
			case 3:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				cout << "you need input the name youwant borrow" << endl;
				string temp_c;
				cin >> temp_c;
				library.borr_book(temp_c,inttostring(Cur_log_denti));
				ReleaseMutex(g_hMutex);
				break;
			}
			case 4:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				cout << "input the name you want return!" << endl;
				string temp_b;
				library.back_book(temp_b);
				ReleaseMutex(g_hMutex);
				break;
			}
			case 5:
			{
				WaitForSingleObject(g_hMutex, INFINITE);
				cout << "input the book you want order";
				string temp_b;
				cin >> temp_b;
				library.order_book(temp_b, inttostring(Cur_log_denti));
				ReleaseMutex(g_hMutex);
				break;
			}
			case 6:
			{
				goto reload_systerm;
			}
			case 7:
				return 0;
			default:
			{
				cout << "Please re-enter" << endl;
			}
			}

		}
	}
    return 0;
}