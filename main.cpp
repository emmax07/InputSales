//I am single person in this projet so I am assuming that stocks are indivisible and all have limiting price. This is all mentioned in the doc given.
//Furthermore, I have done only file outputs as mentioned.
#include<iostream>
using namespace std;
#include<vector>
#include<queue>
#include<fstream>

//buying order with limit price
struct limitOrderBuying {
	string time;//order id
	float limitPrice;//limit price
	int shares;//quantity of shares available
	bool available;//if not sold
};


//operator overloading of struct limitOrderBuying for use in priority queue
bool operator < (const limitOrderBuying& p1, const limitOrderBuying& p2) {
	if (p1.limitPrice < p2.limitPrice)
		return true;
	else if (p2.limitPrice < p1.limitPrice)
		return false;
	else
	{
		return p1.time < p2.time;
	}
}

//selling order with limit price
struct limitOrderSelling {
	string time;
	float limitPrice;
	int shares;
	char available;
};

//operator overloading of struct limitOrderSelling for use in priority queue
bool operator < (const limitOrderSelling& p1, const limitOrderSelling& p2) {
	if (p1.limitPrice > p2.limitPrice)
		return true;
	else if (p2.limitPrice > p1.limitPrice)
		return false;
	else
	{
		return p1.time < p2.time;
	}
}

int main(int argc,char** argv)
{
	vector<limitOrderBuying> vB;//to check available orders(buying)
	vector<limitOrderSelling> vS;//to check available orders(selling)
	priority_queue<limitOrderBuying> pB;//to prioritize according to conditions
	priority_queue<limitOrderSelling> pS;
	float mValue;
	string time;
	char BorS;
	char LorM;
	char DorI;
	float lValue;
	int q;

	string inputString=argv[1];
    	ifstream fin(inputString);
	string timePrev;
	
	if (fin.is_open())
	{
        	ofstream fout("executions.txt");
		fin >> mValue;
		while (fin.eof()==0)
		{
			while (!pB.empty())pB.pop();//maintain new priority wise queue
			for (int i = 0; i < vB.size(); i++)
			{
				if(vB[i].available==true)
				pB.push(vB[i]);
			}
			while (!pS.empty())pS.pop();
			for (int i = 0; i < vS.size(); i++)
			{
				if (vS[i].available == true)
				pS.push(vS[i]);
			}
			fin >> time;
			fin >> BorS;
			fin >> LorM;
			fin >> DorI;
			fin >> lValue;
			fin >> q;
			if(timePrev!=time)
			if (BorS == 'B')
			{
				limitOrderBuying b;
				b.time = time;
				b.available = true;
				b.limitPrice = lValue;
				b.shares = q;
				bool flag = false;
				while (pS.empty()==false && flag == false)
				{
					if (pS.top().limitPrice <= b.limitPrice && pS.top().shares==b.shares)//for matching
					{
						flag = true;
						fout << time <<" "<< pS.top().shares<<" shares "<< "purchased at price " << pS.top().limitPrice << endl;
						fout << time <<" "<< b.shares<<" shares "<<"sold at price " << b.limitPrice << endl;
						for (int i = 0; i < vS.size(); i++)
						{
							if (vS[i].time == pS.top().time)
							{
								vS[i].available = false;//set now unavailable
							}
						}
					}
					pS.pop();
				}
				if (flag == false)
				{
					b.available = true;
					vB.push_back(b);
				}
			}
			else
			{
				limitOrderSelling s;
				s.time = time;
				s.available = true;
				s.limitPrice = lValue;
				s.shares = q;
				bool flag = false;
				while (!pB.empty()==false && flag == false)
				{
					if (pB.top().limitPrice >= s.limitPrice && pB.top().shares==s.shares)//for matching
					{
						
						flag = true;
						fout << time <<" "<< pB.top().shares<<" shares "<<"sold at price " << pB.top().limitPrice << endl;
						fout << pB.top().time <<" "<< s.shares << " shares "<< "bought at price " << s.limitPrice << endl;

						for (int i = 0; i < vB.size(); i++)
						{
							if (vB[i].time == pB.top().time)
							{
								vB[i].available = false;
							}
						}
					}
					pB.pop();
				}
				if (flag == false)
				{
					s.available = true;
					vS.push_back(s);
				}
			}
			timePrev=time;
		}

        for (int i = 0; i < vB.size(); i++)//writing for unmatched or unexecuted orders
        {
            if (vB[i].available==true)fout << vB[i].time << " " <<  vB[i].shares <<" shares"  << " unexecuted\n";
        }
        for (int i = 0; i < vS.size(); i++)
        {
            if (vS[i].available==true)fout <<  vS[i].time <<" " << vS[i].shares <<" shares"  << " unexecuted\n";
        }
        
	}
    else//file not opened
    {
        cout<<"not opened";
    }
    
	
}


