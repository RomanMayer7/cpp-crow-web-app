#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <boost/filesystem.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::cursor;

using namespace std;
using namespace crow;

void sendFile(response &res,string filename,string contentType){
  char *cwd = get_current_dir_name();
  cout << "Current working directory: " << cwd << endl;
  free(cwd);

  string publicFolderPath="/usr/src/cppweb/hello_crow/public/"+filename;
  ifstream in(publicFolderPath,ifstream::in);
  if( in.is_open() ){
    cout<<"-----------static content exist in public folder------------"<<endl;
    cout<<publicFolderPath<<endl;
        //  in.close();
      } else {
          perror(publicFolderPath.c_str());
      }

  if(in){
    ostringstream contents;
    contents<<in.rdbuf();
    in.close();
    res.set_header("Content-Type",contentType);
    res.write(contents.str());
  } else {
    res.code =404;
    res.write("Not Found");
  }
  res.end();
}
//-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

void sendHTML(response &res,string filename){
   sendFile(res,filename + ".html","text/html");
}

void sendImage(response &res,string filename){
   sendFile(res,"images/"+filename,"image/jpeg");
}

void sendScript(response &res,string filename){
   sendFile(res,"scripts/"+filename,"text/javascript");
}

void sendStyle(response &res,string filename){
   sendFile(res,"styles/"+filename,"text/css");
}

//---------------<-MAIN->-----------------------------------------------------------------------
int main(int argc, char* argv[]) {
  crow::SimpleApp app;

  //ESTABLISH CONNECTION WITH MONGO DATABASE
  mongocxx::instance inst{};
  string mongoConnect = std::string(getenv("MONGODB_URI"));
  mongocxx::client conn{mongocxx::uri{mongoConnect}};
  auto collection = conn["crow-web-app_db"]["contacts"];
/*  mongocxx::uri uri("mongodb://localhost:27017");
  mongocxx::client client(uri);
  mongocxx::database db = client["crow-web-app_db"];
  mongocxx::collection collection = db["contacts"];*/

 //'STATIC CONTENT' HANDLERS
  //------sending static content(images,styles,scripts)---------
  CROW_ROUTE(app, "/styles/<string>")
    ([](const request &req,response &res, string filename){
        sendStyle(res,filename);
    });

  CROW_ROUTE(app, "/scripts/<string>")
      ([](const request &req,response &res, string filename){
          sendScript(res,filename);
      });

  CROW_ROUTE(app, "/images/<string>")
        ([](const request &req,response &res, string filename){
            sendImage(res,filename);
        });
  //----------------------------------------------------------------

  //ROUTE HANDLERS
  CROW_ROUTE(app, "/contacts")
    ([&collection](){
      mongocxx::options::find opts;
      opts.skip(9);
      opts.limit(10);
      auto docs = collection.find({}, opts);
      std::ostringstream os;
      for(auto &&doc : docs){
        os << bsoncxx::to_json(doc) << "\n";
      }
      return crow::response(os.str());
    });

  CROW_ROUTE(app, "/")
    ([](const request &req,response &res){
        sendHTML(res,"index");
    });

    CROW_ROUTE(app, "/about")
      ([](const request &req,response &res){
          sendHTML(res,"about");
      });
//---------------------------------------------------------------------
  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  cout<<"Mongo DB Connection string:"<<mongoConnect<<endl;
  app.port(iPort).multithreaded().run();
}
