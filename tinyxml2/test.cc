#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<regex>
#include<vector>
#include<fstream>
using namespace std;
using namespace tinyxml2;

void test0(){
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID()){
        cerr<<"ErrorID is"<<doc.ErrorID()<<endl;
    }
    XMLElement *itemNode=doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    string title=itemNode->FirstChildElement("title")->GetText();
    string link=itemNode->FirstChildElement("link")->GetText();
    
    string description =itemNode->FirstChildElement("description")->GetText();
    string content=itemNode->FirstChildElement("content:encoded")->GetText();
    std::regex rg("<[^>]*>");
    description = std::regex_replace(description, rg, "");
    content = std::regex_replace(content, rg, "");
    cout<<"title "<< title<<endl;
    cout<<"link "<< link<<endl;
    cout<<"description "<< description<<endl;
    cout<<"content "<< content<<endl;


}
struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};
class RssReader{
public:
    
    void parseRss();//解析
    void dump(const string &filename);//输出
private:
    vector<RssItem> _rss;
};
void RssReader:: parseRss(){
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID()){
        cerr<<"RssReader::parseRss is Error"<<endl;
        return ;
    }
    XMLElement *itemNode=doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    while(itemNode){
        RssItem rs;
        rs.title=itemNode->FirstChildElement("title")->GetText();
        rs.link=itemNode->FirstChildElement("link")->GetText();
        rs.description=itemNode->FirstChildElement("description")->GetText();
        rs.content=itemNode->FirstChildElement("content:encoded")->GetText();
        regex rg("<[^>]>");
        rs.description=regex_replace(rs.description,rg,"");
        rs.content =regex_replace(rs.content,rg,"");
        _rss.push_back(rs);
        itemNode=itemNode->NextSiblingElement("item");
    }
}
void RssReader::dump(const string &filename){
    ofstream  fp(filename);
    if(!fp){
        cerr<<"无法打开文件"<<endl;
        return ;
    }
    for(int idx=0;idx<_rss.size();idx++){
        fp<<"<doc>"<<endl
        <<"  <docid>"<<idx+1<<"</docid>"<<endl
        <<"  <title>"<<_rss[idx].title<<"/title"<<endl
        <<"  <link>"<<_rss[idx].link<<"/link"<<endl
        <<"  <description>"<<_rss[idx].description<<"/description"<<endl
        <<"  <content>"<<_rss[idx].content<<"/content"<<endl
        <<"</doc>"<<endl;
    }
    
    fp.close();
    return;
}
int main(){
    //test0();
    RssReader rss;
    rss.parseRss();
    rss.dump("pagelib.dat");
    return 0;
}