#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "utils.h"



/* Funzioni di utilità generale */
void debug_(String nomefile,int riga, String what)
{
  cout<<"File "<<nomefile<<": "<<riga<<" "<<what<<endl;
}

String underString(String anc, int pos)
{
 StringTokenizer mytoken((String&)anc,",\r\n");
 String newString,lastToken;
 int cont;

 cont=1;
 newString="";
 while (mytoken.hasMoreTokens())
  {
    lastToken=(String) mytoken.nextToken();
    if (pos!=cont)
       newString=newString+lastToken+",";
    cont++;
  }

  return newString;

}

String genBit(double value,double lung)
{
// cout<<"genBit"<<endl;
 String result;
 unsigned c,v;
 unsigned displayMask = 1<<(unsigned)lung;
 v=(unsigned) value;


 for(c=1;c<=((unsigned)lung+1); c++)
    {
	result+=(v & displayMask? '1': '0');
	v<<=1;

    }
 // cout<<"result"<<result<<endl;
  return result;

}


void combina(String rule, String node)
{
// cout<<"==========================="<<endl;
// cout<<"combina nodo "<<node<<endl;
// cout<<"Combina "<<rule<<endl;
 vector <String> vectToken;
 String lastToken;
 StringTokenizer mytoken((String&)rule,",\r\n");
 String criptRule;
 String suffix;

 while (mytoken.hasMoreTokens())
  {
    lastToken=(String) mytoken.nextToken();
    vectToken.push_back(lastToken);
  }
  suffix=node;
  //cout<<"Dimensione vettore "<<vectToken.size()<<endl;
//  cout<<"LAST Elem "<<lastToken<<endl;
//  last=vectToken.size();
 // vectToken.erase(last);
/*  for (int i=0; i<vectToken.size(); ++i)
	{
		cout<<vectToken[i]<<" ";
	}
*/
 //lunghezza massima della stringa (last-1)+2
// cout<<endl;
 double y=vectToken.size();
// cout<<" y "<<y;
 double tot=pow(2,y);
// cout<<" tot "<<tot<<endl;

 for (double k=1;k<tot;k++)
 {
    criptRule=genBit(k,y);
   decripta(criptRule,vectToken,suffix);
 }
}

void decripta(String rule,vector <String> vectToken,String suffix )
{
// cout<<"=====DECRIPTA===="<<endl;
 String body;
 String head;
 int lunga=rule.size();
// cout<<"lung stringa"<<lunga<<endl;

 for (int k=1;k<lunga;k++)
 {
  // cout<<rule[k]<<endl;
   if (rule[k]=='1')
      body+=vectToken[k-1]+",";
   else
      head+=vectToken[k-1]+",";
 }
 head+=suffix;
 cout<<"regola: "<<body<<"->"<<head<<endl;
}


void combina2(String rule)
{
// cout<<"==========================="<<endl;
// cout<<"Combina "<<rule<<endl;
 vector <String> vectToken;
 String lastToken;
 StringTokenizer mytoken((String&)rule,",\r\n");
 String criptRule;
// String suffix;

 while (mytoken.hasMoreTokens())
  {
    lastToken=(String) mytoken.nextToken();
    vectToken.push_back(lastToken);
  }
  //cout<<"Dimensione vettore "<<vectToken.size()<<endl;
//  cout<<"LAST Elem "<<lastToken<<endl;
//  last=vectToken.size();
 // vectToken.erase(last);
/*  for (int i=0; i<vectToken.size(); ++i)
	{
		cout<<vectToken[i]<<" ";
	}
*/
 //lunghezza massima della stringa (last-1)+2
// cout<<endl;
 double y=vectToken.size();
// cout<<" y "<<y;
 double tot=pow(2,(y-1));
// cout<<" tot "<<tot<<endl;

 for (double k=1;k<tot;k++)
 {
   criptRule=genBit(k,y);
   decripta2(criptRule,vectToken);
 }
}


void decripta2(String rule,vector <String> vectToken)
{
// cout<<"=====DECRIPTA===="<<endl;
 String body;
 String head;
 int lunga=rule.size();
// cout<<"lung stringa"<<lunga<<endl;

 for (int k=1;k<lunga;k++)
 {
  // cout<<rule[k]<<endl;
   if (rule[k]=='1')
      body+=vectToken[k-1]+",";
   else
      head+=vectToken[k-1]+",";
 }
// head+=suffix;
 cout<<"regola: "<<body<<"->"<<head<<endl;
}

